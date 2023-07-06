/*
 * parse.c
 *
 *  Created on: Jun 29, 2023
 *      Author: nhat.tranminh
 */
#include "parse.h"


parseResult parseData(cavATCommandParser *parser, char *input, uint16_t input_length) {
    int commandIndex = 0;
    int readIndex = 0;
    int ret = INVALID_AT;
    parseState state = STATE_IDLE;

    parser->commandCount = 0;
    char currentChar;
    while (readIndex < input_length) {
        currentChar = input[readIndex];
        readIndex++;

        switch (state) {
            case STATE_IDLE:
                if (currentChar == 'A') {
                    parser->commands[parser->commandCount][commandIndex] = currentChar;
                    commandIndex++;
                    state = STATE_AT;
                }
                break;
            case STATE_AT:
                if (currentChar == 'T') {
                    parser->commands[parser->commandCount][commandIndex] = currentChar;
                    commandIndex++;
                    state = STATE_CMD;
                }else if (currentChar == 'A') {
                    ;
                } else {
                    commandIndex = 0;
                    state = STATE_IDLE;
                }
                break;
            case STATE_CMD:
                parser->commands[parser->commandCount][commandIndex] = currentChar;
                commandIndex++;
                if (currentChar == '\r') { // \r received
                    state = STATE_CR;
                }
                break;
            case STATE_CR:
                if (currentChar == '\n') {   // full \r\n found
                    state = STATE_IDLE;
                    parser->commands[parser->commandCount]  [commandIndex]  = currentChar;
                    parser->commands[parser->commandCount][commandIndex + 1]  = '\0';

                    commandIndex = 0;               // ready to receive next command
                    parser->commandCount++;
                    ret = VALID_AT;
                } else {  //\r is followed by any other character will be ignore.
                    commandIndex = 0;
                    readIndex--;
                    state = STATE_IDLE;
                }
                break;
            case STATE_LF:
                state = STATE_IDLE;
                break;
        }
    }
    return ret;
}

void cleanParserData(cavATCommandParser *parser)
{
    memset(parser->commands, 0, sizeof(parser->commands));
    parser->commandCount = 0;
}
