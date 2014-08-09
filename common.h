/*
 * =========================================================================================
 * name        : maman_14/common.h
 * author      : Ariel Mashraki, Osnat Izic
 * email       :
 * description :
 * =========================================================================================
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "parser.h"
#include "utils.h"

/* >>>>>>>>>>>>>>>>>>>>>>>>> GLOBAL DEFINE >>>>>>>>>>>>>>>>>>>>>>>>> */
#define EXIT                        1
#define TRUE                        1
#define FALSE                       0
#define MAX_FILE_NAME               20
#define MAX_LINE_LENGTH             80
#define MAX_LINES                   100


/* >>>>>>>>>>>>>>>>>>>>>>>>>> FILE ERRORS >>>>>>>>>>>>>>>>>>>>>>>>>> */
#define NO_FILES                    "Error: You have to add an input \'.as\' files as an arguments\n"
#define FILE_ERROR                  "Error: Can't find file %s\n"
#define LABEL_ERR                   "Error: Symbols should start with uppercase or lowercase char\n"


#endif
