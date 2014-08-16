/*
 * =========================================================================================
 * name        : maman_14/utils.h
 * author      : Ariel Mashraki, Osnat Izic
 * email       :
 * description :
 * =========================================================================================
 */

#include "common.h"

int DecToBase8(int num)
{
	int outputBase = 8;
	int base8_num = 0;
	int power;

	power =1;
	while(num > 0)
	{
		base8_num += power*(num%outputBase);
		num/=outputBase;
		power*=10;
	}
	return base8_num;
}

void trimSpaces(char **str)
{
    while(isspace(**str) && (**str != '\n'))
        (*str)++;
}

/* this function returning data_line containing the char in the argument */
data_line char2data(char ch)
{
    data_line tmp;
    init_data_line(&tmp);
    tmp.data = ch;
    return tmp;
}