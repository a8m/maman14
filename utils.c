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

void init_data_line(data_line *dl)
{
    dl->data = 0;
}

data_line char2data(char ch)
{
    data_line tmp;
    init_data_line(&tmp);
    tmp.data = ch;
    return tmp;
}

/*
 * The botwise 0x7FFF(hexadecimal) will clear the sign biy of a 16-bit number
 * when used with bitwise(j &= 0x7FFFF), as illustrate in the truth table
 */
data_line num2data(int num)
{
    unsigned int bit_mask_19 = 0x7FFFF;
    data_line tmp;
    init_data_line(&tmp);
    
    if (num >= 0)
    {
        tmp.data = num & bit_mask_19;
    }
    else
    {
		tmp.data = (num * -1) & bit_mask_19;
		tmp.data = ~(tmp.data);
		(tmp.data)++;
    }
    
    return tmp;
}



