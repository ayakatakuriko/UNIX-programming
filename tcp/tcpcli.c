/**
 * @brieg tcpsrv.c TCPで通信するクライアント。標準入力からの入力をサーバに送信する。
 * Ctrl-Dでコネクションを切る。
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
#define CLI_IP_ADDR "TODO: Write IP address like 0.0.0.0" /* IP address to send data*/
#define END "FIN"

int main() {
        int s;
        int count;
        int c;
        int flag = 0;
        struct sockaddr_in skt; // client socket address
        in_port_t port;
        char buf[BUF_SIZE], sbuf[BUF_SIZE];

        if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                perror("socket");
                exit(1);
        }

        port = 49152;

        /* Initialize skt address*/
        memset(&skt, 0, sizeof skt);
        skt.sin_family = AF_INET;
        skt.sin_port = htons(port);
        skt.sin_addr.s_addr = inet_addr(CLI_IP_ADDR);

        if (connect(s, (struct sockaddr *)&skt, sizeof skt) < 0) {
                perror("connect");
                exit(1);
        }

        while (1) {
                int position = 0;
                memset(buf, 0, sizeof(buf));
                memset(sbuf, 0, sizeof(sbuf));
                while ((c = getchar()) != '\n') {
                        if (c == EOF) {
                                strncpy(sbuf, "FIN\0", 4);
                                flag = 1;
                                break;
                        }
                        else sbuf[position++] = c;
                }
                printf("Start to send %s\n", sbuf);
                /* Send data*/
                if ((count = send(s, sbuf, sizeof sbuf, 0)) < 0) {
                        perror("send");
                        exit(1);
                }
                /* Receive data*/
                if ((count = recv(s, buf, sizeof buf, 0)) < 0) {
                        perror("recv");
                        exit(1);
                }
                printf("Return from %s: %s (%d byte)\n", inet_ntoa(skt.sin_addr)
                       , buf, (int)strlen(buf));
                if (flag) {
                        fprintf(stderr, "Conecction terminated...\n");
                        close(s);
                        return 0;
                }
        }

}
