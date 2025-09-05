#ifndef __CONSTELLATE_H__
#define __CONSTELLATE_H__

#include <stdbool.h>
#include <stdint.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include "commonerrors.h"

#define CONSTELLAWESOME

//addr.c
extern struct sockaddr_in **enum_local_addresses(void);
extern struct sockaddr_in *enum_global_address(void);
extern struct sockaddr_in *enum_stun_address(void);
//ping.c
extern bool send_ping(struct sockaddr_in *host);
extern bool send_ping_v6(struct sockaddr_in6 *host);
extern bool recv_ping(struct sockaddr_in *host);
extern bool recv_ping_v6(struct sockaddr_in6 *host);
uint16_t compute_cksum(struct icmphdr header, uint16_t *payload, size_t length);

#endif //__CONSTELLATE_H__
