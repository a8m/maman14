/* duplicate the given string in the argument, returnes the copied data */
char *strdup(const char *str);
/* removing any white space from the begining of the string, incrementing the original pointer to the first place that is not space */
void remove_pre_spaces(char **str);
/* this function converting integers to ant requested base. returning the pointer to the char in the first of the converted number
	if pad is '1', the function will add leading '0' to the number */
char *to_base(int num, int base, char *result, int pad);
/* macro for error printing */
#define ERROR(MSG,LINE) fprintf(stderr, "Error in line %d: %s\n", LINE, MSG);
/* define the max digit for the to_base function */
#define MAX_DIGIT 8
