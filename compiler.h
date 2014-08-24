/*
 * =========================================================================================
 * name        : maman_14/compiler.h
 * author      : Ariel Mashraki, Osnat Izic
 * =========================================================================================
 */

#ifndef COMPILER_H_
#define COMPILER_H_

#include <stdio.h>
#include <stdlib.h>
#include "hashTable.h"
#include "systemData.h"
#include "parse.h"
 
#define MAX_LINE 80
#define BUF_SIZE 100

int startInterpretation(int argc, char **filesList);


 /* 
 * @descipriton 
 * MACRO - SYSTEM_DONE printing, print succesing message for each file
 * @param msg {String}
 * @example[usage]
 * SYSTEM_DONE(FILE_NAME)
 */
#define SYSTEM_DONE(FILE_NAME) printf("\nSYSTEM: Interpretation Status for %s - DONE.\n", FILE_NAME);

#endif
