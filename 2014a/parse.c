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
data_line dataArrayList[MAX_ARR_SIZE];
/* linked list for the symbols addresses of data pointers */
static intLinkedListNode *dataSymbolsList[HASHSIZE];
/* linked list for the symbols addresses of instructions pointers */
static intLinkedListNode *instructionsSymbolsList[HASHSIZE];
/* linked list for the external commands */
static intLinkedListNode *extrnalCmdList[HASHSIZE];
/* linked list for the opcode replacements */
static intLinkedListNode *operationList[HASHSIZE];
/* temporary node for lookuping in the linkde lists */
intLinkedListNode *tempNode, *tempDynamicNode;
/* array for entries */
char *entryArrayList[MAX_ARR_SIZE];
/* counters for entries end externals */
int counterForEntry, counterForExtern;
/* general counter, and data and instructions counter */
int i, ic, dc;
/* flags for decide if to create the files */
char flagForError = 0, flagForExtern = 0, flagForEntry = 0;

/* this function is checking if there is a symbol. if yes, its returning a pointer to it, if not, returns null */
char *fetchSymbol(codeLineObject *codeLine)
{
    char *tmp = (codeLine->line);
	/* while its not end of line */
    while (*(codeLine->line) != '\0')
    {
		/* if there is a : char, its probably has symbol */
        if (*(codeLine->line) == ':')
        {
			/* put null char at end of symbol */
            *((codeLine->line)++) = '\0';
			/* make sure its starts with a char */
            if (!isalpha(tmp[0]))
            {
                PRINT_ERROR("Symbol should start with a char", codeLine->line_number)
                flagForError = 1;
            }
            return tmp;
        }
        (codeLine->line)++;
    }
    codeLine->line = tmp;
    return NULL;
}

/* this function extract the data from a data command */
void parseData(codeLineObject *codeLine)
{
    int sign;
    int sum;
	char expecting_number = 0;
	/* while its not end of line */
    while ((*codeLine).line[0] != '\n')
    {
		/* remove the spaces from the begining of the line */
		trim(&((*codeLine).line));
		/* check sign */
        if (codeLine->line[0] == '-')
        {
            sign = -1;
            (codeLine->line)++;
        }
		/* make sure its start with digit */
        if (!isdigit((*codeLine).line[0]))
        {
            PRINT_ERROR("Wrong .data format, expecting number", (*codeLine).line_number)
			flagForError = 1;
        }
        else
        {
            sum = 0;
            sign = 1;
            while (isdigit((*codeLine).line[0]))
            {
                sum *= 10;
                sum += ((*codeLine).line[0]) - '0';
                (codeLine->line)++;
            }
			/* add the number to data arr */
            dataArrayList[dc++] = num2data(sum * sign);
        }
		
		trim(&((*codeLine).line));
		/* if there is more numbers */
        if (codeLine->line[0] == ',')
		{
			expecting_number = 1;
			(codeLine->line)++;
		}
		else
		{
			expecting_number = 0;
		}
    }
	/* if the line ended and we still waiting for another number */
	if (expecting_number == 1)
	{
		PRINT_ERROR("Expecting number after \',\'", (*codeLine).line_number)
		flagForError = 1;
	}

}

/* this function extract the string from a string command */
void parseString(codeLineObject *codeLine)
{
    trim(&(codeLine->line));
	/* make sure its starts with " */
    if (!(codeLine->line[0] == '\"'))
    {
        PRINT_ERROR(".string argument must start with \"", (*codeLine).line_number)
		flagForError = 1;
        return;
    }

    (codeLine->line)++;
	/* while its not reached the second " */
    while ((codeLine->line[0] != '\"') && (codeLine->line[0] != '\0'))
    {
		/* add the new string to the array */
        dataArrayList[dc++] = char2data(codeLine->line[0]);
        (codeLine->line)++;
    }

	/* at end, make sure it ends with " */
    if (codeLine->line[0] == '\"')
    {
        dataArrayList[dc++] = char2data('\0');
    }
    else
    {
        PRINT_ERROR(".string argument must end with \"", (*codeLine).line_number)
		flagForError = 1;
        return;
    }
    
}

/* this function extract the entry from a entry command */
void parseEntry(codeLineObject *codeLine)
{
	/* skip on tabs and spaces */
	codeLine->line = strtok(codeLine->line, " \t\n");
	/* if there is no entry */
	if (!codeLine->line)
	{
        PRINT_ERROR("No value found after entry command", (*codeLine).line_number)
		flagForError = 1;
		return;
	}
	/* add entry to entries array */
	entryArrayList[counterForEntry++] = strCopy(codeLine->line);
	codeLine->line = strtok(NULL, " \t\n");
	/* make sure there is no other words */
	if (codeLine->line)
	{
        PRINT_ERROR("Unexpected words in entry command", (*codeLine).line_number)
		flagForError = 1;
	}
}

/* this function extract the entry from a entry command */
void parseExtern(codeLineObject *codeLine)
{
	/* skip on tabs and spaces */
	codeLine->line = strtok(codeLine->line, " \t\n");
	/* if there is no extern */
	if (!codeLine->line)
	{
        PRINT_ERROR("No value found after extern command", (*codeLine).line_number)
		flagForError = 1;
		return;
	}
	/* add to the extern hashtable with value 0 */
    registerIntToHashTab(codeLine->line, '0', extrnalCmdList);
	counterForExtern++;
	codeLine->line = strtok(NULL, " \t\n");
	/* make sure there is no other words */
	if (codeLine->line)
	{
        PRINT_ERROR("Unexpected words in extern command", (*codeLine).line_number)
		flagForError = 1;
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
void parsingInstruction(codeLineObject *codeLine)
{
	/* count how many instruction lines this line using, including repeats */
    int localCounter = 0;
	char numOfValidOperands;
	/* isolate the opcode */
	codeLine->line = strtok(codeLine->line, "/");
	/* find it in the opcode hashtable */
	tempNode = fetchIntFromHashTab(codeLine->line, operationList);
	/* if its not a valid opcode */
	if (!tempNode)
	{
		/* check if its a comment or empty line */
        if ((codeLine->line[0] == ';') || (codeLine->line[0] = '\n'))
        {
            codeLine->done = 1;
            return;
        }
        PRINT_ERROR("Ilegal instruction", (*codeLine).line_number)
		flagForError = 1;
		return;
	}
	/* use the auxilary function to count the expected operands */
	numOfValidOperands = checkExpOperands(codeLine->line);
	codeLine->instruction = (instructionLineObject *) malloc(sizeof(instructionLineObject) * 8);
	/* clear the instruction line */
    registerInstructionLine(codeLine->instruction);
    localCounter++;
	codeLine->instruction->opcode = tempNode->defn;
	/* seek for null char */
	while (codeLine->line[0] != '\0')
    {
        (codeLine->line)++;
    }
    (codeLine->line)++;

	/* check for the type field, and comb field */
	switch (codeLine->line[0])
	{
		case '0': /* type is 0, comb irelevant */
			codeLine->instruction->type = 0;
			break;
		case '1': /* type is 1, need to check comb value */
			codeLine->instruction->type = 1;
			(codeLine->line)++;
			if (!(codeLine->line[0] == '/'))
			{
				PRINT_ERROR("Expecting comb values after type 1", (*codeLine).line_number)
				flagForError = 1;
				return;
			}
			(codeLine->line)++;
			switch (codeLine->line[0])
			{
				case '0': /* keep default value */
					break;
				case '1':
					codeLine->instruction->comb |= 2; /* left bit of comb should be 1 */
					break;
				default:
					PRINT_ERROR("Ilegal value for comb", (*codeLine).line_number)
					flagForError = 1;
					return;
			}
			(codeLine->line)++;
			/* expecting / */
			if (!(codeLine->line[0] == '/'))
			{
				PRINT_ERROR("Expecting cobm values after type 1", (*codeLine).line_number)
				flagForError = 1;
				return;
			}
			(codeLine->line)++;
			switch (codeLine->line[0])
			{
				case '0':
					break; /* keep default value */
				case '1':
					codeLine->instruction->comb |= 1; /* right bit of comb should be 1 */
					break;
				default:
					PRINT_ERROR("Ilegal value for comb", (*codeLine).line_number)
					flagForError = 1;
					return;
			}
            break;
		default:
			PRINT_ERROR("Ilegal value for comb", (*codeLine).line_number)
			flagForError = 1;
			return;
	}
	(codeLine->line)++;
	if (!(codeLine->line[0] == ','))
	{
		PRINT_ERROR("Expecting ',' after type and comb values", (*codeLine).line_number)
		flagForError = 1;
		return;
	}
	(codeLine->line)++;

    codeLine->repeat = codeLine->line[0] - '0';
   	/* check for repeat number, dbl bit */
    if(codeLine->repeat < 0 || codeLine->repeat > 1)
	{
		PRINT_ERROR("Invalid command dbl value. Valid values are 0,1", (*codeLine).line_number);
		return;
	}
	else
	{
		codeLine->instruction->dbl = codeLine->repeat;
	}
	(codeLine->line)++;
	trim(&(codeLine->line));
    
	 /*read the expected number of operands */
    codeLine->line = strtok(codeLine->line, " \t,\n");
	while (numOfValidOperands-- > 0)
	{
        if (numOfValidOperands == 1)
        {
			/* its the first operand */
            codeLine->src_opr = strCopy(codeLine->line);
        }
        else
        {
			/* its the second operand */
            codeLine->dest_opr = strCopy(codeLine->line);
        }
		/* check for the type of the oprands and decide how much ic should be incremented */
        switch (codeLine->line[0])
		{
			case '#':
                localCounter++;
                if (numOfValidOperands == 1)
                {
                    codeLine->instruction->src_addr = 0;
                }
                else
                {
                    codeLine->instruction->dest_addr = 0;
                }
                break;
			case 'r':
                (codeLine->line)++;
                if ((codeLine->line[0] >= '0') && (codeLine->line[0] <='7'))
                {
                    if (numOfValidOperands == 1)
                    {
                        codeLine->instruction->src_reg = codeLine->line[0] - '0';
                        free(codeLine->src_opr);
                        codeLine->instruction->src_addr = 3;
                    }
                    else
                    {
                        codeLine->instruction->dest_reg = codeLine->line[0] - '0';
                        free(codeLine->dest_opr);
                        codeLine->instruction->dest_addr = 3;
                    }
                    break;
                }
		    default:
                localCounter++;
                if (strstr(codeLine->line, "{!"))
                {
                    localCounter++;
                    if (numOfValidOperands == 1)
                    {
                        codeLine->instruction->src_addr = 2;
                    }
                    else
                    {
                        codeLine->instruction->dest_addr = 2;
                    }

                }
                else
                {
                    if (numOfValidOperands == 1)
                    {
                        codeLine->instruction->src_addr = 1;
                    }
                    else
                    {
                        codeLine->instruction->dest_addr = 1;
                    }
                }
		}
        codeLine->line = strtok(NULL, " \t,\n");
	}
    ic += (localCounter * codeLine->repeat);
	
}

/* this function parsing the commands line (string, data, extern and entry) */
void parsingCmd(codeLineObject *codeLine, char *symbol)
{
	/* if string */
	if (strncmp(codeLine->line, ".string", sizeof(".string") - 1) == 0)
	{
		/* if there is a symbol */
		if (symbol)
		{
			tempNode = registerIntToHashTab(symbol, dc, dataSymbolsList);
		}
        codeLine->line += (sizeof(".string") - 1);
		/* use the auxilary function to extract the string */
		parseString(codeLine);
	}
	/* if data */
	else if (strncmp((*codeLine).line, ".data", sizeof(".data") - 1) == 0)
	{
		if (symbol)
		{
			tempNode = registerIntToHashTab(symbol, dc, dataSymbolsList);
		}
        codeLine->line += (sizeof(".data") - 1);
		/* use the auxilary function to extract the data */
		parseData(codeLine);
	}
	/* if entry */
	else if (strncmp(codeLine->line, ".entry", sizeof(".entry") - 1) == 0)
	{
        codeLine->line += (sizeof(".entry") - 1);
		/* use the auxilary function */
		parseEntry(codeLine);
	}
	/* if extern */
	else if (strncmp(codeLine->line, ".extern", sizeof(".extern") - 1) == 0)
	{
        codeLine->line += (sizeof(".extern") - 1);
		/* use the auxilary function */
		parseExtern(codeLine);
	}
	else
	{
		PRINT_ERROR("Unrecognized command", (*codeLine).line_number)
		flagForError = 1;
	}
    codeLine->done = 1;
}

/* 
 * @description Do the first parsing to the line, 
 * and leaving for the second parsing to check for the symbols addresses and parsing the operands 
 * @param *file {codeLineObject}
 * @param numberOfLine {int}
 */
int firstPhase(codeLineObject *file, int numberOfLine)
{
    char *symbol;
	flagForError = 0;
	ic = 0;
	dc = 0;
	counterForExtern = 0;
	counterForEntry = 0;

    for (i = 0; i < numberOfLine; i++) /* loop over all the code lines */
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
                tempNode = registerIntToHashTab(symbol, ic, instructionsSymbolsList);
            }
			parsingInstruction(&file[i]);
		}
    }

    return 0;
}

/* this function is for the second parsing, parsing each operand */
void parsingOperand(char *op, int addr, FILE *obj, FILE *ext, int *line_count, int org_line_num)
{
	/* 2 temporary arrays to get the base 8 results */
	char resultBaseArray[MAX_DIGIT + 1], tempBaseArray[MAX_DIGIT + 1], tempChar[10];
	int sign = 1, sum = 0, cIndex = 0;
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
			fprintf(obj, "%s\t%s\t%c\n",baseConvertor((*line_count) + LINE_OFFSET, BASE, resultBaseArray, NO_PAD) , baseConvertor(num2data(sum * sign).data, BASE, tempBaseArray, PAD), 'A');
            (*line_count)++;
			break;
		case 1: /* its symbol */
			/* check in the data hashtable */
			if ((tempNode = fetchIntFromHashTab(op, dataSymbolsList)))
			{
				/* write the symbol address to the obj file */
				fprintf(obj, "%s\t%s\t%c\n", baseConvertor((*line_count) + LINE_OFFSET, BASE, resultBaseArray, NO_PAD), baseConvertor(num2data(tempNode->defn + ic + LINE_OFFSET + 1).data, BASE, tempBaseArray, PAD), 'R');
                (*line_count)++;
			}
			/* check in the instruction hashtable */
            else if ((tempNode = fetchIntFromHashTab(op, instructionsSymbolsList)))
            {
				/* write the symbol address to the obj file */
				fprintf(obj, "%s\t%s\t%c\n", baseConvertor((*line_count) + LINE_OFFSET, BASE, resultBaseArray, NO_PAD), baseConvertor(num2data(tempNode->defn + LINE_OFFSET + 1).data, BASE, tempBaseArray, PAD), 'R');
                (*line_count)++;
            }
			/* check in the externals hashtable */
            else if ((tempNode = fetchIntFromHashTab(op, extrnalCmdList)))
			{
				/* write 0 to the obj file */
				fprintf(obj, "%s\t%s\t%c\n", baseConvertor((*line_count)+ LINE_OFFSET, BASE, resultBaseArray, NO_PAD), baseConvertor(num2data(0).data, BASE, tempBaseArray, PAD), 'E');
				/* add line to the ext file */
				fprintf(ext, "%s\t%s\n", op, baseConvertor(*line_count + LINE_OFFSET, BASE, resultBaseArray, NO_PAD));
				(*line_count)++;
			}
            else
            {
                PRINT_ERROR("Can't find symbol in symbol table or in extern table", org_line_num)
                flagForError = 1;
            }
            break;
        case 2: /* Elaborate addressing */
			/* seek the { char */
            for (i = 0 ;op[i] != '{'; i++);
			/* add there \0 char */
            op[i] = '\0';
			/* check for the symbol in data hashtable */
            if ((tempNode = fetchIntFromHashTab(op, dataSymbolsList)))
            {
				fprintf(obj, "%s\t%s\t%c\n", baseConvertor((*line_count) + LINE_OFFSET, BASE, resultBaseArray, NO_PAD), baseConvertor(num2data(tempNode->defn + ic + LINE_OFFSET + 1).data, BASE, tempBaseArray, PAD), 'R');
                (*line_count)++;
            }
			/* check for the symbol in instruction hashtable */
            else if ((tempNode = fetchIntFromHashTab(op, instructionsSymbolsList)))
            {
				fprintf(obj, "%s\t%s\t%c\n", baseConvertor((*line_count) + LINE_OFFSET, BASE, resultBaseArray, NO_PAD), baseConvertor(num2data(tempNode->defn + LINE_OFFSET + 1).data, BASE, tempBaseArray, PAD), 'R');
                (*line_count)++;
            }
			/* check for the symbol in externals hashtable */
            else if ((tempNode = fetchIntFromHashTab(op, extrnalCmdList)))
            {
				fprintf(obj, "%s\t%s\t%c\n", baseConvertor((*line_count) + LINE_OFFSET, BASE, resultBaseArray, NO_PAD), baseConvertor(num2data(0).data, BASE, tempBaseArray, PAD), 'E');
				fprintf(ext, "%s\t%s\n", op, baseConvertor(*line_count + LINE_OFFSET, BASE, resultBaseArray, NO_PAD));
				(*line_count)++;
            }
            else
            {
                PRINT_ERROR("Can't find symbol in symbol table or in extern table", org_line_num)
                flagForError = 1;
            }
            op[i] = '{'; /* put back { char for the case this line will repeat */
			/* seek for ! char */
            for (;op[0] != '!'; op++) {};
            op++;

            for (;op[0] != '}'; op++) {
            	tempChar[cIndex]= op[0];
				cIndex++;
            };
            tempChar[cIndex] = '\0';
            tempDynamicNode = fetchIntFromHashTab(tempChar, instructionsSymbolsList);
           	if(tempDynamicNode == NULL) 
           	{
           		sum *= tempNode->defn;
           	} 
           	else
           	{
           		sum *= tempDynamicNode->defn;
           	}
			/* make sure it ends with ) */
            if (!(op[0] == '}'))
            {
                PRINT_ERROR("Wrong format for elaborate addressing", org_line_num)
                flagForError = 1;
                break;
            }
			/* make sure you are not going out of the data array bounds */
            if (sum >= dc)
            {
                PRINT_ERROR("Given index is out of the data bounds", org_line_num)
                flagForError = 1;
            }
            else
            {
                fprintf(obj, "%s\t%s\t%c\n", baseConvertor((*line_count) + LINE_OFFSET, BASE, resultBaseArray, NO_PAD), baseConvertor(dataArrayList[sum].data, BASE, tempBaseArray, PAD), 'A');
                (*line_count)++;
            }
            break;
	}

}

/* this function checks if the given addressing is valid */
void checkForAdress(instructionLineObject *inst_line, int line_number)
{
    switch(inst_line->opcode)
    {
        case MOV:
        case ADD:
        case SUB:
            if (inst_line->dest_addr == 0)
            {
                PRINT_ERROR("Ilegal addressing",line_number)
                flagForError = 1;
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
                PRINT_ERROR("Ilegal addressing",line_number)
                flagForError = 1;
            }
            break;
        case RTS:
        case STOP:
            if ((inst_line->dest_addr != 0) || (inst_line->src_addr != 0))
            {
                PRINT_ERROR("Ilegal addressing",line_number)
                flagForError = 1;
            }
            break;
        case LEA:
            if ((inst_line->dest_addr == 0) || (inst_line->src_addr == 0))
            {
                PRINT_ERROR("Ilegal addressing",line_number)
                flagForError = 1;
            }
            break;
        case PRN:
            if (inst_line->src_addr != 0)
            {
                PRINT_ERROR("Ilegal addressing",line_number)
                flagForError = 1;
            }
            break;
        case JSR:
            if ((inst_line->dest_addr != 1) || (inst_line->src_addr != 0))
            {
                PRINT_ERROR("Ilegal addressing",line_number)
                flagForError = 1;
            }
            break;

    }
}

/* 
 * @description Do the second parsing for the line, resolving the symbols and parsing the operands
 * @param *file {codeLineObject}
 * @param numberOfLine {int}
 * @param file_path {String} argv argument
 */
int secondPhase(codeLineObject *file, int numberOfLine, char *filePath)
{
	/* temporary arrays for converting to base 8 */
	char resultBaseArray[MAX_DIGIT + 1], tempBaseArray[MAX_DIGIT +1], fileName[MAX_FILENAME];
    int i, j, line_count = 0;
	/* pointers for the files */
    FILE *obj, *ext, *ent;
	/* open all the relevant files */
    sprintf(fileName, "%s.obj", filePath);
    obj = fopen(fileName, "w");
    sprintf(fileName, "%s.ext", filePath);
    ext = fopen(fileName, "w");
    sprintf(fileName, "%s.ent", filePath);
    ent = fopen(fileName, "w");
	/* print header */
    fprintf(obj, "%s %s\n", baseConvertor(ic, BASE, resultBaseArray, NO_PAD), baseConvertor(dc, BASE, tempBaseArray, NO_PAD));
    line_count++;
    for (i = 0; i < numberOfLine; i++) /* loop over all the code lines */
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
                fprintf(obj, "%s\t%s\t%c\n",baseConvertor(line_count + LINE_OFFSET, BASE, resultBaseArray, NO_PAD) , baseConvertor(bline2data(*(file[i].instruction)).data, BASE, tempBaseArray, PAD), 'A');
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
        fprintf(obj, "%s\t%s\n", baseConvertor(line_count + LINE_OFFSET, BASE, resultBaseArray, NO_PAD), baseConvertor(dataArrayList[i].data, BASE, tempBaseArray, PAD));
        line_count++;
    }
	/* write to ent file the entries */
    for (i = 0; entryArrayList[i]; i++)
    {
		/* try to find the address in the data hashtable */
        if ((tempNode = fetchIntFromHashTab(entryArrayList[i], dataSymbolsList)))
        {
            fprintf(ent, "%s\t%s\n", entryArrayList[i], baseConvertor(tempNode->defn + LINE_OFFSET + 1 + ic, BASE, resultBaseArray, NO_PAD));
        }
		/* try to find the address in the data hashtable */
        else if((tempNode = fetchIntFromHashTab(entryArrayList[i], instructionsSymbolsList)))
        {
            fprintf(ent, "%s\t%s\n", entryArrayList[i], baseConvertor(tempNode->defn + LINE_OFFSET + 1, BASE, resultBaseArray, NO_PAD));
        }
        else
        {
            fprintf(stderr, "PRINT_ERROR, can't find assress for entry %s\n", entryArrayList[i]);
            flagForError = 1;
        }
    }
	/* close all the open files */
    fclose(obj);
    fclose(ext);
    fclose(ent);
	
	/* clear the hashtable for the next file */
	for (i = 0; i < HASHSIZE; i++)
	{
		dataSymbolsList[i] = NULL;
		instructionsSymbolsList[i] = NULL;
		extrnalCmdList[i] = NULL;
	}
	/* if there is an PRINT_ERROR, delete all the files */
    if (flagForError)
    {
        sprintf(fileName, "%s.obj", filePath);
        remove(fileName);
        sprintf(fileName, "%s.ext", filePath);
        remove(fileName);
        sprintf(fileName, "%s.ent", filePath);
        remove(fileName);
    }
	/* if there was'nt any externs */
    else if (counterForExtern == 0)
    {
        sprintf(fileName, "%s.ext", filePath);
        remove(fileName);
    }
	/* if there was'nt any externs */
    else if (counterForEntry == 0)
    {
        sprintf(fileName, "%s.ent", filePath);
        remove(fileName);
    }
    return 0;
}

/* 
 * @description Register the operations code to hash table
 */
void registerOperations()
{
    tempNode = registerIntToHashTab("mov" , MOV, operationList);
    tempNode = registerIntToHashTab("cmp" , CMP, operationList);
    tempNode = registerIntToHashTab("add" , ADD, operationList);
    tempNode = registerIntToHashTab("sub" , SUB, operationList);
    tempNode = registerIntToHashTab("not" , NOT, operationList);
    tempNode = registerIntToHashTab("clr" , CLR, operationList);
    tempNode = registerIntToHashTab("lea" , LEA, operationList);
    tempNode = registerIntToHashTab("inc" , INC, operationList);
    tempNode = registerIntToHashTab("dec" , DEC, operationList);
    tempNode = registerIntToHashTab("jmp" , JMP, operationList);
    tempNode = registerIntToHashTab("bne" , BNE, operationList);
    tempNode = registerIntToHashTab("red" , RED, operationList);
    tempNode = registerIntToHashTab("prn" , PRN, operationList);
    tempNode = registerIntToHashTab("jsr" , JSR, operationList);
    tempNode = registerIntToHashTab("rts" , RTS, operationList);
    tempNode = registerIntToHashTab("stop" , STOP, operationList);
}
