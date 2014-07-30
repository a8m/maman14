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
    
    if (argc == 1) {
        printf(NO_FILES);
        return FALSE;
    }
    /*loop over the file names arguments*/
    while (++i < argc) {
        /*create the string to open the file*/
        sprintf(file_name, "%s.as", argv[i]);
        fp = fopen(file_name, "r");
        
        /*if there is any problem with the file*/
        if(!fp) {
            printf(FILE_ERROR, file_name);
            return FALSE;
        }
        
        /* allocate memory for second phase fprintf*/
        
        printf("success %s", file_name);
    }
    
    
    
    return EXIT;
}

