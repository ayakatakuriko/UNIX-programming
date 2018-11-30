/**
   udpsrv.c
   @brief UDPでメッセージを受信して、UDPで同一メッセージを返信するプログラム。
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
#include <arpa/inet.h>
#include <netdb.h>

#define BUF_SIZE 512

int main() {
        int s; /* File descripter of socket*/
        int count; /* Byte size of sent data*/
        in_port_t myport, port;
        struct sockaddr_in myskt, skt;
        char sbuf[BUF_SIZE], buf[BUF_SIZE];
        socklen_t sktlen; /* Size of skt*/

        /* Set port*/
        myport = 12345;
        port = 12345;

        /* Generate socket*/
        if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
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

        while (1) {
                /* Receive data*/
                sktlen = sizeof skt;
                memset(buf, 0, sizeof(buf));
                if ((count = recvfrom(s, buf, sizeof buf, 0,
                                      (struct sockaddr *)&skt, &sktlen)) < 0) {
                        perror("recvfrom");
                        exit(1);
                }
                printf("Sent from %s: %s (%d byte)\n", inet_ntoa(skt.sin_addr)
                       , buf, strlen(buf));

                /* Send data*/
                if ((count = sendto(s, buf, sizeof buf, 0,
                                    (struct sockaddr *)&skt, sizeof skt)) < 0) {
                        perror("sendto");
                        exit(1);
                }
        }

        close(s);

        return 0;

}
