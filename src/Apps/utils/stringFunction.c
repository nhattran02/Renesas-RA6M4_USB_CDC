/*
 * stringFunction.c
 *
 *  Created on: Jun 25, 2023
 *      Author: nhat.tranminh
 */
#include "stringFunction.h"


/**
 * Check if the string is in a valid format and extract AT commands.
 *
 * @param ptrBuff      A pointer to a character array containing user input.
 * @param buffSize     The size of the buffer pointed to by ptrBuff, which is the maximum number of
 *                     characters that can be stored in the buffer.
 * @param ptrBuffOut   A pointer to a two-dimensional character array used to store the parsed commands
 *                     from the input string.
 *
 * @return             The number of valid AT commands found in the input string, or -1 if the string
 *                     format is invalid.
 */
int checkUserInput(char *ptrBuff, unsigned int buffSize, char (*ptrBuffOut)[READ_BUF_SIZE])
{
    char *end = ptrBuff;
    char *start = ptrBuff;


    if(ptrBuff == NULL || buffSize == 0){
        return -1;
    }
    //Check if the string begins with "AT" and ends with "\r\n"
    while (*start == ' '){
        start ++;
    }
    if(*start != 'A' || *(start + 1) != 'T'){
        return -1;
    }
    //Ignore "AT" if string begins with "ATAT"
    if(*(start + 2) == 'A' && *(start + 3) == 'T'){
        start = start + strlen("AT");
    }
    ptrBuff = start;
    end = start + strlen(start) - 1;
    while (*end == ' '){
        end--;
    }
    end -= (strlen("\r\n") - 1);
    if(*end != '\r' || *(end + 1) != '\n'){
        return -1;
    }
    int countCmds = 1;
    while ((end = strstr(start + 1, "AT")) != NULL) {
        strncpy(ptrBuffOut[countCmds - 1], start, (size_t)(end - start));
        ptrBuffOut[countCmds - 1][end - start] = '\0';
        start = end;
        countCmds++;
    }
    end = strstr(start + 1, "\r\n");
    strncpy(ptrBuffOut[countCmds - 1], start, (size_t)(end - start));
    ptrBuffOut[countCmds - 1][end - start] = '\0';
    return countCmds;
}



