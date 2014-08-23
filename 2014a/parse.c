/*
 * =========================================================================================
 * name        : maman_14/parse.c
 * author      : Ariel Mashraki, Osnat Izic
 * =========================================================================================
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "data.h"
#include "parse.h"
#include "hashtable.h"
 #include "common.h"

/* array to store the data parsed from the code */
data_line data_arr[MAX_ARR_SIZE];
/* linked list for the symbols addresses of data pointers */
static int_hash_node *data_symtab[HASHSIZE];
/* linked list for the symbols addresses of instructions pointers */
static int_hash_node *inst_symtab[HASHSIZE];
/* linked list for the external commands */
static int_hash_node *exttab[HASHSIZE];
/* linked list for the opcode replacements */
static int_hash_node *optab[HASHSIZE];
/* temporary node for lookuping in the linkde lists */
int_hash_node *ihn;
/* array for entries */
char *entry_arr[MAX_ARR_SIZE];
/* counters for entries end externals */
int entry_counter, extern_counter;
/* general counter, and data and instructions counter */
int i, ic, dc;
/* flags for decide if to create the files */
char error_flag = 0, extern_flag = 0, entry_flag = 0;

/* this function is checking if there is a symbol. if yes, its returning a pointer to it, if not, returns null */
char *fetchSymbol(code_line *c_line)
{
    char *tmp = (c_line->line);
	/* while its not end of line */
    while (*(c_line->line) != '\0')
    {
		/* if there is a : char, its probably has symbol */
        if (*(c_line->line) == ':')
        {
			/* put null char at end of symbol */
            *((c_line->line)++) = '\0';
			/* make sure its starts with a char */
            if (!isalpha(tmp[0]))
            {
                ERROR("Symbol should start with a char", c_line->line_number)
                error_flag = 1;
            }
            return tmp;
        }
        (c_line->line)++;
    }
    c_line->line = tmp;
    return NULL;
}

/* this function extract the data from a data command */
void parseData(code_line *c_line)
{
    int sign;
    int sum;
	char expecting_number = 0;
	/* while its not end of line */
    while ((*c_line).line[0] != '\n')
    {
		/* remove the spaces from the begining of the line */
		trim(&((*c_line).line));
		/* check sign */
        if (c_line->line[0] == '-')
        {
            sign = -1;
            (c_line->line)++;
        }
		/* make sure its start with digit */
        if (!isdigit((*c_line).line[0]))
        {
            ERROR("Wrong .data format, expecting number", (*c_line).line_number)
			error_flag = 1;
        }
        else
        {
            sum = 0;
            sign = 1;
            while (isdigit((*c_line).line[0]))
            {
                sum *= 10;
                sum += ((*c_line).line[0]) - '0';
                (c_line->line)++;
            }
			/* add the number to data arr */
            data_arr[dc++] = num2data(sum * sign);
        }
		
		trim(&((*c_line).line));
		/* if there is more numbers */
        if (c_line->line[0] == ',')
		{
			expecting_number = 1;
			(c_line->line)++;
		}
		else
		{
			expecting_number = 0;
		}
    }
	/* if the line ended and we still waiting for another number */
	if (expecting_number == 1)
	{
		ERROR("Expecting number after \',\'", (*c_line).line_number)
		error_flag = 1;
	}

}

/* this function extract the string from a string command */
void parseString(code_line *c_line)
{
    trim(&(c_line->line));
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

/* this function extract the entry from a entry command */
void parseEntry(code_line *c_line)
{
	/* skip on tabs and spaces */
	c_line->line = strtok(c_line->line, " \t\n");
	/* if there is no entry */
	if (!c_line->line)
	{
        ERROR("No value found after entry command", (*c_line).line_number)
		error_flag = 1;
		return;
	}
	/* add entry to entries array */
	entry_arr[entry_counter++] = strdup(c_line->line);
	c_line->line = strtok(NULL, " \t\n");
	/* make sure there is no other words */
	if (c_line->line)
	{
        ERROR("Unexpected words in entry command", (*c_line).line_number)
		error_flag = 1;
	}
}

/* this function extract the entry from a entry command */
void parseExtern(code_line *c_line)
{
	/* skip on tabs and spaces */
	c_line->line = strtok(c_line->line, " \t\n");
	/* if there is no extern */
	if (!c_line->line)
	{
        ERROR("No value found after extern command", (*c_line).line_number)
		error_flag = 1;
		return;
	}
	/* add to the extern hashtable with value 0 */
    registerIntToHashTab(c_line->line, '0', exttab);
	extern_counter++;
	c_line->line = strtok(NULL, " \t\n");
	/* make sure there is no other words */
	if (c_line->line)
	{
        ERROR("Unexpected words in extern command", (*c_line).line_number)
		error_flag = 1;
	}
}

/* this functions returnes the number of operands expected per opcode */
char checkExpOperands(char *s)
{
	if (strcmp(s, "rts") == 0 || strcmp(s, "stop") == 0)
	{
		return 0;
	}
	else if (strcmp(s, "mov") == 0 || strcmp(s, "cmp") == 0 || strcmp(s, "add") == 0 ||
			strcmp(s, "sub") == 0 || strcmp(s, "lea") == 0)
	{
		return 2;
	}
	else
	{
		return 1;
	}
}

/* this function parsing the instruction given in the line */
void parsingInstruction(code_line *c_line)
{
	/* count how many instruction lines this line using, including repeats */
    int local_ic = 0;
	char num_of_exp_operands;
	/* isolate the opcode */
	c_line->line = strtok(c_line->line, "/");
	/* find it in the opcode hashtable */
	ihn = fetchIntFromHashTab(c_line->line, optab);
	/* if its not a valid opcode */
	if (!ihn)
	{
		/* check if its a comment or empty line */
        if ((c_line->line[0] == ';') || (c_line->line[0] = '\n'))
        {
            c_line->done = 1;
            return;
        }
        ERROR("Ilegal instruction", (*c_line).line_number)
		error_flag = 1;
		return;
	}
	/* use the auxilary function to count the expected operands */
	num_of_exp_operands = checkExpOperands(c_line->line);
	c_line->instruction = (instruction_line *) malloc(sizeof(instruction_line) * 8);
	/* clear the instruction line */
    registerInstructionLine(c_line->instruction);
    local_ic++;
	c_line->instruction->opcode = ihn->defn;
	/* seek for null char */
	while (c_line->line[0] != '\0')
    {
        (c_line->line)++;
    }
    (c_line->line)++;

	/* check for the type field, and comb field */
	switch (c_line->line[0])
	{
		case '0': /* type is 0, comb irelevant */
			c_line->instruction->type = 0;
			break;
		case '1': /* type is 1, need to check comb value */
			c_line->instruction->type = 1;
			(c_line->line)++;
			if (!(c_line->line[0] == '/'))
			{
				ERROR("Expecting comb values after type 1", (*c_line).line_number)
				error_flag = 1;
				return;
			}
			(c_line->line)++;
			switch (c_line->line[0])
			{
				case '0': /* keep default value */
					break;
				case '1':
					c_line->instruction->comb |= 2; /* left bit of comb should be 1 */
					break;
				default:
					ERROR("Ilegal value for comb", (*c_line).line_number)
					error_flag = 1;
					return;
			}
			(c_line->line)++;
			/* expecting / */
			if (!(c_line->line[0] == '/'))
			{
				ERROR("Expecting cobm values after type 1", (*c_line).line_number)
				error_flag = 1;
				return;
			}
			(c_line->line)++;
			switch (c_line->line[0])
			{
				case '0':
					break; /* keep default value */
				case '1':
					c_line->instruction->comb |= 1; /* right bit of comb should be 1 */
					break;
				default:
					ERROR("Ilegal value for comb", (*c_line).line_number)
					error_flag = 1;
					return;
			}
            break;
		default:
			ERROR("Ilegal value for comb", (*c_line).line_number)
			error_flag = 1;
			return;
	}
	(c_line->line)++;
	if (!(c_line->line[0] == ','))
	{
		ERROR("Expecting ',' after type and comb values", (*c_line).line_number)
		error_flag = 1;
		return;
	}
	(c_line->line)++;

    c_line->repeat = c_line->line[0] - '0';

   	/* check for repeat number, dbl bit */
    if(c_line->repeat < 0 || c_line->repeat > 1)
	{
		ERROR("Invalid command dbl value. Valid values are 0,1", (*c_line).line_number);
		return;
	}
	else
	{
		c_line->instruction->dbl = c_line->repeat;
	}
	(c_line->line)++;
	trim(&(c_line->line));
    
	 /*read the expected number of operands */
    c_line->line = strtok(c_line->line, " \t,\n");
	while (num_of_exp_operands-- > 0)
	{
        if (num_of_exp_operands == 1)
        {
			/* its the first operand */
            c_line->src_opr = strdup(c_line->line);
        }
        else
        {
			/* its the second operand */
            c_line->dest_opr = strdup(c_line->line);
        }
		/* check for the type of the oprands and decide how much ic should be incremented */
        switch (c_line->line[0])
		{
			case '#':
                local_ic++;
                if (num_of_exp_operands == 1)
                {
                    c_line->instruction->src_addr = 0;
                }
                else
                {
                    c_line->instruction->dest_addr = 0;
                }
                break;
			case 'r':
                (c_line->line)++;
                if ((c_line->line[0] >= '0') && (c_line->line[0] <='7'))
                {
                    if (num_of_exp_operands == 1)
                    {
                        c_line->instruction->src_reg = c_line->line[0] - '0';
                        free(c_line->src_opr);
                        c_line->instruction->src_addr = 3;
                    }
                    else
                    {
                        c_line->instruction->dest_reg = c_line->line[0] - '0';
                        free(c_line->dest_opr);
                        c_line->instruction->dest_addr = 3;
                    }
                    break;
                }
		    default:
                local_ic++;
                if (strstr(c_line->line, "{!"))
                {
                    local_ic++;
                    if (num_of_exp_operands == 1)
                    {
                        c_line->instruction->src_addr = 2;
                    }
                    else
                    {
                        c_line->instruction->dest_addr = 2;
                    }

                }
                else
                {
                    if (num_of_exp_operands == 1)
                    {
                        c_line->instruction->src_addr = 1;
                    }
                    else
                    {
                        c_line->instruction->dest_addr = 1;
                    }
                }
		}
        c_line->line = strtok(NULL, " \t,\n");
	}
    ic += (local_ic * c_line->repeat);
	
}

/* this function parsing the commands line (string, data, extern and entry) */
void parsingCmd(code_line *c_line, char *symbol)
{
	/* if string */
	if (strncmp(c_line->line, ".string", sizeof(".string") - 1) == 0)
	{
		/* if there is a symbol */
		if (symbol)
		{
			ihn = registerIntToHashTab(symbol, dc, data_symtab);
		}
        c_line->line += (sizeof(".string") - 1);
		/* use the auxilary function to extract the string */
		parseString(c_line);
	}
	/* if data */
	else if (strncmp((*c_line).line, ".data", sizeof(".data") - 1) == 0)
	{
		if (symbol)
		{
			ihn = registerIntToHashTab(symbol, dc, data_symtab);
		}
        c_line->line += (sizeof(".data") - 1);
		/* use the auxilary function to extract the data */
		parseData(c_line);
	}
	/* if entry */
	else if (strncmp(c_line->line, ".entry", sizeof(".entry") - 1) == 0)
	{
        c_line->line += (sizeof(".entry") - 1);
		/* use the auxilary function */
		parseEntry(c_line);
	}
	/* if extern */
	else if (strncmp(c_line->line, ".extern", sizeof(".extern") - 1) == 0)
	{
        c_line->line += (sizeof(".extern") - 1);
		/* use the auxilary function */
		parseExtern(c_line);
	}
	else
	{
		ERROR("Unrecognized command", (*c_line).line_number)
		error_flag = 1;
	}
    c_line->done = 1;
}

/* 
 * @description Do the first parsing to the line, 
 * and leaving for the second parsing to check for the symbols addresses and parsing the operands 
 * @param *file {code_line}
 * @param num_of_lines {int}
 */
int firstPhase(code_line *file, int num_of_lines)
{
    char *symbol;
	error_flag = 0;
	ic = 0;
	dc = 0;
	extern_counter = 0;
	entry_counter = 0;

    for (i = 0; i < num_of_lines; i++) /* loop over all the code lines */
    {
        symbol = fetchSymbol(file + i);
        trim(&(file[i].line));
		if (file[i].line[0] == '.')
		{
			parsingCmd(&file[i], symbol);
		}
		else
		{
            if (symbol)
            {
                ihn = registerIntToHashTab(symbol, ic, inst_symtab);
            }
			parsingInstruction(&file[i]);
		}
    }

    return 0;
}

/* this function is for the second parsing, parsing each operand */
void parsingOperand(char *op, int addr, FILE *obj, FILE *ext, int *line_count, int org_line_num)
{
	/* 2 temporary arrays to get the base 6 results */
	char base_result[MAX_DIGIT + 1], base_result1[MAX_DIGIT + 1];
	int sign = 1, sum = 0;
	/* check what tyoe of operand */
	switch (addr)
	{
		case 0: /* direct addressing */
			op++;
			if (op[0] == '-')
			{
				sign = -1;
				op++;
			}
			while (isdigit(op[0]))
			{
				sum *= 10;
				sum += op[0] - '0';
				op++;
			}
			/* write the given number to the obj file */
			fprintf(obj, "%s\t%s\t%c\n",to_base((*line_count) + LINE_OFFSET, BASE, base_result, NO_PAD) , to_base(num2data(sum * sign).data, BASE, base_result1, PAD), 'A');
            (*line_count)++;
			break;
		case 1: /* its symbol */
			/* check in the data hashtable */
			if ((ihn = fetchIntFromHashTab(op, data_symtab)))
			{
				/* write the symbol address to the obj file */
				fprintf(obj, "%s\t%s\t%c\n", to_base((*line_count) + LINE_OFFSET, BASE, base_result, NO_PAD), to_base(num2data(ihn->defn + ic + LINE_OFFSET + 1).data, BASE, base_result1, PAD), 'R');
                (*line_count)++;
			}
			/* check in the instruction hashtable */
            else if ((ihn = fetchIntFromHashTab(op, inst_symtab)))
            {
				/* write the symbol address to the obj file */
				fprintf(obj, "%s\t%s\t%c\n", to_base((*line_count) + LINE_OFFSET, BASE, base_result, NO_PAD), to_base(num2data(ihn->defn + LINE_OFFSET + 1).data, BASE, base_result1, PAD), 'R');
                (*line_count)++;
            }
			/* check in the externals hashtable */
            else if ((ihn = fetchIntFromHashTab(op, exttab)))
			{
				/* write 0 to the obj file */
				fprintf(obj, "%s\t%s\t%c\n", to_base((*line_count)+ LINE_OFFSET, BASE, base_result, NO_PAD), to_base(num2data(0).data, BASE, base_result1, PAD), 'E');
				/* add line to the ext file */
				fprintf(ext, "%s\t%s\n", op, to_base(*line_count + LINE_OFFSET, BASE, base_result, NO_PAD));
				(*line_count)++;
			}
            else
            {
                ERROR("Can't find symbol in symbol table or in extern table", org_line_num)
                error_flag = 1;
            }
            break;
        case 2: /* Elaborate addressing */
			/* seek the { char */
            for (i = 0 ;op[i] != '{'; i++);
			/* add there \0 char */
            op[i] = '\0';
			/* check for the symbol in data hashtable */
            if ((ihn = fetchIntFromHashTab(op, data_symtab)))
            {
				fprintf(obj, "%s\t%s\t%c\n", to_base((*line_count) + LINE_OFFSET, BASE, base_result, NO_PAD), to_base(num2data(ihn->defn + ic + LINE_OFFSET + 1).data, BASE, base_result1, PAD), 'R');
                (*line_count)++;
            }
			/* check for the symbol in instruction hashtable */
            else if ((ihn = fetchIntFromHashTab(op, inst_symtab)))
            {
				fprintf(obj, "%s\t%s\t%c\n", to_base((*line_count) + LINE_OFFSET, BASE, base_result, NO_PAD), to_base(num2data(ihn->defn + LINE_OFFSET + 1).data, BASE, base_result1, PAD), 'R');
                (*line_count)++;
            }
			/* check for the symbol in externals hashtable */
            else if ((ihn = fetchIntFromHashTab(op, exttab)))
            {
				fprintf(obj, "%s\t%s\t%c\n", to_base((*line_count) + LINE_OFFSET, BASE, base_result, NO_PAD), to_base(num2data(0).data, BASE, base_result1, PAD), 'E');
				fprintf(ext, "%s\t%s\n", op, to_base(*line_count + LINE_OFFSET, BASE, base_result, NO_PAD));
				(*line_count)++;
            }
            else
            {
                ERROR("Can't find symbol in symbol table or in extern table", org_line_num)
                error_flag = 1;
            }
            op[i] = '{'; /* put back { char for the case this line will repeat */
			/* seek for ! char */
            for (;op[0] != '!'; op++) {};
            op++;

            sum *= ihn->defn;
            for (;op[0] != '}'; op++) {};

			/* make sure it ends with ) */
            if (!(op[0] == '}'))
            {
                ERROR("Wrong format for elaborate addressing", org_line_num)
                error_flag = 1;
                break;
            }
			/* make sure you are not going out of the data array bounds */
            if (sum >= dc)
            {
                ERROR("Given index is out of the data bounds", org_line_num)
                error_flag = 1;
            }
            else
            {
                fprintf(obj, "%s\t%s\t%c\n", to_base((*line_count) + LINE_OFFSET, BASE, base_result, NO_PAD), to_base(data_arr[sum].data, BASE, base_result1, PAD), 'A');
                (*line_count)++;
            }
            break;
	}

}

/* this function checks if the given addressing is valid */
void checkForAdress(instruction_line *inst_line, int line_number)
{
    switch(inst_line->opcode)
    {
        case MOV:
        case ADD:
        case SUB:
            if (inst_line->dest_addr == 0)
            {
                ERROR("Ilegal addressing",line_number)
                error_flag = 1;
            }
            break;
        case NOT:
        case CLR:
        case INC:
        case DEC:
        case JMP:
        case BNE:
        case RED:
            if ((inst_line->dest_addr == 0) || (inst_line->src_addr != 0))
            {
                ERROR("Ilegal addressing",line_number)
                error_flag = 1;
            }
            break;
        case RTS:
        case STOP:
            if ((inst_line->dest_addr != 0) || (inst_line->src_addr != 0))
            {
                ERROR("Ilegal addressing",line_number)
                error_flag = 1;
            }
            break;
        case LEA:
            if ((inst_line->dest_addr == 0) || (inst_line->src_addr == 0))
            {
                ERROR("Ilegal addressing",line_number)
                error_flag = 1;
            }
            break;
        case PRN:
            if (inst_line->src_addr != 0)
            {
                ERROR("Ilegal addressing",line_number)
                error_flag = 1;
            }
            break;
        case JSR:
            if ((inst_line->dest_addr != 1) || (inst_line->src_addr != 0))
            {
                ERROR("Ilegal addressing",line_number)
                error_flag = 1;
            }
            break;

    }
}

/* 
 * @description Do the second parsing for the line, resolving the symbols and parsing the operands
 * @param *file {code_line}
 * @param num_of_lines {int}
 * @param file_path {String} argv argument
 */
int secondPhase(code_line *file, int num_of_lines, char *module_name)
{
	/* temporary arrays for converting to base 6 */
	char base_result[MAX_DIGIT + 1], base_result1[MAX_DIGIT +1], file_name[MAX_FILENAME];
    int i, j, line_count = 0;
	/* pointers for the files */
    FILE *obj, *ext, *ent;
	/* open all the relevant files */
    sprintf(file_name, "%s.obj", module_name);
    obj = fopen(file_name, "w");
    sprintf(file_name, "%s.ext", module_name);
    ext = fopen(file_name, "w");
    sprintf(file_name, "%s.ent", module_name);
    ent = fopen(file_name, "w");
	/* print header */
    fprintf(obj, "%s %s\n", to_base(ic, BASE, base_result, NO_PAD), to_base(dc, BASE, base_result1, NO_PAD));
    line_count++;
    for (i = 0; i < num_of_lines; i++) /* loop over all the code lines */
    {
		/* only if this file need more parsing */
        if (!file[i].done)
        {
			/* check if the addressing is valid */
            checkForAdress(file[i].instruction, file[i].line_number);
			/* parse the operands * repeat times */
            for (j = 0; j < file[i].repeat; j++)
            {
				/* write to file the actual instruction */
                fprintf(obj, "%s\t%s\t%c\n",to_base(line_count + LINE_OFFSET, BASE, base_result, NO_PAD) , to_base(bline2data(*(file[i].instruction)).data, BASE, base_result1, PAD), 'A');
                line_count++;
				/* if there is source operand */
                if (file[i].src_opr)
                {
					/* use the auxilary function to parse the operand */
					parsingOperand(file[i].src_opr, file[i].instruction->src_addr, obj, ext, &line_count, file[i].line_number);
                }
				/* if there is source operand */
                if (file[i].dest_opr)
                {
					/* use the auxilary function to parse the operand */
					parsingOperand(file[i].dest_opr, file[i].instruction->dest_addr, obj, ext, &line_count, file[i].line_number);
                }
            }
        }
    }
	/* write to obj file the data array */
    for (i = 0; i < dc; i++)
    {
        fprintf(obj, "%s\t%s\n", to_base(line_count + LINE_OFFSET, BASE, base_result, NO_PAD), to_base(data_arr[i].data, BASE, base_result1, PAD));
        line_count++;
    }
	/* write to ent file the entries */
    for (i = 0; entry_arr[i]; i++)
    {
		/* try to find the address in the data hashtable */
        if ((ihn = fetchIntFromHashTab(entry_arr[i], data_symtab)))
        {
            fprintf(ent, "%s\t%s\n", entry_arr[i], to_base(ihn->defn + LINE_OFFSET + 1 + ic, BASE, base_result, NO_PAD));
        }
		/* try to find the address in the data hashtable */
        else if((ihn = fetchIntFromHashTab(entry_arr[i], inst_symtab)))
        {
            fprintf(ent, "%s\t%s\n", entry_arr[i], to_base(ihn->defn + LINE_OFFSET + 1, BASE, base_result, NO_PAD));
        }
        else
        {
            fprintf(stderr, "Error, can't find assress for entry %s\n", entry_arr[i]);
            error_flag = 1;
        }
    }
	/* close all the open files */
    fclose(obj);
    fclose(ext);
    fclose(ent);
	
	/* clear the hashtable for the next file */
	for (i = 0; i < HASHSIZE; i++)
	{
		data_symtab[i] = NULL;
		inst_symtab[i] = NULL;
		exttab[i] = NULL;
	}
	/* if there is an error, delete all the files */
    if (error_flag)
    {
        sprintf(file_name, "%s.obj", module_name);
        remove(file_name);
        sprintf(file_name, "%s.ext", module_name);
        remove(file_name);
        sprintf(file_name, "%s.ent", module_name);
        remove(file_name);
    }
	/* if there was'nt any externs */
    else if (extern_counter == 0)
    {
        sprintf(file_name, "%s.ext", module_name);
        remove(file_name);
    }
	/* if there was'nt any externs */
    else if (entry_counter == 0)
    {
        sprintf(file_name, "%s.ent", module_name);
        remove(file_name);
    }
    return 0;
}

/* 
 * @description Register the operations code to hash table
 */
void registerOperations()
{
    ihn = registerIntToHashTab("mov" , MOV, optab);
    ihn = registerIntToHashTab("cmp" , CMP, optab);
    ihn = registerIntToHashTab("add" , ADD, optab);
    ihn = registerIntToHashTab("sub" , SUB, optab);
    ihn = registerIntToHashTab("not" , NOT, optab);
    ihn = registerIntToHashTab("clr" , CLR, optab);
    ihn = registerIntToHashTab("lea" , LEA, optab);
    ihn = registerIntToHashTab("inc" , INC, optab);
    ihn = registerIntToHashTab("dec" , DEC, optab);
    ihn = registerIntToHashTab("jmp" , JMP, optab);
    ihn = registerIntToHashTab("bne" , BNE, optab);
    ihn = registerIntToHashTab("red" , RED, optab);
    ihn = registerIntToHashTab("prn" , PRN, optab);
    ihn = registerIntToHashTab("jsr" , JSR, optab);
    ihn = registerIntToHashTab("rts" , RTS, optab);
    ihn = registerIntToHashTab("stop" , STOP, optab);
}
