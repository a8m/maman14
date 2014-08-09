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








/* loading all the opcodes into the hashtable
    void init_opcode_table()
{
    temp = insert("mov" , MOV, optab);
    temp = insert("cmp" , CMP, optab);
    temp = insert("add" , ADD, optab);
    temp = insert("sub" , SUB, optab);
    temp = insert("not" , NOT, optab);
    temp = insert("clr" , CLR, optab);
    temp = insert("lea" , LEA, optab);
    temp = insert("inc" , INC, optab);
    temp = insert("dec" , DEC, optab);
    temp = insert("jmp" , JMP, optab);
    temp = insert("bne" , BNE, optab);
    temp = insert("red" , RED, optab);
    temp = insert("prn" , PRN, optab);
    temp = insert("jsr" , JSR, optab);
    temp = insert("rts" , RTS, optab);
    temp = insert("stop" , STOP, optab);
}*/
