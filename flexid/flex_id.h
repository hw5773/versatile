#include<stdint.h>
#include<stdlib.h>
#include<stdio.h>
#include <string.h>

#define SUCCESS 0
#define FAILURE 1
#define TRUE 1
#define FALSE 0

typedef struct flexidhdr {
    uint8_t version;
    uint8_t packet_type;
    uint8_t hash_type;
    uint8_t hash_len;
    uint16_t packet_len;
    uint16_t checksum;
    uint16_t packet_id;
    uint16_t frag_off;
    uint16_t header_len;
    uint8_t hop_limit;
    uint8_t reserved;
    uint8_t *saddr;
    uint8_t *daddr;
} Flexhdr;

int flexidhdr_init(Flexhdr **fhdr, int length);
int flexidhdr_free(Flexhdr *fhdr);
int serialize(Flexhdr *fhdr, uint8_t** buffer);
int deserialize(Flexhdr **fhdr, uint8_t* buffer);
int have_more_frag(Flexhdr *fhdr);

