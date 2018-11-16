/**
   udpcil.c
   @brief あるIPアドレスをもつPCにUDPでメッセージを送信して、UDPでメッセージを受信するプログラム。
   @author ayakatakuriko
   @date 2018/11/16
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
/*
   netinet/in.h: Defines the in_addr structure and the sockaddr_in structure.
   Contain definition of Internet protocol family
 */
#include <netinet/in.h>
/*
   arpa/inet.h: Makes available the type in_port_t, the type in_addr_t and the
   in_addr structure.
   Contain definition to handle Internet.
 */
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CLI_IP_ADDR "0.0.0.0" /* IP address to send data*/
#define BUF_SIZE 512

int main() {
        int s; /* File descripter of socket*/
        int count; /* Byte size of sent data*/
        int datalen; /* Length of sent data*/
        in_port_t myport, port;
        struct sockaddr_in myskt, skt;
        char sbuf[BUF_SIZE], buf[BUF_SIZE];
        char *c; /* Used to check input error*/
        socklen_t sktlen; /* Size of skt*/

        /*Set port*/
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

        /* Initialize skt address*/
        memset(&skt, 0, sizeof skt);
        skt.sin_family = AF_INET;
        skt.sin_port = htons(port);
        skt.sin_addr.s_addr = inet_addr(CLI_IP_ADDR);

        while (1) {
                /* Initialize sbuf.*/
                memset(sbuf, 0, sizeof(sbuf));

                /*Input data.*/
                fprintf(stderr, "When you finish input characters, put Enter\n");
                if ((c = fgets(sbuf, BUF_SIZE, stdin)) == NULL) {
                        perror("input");
                        exit(1);
                }

                datalen = strlen(sbuf) - 1;

                /* Send data*/
                if ((count = sendto(s, sbuf, datalen, 0,
                                    (struct sockaddr *)&skt, sizeof skt)) < 0) {
                        /* "count < 0" means error.*/
                        perror("sendto");
                        exit(1);
                }

                printf("Success to send %s (%d byte) to %s\n", sbuf, count,
                       CLI_IP_ADDR);

                /* Receive data*/
                sktlen = sizeof skt;
                memset(buf, 0, sizeof(buf));
                if ((count = recvfrom(s, buf, (sizeof(char)) * datalen, 0,
                                      (struct sockaddr *)&skt, &sktlen)) < 0) {
                        perror("recvfrom");
                        exit(1);
                }
                printf("Return from %s: %s (%d byte)\n", inet_ntoa(skt.sin_addr)
                       , buf, strlen(buf));
        }

        close(s);

        return 0;
}
