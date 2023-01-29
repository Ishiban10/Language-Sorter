#include "bf.h"
#include "city.h"
#include "bv.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>


static uint64_t default_salts [] = {
    0x5adf08ae86d36f21 ,
    0x419d292ea2ffd49e ,
    0x50d8bb08de3818df ,
    0x272347aea4045dd5 ,
    0x7c8e16f768811a21
};

#define N_HASHES 5

struct BloomFilter {
    uint64_t salts[N_HASHES ]; // Salts for five hash functions
    uint32_t n_keys;
    uint32_t n_hits;
    uint32_t n_misses;
    uint32_t n_bits_examined;
    BitVector *filter;
};

BloomFilter *bf_create(uint32_t size) {
    BloomFilter *bf = (BloomFilter *) malloc(sizeof(BloomFilter));
    if (bf) {
        bf->n_keys = bf ->n_hits = 0;
        bf->n_misses = bf->n_bits_examined = 0;
        for (int i = 0; i < N_HASHES; i++) {
            bf ->salts[i] = default_salts[i];
        }
        bf->filter = bv_create(size);
        if (bf->filter == NULL) {
            free(bf);
            bf = NULL;
        }
    }
    return bf;
}

void bf_delete(BloomFilter **bf) {
	if ((*bf) != NULL) {
	    bv_delete(&((*bf)->filter));
	    free(*bf);
	    *bf = NULL;
	}
}

uint32_t bf_size(BloomFilter *bf) {
    if (bf != NULL) {
    	return bv_length(bf->filter);
    } else {
    	return 0;
    }
}

void bf_insert(BloomFilter *bf, char *oldspeak) {
    if (bf != NULL) {
	    for (int i = 0; i < 5; i ++) {

		uint32_t bit = hash(default_salts[i], oldspeak) % bv_length(bf->filter);

		bv_set_bit(bf->filter , bit);
		
		bf->n_keys += 1;

	    }
    }
}

bool bf_probe(BloomFilter *bf, char *oldspeak) {
    if (bf != NULL) {
	    for (int i = 0; i < 5; i ++) {
		bf->n_bits_examined += 1;
		
		uint32_t bit = hash(default_salts[i], oldspeak) % bv_length(bf->filter);

		uint8_t got_bit = bv_get_bit(bf->filter, bit);

		if (got_bit == 0) {
		    bf->n_misses += 1;
		    return false;
		}

	    }
	    bf->n_hits += 1;
	    return true;
    } else {
    	return false;
    }
}

uint32_t bf_count(BloomFilter *bf) {
	if (bf != NULL) {
	    uint32_t count = 0;
	    for (uint32_t i = 0; i < bv_length(bf->filter); i += 1) {
		if (bv_get_bit(bf->filter, i) == 1) {
		    count += 1;
		}
	    }

	    return count;
	} else {
		return 0;
	}

}

void bf_print(BloomFilter *bf) { 
    if (bf != NULL) {
        bv_print(bf->filter);
    }
}


void bf_stats(BloomFilter *bf, uint32_t *nk, uint32_t *nh, uint32_t *nm, uint32_t *ne) {
    if (bf != NULL) {
	    *nk = bf->n_keys;
	    *nh = bf->n_hits;
	    *nm = bf->n_misses;
	    *ne = bf->n_bits_examined;
    }
}

