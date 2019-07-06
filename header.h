#pragma once

#include <string>
#include "dns_header.h"

const unsigned char SAME_RNAME[2] = {0xc0, 0x0c};
const unsigned char TYPE_HOST_ADDRESS[2] = {0x00, 0x01};
const unsigned char CLASS_IN[2] = {0x00, 0x01};
const unsigned char TTL[4] = {0x00, 0x00, 0x02, 0x58};
const unsigned char DATA_LENGTH[2] = {0x00, 0x04};


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
//    unsigned short ID;        // Identification
//    unsigned char QR;         // 1: Answer | 0: Query
//    unsigned char opcode;     // Type
//    unsigned char AA;         // Authoritative answer
//    unsigned char TC;         // Truncated
//    unsigned char RD;         // Recursion desired
//    unsigned char RA;         // Recursion Available
//    unsigned char zero;       // 0
//    unsigned char rcode;      // Return error
//    unsigned short QDCOUNT;    // Number of entries in the question section
//    unsigned short ANCOUNT;    // Number of RRs in the answer section
//    unsigned short NSCOUNT;    // Number of name server RRs in authority records section
//    unsigned short ARCOUNT;    // Number of RRs in additional records section
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
//    unsigned short QTYPE;
//    unsigned short QCLASS;
//    std::string RNAME;
//    unsigned short RTYPE;
//    unsigned short RCLASS;
//    unsigned int TTL;
//    unsigned short RSLENGTH;
//    std::string resource;
//};

