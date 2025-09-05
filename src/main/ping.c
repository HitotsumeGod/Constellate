#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/ether.h>
#include <unistd.h>
#include "constellate.h"

#define ICMP_ID 4096
#define SEQ_MAX 25702
#define SEQ_MIN 2222

bool send_ping(struct sockaddr_in *host)
{
	struct icmphdr hdr;
	int sock;

	if (!host) {
		errno = BAD_ARGS_ERR;
		return false;
	}
	srand(time(NULL));
	//construct icmp echo header
	hdr.type = ICMP_ECHO;
	hdr.code = 0;
	hdr.checksum = 0;
	hdr.un.echo.id = ICMP_ID;
	hdr.un.echo.sequence = rand() % (SEQ_MAX + 1 - SEQ_MIN) + SEQ_MIN;
	hdr.checksum = compute_cksum(hdr, NULL, 0);
	//send through raw socket
	if ((sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1) {
		perror("socket err");
		return false;
	}
	if (sendto(sock, &hdr, sizeof(hdr), 0, (struct sockaddr *) host, sizeof(struct sockaddr)) == -1) {
		perror("sendto err");
		return false;
	}
	close(sock);
	return true;
}

bool recv_ping(struct sockaddr_in *host)
{
	struct icmphdr *hdr;
	struct sockaddr_in res;
	int sock, len;
	socklen_t sus;
	char buf[120], *data;

	if (!host) {
		errno = BAD_ARGS_ERR;
		return false;
	}
	sus = sizeof(res);
	if ((sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1) {
		perror("socket err");
		return false;
	}
	while (true) {
		if ((len = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *) &res, &sus)) == -1) {
			perror("recvfrom err");
			return false;
		}
		//ping from the correct host
		if (memcmp(&(host -> sin_addr), &(res.sin_addr), sizeof(struct in_addr)) == 0) {
			hdr = (struct icmphdr *) buf + sizeof(struct ethhdr) + sizeof(struct iphdr);
			data = buf + sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct icmphdr);
			break;
		}
	}
	close(sock);
	printf("ICMP Type : %u\n", hdr -> type);
	return true;
}

uint16_t compute_cksum(struct icmphdr hdr, uint16_t *payload, size_t plen)
{
	uint16_t cksum, carry;

	cksum = carry = 0;
	if ((cksum += hdr.type) > 0xFFFF)
		carry++;
	if ((cksum += hdr.un.echo.id) > 0xFFFF)
		carry++;
	if ((cksum += hdr.un.echo.sequence) > 0xFFFF)
		carry++;
	for (int i = 0; i < plen; i++)
		if ((cksum += payload[i]) > 0xFFFF)
			carry++;
	return ~(cksum + carry);
}
