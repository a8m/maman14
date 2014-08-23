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

#endif
