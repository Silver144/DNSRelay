#ifndef DNS_HEADER_H
#define DNS_HEADER_H

#include <string>


class DNSHeader
{
public:
    explicit DNSHeader(unsigned char *data);
    unsigned short getID() const;
    unsigned char getQR() const;
    unsigned char getOpcode() const;
    unsigned char getAA() const;
    unsigned char getTC() const;
    unsigned char getRD() const;
    unsigned char getRA() const;
    unsigned char getZero() const;
    unsigned char getRcode() const;
    unsigned short getQDCOUNT() const;
    unsigned short getANCOUNT() const;
    unsigned short getNSCOUNT() const;
    unsigned short getARCOUNT() const;
private:
    unsigned short ID;        // Identification
    unsigned char QR;         // 1: Answer | 0: Query
    unsigned char opcode;     // Type
    unsigned char AA;         // Authoritative answer
    unsigned char TC;         // Truncated
    unsigned char RD;         // Recursion desired
    unsigned char RA;         // Recursion Available
    unsigned char zero;       // 0
    unsigned char rcode;      // Return error
    unsigned short QDCOUNT;    // Number of entries in the question section
    unsigned short ANCOUNT;    // Number of RRs in the answer section
    unsigned short NSCOUNT;    // Number of name server RRs in authority records section
    unsigned short ARCOUNT;    // Number of RRs in additional records section
};

class QueryHeader : public DNSHeader
{
public:
    explicit QueryHeader(DNSHeader header);
    explicit QueryHeader(DNSHeader header, unsigned char *data);
public:
    std::string QNAME;
    unsigned short QTYPE;
    unsigned short QCLASS;
};

class AnswerHeader : public DNSHeader
{
public:
    explicit AnswerHeader(DNSHeader header);
    explicit AnswerHeader(DNSHeader header, unsigned char *data);
	std::string getQNAME() { return QNAME; }
	unsigned short getQTYPE() { return QTYPE; }
	unsigned short getQCLASS() { return QCLASS; }
	unsigned short getRNAME() { return RNAME; }
	unsigned short getRTYPE() { return RTYPE; }
	unsigned short getRCLASS() { return RCLASS; }
	unsigned int getTTL() { return TTL; }
	unsigned short getRSLENGTH() { return RSLENGTH; }
	char *getResource() { return resource; }

private:
    std::string QNAME;
    unsigned short QTYPE;
    unsigned short QCLASS;
    unsigned short RNAME;
    unsigned short RTYPE;
    unsigned short RCLASS;
    unsigned int TTL;
    unsigned short RSLENGTH;
    char *resource;
};
#endif // DNS_HEADER_H
