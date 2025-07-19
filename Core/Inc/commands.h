//
// Created by niclas on 7/8/25.
//

#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H
#include <stdint.h>

#define MAX_COMMAND_LENGTH 30
#define REQUEST_BUFFER_LENGTH 16
#define RESPONSE_BUFFER_LENGTH 1024

typedef struct {
    char *cmd_str;
    void (*cmd_func)(char *, char *response, uint32_t arg);
    uint32_t arg;
} cmd_mapping;


void cmd_execute(char *buffer, char * response);

#endif //COMMAND_PARSER_H
