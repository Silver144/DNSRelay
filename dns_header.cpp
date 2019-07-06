#include "dns_header.h"

DNSHeader::DNSHeader(unsigned char *data)
{
    this->ID = static_cast<unsigned short>(static_cast<unsigned short>(data[0]) << 8 | data[1]);
    this->QR = static_cast<unsigned char>(((static_cast<unsigned short>(data[2]) << 8 | data[3]) & 0x8000) >> 15);
    this->opcode = static_cast<unsigned char>(((static_cast<unsigned short>(data[2]) << 8 | data[3]) & 0x7800) >> 11);
    this->AA = static_cast<unsigned char>(((static_cast<unsigned short>(data[2]) << 8 | data[3]) & 0x0400) >> 10);
    this->TC = static_cast<unsigned char>(((static_cast<unsigned short>(data[2]) << 8 | data[3]) & 0x0200) >> 9);
    this->RD = static_cast<unsigned char>(((static_cast<unsigned short>(data[2]) << 8 | data[3]) & 0x0100) >> 8);
    this->RA = static_cast<unsigned char>(((static_cast<unsigned short>(data[2]) << 8 | data[3]) & 0x0080) >> 7);
    this->zero = static_cast<unsigned char>(((static_cast<unsigned short>(data[2]) << 8 | data[3]) & 0x0070) >> 4);
    this->rcode = static_cast<unsigned char>((static_cast<unsigned short>(data[2]) << 8 | data[3]) & 0x000F);
    this->QDCOUNT = static_cast<unsigned short>(static_cast<unsigned short>(data[4]) << 8 | data[5]);
    this->ANCOUNT = static_cast<unsigned short>(static_cast<unsigned short>(data[6]) << 8 | data[7]);
    this->NSCOUNT = static_cast<unsigned short>(static_cast<unsigned short>(data[8]) << 8 | data[9]);
    this->ARCOUNT = static_cast<unsigned short>(static_cast<unsigned short>(data[10]) << 8 | data[11]);
}

unsigned short DNSHeader::getARCOUNT() const
{
    return ARCOUNT;
}

unsigned short DNSHeader::getNSCOUNT() const
{
    return NSCOUNT;
}

unsigned short DNSHeader::getANCOUNT() const
{
    return ANCOUNT;
}

unsigned short DNSHeader::getQDCOUNT() const
{
    return QDCOUNT;
}

unsigned char DNSHeader::getRcode() const
{
    return rcode;
}

unsigned char DNSHeader::getZero() const
{
    return zero;
}

unsigned char DNSHeader::getRA() const
{
    return RA;
}

unsigned char DNSHeader::getRD() const
{
    return RD;
}

unsigned char DNSHeader::getTC() const
{
    return TC;
}

unsigned char DNSHeader::getAA() const
{
    return AA;
}

unsigned char DNSHeader::getOpcode() const
{
    return opcode;
}

unsigned char DNSHeader::getQR() const
{
    return QR;
}

unsigned short DNSHeader::getID() const
{
    return ID;
}


QueryHeader::QueryHeader(DNSHeader header) :DNSHeader (header)
{

}

QueryHeader::QueryHeader(DNSHeader header, unsigned char *data) : DNSHeader (header)
{
    unsigned char *e_data = data + 12;
    std::string t_str = reinterpret_cast<char *>(e_data);
    int cnt = 0;
    while (e_data[cnt] != 0)
    {
        QNAME.append(t_str.substr(size_t(cnt + 1), e_data[cnt]) + ".");
        cnt = cnt + 1 + e_data[cnt];
    } cnt++;
    QTYPE = static_cast<unsigned short>(static_cast<unsigned short>(e_data[cnt]) << 8 | e_data[cnt + 1]);
    QCLASS = static_cast<unsigned short>(static_cast<unsigned short>(e_data[cnt + 2]) << 8 | e_data[cnt + 3]);
}

AnswerHeader::AnswerHeader(DNSHeader header) : DNSHeader (header)
{

}

AnswerHeader::AnswerHeader(DNSHeader header, unsigned char *data) : DNSHeader (header)
{
    unsigned char *e_data = data + 12;
    std::string t_str = reinterpret_cast<char*>(e_data);
    int cnt = 0;
    while (e_data[cnt] != 0)
    {
        QNAME.append(t_str.substr(size_t(cnt + 1), e_data[cnt]) + ".");
        cnt = cnt + 1 + e_data[cnt];
    } cnt++;
    QTYPE = static_cast<unsigned short>(static_cast<unsigned short>(e_data[cnt]) << 8 | e_data[cnt + 1]);
    QCLASS = static_cast<unsigned short>(static_cast<unsigned short>(e_data[cnt + 2]) << 8 | e_data[cnt + 3]);
    RNAME = static_cast<unsigned short>(static_cast<unsigned short>(e_data[cnt + 4]) << 8 | e_data[cnt + 5]);
    RTYPE = static_cast<unsigned short>(static_cast<unsigned short>(e_data[cnt + 6]) << 8 | e_data[cnt + 7]);
    RCLASS = static_cast<unsigned short>(static_cast<unsigned short>(e_data[cnt + 8]) << 8 | e_data[cnt + 9]);
    TTL = static_cast<unsigned int>(e_data[cnt + 10]) << 24 | static_cast<unsigned int>(e_data[cnt + 11]) << 16 | static_cast<unsigned int>(e_data[cnt + 12]) << 8 | e_data[cnt + 13];
    RSLENGTH = static_cast<unsigned short>(e_data[cnt + 14]) | e_data[cnt + 15];
    cnt = cnt + 16;
    resource = reinterpret_cast<char*>(e_data + cnt);
}
