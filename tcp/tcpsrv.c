/**
 * @brieg Server communicating over TCP.
 * @author ayakatakuriko
 * @date 2018/11/27
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#define BUF_SIZE 512
#define END "FIN"

int main() {
        int s, s2;
        int count;
        struct sockaddr_in myskt; // my socket address
        struct sockaddr_in skt; // client socket address
        socklen_t sktlen;
        in_port_t myport = 49152;
        char buf[BUF_SIZE];

        if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                perror("socket");
                exit(1);
        }

        /* Initialize myskt and allocate socket address*/
        memset(&myskt, 0, sizeof myskt);
        myskt.sin_family = AF_INET;
        myskt.sin_port = htons(myport);
        myskt.sin_addr.s_addr = INADDR_ANY;
		
        if (bind(s, (struct sockaddr *)&myskt, sizeof myskt) < 0) {
                perror("bind");
                exit(1);
        }

		/* Prepare for receiving data*/
        if (listen(s, 5) < 0) {
                perror("listen");
                exit(1);
        }

        while (1) {
                int flag = 0;
                sktlen = sizeof skt;
                if ((s2 = accept(s, (struct sockaddr*)&skt, &sktlen)) < 0) {
                        perror("accept");
                        exit(1);
                }
                fprintf(stderr, "---Connection established---\n");

                while (1) {
                        /* Receive data*/
                        memset(buf, 0, sizeof(buf));
                        if ((count = recv(s2, buf, sizeof buf, 0)) < 0) {
                                perror("recv");
                                exit(1);
                        }
                        printf("Sent from %s: %s (%d byte)\n", inet_ntoa(skt.sin_addr)
                               , buf, (int)strlen(buf));

                        /* Send data*/
                        if ((count = send(s2, buf, sizeof buf, 0)) < 0) {
                                perror("send");
                                exit(1);
                        }

						/* If flag = 0, terminate connection*/
                        flag = strncmp(buf, END, count);
                        if (!flag) {
                                fprintf(stderr, "Connection terminated...\n");
                                close(s2);
                                break;
                        }
                }
                close(s2);
        }
}
