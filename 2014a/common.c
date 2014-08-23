/*
 * =========================================================================================
 * name        : maman_14/common.c
 * author      : Ariel Mashraki, Osnat Izic
 * =========================================================================================
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>/* DEBUG */
#include <ctype.h>
#include "common.h"
/* 
 * @description copy the given string in the argument;
 * @param str {String}
 * @return copied string {String}
 */
char *strdup(const char *str)
{
    int n = strlen(str) + 1;
    char *dup = malloc(n);
    if(dup)
    {
        strcpy(dup, str);
    }
    return dup;
}

/* 
 * @description trim white space from the begining of the string, 
 * incrementing the original pointer to the first place that is not space 
 * @param str {String}
 */
 void trim(char **str)
{
    while(isspace(**str) && (**str != '\n'))
        (*str)++;
}

/* 
 * @descipriton converting int to ant requested base, if pad is '1', the function will add leading '0' to the number 
 * @param number {int}
 * @param base {int}
 * @param str {String}
 * @param pad {int}
 * @return pointer to the char in the first of the converted number
 */
char *to_base(int num, int base, char *result, int pad)
{
    int index = 0, i;
    char tmp[MAX_DIGIT + 1];
    for (i = 0; i < MAX_DIGIT; i++) /* reset all the cells */
    {
        result[i] = '0';
        tmp[i] = '0';
    }

    while (num != 0)
    {
        tmp[index] = (num % base) + '0';
        num /= base;
        index++;
    }

    for (i = 0; i < MAX_DIGIT; i++)
    {
        result[i] = tmp[MAX_DIGIT - 1 - i];
    }
    result[i] = '\0';

    if (pad)
    {
        return result;
    }
    else
    {
        return result + MAX_DIGIT - index;
    }
}
