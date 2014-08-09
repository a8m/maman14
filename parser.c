/*
 * =========================================================================================
 * name        : maman_14/parser.c
 * author      : Ariel Mashraki, Osnat Izic
 * email       :
 * description :
 * =========================================================================================
 */

#include "parser.h"
#include "hash_table.h"
#include "common.h"

/* temporary node for lookuping in the linked list */
hash_node *temp;

/* linked list for the opcode replacements
static hash_node *optab;*/


/* this function is checking if there is a symbol. if yes, its returning a pointer to it, if not, returns null */
char *check_if_symbol(code_line *c_line)
{
    char *temp = (c_line->line);
	/* while its not end of line */
    while (*(c_line->line) != '\0')
    {
		/* if there is a : char, its probably has symbol */
        if (*(c_line->line) == ':')
        {
			/* put null char at end of symbol */
            *((c_line->line)++) = '\0';
			/* make sure its starts with a alpahbetic char  */
            if (!isalpha(temp[0]))
            {

                /*eror need to be printed*/
            }
            return temp;
        }
        (c_line->line)++;
    }
    c_line->line = temp;
    return NULL;
}

/* first parsing */
int firstParsing(code_line *file, int num_of_lines)
{
    char *symbol;
    int i = -1;
    
    while (++i < num_of_lines) {
        
    }
    
    return 0;
}


char *getSymbol(code_line *c_line)
{
    char *temp = (c_line->line);
    /* '\0' = end of string */
    while (*(c_line->line) != '\0') {
        
        /*if there is a ':' it's probably end of symbol*/
        if(*(c_line->line) == ':') {
            /* put null char at end of symbol */
            *((c_line->line)++) = '\0';
            /* make sure its starts with a char */
            if (!isalpha(temp[0]))
            {
                printf(LABEL_ERR);
            }
            return temp;
        }
        (c_line->line)++;
    }
    /* return line to previous state */
    c_line->line = temp;
    return NULL;
}
















