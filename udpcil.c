/**
   udpcil.c
   @brief あるIPアドレスをもつPCにUDPでメッセージを送信して、UDPでメッセージを受信するプログラム。
   @author ayakatakuriko
   @date 2018/11/16
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CLI_IP_ADDR "131.113.108.53" // Client IP address

int main() {
        int s, count, datalen;
        in_port_t myport, port;
        struct sockaddr_in myskt, skt;
        char sbuf[512], buf[512], c;
        int position;
        socklen_t sktlen;

        myport = 12345;
        port = 12345;

        /*Input data.*/
        position = 0;
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
        myskt.sin_addr.s_addr = INADDR_ANY;
        if (bind(s, (struct sockaddr *)&myskt, sizeof myskt) < 0) {
                perror("bind");
                exit(1);
        }

        memset(&skt, 0, sizeof skt);
        skt.sin_family = AF_INET;
        skt.sin_port = htons(port);
        skt.sin_addr.s_addr = inet_addr(CLI_IP_ADDR);
        if ((count = sendto(s, sbuf, datalen, 0,
                            (struct sockaddr *)&skt, sizeof skt)) < 0) {
                perror("sendto");
                exit(1);
        }

        sktlen = sizeof skt;
        memset(buf, 0, sizeof(buf));
        if ((count = recvfrom(s, buf, sizeof buf, 0,
                              (struct sockaddr *)&skt, &sktlen)) < 0) {
                perror("recvfrom");
                exit(1);
        }
        printf("%s: %s\n", inet_ntoa(skt.sin_addr), buf);

        close(sock);

        return 0;
}
