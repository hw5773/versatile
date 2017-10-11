#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <linux/ip.h>

int main()
{
    int on = 1;
    int raw_socket = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    printf("\nraw_socket : %d\n", raw_socket);
    unsigned char packet[22];

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(1234);
    address.sin_addr.s_addr = inet_addr("147.46.219.48");
    setsockopt(raw_socket, IPPROTO_IP, IP_HDRINCL, (char *)&on, sizeof(on));


    struct iphdr *ip_hdr;
    ip_hdr = (struct iphdr*) malloc(sizeof(struct iphdr));
    memset((char *)ip_hdr, 0, 20);
    ip_hdr->version = 4;
    ip_hdr->ihl = 5;
    ip_hdr->protocol = IPPROTO_RAW;
    ip_hdr->tot_len = 40;
    ip_hdr->id = htons(777);
    ip_hdr->ttl = 60;
    ip_hdr->check = 1;
    ip_hdr->saddr = inet_addr("111.111.111.111");
    ip_hdr->daddr = inet_addr("147.46.219.48");

    uint16_t flex_test = htons(2);

    memcpy(packet, ip_hdr, sizeof(struct iphdr));
    memcpy(packet + sizeof(struct iphdr), &flex_test, sizeof(uint16_t));

    int i=0;

    for (i=0; i<sizeof(packet); i++)
    {
        if (i % 10 == 0)
            printf("\n");
        printf("%02X ", packet[i]);
    }

    printf("\n%d\n", sendto(raw_socket, &packet, sizeof(packet), 0x0, (struct sockaddr*)&address, sizeof(address)));

}
