/*
 * parse.h
 *
 *  Created on: Jun 29, 2023
 *      Author: nhat.tranminh
 */

#ifndef APPS_UTILS_PARSE_H_
#define APPS_UTILS_PARSE_H_

#include "ringBuf.h"
#include "stdint.h"
#include <string.h>

#define MAX_CMD_SUPPORT 10

typedef enum{
    STATE_IDLE,
    STATE_AT,
    STATE_CMD,
    STATE_CR,
    STATE_LF
}parseState;

typedef struct{
    char commands[MAX_CMD_SUPPORT][RING_BUFFER_SIZE];
    int commandCount;
}cavATCommandParser;

typedef enum{
    VALID_AT,
    INVALID_AT
}parseResult;

parseResult parseData(cavATCommandParser *parser, char *input, uint16_t input_length);
void cleanParserData(cavATCommandParser *parser);

#endif /* APPS_UTILS_PARSE_H_ */







