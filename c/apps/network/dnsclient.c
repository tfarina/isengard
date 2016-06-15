#include <arpa/inet.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

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

static const uint16_t kFlagRD = 0x100;

/* Resource Record definitions. */

/* A host address type. */
#define RR_TYPE_A 1

/* The Internet class. */
#define RR_CLASS_IN 1

#define MAX_PACKETLEN 512
#define MAX_DOMAINLEN 255
#define DNS_PORT 53

static uint16_t read_uint16(void *src) {
  return ntohs(*(uint16_t *)src);
}

int main(int argc, char **argv) {
  struct dnsheader* header;
  struct dnsquestion* question;
  char hostname[MAX_DOMAINLEN];
  uint8_t *query_pkt;
  struct sockaddr_in to;
  socklen_t tolen = sizeof(to);
  int sockfd;
  uint8_t answer_pkt[MAX_PACKETLEN];
  struct sockaddr_storage from;
  socklen_t fromlen = sizeof(from);
  ssize_t rlen;

  if (argc != 2) {
    fprintf(stderr, "usage: %s domain\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  strcpy(hostname, argv[1]);

  if (strlen(hostname) > MAX_DOMAINLEN) {
    exit(EXIT_FAILURE);
  }

  header = malloc(sizeof(*header));
  memset(header, 0, sizeof(*header));

  header->id = rand();
  header->flags = htons(kFlagRD); //htons(1 << 8) htons(0x0100);
  header->qdcount = htons(1);

  question = malloc(sizeof(*question) + strlen(hostname) + 2);
  memset(question, 0, sizeof(*question));

  question->qnamelen = strlen(hostname) + 2;
  question->qname = malloc(question->qnamelen);

  // dns_domain_fromdot
  char *label = NULL;
  char *hostnamedup = strdup(hostname);
  int len, total = 0;
  while ((label = strsep(&hostnamedup, ".")) != NULL) {
    len = strlen(label);
    if (len > MAX_DOMAINLEN) {
      fprintf(stderr, "Label is too long");
      free(hostnamedup);
      return -1;
    }
    question->qname[total++] = len;
    strcat(&question->qname[total], label);
    total += len;
  }
  free(hostnamedup);

  question->qtype = htons(RR_TYPE_A);
  question->qclass = htons(RR_CLASS_IN);

  // Prepare the packet (header + question) with the query that will be sent to
  // the DNS server.
  size_t questionsize =
      question->qnamelen + sizeof(question->qtype) + sizeof(question->qclass);
  size_t query_pktlen = sizeof(*header) + questionsize;

  query_pkt = malloc(query_pktlen);

  int offset = 0;

  // HEADER
  memcpy(query_pkt + offset, header, sizeof(*header));
  offset += sizeof(*header);

  // QUESTION: QNAME + QTYPE + QCLASS

  // QNAME
  memcpy(query_pkt + offset, question->qname, question->qnamelen);
  offset += question->qnamelen;

  // QTYPE
  memcpy(query_pkt + offset, &question->qtype, sizeof(question->qtype));
  offset += sizeof(question->qtype);

  // QCLASS
  memcpy(query_pkt + offset, &question->qclass, sizeof(question->qclass));

  // Prepare the UDP socket that will be used to send the query to the DNS
  // server.
  memset(&to, 0, tolen);
  to.sin_family = AF_INET;
  to.sin_port = htons(DNS_PORT);
  inet_pton(AF_INET, "8.8.8.8", &to.sin_addr);

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    fprintf(stderr, "socket creation failed: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  // Send the query.
  if (sendto(sockfd, query_pkt, query_pktlen, 0, (struct sockaddr *)&to,
             tolen) == -1) {
    fprintf(stderr, "sendto failed\n");
    exit(EXIT_FAILURE);
  }

  // Receive the response.
  if ((rlen = recvfrom(sockfd, answer_pkt, sizeof(answer_pkt), 0,
                       (struct sockaddr *)&from, &fromlen)) == -1) {
    fprintf(stderr, "recvfrom failed\n");
    exit(EXIT_FAILURE);
  }

  // Parse the response.
  struct dnsheader *response_header;
  response_header = malloc(sizeof(struct dnsheader));

  response_header->id = read_uint16(answer_pkt);
  response_header->flags = read_uint16(answer_pkt + 2);
  response_header->qdcount = read_uint16(answer_pkt + 4);
  response_header->ancount = read_uint16(answer_pkt + 6);
  response_header->nscount = read_uint16(answer_pkt + 8);
  response_header->arcount = read_uint16(answer_pkt + 10);

  printf("QUERY: %d, ", response_header->qdcount);
  printf("ANSWER: %d, ", response_header->ancount);
  printf("AUTHORITY: %d, ", response_header->nscount);
  printf("ADDITIONAL: %d\n", response_header->arcount);

  printf(";; MSG SIZE rcvd: %zd\n", rlen);

  return 0;
}
