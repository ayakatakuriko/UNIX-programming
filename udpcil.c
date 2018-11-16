/**
udpcil.c
@brief PCにUDPでメッセージを送信して、UDPでメッセージを受信するプログラム。
@author ayakatakuriko
@date 2018/11/16
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

int main() {
	int s, count, datalen;
	in_port_t myport, port;
	struct sockaddr_in myskt, skt;
	unsigned long ipaddr;
	char sbuf[512], c;
	int position = 0;

	myport = 49151;
	port = 49152;
	ipaddr = 13111310853ul;

	/*Input data.*/
	while ((c = getchar()) != EOF) {
		if (c == '\n') continue;
		else {
			sbuf[position++] = c;
		}
	}

	datalen = position;

	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
			perror("socket");
			exit(1);
	}

		memset(&myskt, 0, sizeof myskt);
		myskt.sin_family = AF_INET;
		myskt.sin_port = htons(myport);
		myskt.sin_addr.s_addr = htonl(INADDR_ANY);
		if (bind(s, (struct sockaddr *)&myskt, sizeof myskt) < 0) {
			perror("bind");
			exit(1);
		}

		memset(&skt, 0, sizeof skt);
		skt.sin_family = AF_INET;
		skt.sin_port = htons(port);
		skt.sin_addr.s_addr = htonl(ipaddr);
		if ((count = sendto(s, sbuf, datalen, 0,
							(struct sockaddr *)&skt, sizeof skt)) < 0) {
				perror("sendto");
				exit(1);
			}
}