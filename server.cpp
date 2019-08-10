#include "server.h"
#include "header.h"

DNSRelay::DNSRelay(std::int32_t argc, char *argv[])
{
    cli_socket = new QUdpSocket(this);
    sender = new QUdpSocket(this);

    cli_socket->bind(QHostAddress::Any, 53);
	time_init = time(NULL);
    timer = new QTimer(this);
    timer->start(1000);

	write_back = new QTimer(this);
	write_back->start(10000);
	handle_param(argc, argv);

    /* 初始化url_ip_table */
	if (get_url_ip_table() == false)
		throw "open url_ip_file error\n";
	/* 调试是否成功读入 */

    for(std::map<std::string,std::string>::iterator iter = url_ip_cache.begin(); iter !=url_ip_cache.end(); iter++)
        std::cout << iter->first << " " << iter->second << std::endl;

	clock_c = new clock_ctl();

    connect(cli_socket, &QUdpSocket::readyRead, this, &DNSRelay::data_read_ready);
    connect(sender, &QUdpSocket::readyRead, this, &DNSRelay::data_recv_ready);
    connect(timer, &QTimer::timeout, this, &DNSRelay::change_ttl);
	connect(write_back, &QTimer::timeout, this, &DNSRelay::cache_back);

    n_count = 0;
}

void DNSRelay::cache_back()
{
	std::ofstream of(filename);
	for (std::map<std::string, std::string>::iterator iter = url_ip_cache.begin(); iter != url_ip_cache.end(); iter++)
		of << iter->second << " " << iter->first << "\n";
	of.close();
}

void DNSRelay::handle_param(std::int32_t argc, char *argv[])
{
	if (argc == 1)
		return;
	if (strlen(argv[1]) == 2 && !strcmp(argv[1], "-d"))
		debug_level = 1;
	else if (strlen(argv[1]) == 3 && !strcmp(argv[1], "-dd"))
		debug_level = 2;
	else if ('0' <= argv[1][0] && argv[1][0] <= '9')
		DNS_server = QHostAddress(argv[1]);
	else
		filename = argv[1];
	if (argc == 3)
	{
		if ('0' <= argv[2][0] && argv[2][0] <= '9')
			DNS_server = QHostAddress(argv[2]);
		else
			filename = argv[2];
	}
	if (argc == 4)
	{
		DNS_server = QHostAddress(argv[2]);
		filename = argv[3];
	}
}

bool DNSRelay::get_url_ip_table()
{
    std::ifstream in(filename, std::ios::in);
    std::string tmp_url, tmp_ip;
    if(!in.is_open())
		return false;
    else
    {
        url_ip_cache.clear();
        while(!in.eof())
        {
            in >> tmp_ip >> tmp_url;
            url_ip_cache.insert(std::map<std::string, std::string>::value_type(tmp_url, tmp_ip));
			url_ttl_cache.insert(std::map<std::string, time_t>::value_type(tmp_url, 6000000));
        }
        in.close();
        return true;
    }
}

bool DNSRelay::get_ans_from_cache(std::string url, const QByteArray &t_array, const QHostAddress &addr, const quint16 &port)
{
    std::map<std::string, std::string>::iterator iter = url_ip_cache.find(url.substr(0,url.size() - 1));
    //std::cout << url << std::endl;
    if(iter != url_ip_cache.end())
    {
        if(iter->second == "0.0.0.0")
            build_blk_seg(t_array, addr, port);                //不良网页拦截
        else
            build_res_seg(iter->second, t_array, addr, port);  //构造回复包
        return true;
    }
    return false;
}

void DNSRelay::build_blk_seg(const QByteArray &t_array, const QHostAddress &addr, const quint16 port)
{
    QByteArray blk_seg;
    for (std::int32_t i = 0; i < t_array.size(); ++i) {
        if(i == 2)
            blk_seg.append(t_array.at(i) | 0x80);
        else if(i == 3)
            blk_seg.append(t_array.at(i) | 0x03);
        else
            blk_seg.append(t_array.at(i));
    }
    if(cli_socket->writeDatagram(blk_seg, addr, port) == -1)
    {
        qDebug() << "send blk_seg error";
        return;
    }
	printf("url blocked!\nsend to %s:%d\n============================================\n", addr.toString().toStdString().c_str(), port);
    return;
}

void DNSRelay::build_res_seg(std::string ip,const QByteArray &t_array, const QHostAddress &addr, const quint16 port)
{
    QByteArray res_seg;

    for (std::int32_t i = 0; i < t_array.size(); ++i) {
		if (i == 2)
			res_seg.append(t_array.at(i) | 0x80);
		else if (i == 7)
			res_seg.append(0x01);
		else if (i == 3)
			res_seg.append(0x80);
        else
            res_seg.append(t_array.at(i));
    }
    /* response part */
    for (std::int32_t i = 0; i < 2; ++i)
        res_seg.append(static_cast<char>(SAME_RNAME[i]));
    for(std::int32_t i = 0; i < 2; ++i)
        res_seg.append(static_cast<char>(TYPE_HOST_ADDRESS[i]));
    for(std::int32_t i = 0; i < 2; ++i)
        res_seg.append(static_cast<char>(CLASS_IN[i]));
    for(std::int32_t i = 0; i < 4; ++i)
        res_seg.append(static_cast<char>(TTL[i]));
    for(std::int32_t i = 0; i < 2; ++i)
        res_seg.append(static_cast<char>(DATA_LENGTH[i]));
    while(ip.find('.') != ip.npos) {
        ip.at(ip.find('.')) = ' ';
    }
    std::istringstream in(ip);
    std::int32_t tmp_num;
	while (in >> tmp_num)
		res_seg.append(static_cast<char>(tmp_num));

    cli_socket->writeDatagram(res_seg, addr, port);
	printf("cache hits!\nsend to %s:%d\n============================================\n", addr.toString().toStdString().c_str(), port);
	return;
}

void DNSRelay::data_read_ready()
{
	//printf("recv packet\n");
	QByteArray datagram;
	datagram.resize(static_cast<std::int32_t>(cli_socket->pendingDatagramSize()));
	QHostAddress addr;
	quint16 port;
	cli_socket->readDatagram(datagram.data(), datagram.size(), &addr, &port);
	if (datagram.size() != 0)
	{
		printf("recv from %s:%d\n", addr.toString().toStdString().c_str(), port);
		handle_packet(datagram, addr, port);
	}
}

void DNSRelay::data_recv_ready()
{
	printf("recv packet\n");
    QByteArray datagram;
	QHostAddress addr;
	quint16 port;
    datagram.resize(static_cast<std::int32_t>(sender->pendingDatagramSize()));
    sender->readDatagram(datagram.data(), datagram.size(), &addr, &port);
	printf("recv from %s:%u\n", addr.toString().toStdString().c_str(), port);
	if (datagram.size() == 0)
		return;
    std::uint8_t *datar = new std::uint8_t[datagram.size()];
    std::int32_t it = 0;
    for (auto &c : datagram)
        datar[it++] = static_cast<std::uint8_t>(c);
    DNSHeader t_header(datar);
	if (clock_c->check_timer(t_header.getID()))
		clock_c->delete_timer(t_header.getID());
	QueryHeader q_header(t_header, datar);
	if (debug_level == 1)
		printf("Time: %llds\nID: 0x%04x\nQNAME: %s\n", time(NULL) - time_init, t_header.getID(), q_header.QNAME.c_str());
	char rs_copy[4];
	if (t_header.getQR() == 1 && t_header.getQDCOUNT() == 1)
	{
		AnswerHeader a_header(t_header, datar);
		if (a_header.getRTYPE() == 0x01 && a_header.getANCOUNT() >= 1) 
		{
			std::string ip;
			for (std::int32_t i = 0; i < 4; i++)
				rs_copy[i] = a_header.getResource()[3 - i];
			ip = QHostAddress(*((std::int32_t *)(rs_copy))).toString().toStdString();
			//std::cout << "ip after transform : " << ip << std::endl;
			url_ip_cache.insert(std::map<std::string, std::string>::value_type(a_header.getQNAME().substr(0, a_header.getQNAME().size() - 1), ip));
			url_ttl_cache.insert(std::map<std::string, time_t>::value_type(a_header.getQNAME().substr(0, a_header.getQNAME().size() - 1), a_header.getTTL()));
			for(std::map<std::string,std::string>::iterator iter = url_ip_cache.begin(); iter != url_ip_cache.end(); iter++)
			    std::cout << iter->first << " " << iter->second << std::endl;
			for (std::map<std::string, time_t>::iterator iter = url_ttl_cache.begin();iter != url_ttl_cache.end(); iter++)
			    std::cout << iter->first << " " << iter->second << std::endl;
		}
		if (debug_level == 2)
		{
			for (std::uint8_t c : datagram)
				printf("%02x ", c);
			printf("\n");
			printf("QR: %d\nopcode: %d\nAA: %d\nTC: %d\nRD: %d\nRA: %d\nrcode: %d\nQDCOUNT: %d\nANCOUNT: %d\nNSCOUNT: %d\nARCOUNT: %d\n",
					t_header.getQR(), t_header.getOpcode(), t_header.getAA(), t_header.getTC(), t_header.getRD(), t_header.getRA(), 
					t_header.getRcode(), t_header.getQDCOUNT(), t_header.getANCOUNT(), t_header.getNSCOUNT(), t_header.getARCOUNT());
			if (t_header.getANCOUNT() >= 1)
			{
				printf("QNAME: %s\nQTYPE: %d\nQCLASS: %d\nRNAME: %s\nRTYPE: %d\nRCLASS: %d\nTTL: %u\nRSLENGTH: %d\n",
				a_header.getQNAME().c_str(), a_header.getQTYPE(), a_header.getQCLASS(), a_header.getQNAME().c_str(), a_header.getRTYPE(),
				a_header.getRCLASS(), a_header.getTTL(), a_header.getRSLENGTH());
				if (a_header.getRTYPE() == 0x01)
					printf("resource: %s\n", QHostAddress(*((std::int32_t *)(rs_copy))).toString().toStdString().c_str());
				else if (a_header.getRTYPE() == 0x05)
					printf("resource: %s\n", get_cname(std::string(a_header.getResource()), a_header.getRSLENGTH(), datar).c_str());
				else if (a_header.getRTYPE() == 0x1c)
					printf("resource: %s\n", QHostAddress(*((Q_IPV6ADDR *)(a_header.getResource()))).toString().toStdString().c_str());
			}
		}

	}
	std::uint16_t ID = static_cast<std::uint16_t>(static_cast<std::uint16_t>(datar[0]) << 8 | datar[1]);
	std::uint16_t o_ID = std::get<0>(s_map[ID]);
	addr = std::get<1>(s_map[ID]);
	port = std::get<2>(s_map[ID]);
	datar[0] = static_cast<std::uint8_t>(o_ID >> 8);
	datar[1] = static_cast<std::uint8_t>(o_ID);
	QByteArray t_a;

	for (std::int32_t i = 0; i < datagram.size(); i++)
		t_a.append(static_cast<char>(datar[i]));
	if (cli_socket->writeDatagram(t_a, addr, port))
		printf("success\n");
	else
		printf("send fail\n");
	printf("ID: 0x%04x -> 0x%04x\nsend to %s:%d\n============================================\n", ID, o_ID, addr.toString().toStdString().c_str(), port);
	delete[] datar;
}


std::string DNSRelay::get_cname(std::string resource, quint16 _size, std::uint8_t *data)
{
	std::string t_s;
	std::int32_t cnt = 0;
	while (cnt < _size - 1)
	{
		if (((resource[cnt] & 0xc0) >> 6) == 0x03)
		{
			std::uint16_t us = static_cast<std::uint16_t>(static_cast<std::uint16_t>(resource[cnt]) << 8 | resource[cnt + 1]);
			resource = (char *)(data + (us & 0x3fff));
			_size = resource.size(); 
			cnt = 0;
		}
		std::int32_t clen = resource[cnt];
		t_s += resource.substr(cnt + 1, clen);
		t_s += ".";
		cnt = cnt + clen + 1;
	}
	return t_s;
}

void DNSRelay::handle_packet(const QByteArray &t_array, const QHostAddress &addr, const quint16 &port)
{
    std::uint8_t *data;
    data = new std::uint8_t[t_array.size() + 10];
    std::int32_t it = 0;
    for (auto &c : t_array)
        data[it++] = static_cast<std::uint8_t>(c);
    DNSHeader t_header(data);

    QueryHeader q_header(t_header, data);
	printf("Time %llds\nID: 0x%04x\n", time(NULL) - time_init, t_header.getID());
	if (debug_level == 1)
		printf("QNAME: %s\n", q_header.QNAME.c_str());
    //if (t_header.getQR() == 0 && t_header.getQDCOUNT() == 1)
    //{
	if (debug_level == 2)
		printf("QR: %d\nopcode: %d\nAA: %d\nTC: %d\nRD: %d\nRA: %d\nrcode: %d\nQDCOUNT: %d\nANCOUNT: %d\nNSCOUNT: %d\nARCOUNT: %d\nQNAME: %s\nQTYPE: %d\nQCLASS: %d\n", 
				t_header.getQR(), t_header.getOpcode(), t_header.getAA(), t_header.getTC(),t_header.getRD(), t_header.getRA(), t_header.getRcode(), t_header.getQDCOUNT(),
				t_header.getANCOUNT(), t_header.getNSCOUNT(), t_header.getARCOUNT(), q_header.QNAME.c_str(), q_header.QTYPE, q_header.QCLASS);
    //}
    if(!get_ans_from_cache(q_header.QNAME, t_array, addr, port))
    {
		QByteArray t_a;
        /* ID transfrom */
        data[0] = static_cast<std::uint8_t>(n_count >> 8);
        data[1] = static_cast<std::uint8_t>(n_count);
        /* insert into ID table */
        s_map[n_count++] = std::tuple<std::uint16_t, QHostAddress, quint16>(t_header.getID(), addr, port);
        for (std::int32_t i = 0; i < t_array.size(); i++)
            t_a.append(static_cast<char>(data[i]));
		if (sender->writeDatagram(t_a, DNS_server, 53))
			printf("success\n");
		else
			printf("send fail\n");
		printf("ID: 0x%04x -> 0x%04x\nsend to %s:%d\n============================================\n", t_header.getID(), n_count - 1, DNS_server.toString().toStdString().c_str(), 53);
		clock_c->alloc_timer(n_count);
    }
    delete[] data;
}

void DNSRelay::change_ttl()
{
	for (auto iter = url_ttl_cache.begin(); iter != url_ttl_cache.end();)
	{
		url_ttl_cache[iter->first]--;
		if (iter->second == 0)
		{
			std::cout << "delete a timeout item in cache : " << iter->first << std::endl;
			auto tmp_iter = url_ip_cache.find(iter->first);
			url_ttl_cache.erase(iter++);
			url_ip_cache.erase(tmp_iter++);
		}
		else
			++iter;
	}
}
