#include "flex_id.h"

int flexidhdr_init(Flexhdr **fhdr, int len) {
    *fhdr = (Flexhdr *) malloc(sizeof(Flexhdr));
    (*fhdr)->hash_len = len;
    (*fhdr)->saddr = (uint8_t *) malloc(len);
    (*fhdr)->daddr = (uint8_t *) malloc(len);
    (*fhdr)->header_len = sizeof(Flexhdr) - 2 * sizeof(uint8_t*) + 2 * len;

    return SUCCESS;
}

int flexidhdr_free(Flexhdr *fhdr) {
    free(fhdr->saddr);
    free(fhdr->daddr);
    free(fhdr);

    return SUCCESS;
}

int have_more_frag(Flexhdr *fhdr) {
    return (fhdr->frag_off & 0x2000) >> 13;
}

int serialize(Flexhdr *fhdr, uint8_t** buffer) {
    (*buffer) = (uint8_t*) malloc(sizeof(fhdr->header_len));
    memcpy(*buffer, fhdr, sizeof(Flexhdr) - 2 * sizeof(uint8_t*));
    memcpy((*buffer) + sizeof(Flexhdr) - 2 * sizeof(uint8_t*), fhdr->saddr, fhdr->hash_len);
    memcpy((*buffer) + sizeof(Flexhdr) - 2 * sizeof(uint8_t*) + fhdr->hash_len, fhdr->daddr, fhdr->hash_len);

    return SUCCESS;
}

int deserialize(Flexhdr **fhdr, uint8_t* buffer) {
    *fhdr = (Flexhdr *) malloc(sizeof(Flexhdr));
    memcpy(*fhdr, buffer, sizeof(Flexhdr) - 2 * sizeof(uint8_t*));
    (*fhdr)->saddr = (uint8_t *) malloc((*fhdr)->hash_len);
    (*fhdr)->daddr = (uint8_t *) malloc((*fhdr)->hash_len);
    memcpy((*fhdr)->saddr, buffer + sizeof(Flexhdr) - 2 * sizeof(uint8_t*), (*fhdr)->hash_len);
    memcpy((*fhdr)->daddr, buffer + sizeof(Flexhdr) - 2 * sizeof(uint8_t*) + (*fhdr)->hash_len, (*fhdr)->hash_len);

    return SUCCESS;
}