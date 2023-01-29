#include "bv.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>


struct BitVector {
    uint32_t length;
    uint64_t *vector;
};

BitVector *bv_create(uint32_t length) {

    BitVector *bv = (BitVector *) malloc(sizeof(BitVector));

    if (bv != NULL) {
        bv->length = length;
        bv->vector = (uint64_t *) calloc(length, sizeof(uint64_t));
    } else {
        return NULL;
    }
    return bv;
}

void bv_print(BitVector *bv) {
	if (bv != NULL) {
	    uint64_t c = 0;
	    //uint64_t t = 0;
	    for (uint32_t i = 0; i < (bv->length * 64); i ++) {
	    	if (c == 64) {
	    	    c = 0;
	    	    printf("\n");
	    	}
	    	if (bv_get_bit(bv, i)) {
	    	    c += 1;
	    	    //t += 1;
	    	    printf("1");
	    	} else {
	    	    c += 1;
	    	    //t += 1;
	    	    printf("0");
	    	}
	    }
	    printf("\n");
	    //printf("%lu\n", t); 
	}
}

void bv_delete(BitVector **bv) {
	if ((*bv) != NULL) {
	    free((*bv)->vector);
	    free(*bv);
	    *bv = NULL;
	}
}

uint32_t bv_length(BitVector *bv) {
    if (bv != NULL) {
        return bv->length;
    } else {
    	return 0;
    }
}

void bv_set_bit(BitVector *bv, uint32_t i) {
	if (bv != NULL) {
	    uint64_t byte = i / 64;
	    uint64_t bit = i % 64;
	    
	    bv->vector[byte] |= 1UL << bit;
	}

}

void bv_clr_bit(BitVector *bv, uint32_t i) {
	if (bv != NULL) {
	    uint64_t byte = i / 64;
	    uint64_t bit = i % 64;

	    bv->vector[byte] &= ~(1UL << bit);
	}
}

uint8_t bv_get_bit(BitVector *bv, uint32_t i) {
	if (bv != NULL) {
	    uint64_t byte = i / 64;
	    uint64_t bit = i % 64;

	    return ((bv->vector[byte] & (1UL << bit)) != 0);
	} else {
		return 0;
	}
}
