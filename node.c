#include "node.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

void copy_words(char *oldspeak, char *newspeak, char *oldspeak_copy, char *newspeak_copy) {
    if (oldspeak != NULL) {
        int i = 0;
        while (oldspeak[i] != '\0') {
            oldspeak_copy[i] = oldspeak[i];
            i += 1;
        }
        oldspeak_copy[i] = '\0';
    } else {
        oldspeak_copy = NULL;
    }

    if (newspeak != NULL) {
        int j = 0;
        while (newspeak[j] != '\0') {
            newspeak_copy[j] = newspeak[j];
            j += 1;
        }
        newspeak_copy[j] = '\0';
    } else {
        newspeak_copy = NULL;
    }
}


Node *node_create(char *oldspeak, char * newspeak) {
    char *oldspeak_copy = calloc(50, sizeof(char));

    char *newspeak_copy = calloc(50, sizeof(char));

    copy_words(oldspeak, newspeak, oldspeak_copy, newspeak_copy);

    Node *n = (Node *) malloc(sizeof(Node));

    if (n != NULL) {
        n->oldspeak = oldspeak_copy;
        n->newspeak = newspeak_copy;
        n->next = NULL;
        n->prev = NULL;
    } else {
        return NULL;
    }
    return n;
}

void node_delete(Node **n) {
	if ((*n) != NULL) {
	    free((*n)->oldspeak);
	    free((*n)->newspeak);
	    free(*n);
	    *n = NULL;
	}
}

void node_print(Node *n) {
    if (n != NULL) {
	    if (n->newspeak[0] != '\0') {
	       printf("%s -> %s\n", n->oldspeak , n->newspeak); 
	    } else {
		printf("%s\n", n->oldspeak);
	    }
    }
}
