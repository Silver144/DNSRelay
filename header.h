#pragma once

#include <string>
#include "dns_header.h"

const std::uint8_t SAME_RNAME[2] = {0xc0, 0x0c};
const std::uint8_t TYPE_HOST_ADDRESS[2] = {0x00, 0x01};
const std::uint8_t CLASS_IN[2] = {0x00, 0x01};
const std::uint8_t TTL[4] = {0x00, 0x00, 0x02, 0x58};
const std::uint8_t DATA_LENGTH[2] = {0x00, 0x04};


/*
    DNS HEADER
    +==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+
    |                      ID                       |
    +==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+
    |QR|   Opcode  |AA|TC|RD|RA|    Z   |   RCODE   |
    +==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+
    |                    QDCOUNT                    |
    +==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+
    |                    ANCOUNT                    |
    +==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+
    |                    NSCOUNT                    |
    +==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+
    |                    ARCOUNT                    |
    +==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+

    DNS QUESTION

    +==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+
    |                                               |
    /                    QNAME                      /
    /                                               /
    +==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+
    |                    QTYPE                      |
    +==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+
    |                    QCLASS                     |
    +==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+
*/
//class DNSHeader
//{
//public:
//    std::uint16_t ID;        // Identification
//    std::uint8_t QR;         // 1: Answer | 0: Query
//    std::uint8_t opcode;     // Type
//    std::uint8_t AA;         // Authoritative answer
//    std::uint8_t TC;         // Truncated
//    std::uint8_t RD;         // Recursion desired
//    std::uint8_t RA;         // Recursion Available
//    std::uint8_t zero;       // 0
//    std::uint8_t rcode;      // Return error
//    std::uint16_t QDCOUNT;    // Number of entries in the question section
//    std::uint16_t ANCOUNT;    // Number of RRs in the answer section
//    std::uint16_t NSCOUNT;    // Number of name server RRs in authority records section
//    std::uint16_t ARCOUNT;    // Number of RRs in additional records section
//};


/*
    Resource Record Format (RFC1035 4.1.3)

    +==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+
    |                                               |
    /                    QNAME                      /
    /                                               /
    +==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+
    |                    QTYPE                      |
    +==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+
    |                    QCLASS                     |
    +==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+
    |                     TTL                       |
    |                                               |
    +==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+
    |                   RELENGTH                    |
    +==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+
    /                    RDATA                      /
    /                                               /
    +==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+==+
*/
//class AnswerHeader : public DNSHeader
//{
//public:
//    std::string QNAME;
//    std::uint16_t QTYPE;
//    std::uint16_t QCLASS;
//    std::string RNAME;
//    std::uint16_t RTYPE;
//    std::uint16_t RCLASS;
//    unsigned std::int32_t TTL;
//    std::uint16_t RSLENGTH;
//    std::string resource;
//};

