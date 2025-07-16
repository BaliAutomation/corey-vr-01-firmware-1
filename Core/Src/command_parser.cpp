//
// Created by niclas on 7/8/25.
//

#include <string.h>
#include "../Inc/command_parser.h"

command_t command_parse(char *buffer, int max) {
    char cmd[MAX_COMMAND_LENGTH];
    for ( int i = 0; i < max; i++ ) {
        char ch = buffer[i];
        if ( ch != ' ' && ch != '\n' && ch != '\r' && ch != '\t' ) {
            cmd[i] = ch;
        } else {
            cmd[i] = '\0';
            break;
        }
    }
    if ( strlen( cmd ) ) {
        return NONE;
    }
    if ( strcmp( cmd, "help" ) == 0 ) {
        return HELP;
    }
    if ( strcmp( cmd, "da1_on" ) == 0 ) {
        return DA1_ON;
    }
    if ( strcmp( cmd, "da2_on" ) == 0 ) {
        return DA2_ON;
    }
    if ( strcmp( cmd, "da3_on" ) == 0 ) {
        return DA3_ON;
    }
    if ( strcmp( cmd, "da1_off" ) == 0 ) {
        return DA1_OFF;
    }
    if ( strcmp( cmd, "da2_off" ) == 0 ) {
        return DA2_OFF;
    }
    if ( strcmp( cmd, "da3_off" ) == 0 ) {
        return DA3_OFF;
    }
    if ( strcmp( cmd, "out1_on" ) == 0 ) {
        return OUT1_ON;
    }
    if ( strcmp( cmd, "out1_on" ) == 0 ) {
        return OUT1_ON;
    }
    if ( strcmp( cmd, "out2_on" ) == 0 ) {
        return OUT2_ON;
    }
    if ( strcmp( cmd, "out3_off" ) == 0 ) {
        return OUT3_OFF;
    }
    if ( strcmp( cmd, "out2_off" ) == 0 ) {
        return OUT2_OFF;
    }
    if ( strcmp( cmd, "out3_off" ) == 0 ) {
        return OUT3_OFF;
    }
    if ( strcmp( cmd, "read_ds1" ) == 0 ) {
        return READ_DS1;
    }
    if ( strcmp( cmd, "read_ds2" ) == 0 ) {
        return READ_DS2;
    }
    if ( strcmp( cmd, "read_ds3" ) == 0 ) {
        return READ_DS3;
    }
    if ( strcmp( cmd, "read_hu" ) == 0 ) {
        return READ_HU;
    }
    if ( strcmp( cmd, "read_hu_count" ) == 0 ) {
        return READ_HU_COUNT;
    }
    if ( strcmp( cmd, "read_hv" ) == 0 ) {
        return READ_HV;
    }
    if ( strcmp( cmd, "read_hv_count" ) == 0 ) {
        return READ_HV_COUNT;
    }
    if ( strcmp( cmd, "read_hw" ) == 0 ) {
        return READ_HW;
    }
    if ( strcmp( cmd, "read_hw_count" ) == 0 ) {
        return READ_HW_COUNT;
    }
    return NONE;
}