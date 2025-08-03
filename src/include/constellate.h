#ifndef __CONSTELLATE_H__
#define __CONSTELLATE_H__

#include <errno.h>
#include <netinet/in.h>

#define CONSTELLAWESOME

extern struct sockaddr_in **enum_local_addresses(void);
extern struct sockaddr_in *enum_global_address(void);
extern struct sockaddr_in *enum_stun_address(void);

#endif //__CONSTELLATE_H__
