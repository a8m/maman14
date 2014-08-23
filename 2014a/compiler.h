/*
 * =========================================================================================
 * name        : maman_14/compiler.h
 * author      : Ariel Mashraki, Osnat Izic
 * =========================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"
#include "data.h"
#include "parse.h"
 
#define MAX_LINE 80
#define BUF_SIZE 100

int startInterpretation(int argc, char **filesList);
