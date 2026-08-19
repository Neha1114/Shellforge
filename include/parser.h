#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef struct {
    char **argv;
    int argc;
    int capacity;

    char *input_file;
    char *output_file;
    char *error_file;

    int append_output;
    int background;
} Command;

typedef struct {
    Command **commands;
    int count;
    int capacity;
} CommandList;

Command *create_command(void);
void free_command(Command *command);

CommandList *create_command_list(void);
void free_command_list(CommandList *list);

CommandList *parse_tokens(TokenList *tokens);

void print_commands(CommandList *list);

#endif
