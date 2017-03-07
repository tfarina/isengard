#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

struct dnsheader {
  uint16_t id;
  uint16_t flags;
  uint16_t qdcount;
  uint16_t ancount;
  uint16_t nscount;
  uint16_t arcount;
};

struct dnsquestion {
  char *qname;
  size_t qnamelen;
  uint16_t qtype;
  uint16_t qclass;
};

#define FLAG_QR 0x8000U /* Query Response flag */
#define FLAG_AA 0x0400U /* Authoritative Answer - server flag */
#define FLAG_TC 0x0200U /* TrunCated - server flag */
#define FLAG_RD 0x0100U /* Recursion Desired - query flag */
#define FLAG_RA 0x0080U /* Recursion Available - server flag */
#define FLAG_AD 0x0020U /* Authenticated Data - server flag */
#define FLAG_CD 0x0010U /* Checking Disabled - query flag */


/* Resource Record definitions. */

/*
 * Resource record class codes.
 *
 * http://www.iana.org/assignments/dns-parameters/dns-parameters.xhtml
 */
enum dns_rr_class {
  DNS_RR_CLASS_IN = 1, /* Internet */
  DNS_RR_CLASS_CH = 3, /* Chaos */
  DNS_RR_CLASS_NONE = 254, /* QCLASS NONE */
  DNS_RR_CLASS_ANY = 255 /* QCLASS * (ANY) */
};

/*
 * Resource record types.
 *
 * http://www.iana.org/assignments/dns-parameters/dns-parameters.xhtml
 */
enum dns_rr_type {
  DNS_RR_TYPE_A = 1, /* An IPv4 host address. */
};

/*
 * RFC 1035, section 4.2.1: Messages carried by UDP are restricted to 512
 * bytes (not counting the IP nor UDP headers).
 */
#define MAX_UDP_SIZE 512
#define MAX_DOMAINLEN 255
#define DNS_DNAME_MAXLABELLEN 63 /* 2^6 - 1 */
#define DNS_DEFAULT_PORT "53"

#define OPCODE_MASK 0x7800U
#define OPCODE_SHIFT 11

#define RCODE_MASK 0x000fU

/* DNS operation codes. */
enum dns_opcode {
  DNS_OPCODE_QUERY = 0, /* Standard query. */
  DNS_OPCODE_IQUERY = 1, /* Inverse query. */
  DNS_OPCODE_STATUS = 2, /* Server status request */
  DNS_OPCODE_NOTIFY = 4, /* Notify message. */
  DNS_OPCODE_UPDATE = 5 /* Dynamic update. */
};

 /* DNS reply codes. */
enum dns_rcode {
  DNS_RCODE_NOERROR = 0, /* No error. */
  DNS_RCODE_FORMERR = 1, /* Format error. */
  DNS_RCODE_SERVFAIL = 2, /* Server failure. */
  DNS_RCODE_NXDOMAIN = 3, /* Non-existent domain. */
  DNS_RCODE_NOTIMPL = 4, /* Not implemented. */
  DNS_RCODE_REFUSED = 5, /* Refused. */
  DNS_RCODE_YXDOMAIN = 6, /* Name should not exist. */
  DNS_RCODE_YXRRSET = 7, /* RR set should not exist. */
  DNS_RCODE_NXRRSET = 8, /* RR set does not exist. */
  DNS_RCODE_NOTAUTH = 9, /* Server not authoritative / Query not authorized. */
  DNS_RCODE_NOTZONE = 10 /* Name is not inside zone. */
};

/* A general purpose lookup table. */
struct lookup_table {
  int id;
  const char *name;
};

/* Looks up the given id in the lookup table. */
static const struct lookup_table *lookup_by_id(const struct lookup_table *table, int id) {
  while (table->name != NULL) {
    if (table->id == id) {
      return table;
    }
    table++;
  }
  return NULL;
}

static const struct lookup_table rcodes[] = {
  { DNS_RCODE_NOERROR, "NOERROR" },
  { DNS_RCODE_FORMERR, "FORMERR" },
  { DNS_RCODE_SERVFAIL, "SERVFAIL" },
  { DNS_RCODE_NXDOMAIN, "NXDOMAIN" },
  { DNS_RCODE_NOTIMPL, "NOTIMPL" },
  { DNS_RCODE_REFUSED, "REFUSED" },
  { DNS_RCODE_YXDOMAIN, "YXDOMAIN" },
  { DNS_RCODE_YXRRSET, "YXRRSET" },
  { DNS_RCODE_NXRRSET, "NXRRSET" },
  { DNS_RCODE_NOTAUTH, "NOTAUTH" },
  { DNS_RCODE_NOTZONE, "NOTZONE" },
  { 0, NULL }
};

static const struct lookup_table opcodes[] = {
  { DNS_OPCODE_QUERY, "QUERY" },
  { DNS_OPCODE_IQUERY, "IQUERY" },
  { DNS_OPCODE_STATUS, "STATUS" },
  { DNS_OPCODE_NOTIFY, "NOTIFY" },
  { DNS_OPCODE_UPDATE, "UPDATE" },
  { 0, NULL }
};

static void write_uint16(void *dst, uint16_t data) {
  *(uint16_t*)dst = htons(data);
}

static uint16_t read_uint16(void *src) {
  return ntohs(*(uint16_t *)src);
}

#define DNS_OFFSET_ID 0
#define DNS_OFFSET_FLAGS 2
#define DNS_OFFSET_QDCOUNT 4
#define DNS_OFFSET_ANCOUNT 6
#define DNS_OFFSET_NSCOUNT 8
#define DNS_OFFSET_ARCOUNT 10

static uint16_t get_question_size(const struct dnsquestion *q) {
  if (q == NULL || q->qnamelen == 0) {
    return 0;
  }

  return q->qnamelen + sizeof(q->qtype) + sizeof(q->qclass);
}

int main(int argc, char **argv) {
  char dname[MAX_DOMAINLEN];
  struct dnsheader* header;
  struct dnsheader *response_header;
  struct dnsquestion* question;
  uint8_t *query_pkt;
  struct addrinfo hints, *addrlist;
  int rv;
  int sockfd;
  uint8_t reply_pkt[MAX_UDP_SIZE];
  struct sockaddr_storage from;
  socklen_t fromlen = sizeof(from);
  ssize_t rlen;

  if (argc != 2) {
    fprintf(stderr, "usage: %s domain\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  strcpy(dname, argv[1]);

  if (strlen(dname) > MAX_DOMAINLEN) {
    exit(EXIT_FAILURE);
  }

  header = malloc(sizeof(*header));
  memset(header, 0, sizeof(*header));

  unsigned int rand_seed;
  struct timeval tv;
  gettimeofday(&tv, NULL);
  rand_seed = tv.tv_sec ^ tv.tv_usec;
  srandom(rand_seed);

  header->id = random();
  header->flags = FLAG_RD;
  header->qdcount = 1;
  header->ancount = 0;
  header->nscount = 0;
  header->arcount = 0;

  size_t dname_len = strlen(dname);
  size_t alloc_size = 0;
  if (dname[0] == '.') {
    alloc_size = 1;
  } else if (dname[dname_len - 1] != '.') {
    alloc_size = 1 + dname_len + 1;
  } else {
    alloc_size = 1+ dname_len;
  }

  if (alloc_size > MAX_DOMAINLEN) {
    alloc_size = MAX_DOMAINLEN;
  }

  question = malloc(sizeof(*question) + alloc_size);
  memset(question, 0, sizeof(*question));

  question->qnamelen = alloc_size;
  question->qname = malloc(question->qnamelen);

  // dns_domain_fromdot
  char *label = NULL;
  char *dnamedup = strdup(dname);
  int len, total = 0;
  while ((label = strsep(&dnamedup, ".")) != NULL) {
    len = strlen(label);
    if (len > DNS_DNAME_MAXLABELLEN) {
      fprintf(stderr, "error: label is too long");
      free(dnamedup);
      return -1;
    }
    question->qname[total++] = len;
    strcat(&question->qname[total], label);
    total += len;
  }
  free(dnamedup);

  question->qtype = DNS_RR_TYPE_A;
  question->qclass = DNS_RR_CLASS_IN;

  // Prepare the packet (header + question) with the query that will be sent to
  // the DNS server.
  size_t query_pktlen = sizeof(*header) + get_question_size(question);

  query_pkt = malloc(query_pktlen);

  int offset = 0;

  // HEADER
  write_uint16(query_pkt + DNS_OFFSET_ID, header->id);
  write_uint16(query_pkt + DNS_OFFSET_FLAGS, header->flags);
  write_uint16(query_pkt + DNS_OFFSET_QDCOUNT, header->qdcount);
  write_uint16(query_pkt + DNS_OFFSET_ANCOUNT, header->ancount);
  write_uint16(query_pkt + DNS_OFFSET_NSCOUNT, header->nscount);
  write_uint16(query_pkt + DNS_OFFSET_ARCOUNT, header->arcount);

  offset += sizeof(*header);

  // QUESTION: QNAME + QTYPE + QCLASS

  // QNAME
  memcpy(query_pkt + offset, question->qname, question->qnamelen);
  offset += question->qnamelen;

  // QTYPE
  write_uint16(query_pkt + offset, question->qtype);
  offset += sizeof(question->qtype);

  // QCLASS
  write_uint16(query_pkt + offset, question->qclass);

  // Prepare the UDP socket that will be used to send the query to the DNS
  // server.
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;

  if ((rv = getaddrinfo("8.8.8.8", DNS_DEFAULT_PORT, &hints, &addrlist)) != 0) {
    fprintf(stderr, "getaddrinfo failed: %s\n", gai_strerror(rv));
    exit(EXIT_FAILURE);
  }

  if (addrlist == NULL) {
    fprintf(stderr, "no addrlist");
    exit(EXIT_FAILURE);
  }

  if ((sockfd = socket(addrlist->ai_family, addrlist->ai_socktype, 0)) == -1) {
    fprintf(stderr, "socket creation failed: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  // Send the query.
  if (sendto(sockfd, query_pkt, query_pktlen, 0, addrlist->ai_addr,
             addrlist->ai_addrlen) == -1) {
    fprintf(stderr, "sendto failed\n");
    exit(EXIT_FAILURE);
  }

  // Receive the response.
  if ((rlen = recvfrom(sockfd, reply_pkt, sizeof(reply_pkt), 0,
                       (struct sockaddr *)&from, &fromlen)) == -1) {
    fprintf(stderr, "recvfrom failed\n");
    exit(EXIT_FAILURE);
  }

  freeaddrinfo(addrlist);

  // Parse reply to the dnsheader structure.
  response_header = malloc(sizeof(struct dnsheader));

  response_header->id = read_uint16(reply_pkt + DNS_OFFSET_ID);
  response_header->flags = read_uint16(reply_pkt + DNS_OFFSET_FLAGS);
  response_header->qdcount = read_uint16(reply_pkt + DNS_OFFSET_QDCOUNT);
  response_header->ancount = read_uint16(reply_pkt + DNS_OFFSET_ANCOUNT);
  response_header->nscount = read_uint16(reply_pkt + DNS_OFFSET_NSCOUNT);
  response_header->arcount = read_uint16(reply_pkt + DNS_OFFSET_ARCOUNT);

  uint16_t opcode_id = (response_header->flags & OPCODE_MASK) >> OPCODE_SHIFT;
  const struct lookup_table *opcode = lookup_by_id(opcodes, opcode_id);

  uint16_t rcode_id = response_header->flags & RCODE_MASK;
  const struct lookup_table *rcode = lookup_by_id(rcodes, rcode_id);

  printf(";; ->>HEADER<<- ");

  if (opcode) {
    printf("opcode: %s, ", opcode->name);
  } else {
    printf("opcode: ?? (%u), ", opcode_id);
  }

  if (rcode) {
    printf("rcode: %s, ", rcode->name);
  } else {
    printf("rcode: ?? (%u), ", rcode_id);
  }

  printf("id: %d\n", response_header->id);
  printf(";; flags:");
  if ((response_header->flags & FLAG_QR) != 0) {
    printf(" qr");
  }
  if ((response_header->flags & FLAG_AA) != 0) {
    printf(" aa");
  }
  if ((response_header->flags & FLAG_TC) != 0) {
    printf(" tc");
  }
  if ((response_header->flags & FLAG_RD) != 0) {
    printf(" rd");
  }
  if ((response_header->flags & FLAG_CD) != 0) {
    printf(" cd");
  }
  if ((response_header->flags & FLAG_RA) != 0) {
    printf(" ra");
  }
  if ((response_header->flags & FLAG_AD) != 0) {
    printf(" ad");
  }

  printf("; ");
  printf("QUERY: %u, ", response_header->qdcount);
  printf("ANSWER: %u, ", response_header->ancount);
  printf("AUTHORITY: %u, ", response_header->nscount);
  printf("ADDITIONAL: %u\n", response_header->arcount);

  printf("\n");

  printf(";; MSG SIZE rcvd: %zd\n", rlen);

  close(sockfd);

  return 0;
}
