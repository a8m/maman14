/*
 * =========================================================================================
 * name        : maman_14/compiler.c
 * author      : Ariel Mashraki, Osnat Izic
 * email       :
 * description :
 * =========================================================================================
 */

#include "common.h"

int main(int argc, char **argv)
{
    
    int i = 0;
    FILE *fp;
    char file_name[MAX_FILE_NAME];
    
    /* init() ==> run init function() {} */
    
    if (argc == 1) {
        printf(NO_FILES);
        return FALSE;
    }
    /*loop over the file names arguments*/
    while (++i < argc) {
        
        code_line *file_lines;/*array of code_line*/
        int count = 0;
        
        /*create the string to open the file*/
        sprintf(file_name, "%s", argv[i]);
        fp = fopen(file_name, "r");
        
        /*if there is any problem with the file*/
        if(!fp) {
            printf(FILE_ERROR, file_name);
            return FALSE;
        }
        
        /* allocate memory for reading the files */
        file_lines =  malloc(sizeof(code_line) * MAX_LINES);
        file_lines[count].line = malloc(sizeof(char) * MAX_LINE_LENGTH);
        
		/* read the file into the array */
        while (fgets(file_lines[count].line, MAX_LINE_LENGTH, fp)) {
            
            file_lines[count].line_number = count + 1;
            printf("line %d: %s",file_lines[count].line_number,file_lines[count].line);
            
            file_lines[++count].line = malloc(sizeof(char) * (MAX_LINE_LENGTH));
        } /* END: read current line */
        
    }/* END: read argc */
    
    
    
    return EXIT;
}

