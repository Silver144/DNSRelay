#include "dns_header.h"

DNSHeader::DNSHeader(std::uint8_t *data)
{
    this->ID = static_cast<std::uint16_t>(static_cast<std::uint16_t>(data[0]) << 8 | data[1]);
    this->QR = static_cast<std::uint8_t>(((static_cast<std::uint16_t>(data[2]) << 8 | data[3]) & 0x8000) >> 15);
    this->opcode = static_cast<std::uint8_t>(((static_cast<std::uint16_t>(data[2]) << 8 | data[3]) & 0x7800) >> 11);
    this->AA = static_cast<std::uint8_t>(((static_cast<std::uint16_t>(data[2]) << 8 | data[3]) & 0x0400) >> 10);
    this->TC = static_cast<std::uint8_t>(((static_cast<std::uint16_t>(data[2]) << 8 | data[3]) & 0x0200) >> 9);
    this->RD = static_cast<std::uint8_t>(((static_cast<std::uint16_t>(data[2]) << 8 | data[3]) & 0x0100) >> 8);
    this->RA = static_cast<std::uint8_t>(((static_cast<std::uint16_t>(data[2]) << 8 | data[3]) & 0x0080) >> 7);
    this->zero = static_cast<std::uint8_t>(((static_cast<std::uint16_t>(data[2]) << 8 | data[3]) & 0x0070) >> 4);
    this->rcode = static_cast<std::uint8_t>((static_cast<std::uint16_t>(data[2]) << 8 | data[3]) & 0x000F);
    this->QDCOUNT = static_cast<std::uint16_t>(static_cast<std::uint16_t>(data[4]) << 8 | data[5]);
    this->ANCOUNT = static_cast<std::uint16_t>(static_cast<std::uint16_t>(data[6]) << 8 | data[7]);
    this->NSCOUNT = static_cast<std::uint16_t>(static_cast<std::uint16_t>(data[8]) << 8 | data[9]);
    this->ARCOUNT = static_cast<std::uint16_t>(static_cast<std::uint16_t>(data[10]) << 8 | data[11]);
}

std::uint16_t DNSHeader::getARCOUNT() const
{
    return ARCOUNT;
}

std::uint16_t DNSHeader::getNSCOUNT() const
{
    return NSCOUNT;
}

std::uint16_t DNSHeader::getANCOUNT() const
{
    return ANCOUNT;
}

std::uint16_t DNSHeader::getQDCOUNT() const
{
    return QDCOUNT;
}

std::uint8_t DNSHeader::getRcode() const
{
    return rcode;
}

std::uint8_t DNSHeader::getZero() const
{
    return zero;
}

std::uint8_t DNSHeader::getRA() const
{
    return RA;
}

std::uint8_t DNSHeader::getRD() const
{
    return RD;
}

std::uint8_t DNSHeader::getTC() const
{
    return TC;
}

std::uint8_t DNSHeader::getAA() const
{
    return AA;
}

std::uint8_t DNSHeader::getOpcode() const
{
    return opcode;
}

std::uint8_t DNSHeader::getQR() const
{
    return QR;
}

std::uint16_t DNSHeader::getID() const
{
    return ID;
}


QueryHeader::QueryHeader(DNSHeader header) :DNSHeader (header)
{

}

QueryHeader::QueryHeader(DNSHeader header, std::uint8_t *data) : DNSHeader (header)
{
    std::uint8_t *e_data = data + 12;
    std::string t_str = reinterpret_cast<char *>(e_data);
    std::int32_t cnt = 0;
    while (e_data[cnt] != 0)
    {
        QNAME.append(t_str.substr(size_t(cnt + 1), e_data[cnt]) + ".");
        cnt = cnt + 1 + e_data[cnt];
    } cnt++;
    QTYPE = static_cast<std::uint16_t>(static_cast<std::uint16_t>(e_data[cnt]) << 8 | e_data[cnt + 1]);
    QCLASS = static_cast<std::uint16_t>(static_cast<std::uint16_t>(e_data[cnt + 2]) << 8 | e_data[cnt + 3]);
}

AnswerHeader::AnswerHeader(DNSHeader header) : DNSHeader (header)
{

}

AnswerHeader::AnswerHeader(DNSHeader header, std::uint8_t *data) : DNSHeader (header)
{
    std::uint8_t *e_data = data + 12;
    std::string t_str = reinterpret_cast<char*>(e_data);
    std::int32_t cnt = 0;
    while (e_data[cnt] != 0)
    {
        QNAME.append(t_str.substr(size_t(cnt + 1), e_data[cnt]) + ".");
        cnt = cnt + 1 + e_data[cnt];
    } cnt++;
    QTYPE = static_cast<std::uint16_t>(static_cast<std::uint16_t>(e_data[cnt]) << 8 | e_data[cnt + 1]);
    QCLASS = static_cast<std::uint16_t>(static_cast<std::uint16_t>(e_data[cnt + 2]) << 8 | e_data[cnt + 3]);
    RNAME = static_cast<std::uint16_t>(static_cast<std::uint16_t>(e_data[cnt + 4]) << 8 | e_data[cnt + 5]);
    RTYPE = static_cast<std::uint16_t>(static_cast<std::uint16_t>(e_data[cnt + 6]) << 8 | e_data[cnt + 7]);
    RCLASS = static_cast<std::uint16_t>(static_cast<std::uint16_t>(e_data[cnt + 8]) << 8 | e_data[cnt + 9]);
    TTL = static_cast<std::uint32_t>(e_data[cnt + 10]) << 24 | static_cast<std::uint32_t>(e_data[cnt + 11]) << 16 | static_cast<std::uint32_t>(e_data[cnt + 12]) << 8 | e_data[cnt + 13];
    RSLENGTH = static_cast<std::uint16_t>(e_data[cnt + 14]) | e_data[cnt + 15];
    cnt = cnt + 16;
    resource = reinterpret_cast<char*>(e_data + cnt);
}
