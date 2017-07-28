#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "flex_id.h"


#define TIME_PORT 5010

int main() {
    int sock, sock2;
    struct sockaddr_in server, client;
    int len;
    char buf[256];
    time_t today;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        exit(1);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(TIME_PORT);

    if (bind(sock, (struct sockaddr *)&server, sizeof(server)))
        exit(1);

    if (listen(sock, 5))
        exit(1);

    while(1) {
        if ((sock2 = accept(sock, (struct sockaddr *)&client, (socklen_t *)&len)) == -1)
            exit(1);
//        time(&today);
//        strcpy(buf, ctime(&today));
//        send(sock2, buf, strlen(buf)+1, 0);

        Flexhdr *fhdr;
        flexidhdr_init(&fhdr, 5);
        fhdr->version = 1;
        fhdr->frag_off = 65535;
        uint8_t saddr[5] = "0123";
        uint8_t daddr[5] = "abcd";
        memcpy(fhdr->saddr, saddr, 5);
        memcpy(fhdr->daddr, daddr, 5);
        uint8_t *buffer;
        serialize(fhdr, &buffer);

        ssize_t sent;
        sent = send(sock2, buffer, fhdr->header_len, 0);
        printf("sent: %lu\n", sent);
        printf("hdr size: %lu\n", sizeof(Flexhdr));


        close(sock2);
    }

    return 0;

}
