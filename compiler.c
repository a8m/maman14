/*
 * =========================================================================================
 * name        : maman_14/compiler.c
 * author      : Ariel Mashraki, Osnat Izic
 * =========================================================================================
 */
#include "compiler.h"

int main(int argc, char **argv)
{
    /* Call interpretation function */
    return startInterpretation(argc, argv);
}


/*
 * @description Called from int main,
 * start do the files interpretation
 * @param argc {int} 
 * @param filesList {Array}
 */
int startInterpretation(int argc, char **filesList) 
{
    int i;
    FILE *fp;
    char fileName[20];
    
    registerOperations();

    for (i = 1; i < argc; i++)
    {
        int count = 0, bufferCounter = BUF_SIZE;
        codeLineObject *numOfLines, *tmp;
        /* create the string to open the file and open it */
        sprintf(fileName, "%s.as", filesList[i]);
        fp = fopen(fileName, "r");
        /* if the files can't be found */
        if (fp == NULL)
        {
            printf("ERROR: can't find file %s\n, Please insert files name without suffix .as", fileName);
            return EXIT;
        }
        
        /* allocate memory for reading the files */
        numOfLines =  malloc(sizeof(codeLineObject) * BUF_SIZE);
        numOfLines[count].line = malloc(sizeof(char) * MAX_LINE);
        /* read the file into the array */
        while (fgets(numOfLines[count].line, MAX_LINE, fp))
        {
             /* if there is a need for reallocation */
            if (count == bufferCounter)
            {
                bufferCounter += BUF_SIZE;
                tmp = realloc(numOfLines, sizeof(codeLineObject) * bufferCounter);
                if (tmp)
                {
                    numOfLines = tmp;
                }
                else
                {
                    printf("%s", "ERROR: reallocating");
                    return EXIT;
                }
            }
            numOfLines[count].lineNumber = count + 1;
            numOfLines[++count].line = malloc(sizeof(char) * MAX_LINE);
        }
        /* free the extra line */
        free(numOfLines[count].line);

        /* call the firstPhase */
        firstPhase(numOfLines, count);
        /* call the secondPhase */
        secondPhase(numOfLines, count, filesList[i]);
        /* relocation file lines */
        free(numOfLines);
        /* close the file */
        fclose(fp);

        SYSTEM_DONE(fileName);

    }
    return FALSE;
}
