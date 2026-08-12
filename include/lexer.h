#ifndef LEXER_H
#define LEXER_H

#include "token.h"

typedef struct {
    Token **tokens;
    int count;
    int capacity;
} TokenList;

TokenList *lexer_tokenize(const char *input);
void free_token_list(TokenList *list);
void print_tokens(TokenList *list);

#endif
