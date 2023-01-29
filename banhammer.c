#include "bf.h"
#include "bv.h"
#include "city.h"
#include "ht.h"
#include "ll.h"
#include "messages.h"
#include "node.h"
#include "parser.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include <unistd.h>

#define OPTIONS "ht:f:ms"

int main(int argc, char *argv[]) { 
	int opt = 0;
	uint32_t hash_size = 10000;
	uint32_t bloom_size = 524288;
	bool mtf = false;
	bool stats = false;
	bool helpman = false;
	
	
	
	while ((opt = getopt(argc, argv, OPTIONS)) != -1) { 
		
		switch (opt) {
			
			case 'h':
				helpman = true;
				break;
				
			case 't':
				hash_size = strtoul(optarg, NULL, 10);
				break;
				
			case 'f':
				bloom_size = strtoul(optarg, NULL, 10);
				break;
				
			case 'm':
				mtf = true;
				break;
				
			case 's':
				stats = true;
				break;
			
			default:
				return 1;				
		}		
	
	}	
	
	BloomFilter *bf = bf_create(bloom_size);
	
	HashTable *ht = ht_create(hash_size, mtf);
	
	FILE *badspeak_file;
	FILE *newspeak_file;
	
	//reading from badspeak and adding the words to hashtable and bloomfilter
	badspeak_file  = fopen("badspeak.txt", "r");
	
	Parser *p_badspeak = parser_create(badspeak_file);
	bool next_bad = true;
	char wb[1001];
	char *word_bad = wb;
	
	while (next_bad) {
		next_bad = next_word(p_badspeak, word_bad);
		if (next_bad) {
			if (word_bad[0] != '\0') {
				bf_insert(bf, word_bad);
				ht_insert(ht, word_bad, NULL);
			}	
		}
	}

	
	fclose(badspeak_file);
	
	
	//reading from newspeak and adding oldspeak to bloom filter and hash table and adding new speak to just hashtable
	newspeak_file = fopen("newspeak.txt", "r");
	
	Parser *p_newspeak = parser_create(newspeak_file);
	bool next_new = true;
	char wn[1001];
	char *word_new = wn;
	char sosp[1001];
	char *saved_old_speak = sosp;
	
	int iterator = 0;
	int even = 1;
	while (next_new) {
		next_new = next_word(p_newspeak, word_new);
		if (next_new) {
			if (word_new[0] != '\0') {
				iterator = 0;
				if ((even % 2) == 1) {
					while (word_new[iterator] != '\0') {
						saved_old_speak[iterator] = word_new[iterator];
						iterator += 1;
					}
					saved_old_speak[iterator] = '\0';
					bf_insert(bf, word_new);
					even += 1;
				} else if ((even % 2) == 0) {
					//printf("%s %s\n", saved_old_speak, word_new);
					ht_insert(ht, saved_old_speak, word_new);
					even += 1;
				}
			}	
		}
	}

	
	
	fclose(newspeak_file);
	
	//now I need to read words from stdin using parser
	char w[1001];
        char *word = w;
        //FILE *test = fopen("testtext.txt", "r");
        Parser *p = parser_create(stdin);
	bool next = true;
	Node *probed;
	LinkedList *badspeak_list;
	LinkedList *rightspeak_list;
	badspeak_list = ll_create(false);
	rightspeak_list = ll_create(false);
	
	bool prob;
	
	
	while (next) {
		next = next_word(p, word);
		if (next) {
			if (word[0] != '\0') {
				prob = bf_probe(bf, word);
				if (prob) {
					probed = ht_lookup(ht, word);
					//node_print(probed);
					if (probed == NULL) {
						continue;
					} else if (probed->newspeak[0] == '\0'){
						//badspeak
						ll_insert(badspeak_list, probed->oldspeak, NULL);
						
					} else if (probed->newspeak[0] != '\0') {
						//counselling on rightspeak
						ll_insert(rightspeak_list, probed->oldspeak, probed->newspeak);
					}	
				}
			}	
		}
	}

	
	uint32_t ht_nk;
	uint32_t ht_nh;
	uint32_t ht_nm;
	uint32_t ht_ne;
	
	uint32_t bf_nh;
	uint32_t bf_nm;
	uint32_t bf_nk;
	uint32_t bf_ne;
	
	float bits_ex_per_miss;
	float false_positives;
	float bf_load;
	float avg_seek_len;
	uint32_t bfcount;
	uint32_t bfsize;
	
	if (stats) {
		
		ht_stats(ht, &ht_nk, &ht_nh, &ht_nm, &ht_ne);
		bf_stats(bf, &bf_nk, &bf_nh, &bf_nm, &bf_ne);
		
		
		if (bf_nm > 0) {
			bits_ex_per_miss = ((float)bf_ne - (5 * (float)bf_nh)) / (float)bf_nm;
		} else {
			bits_ex_per_miss = ((float)bf_ne - (5 * (float)bf_nh));
		}
		
		if (bf_nh > 0) {
			false_positives = (float)ht_nm / (float)bf_nh;
		} else {
			false_positives = (float)ht_nm;
		}
		
		bfcount = bf_count(bf);
		bfsize = bf_size(bf);
		if (bfsize > 0) {
			bf_load = (float)bfcount / (float)bfsize;
		} else {
			bf_load = (float)bfcount;
		}
		
		uint32_t den = (ht_nh + ht_nm);
		if (den > 0) {
			avg_seek_len = (float)ht_ne / ((float)ht_nh + (float)ht_nm);
		} else {
			avg_seek_len = (float)ht_ne;
		}
		
		printf("ht keys: %u\n", ht_nk);
		printf("ht hits: %u\n", ht_nh);
		printf("ht misses: %u\n", ht_nm);
		printf("ht probes: %u\n", ht_ne);
		printf("bf keys: %u\n", bf_nk);
		printf("bf hits: %u\n", bf_nh);
		printf("bf misses: %u\n", bf_nm);
		printf("bf bits examined: %u\n", bf_ne);
		printf("Bits examined per miss: %0.6f\n", bits_ex_per_miss);
		printf("False positives: %0.6f\n", false_positives);
		printf("Average seek length: %0.6f\n", avg_seek_len);
		printf("Bloom filter load: %0.6f\n", bf_load);
		
		
	
	} else {
		
		if (helpman) {
			fprintf(stderr, "SYNOPSIS\n");
			fprintf(stderr, "	Program to determine if user is using badspeak or oldspeak with newspeak translations\n");
			fprintf(stderr, "USAGE\n");
			fprintf(stderr, "	./banhammer [-t hash table size] [-f bloom filter size] [-m] [-s] [-h]\n");
			fprintf(stderr, "OPTIONS\n");
			fprintf(stderr, "	-t 	Specify hash table size\n");
			fprintf(stderr, "	-f	Specify bloom filter size\n");
			fprintf(stderr, "	-m	Enables move to front to\n");
			fprintf(stderr, "	-s 	Enables only stats to be printed\n");
			fprintf(stderr, "	-h	Displays program synopsis and usage\n");
		}
	
		if ((ll_length(badspeak_list) > 0) && (ll_length(rightspeak_list) > 0)) {
			printf("%s", mixspeak_message);
			ll_print(badspeak_list);
			ll_print(rightspeak_list);
			
		} else if ((ll_length(badspeak_list) > 0) && (ll_length(rightspeak_list) == 0)) {
			printf("%s", badspeak_message);
			ll_print(badspeak_list);
		
		} else if ((ll_length(badspeak_list) == 0) && (ll_length(rightspeak_list) > 0)) {
			printf("%s", goodspeak_message);
			ll_print(rightspeak_list);
		} 
	}
		
	//clear all data structures here
	parser_delete(&p);
	parser_delete(&p_newspeak);
	bf_delete(&bf);
	ht_delete(&ht);
	parser_delete(&p_badspeak);
	ll_delete(&badspeak_list);
	ll_delete(&rightspeak_list);
	
	
	
	
	return 0;
	
}


