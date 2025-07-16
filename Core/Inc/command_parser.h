//
// Created by niclas on 7/8/25.
//

#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#define MAX_COMMAND_LENGTH 30

typedef enum {
    NONE, HELP,
    DA1_ON, DA2_ON, DA3_ON,
    DA1_OFF, DA2_OFF, DA3_OFF,
    OUT1_ON, OUT2_ON, OUT3_ON, OUT4_ON,
    OUT1_OFF, OUT2_OFF, OUT3_OFF, OUT4_OFF,
    READ_DS1, READ_DS2, READ_DS3,
    READ_HU, READ_HV, READ_HW,
    READ_HU_COUNT, READ_HV_COUNT, READ_HW_COUNT
  } command_t;

command_t command_parse(char *buffer, int max);

#endif //COMMAND_PARSER_H
