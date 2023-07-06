/*
 * stringFunction.h
 *
 *  Created on: Jun 25, 2023
 *      Author: nhat.tranminh
 */

#ifndef STRINGFUNCTION_H_
#define STRINGFUNCTION_H_
#include "usb_pcdc.h"

#define NUM_CMD_MAX 10

int checkUserInput(char *ptrBuff, unsigned int buffSize, char (*ptrBuffOut)[READ_BUF_SIZE]);
int cmdParser(char * input, char *cmd, char *params);

#endif /* STRINGFUNCTION_H_ */

