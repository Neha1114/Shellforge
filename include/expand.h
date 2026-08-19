#ifndef EXPAND_H
#define EXPAND_H

#include "parser.h"

char *expand_variable(const char *value);
void expand_command(Command *command);
void expand_command_list(CommandList *list);

#endif
