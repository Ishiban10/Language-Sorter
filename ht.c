#include "ht.h"
#include <stdio.h>
#include <stdlib.h>
#include "ll.h"
#include "bv.h"
#include "bf.h"
#include <stdbool.h>
#include "node.h"
#include "city.h"

struct HashTable {
    uint64_t salt;
    uint32_t size;
    uint32_t n_keys;
    uint32_t n_hits;
    uint32_t n_misses;
    uint32_t n_examined;
    bool mtf;
    LinkedList ** lists;
};

HashTable *ht_create(uint32_t size , bool mtf) {
    HashTable *ht = (HashTable *) malloc(sizeof(HashTable));
    if (ht != NULL) {
        ht->mtf = mtf;
        ht->salt = 0x9846e4f157fe8840;
        ht->n_hits = ht->n_misses = ht->n_examined = 0;
        ht->n_keys = 0;
        ht->size = size;
        ht->lists = (LinkedList **) calloc(size , sizeof(LinkedList *));
        if (!ht->lists) {
            free(ht);
            ht = NULL;
        }
    }
    return ht;
}

void ht_print(HashTable *ht) {
    if (ht != NULL) {
        for (uint32_t i = 0; i < ht->size; i++) {
            if (ht->lists[i] != NULL) {
    	        ll_print(ht->lists[i]);
    	    }
        }
    }
}

void ht_delete(HashTable **ht) {
    if (*ht != NULL) {
        for (uint32_t i = 0; i < (*ht)->size; i++) {
            ll_delete(&(*ht)->lists[i]);
        }
        free(*ht);
        (*ht) = NULL;
    } 
}

uint32_t ht_size(HashTable *ht) {
    if (ht != NULL) {
   	 return ht->size;
    } else {
    	return 0;
    }
}

Node *ht_lookup(HashTable *ht, char *oldspeak) {
    uint32_t n_links_original;
    uint32_t n_links_updated;

    if (ht != NULL) {
        Node *found;

        ll_stats(NULL, &n_links_original);

        uint32_t index = hash(ht->salt, oldspeak) % ht->size;

       
            if (ht->lists[index] != NULL) {
            	found = ll_lookup(ht->lists[index], oldspeak);
            } else {
            	return NULL;
            }

            if (found != NULL) {
                ht->n_hits += 1;
                ll_stats(NULL, &n_links_updated);
        	uint32_t links_accessed = n_links_updated - n_links_original;
        	ht->n_examined += links_accessed;
                return found;
            }

 
            ht->n_misses += 1;
        


        ll_stats(NULL, &n_links_updated);

        uint32_t links_accessed = n_links_updated - n_links_original;

        ht->n_examined += links_accessed;
	
	
        return NULL;
    } else {
        return NULL;
    }
}

void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
	if (ht != NULL) {
		uint32_t index = hash(ht->salt, oldspeak) % ht->size;
		LinkedList *ll;


		if (ht->lists[index] == NULL) {
			ll = ll_create(ht->mtf);
			ll_insert(ll, oldspeak, newspeak);
			ht->lists[index] = ll;
			ht->n_keys += 1;
		} else {
			ll_insert(ht->lists[index], oldspeak, newspeak);
		}
		
	}
}

uint32_t ht_count(HashTable *ht) {
	if (ht != NULL) {
	    uint32_t count = 0;
	    for (uint32_t i = 0; i < ht->size; i += 1) {
		if (ht->lists[i] != NULL) {
		    count += 1;
		}
	    }
	    return count;
	} else {
	    return 0;
	}
}

void ht_stats(HashTable *ht, uint32_t *nk, uint32_t *nh, uint32_t *nm, uint32_t *ne) {
	if (ht != NULL) {
	    *nk = ht->n_keys;
	    *nh = ht->n_hits;
	    *nm = ht->n_misses;
	    *ne = ht->n_examined;
	}
}
