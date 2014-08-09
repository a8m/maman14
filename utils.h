/*
 * =========================================================================================
 * name        : maman_14/common.h
 * author      : Ariel Mashraki, Osnat Izic
 * email       :
 * description :
 * =========================================================================================
 */

#ifndef UTILS_H_
#define UTILS_H_

/**
 * Convert number in base 10 to number in base 8
 * @param  number type of int
 * @return number in base 8
 */
int DecToBase8(int num);

/* 
 * Trim white space from the begining of the string, 
 * incrementing the original pointer to the first place that is not space
 * @param **str {String}
 */
void trimSpaces(char **str);


#endif
