/*
 * =========================================================================================
 * name        : maman_14/utils.h
 * author      : Ariel Mashraki, Osnat Izic
 * email       :
 * description :
 * =========================================================================================
 */

/**
 * Convert number in base 10 to number in base 8
 * @param  number type of int
 * @return number in base 8
 */
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

