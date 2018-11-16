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
#include <arpa/inet.h>
#include <netdb.h>

int main() {
        int s, count, datalen;
        in_port_t myport, port;
        struct sockaddr_in myskt, skt;
        char sbuf[512], buf[512], c;
        socklen_t sktlen;

        myport = 12345;
        port = 12345;

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


        sktlen = sizeof skt;
        memset(buf, 0, sizeof(buf));
        if ((count = recvfrom(s, buf, sizeof buf, 0,
                              (struct sockaddr *)&skt, &sktlen)) < 0) {
                perror("recvfrom");
                exit(1);
        }
        printf("%s: %s\n", inet_ntoa(skt.sin_addr), buf);

        if ((count = sendto(s, buf, datalen, 0,
                            (struct sockaddr *)&skt, sizeof skt)) < 0) {
                perror("sendto");
                exit(1);
        }

        close(sock);

        return 0;

}
