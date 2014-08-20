#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"
#include "assembler.h"
#include "data.h"
#include "parse.h"

int main(int argc, char **argv)
{
    int i;
    FILE *fp;
    char file_name[20];
	
	/* call the function to intialize the opcode hashtable */
    init_op_table();

	/* loop over the file names arguments */
    for (i = 1; i < argc; i++)
    {
        int count = 0, buf_count = BUF_SIZE;
        code_line *file_lines, *tmp;
		/* create the string to open the file and open it */
        sprintf(file_name, "%s.as", argv[i]);
        fp = fopen(file_name, "r");
		/* if the files can't be found */
        if (fp == NULL)
        {
            printf("Error: can't find file %s\n", file_name);
            return -1;
        }
		
		/* allocate memory for reading the files */
        file_lines =  malloc(sizeof(code_line) * BUF_SIZE);
        file_lines[count].line = malloc(sizeof(char) * MAX_LINE);
		/* read the file into the array */
        while (fgets(file_lines[count].line, MAX_LINE, fp))
        {
			 /* if there is a need for reallocation */
            if (count == buf_count)
            {
                buf_count += BUF_SIZE;
                tmp = realloc(file_lines, sizeof(code_line) * buf_count);
                if (tmp)
                {
                    file_lines = tmp;
                }
                else
                {
                    printf("%s", "error reallocating");
                    return -1;
                }
            }
            file_lines[count].line_number = count + 1;
            file_lines[++count].line = malloc(sizeof(char) * MAX_LINE);
        }
		/* free the extra line */
        free(file_lines[count].line);

		/* call the first_parsing function from parse.c */
        first_parsing(file_lines, count);
		/* call the second_parsing function from parse.c */
        second_parsing(file_lines, count, argv[i]);
		/* free the memory of the file lines */
        free(file_lines);
		/* close the file */
        fclose(fp);

    }
    return 0;
}

