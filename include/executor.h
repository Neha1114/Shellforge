#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "parser.h"

int execute_external(Command *command);

int execute_pipeline(Command **commands, int command_count);

#endif
