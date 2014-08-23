/*
 * =========================================================================================
 * name        : maman_14/common.c
 * author      : Ariel Mashraki, Osnat Izic
 * =========================================================================================
 */

/* 
 * @description trim white space from the begining of the string, 
 * incrementing the original pointer to the first place that is not space 
 * @param str {String}
 */
void trim(char **str);

/* 
 * @description copy the given string in the argument;
 * @param str {String}
 * @return copied string {String}
 */
 char *strdup(const char *str);
/* 
 * @descipriton converting int to ant requested base, if pad is '1', the function will add leading '0' to the number 
 * @param number {int}
 * @param base {int}
 * @param str {String}
 * @param pad {int}
 * @return pointer to the char in the first of the converted number
 */
char *to_base(int num, int base, char *result, int pad);

/* 
 * @descipriton MACRO - PRINT_ERROR printing 
 * @param msg {String}
 * @param line number {int}
 * @example[usage]
 * PRINT_ERROR("Hi, I'm an PRINT_ERROR message", 3)
 */
#define PRINT_ERROR(MSG,LINE) fprintf(stderr, "PRINT_ERROR in line %d: %s\n", LINE, MSG);

/* 
 * max digit for the to_base function 
 */
#define MAX_DIGIT 8
