#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <dirent.h>
#include <unistd.h>
#include "constellate.h"

#define IFDIRPTH "/sys/class/net"

struct sockaddr_in **enum_local_addresses(void)
{
	struct sockaddr_in **lan_addresses, **dummy;
	struct ifreq req;
        struct dirent *ifdirent;
        DIR *ifdir;
        char **ifnames, **temp;
        int sock, len, pos, lim;
        size_t siz;

        pos = 0, lim = 2;
	if ((lan_addresses = malloc(sizeof(struct sockaddr *) * lim)) == NULL) {
		perror("malloc err");
		return false;
	}
        if ((ifnames = malloc(sizeof(char *) * lim)) == NULL) {
                perror("malloc err");
                return false;
        }
        if ((ifdir = opendir(IFDIRPTH)) == NULL) {
                perror("opendir err");
                return false;
        }
        while ((ifdirent = readdir(ifdir))) {
                if (ifdirent -> d_type == DT_DIR)
                        continue;
                if (pos == lim) {
                        if ((temp = realloc(ifnames, sizeof(char *) * (lim *= 2))) == NULL) {
                                perror("realloc err");
                                return false;
                        }
                        ifnames = temp;
                }
                siz = strlen(ifdirent -> d_name);
                if ((*(ifnames + pos) = malloc(sizeof(char) * siz)) == NULL) {
                        perror("malloc err");
                        return false;
                }
                for (int i = 0; i < siz; i++)
                        *(*(ifnames + pos) + i) = ifdirent -> d_name[i];
                *(*(ifnames + pos++) + siz) = '\0';
        }
	len = pos;
	pos = 0;
        if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
                perror("sock err");
                return false;
        }
        req.ifr_addr.sa_family = AF_INET;
        for (int i = 0, lim = 2; i < len; i++) {
		if (pos == lim) {
			if ((dummy = realloc(lan_addresses, sizeof(struct sockaddr_in *) * (lim *= 2))) == NULL) {
				perror("realloc err");
				return false;
			}
			lan_addresses = dummy;
		}
                if (strncpy(req.ifr_name, *(ifnames + i), sizeof(req.ifr_name)) == NULL) {
                        perror("strncpy err");
                        return false;
                }
		free(*(ifnames + i));
                if (ioctl(sock, SIOCGIFADDR, &req) == -1) {
                        if (errno == EADDRNOTAVAIL)
				continue;
			else {
                                perror("ioctl err");
                                return false;
                        }
                }
		*(lan_addresses + pos++) = (struct sockaddr_in *) &req.ifr_addr;
        }
	*(lan_addresses + pos) = NULL;
        close(sock);
	free(ifnames);
	return lan_addresses;
}
