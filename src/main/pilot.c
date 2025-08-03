#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "constellate.h"

int main(void)
{
	struct sockaddr_in *current, **addrs;
	char addr[20];
	int n;

	addrs = enum_local_addresses();
	n = 0;
	while ((current = *(addrs + n++))) {
		printf("%s\n", inet_ntop(AF_INET, &(current -> sin_addr), addr, sizeof(addr)));
	}
}
