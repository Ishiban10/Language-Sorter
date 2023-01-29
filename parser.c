#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_PARSER_LINE_LENGTH 1000

struct Parser {
    FILE *f;
    char current_line[MAX_PARSER_LINE_LENGTH + 1];
    uint32_t line_offset;
};

Parser *parser_create(FILE *f) {
    Parser *p = (Parser *) malloc(sizeof(Parser));
    if (p != NULL) {
        p->f = f;
        p->line_offset = 0;
    } else {
        return NULL;
    }
    return p;
}


void parser_delete(Parser **p) {
    if ((*p) != NULL) {
        free(*p);
        *p = NULL;      
    }
}

bool next_word(Parser *p, char *word) {
	int letter = 0;
	bool add1 = true;
	char lowered;
 
	if (p->line_offset == 0) {
		if (fgets(p->current_line, MAX_PARSER_LINE_LENGTH + 1, p->f) == NULL) {
			return false;
		}
	}
    
	while (p->current_line[p->line_offset] == '\'' || p->current_line[p->line_offset] == '-'|| isalnum(p->current_line[p->line_offset]) || p->current_line[p->line_offset] == '\n'){
		if (p->current_line[p->line_offset] == '\n') {
			p->line_offset = 0;
			add1 = false;
			break;
		}
		lowered = tolower(p->current_line[p->line_offset]); 
		word[letter] = lowered;
		p->line_offset += 1; 
		letter += 1;
	}
	word[letter] = '\0';
	
	if (add1) {
		p->line_offset += 1;
	}
	
	return true;
    
}



