#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "constellate.h"

int main(void)
{
	struct sockaddr_in host;
	char *addr;

	addr = "192.168.0.225";
	host.sin_family = AF_INET;
	host.sin_port = 0;
	if (inet_pton(AF_INET, addr, &(host.sin_addr)) != 1) {
		perror("inet_pton err");
		return -1;
	}
	while(send_ping(&host)) {
		if (!recv_ping(&host)) {
			fprintf(stderr, "Error bruh : %d\n", errno);
			return -1;
		}
		sleep(2);
	}
	return 0;
}
