/**
 * @brief シンプルなサーバ
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int main()
{
        int sock;
        struct sockaddr_in myskt;
        struct sockaddr_in skt;

        char buf[2048];
        socklen_t sktlen;

        sock = socket(AF_INET, SOCK_DGRAM, 0);

        myskt.sin_family = AF_INET;
        myskt.sin_port = htons(12345);
        myskt.sin_addr.s_addr = INADDR_ANY;

        bind(sock, (struct sockaddr *)&myskt, sizeof(myskt));

        sktlen = sizeof skt;
        memset(buf, 0, sizeof(buf));
        recvfrom(sock, buf, sizeof(buf), 0,
                 (struct sockaddr *)&skt, &sktlen);

        printf("%s: %s\n", inet_ntoa(skt.sin_addr), buf);

        close(sock);

        return 0;
}
