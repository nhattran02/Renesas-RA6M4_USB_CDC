/*
 * commandSupport.h
 *
 *  Created on: Jun 25, 2023
 *      Author: nhat.tranminh
 */

#ifndef COMMANDSUPPORT_H_
#define COMMANDSUPPORT_H_

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

struct CDC_Command_St{
    char cmd[16];
    uint32_t hash;
    void (* pfunc)(void *params);
};

typedef struct CDC_Command_St CDC_Commands_Type;
extern CDC_Commands_Type CDC_Command_Table[];

void initHashTable(void);
uint32_t hashFunc(char *str);
uint8_t hashTableLookup(char * cmd);


#endif /* COMMANDSUPPORT_H_ */
