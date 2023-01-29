#include "bf.h"
#include "city.h"
#include "bv.h"
#include "ll.h"
#include "node.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static uint64_t seeks;
static uint64_t links;

struct LinkedList {
    uint32_t length;
    Node *head; // Head sentinel node.
    Node *tail; // Tail sentinel node.
    bool mtf;
};

int string_compare(char *target, char *compared) {
    int i = 0;
    int target_count = 0;
    while (target[i] != '\0') {
        target_count += 1;
        i += 1;
    }

    int j = 0;
    int compared_count = 0;
    while (compared[j] != '\0') {
        compared_count += 1;
        j += 1;
    }

    if (compared_count != target_count) {
        return 0;
    } 

    int k = 0;
    while (target[k] != '\0') {
        if (target[k] != compared[k]) {
            return 0;
        }
        k += 1;
    }
    return 1;
    
}


LinkedList *ll_create(bool mtf) {
    LinkedList *ll = (LinkedList *) malloc(sizeof(LinkedList));

    if (ll != NULL) {
        ll->length = 0;

        Node *head = node_create(NULL, NULL);
        Node *tail = node_create(NULL, NULL);
        
        head->next = tail;
        head->prev = NULL;
        tail->next = NULL;
        tail->prev = head;

        ll->head = head;
        ll->tail = tail;

        ll->mtf = mtf;
    } else {
        return NULL;
    }

    return ll;
}

void ll_delete(LinkedList **ll) {
    if ((*ll) != NULL) {
        while (((*ll)->head) != NULL) {
            Node *next = NULL;
            next = (*ll)->head->next;
            node_delete(&(*ll)->head);
            (*ll)->head = next;
        }
        free(*ll);
        (*ll) = NULL;
    }
}

uint32_t ll_length(LinkedList *ll) {
	if (ll != NULL) {
	    uint32_t length = 0;
	    Node *current = ll->head;
	    while (current != NULL) {
		length += 1;
		current = current->next;
	    }
	    ll->length = (length-2);
	    return (length-2);
	} else {
		return 0;
	}
}

Node *ll_lookup(LinkedList *ll, char *oldspeak) {
	if (ll != NULL) {
	    seeks += 1;
	    if (ll->mtf) {
		for (Node *current = ll->head; current != NULL; current = current->next) {
		    links += 1;
		    if ((string_compare(oldspeak, current->oldspeak) == 1)) {
		    

		        current->prev->next = current->next;
		        current->next->prev = current->prev;

		        current->next = ll->head->next;
		        current->prev = ll->head;

		        ll->head->next->prev = current;
		        ll->head->next = current;
		        
		        return current;
		    }
		}
		return NULL;
	    } else {
		for (Node *current = ll->head; current != NULL; current = current->next) {
		    links += 1;
		 
		    if ((string_compare(oldspeak, current->oldspeak) == 1)) {
		        return current;
		    }	
		}
		return NULL;
	    }
	} else {
    		return NULL;
    	}
}

void ll_insert(LinkedList *ll, char *oldspeak, char *newspeak) {
	if (ll != NULL) {
	    if (ll_lookup(ll, oldspeak) != NULL) {
		return;
	    }

	    Node *inserted = node_create(oldspeak, newspeak);


	    inserted->next = ll->head->next;
	    inserted->prev = ll->head;

	    ll->head->next->prev = inserted;
	    ll->head->next = inserted;

	    ll->length += 1;
	    
	}

}

void ll_print(LinkedList *ll) {
    if (ll != NULL) {
	    for (Node *current = ll->head; current != NULL; current = current->next) {
		if (current->prev != NULL && current->next != NULL) {
		    node_print(current);
		}
	    }
    }
}

void ll_stats(uint32_t *n_seeks, uint32_t *n_links) { 
    if (n_seeks != NULL) {
    	*n_seeks = seeks;
    }  
    
    if (n_links != NULL) {
    	*n_links = links;
    }
}



