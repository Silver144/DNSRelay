#ifndef DNS_HEADER_H
#define DNS_HEADER_H

#include <string>


class DNSHeader
{
public:
    explicit DNSHeader(std::uint8_t *data);
    std::uint16_t getID() const;
    std::uint8_t getQR() const;
    std::uint8_t getOpcode() const;
    std::uint8_t getAA() const;
    std::uint8_t getTC() const;
    std::uint8_t getRD() const;
    std::uint8_t getRA() const;
    std::uint8_t getZero() const;
    std::uint8_t getRcode() const;
    std::uint16_t getQDCOUNT() const;
    std::uint16_t getANCOUNT() const;
    std::uint16_t getNSCOUNT() const;
    std::uint16_t getARCOUNT() const;
private:
    std::uint16_t ID;        // Identification
    std::uint8_t QR;         // 1: Answer | 0: Query
    std::uint8_t opcode;     // Type
    std::uint8_t AA;         // Authoritative answer
    std::uint8_t TC;         // Truncated
    std::uint8_t RD;         // Recursion desired
    std::uint8_t RA;         // Recursion Available
    std::uint8_t zero;       // 0
    std::uint8_t rcode;      // Return error
    std::uint16_t QDCOUNT;    // Number of entries in the question section
    std::uint16_t ANCOUNT;    // Number of RRs in the answer section
    std::uint16_t NSCOUNT;    // Number of name server RRs in authority records section
    std::uint16_t ARCOUNT;    // Number of RRs in additional records section
};

class QueryHeader : public DNSHeader
{
public:
    explicit QueryHeader(DNSHeader header);
    explicit QueryHeader(DNSHeader header, std::uint8_t *data);
public:
    std::string QNAME;
    std::uint16_t QTYPE;
    std::uint16_t QCLASS;
};

class AnswerHeader : public DNSHeader
{
public:
    explicit AnswerHeader(DNSHeader header);
    explicit AnswerHeader(DNSHeader header, std::uint8_t *data);
	std::string getQNAME() { return QNAME; }
	std::uint16_t getQTYPE() { return QTYPE; }
	std::uint16_t getQCLASS() { return QCLASS; }
	std::uint16_t getRNAME() { return RNAME; }
	std::uint16_t getRTYPE() { return RTYPE; }
	std::uint16_t getRCLASS() { return RCLASS; }
	std::uint32_t getTTL() { return TTL; }
	std::uint16_t getRSLENGTH() { return RSLENGTH; }
	char *getResource() { return resource; }

private:
    std::string QNAME;
    std::uint16_t QTYPE;
    std::uint16_t QCLASS;
    std::uint16_t RNAME;
    std::uint16_t RTYPE;
    std::uint16_t RCLASS;
    std::uint32_t TTL;
    std::uint16_t RSLENGTH;
    char *resource;
};
#endif // DNS_HEADER_H
