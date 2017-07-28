#include<sys/types.h>
#include<sys/socket.h>

#include<netinet/in.h>
#include<stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "flex_id.h"
#include <net/if_arp.h>

#define TIME_PORT 5010


int main() {
    int sock;
    struct sockaddr_in server;
//    char buf[256];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        exit(1);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(TIME_PORT);

    if (connect(sock, (struct sockaddr *) &server, sizeof(server))) {
        printf("Connect Failed\n");
        exit(1);
    }

    int rcv = 0, bytes = 0;
    uint8_t buffer[1000];

    while(1) {
        rcv = recv(sock, buffer + bytes, 1000, 0);
        bytes += rcv;
        if (rcv == 0)
            break;
        if (rcv == -1)
            exit(1);
    }

    Flexhdr *fhdr;
    deserialize(&fhdr, buffer);

    printf("Version : %d\n", fhdr->version);
    printf("frag_off : %d\n", fhdr->frag_off);
    printf("Saddr : %s\n", fhdr->saddr);
    printf("Daddr : %s\n", fhdr->daddr);
    printf("M : %d\n", have_more_frag(fhdr));
    close(sock);

    return 0;

}