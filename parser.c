/*
 * =========================================================================================
 * name        : maman_14/parser.c
 * author      : Ariel Mashraki, Osnat Izic
 * email       :
 * description :
 * =========================================================================================
 */

#include "common.h"
#include "parser.h"
#include "hash_table.h"

/* array to store the data parsed from the code */
data_line data_arr[MAX_ARR_SIZE];
/* linked list for the symbols addresses of data pointers */
static int_hash_node *data_symtab[HASHSIZE];
/* linked list for the opcode replacements */
static int_hash_node *optab[HASHSIZE];
/* temporary node for lookuping in the linked list */
int_hash_node *ihn;
/* general, instructions and data counters */
int i, ic, dc;
/* flags for decide if to create the files */
char error_flag = 0, extern_flag = 0, entry_flag = 0;

/* first parsing */
int firstParsing(code_line *file, int num_of_lines)
{
    char *symbol;
    int i = -1;
    
    while (++i < num_of_lines) {
        symbol = getSymbol(file + i);
        trimSpaces(&(file[i].line));
        
        /* if it's operation instruction[extern, entry, data, string]*/
        if(file[i].line[0] == '.')
        {
            parseCommand(&file[i], symbol);
        }
        else
        {
            /*insert symbol to hash table*/
            if (symbol)
                printf("symbol: %s\n", symbol);
            /*parse instruction */
        }
    }
    
    return 0;
}

/* get symbol/label */
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

/* extract each command line */
void parseCommand(code_line *c_line, char *symbol)
{
    /* if string */
	if (strncmp(c_line->line, ".string", sizeof(".string") - 1) == 0)
	{
		/* if there is a symbol */
		if (symbol)
		{
			ihn = int_install(symbol, dc, data_symtab);
		}
        c_line->line += (sizeof(".string") - 1);
		/* use the auxiliary function to extract the string */
		extract_string(c_line);
	}

}

/* this function extract the string from a string command */
void extract_string(code_line *c_line)
{
    remove_pre_spaces(&(c_line->line));
	/* make sure its starts with " */
    if (!(c_line->line[0] == '\"'))
    {
        ERROR(".string argument must start with \"", (*c_line).line_number)
		error_flag = 1;
        return;
    }
    
    (c_line->line)++;
	/* while its not reached the second " */
    while ((c_line->line[0] != '\"') && (c_line->line[0] != '\0'))
    {
		/* add the new string to the array */
        data_arr[dc++] = char2data(c_line->line[0]);
        (c_line->line)++;
    }
    
	/* at end, make sure it ends with " */
    if (c_line->line[0] == '\"')
    {
        data_arr[dc++] = char2data('\0');
    }
    else
    {
        ERROR(".string argument must end with \"", (*c_line).line_number)
		error_flag = 1;
        return;
    }
    
}


/* this function is called only in start of day, and loading all the opcodes into the hashtable */
void init_op_table()
{
    ihn = int_install("mov" , MOV, optab);
    ihn = int_install("cmp" , CMP, optab);
    ihn = int_install("add" , ADD, optab);
    ihn = int_install("sub" , SUB, optab);
    ihn = int_install("not" , NOT, optab);
    ihn = int_install("clr" , CLR, optab);
    ihn = int_install("lea" , LEA, optab);
    ihn = int_install("inc" , INC, optab);
    ihn = int_install("dec" , DEC, optab);
    ihn = int_install("jmp" , JMP, optab);
    ihn = int_install("bne" , BNE, optab);
    ihn = int_install("red" , RED, optab);
    ihn = int_install("prn" , PRN, optab);
    ihn = int_install("jsr" , JSR, optab);
    ihn = int_install("rts" , RTS, optab);
    ihn = int_install("stop" , STOP, optab);
}













