#pragma once

#include <QUdpSocket>
#include <QFile>
#include <QTimer>
#include <iostream>
#include <fstream>
#include <map>
#include <tuple>
#include <QDebug>
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include "clock_ctl.h"

class DNSRelay : public QObject
{
    Q_OBJECT

public:

    explicit DNSRelay(std::int32_t argc, char *argv[]);

private:
    QUdpSocket *cli_socket;
    QUdpSocket *sender;
	
    std::map<std::uint16_t, std::tuple<std::uint16_t, QHostAddress, quint16>> s_map;
    quint16 n_count;
	time_t time_init;
    QTimer *timer;
	QTimer *write_back;
    /* 本地cache */
    std::map<std::string,std::string> url_ip_cache;
    /* 本地TTL */
    std::map<std::string, time_t> url_ttl_cache;
    /* 初始化cache */
    bool get_url_ip_table();
    /* cache detection */
    bool get_ans_from_cache(std::string, const QByteArray &t_array, const QHostAddress& addr, const quint16 &port);
    /* construct&send block segment */
    void build_blk_seg(const QByteArray &t_array, const QHostAddress& addr, const quint16 port);
    /* construct&send response segment */
    void build_res_seg(std::string,const QByteArray &t_array, const QHostAddress& addr, const quint16 port);
	void handle_param(std::int32_t argc, char *argv[]);
    void handle_packet(const QByteArray &t_array, const QHostAddress &addr, const quint16 &port);
	std::string get_cname(std::string resource, quint16 _size, std::uint8_t *data);
	std::int32_t debug_level = 0;
	std::string filename = "dnsrelay.txt";
	QHostAddress DNS_server = QHostAddress("202.106.0.20");
	clock_ctl *clock_c;

signals:
    void data_send_ready(QByteArray t_array);

private slots:
    void change_ttl();
    void data_read_ready();
    void data_recv_ready();
	void cache_back();
};

