#include <string.h>
#include <stdlib.h>
#include <stdio.h>/* DEBUG */
#include <ctype.h>
#include "utils.h"

/* duplicate the given string in the argument, returnes the copied data */
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

/* removing any white space from the begining of the string, incrementing the original pointer to the first place that is not space */
void remove_pre_spaces(char **str)
{
    while(isspace(**str) && (**str != '\n'))
        (*str)++;
}

/* this function converting integers to ant requested base. returning the pointer to the char in the first of the converted number
	if pad is '1', the function will add leading '0' to the number */
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
