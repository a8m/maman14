/*
 * =========================================================================================
 * name        : maman_14/parse.h
 * author      : Ariel Mashraki, Osnat Izic
 * =========================================================================================
 */
#define MAX_ARR_SIZE 1000
#define LINE_OFFSET 99
#define MAX_FILENAME 20
#define PAD 1
#define NO_PAD 0
#define BASE 8

#define MOV 0
#define CMP 1
#define ADD 2
#define SUB 3
#define NOT 4
#define CLR 5
#define LEA 6
#define INC 7
#define DEC 8
#define JMP 9
#define BNE 10
#define RED 11
#define PRN 12
#define JSR 13
#define RTS 14
#define STOP 15

/* 
 * @description Do the first parsing to the line, 
 * and leaving for the second parsing to check for the symbols addresses and parsing the operands 
 * @param *file {code_line}
 * @param num_of_lines {int}
 */
int firstPhase(code_line *file, int num_of_lines);
/* 
 * @description Do the second parsing for the line, resolving the symbols and parsing the operands
 * @param *file {code_line}
 * @param num_of_lines {int}
 * @param file_path {String} argv argument
 */
 int secondPhase(code_line *file, int num_of_lines, char *module_name);
/* 
 * @description Register the operations code to hash table
 */
void registerOperations();
