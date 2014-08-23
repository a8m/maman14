/*
 * =========================================================================================
 * name        : maman_14/systemData.h
 * author      : Ariel Mashraki, Osnat Izic
 * =========================================================================================
 */


#ifndef SYSTEMDATA_H_
#define SYSTEMDATA_H_

/* 
 * global define and const unique for 
 * this operation system
 */
#define EXIT -1
#define FALSE 0
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
 
#define COMB_WIDTH 2
#define DREG_WIDTH 3
#define DADDR_WIDTH 2
#define SREG_WIDTH 3
#define SADDR_WIDTH 2
#define OPCODE_WIDTH 4
#define TYPE_WIDTH 1
#define DBL_WIDTH 1
#define RSVD_WIDTH 3

#define COMB_OFFSET 0
#define DREG_OFFSET 2
#define DADDR_OFFSET 5
#define SREG_OFFSET 7
#define SADDR_OFFSET 10
#define OPCODE_OFFSET 12
#define TYPE_OFFSET 16
#define DBL_OFFSET 17
#define RSVD_OFFSET 18

/* type for 20 bits data */
typedef struct dline {
    unsigned int data : 20;
} data_line;

/* type for an instruction line (using bit fields) */
typedef struct instrline {
    unsigned int comb      : COMB_WIDTH;
    unsigned int destReg  : DREG_WIDTH;
    unsigned int destAddr : DADDR_WIDTH;
    unsigned int srcReg   : SREG_WIDTH;
    unsigned int srcAddr  : SADDR_WIDTH;
    unsigned int opcode    : OPCODE_WIDTH;
    unsigned int type      : TYPE_WIDTH;
    unsigned int dbl       : DBL_WIDTH;
    unsigned int rsvd      : RSVD_WIDTH;
} instructionLineObject;

/* type for a whole line, with relevant fields helping the parsing */
typedef struct cline {
    char *line;
    instructionLineObject *instruction;
    unsigned char oprRepeat;
    unsigned int lineNumber;
    char *srcOpr;
    char *destOpr;
    char done;
} codeLineObject;


/*
 * @decription returning data_line containing the char in the argument 
 */
 data_line char2data(char ch);

/* 
 * @description returning data_line containing the (boolean) instruction line in the argument 
 */
data_line bline2data(instructionLineObject bl);

/* 
 * @description returning data_line containing the int in the argument with the sign bit 
 */
data_line num2data(int num);

/* 
 * @description clearing the values of an instruction line 
 */
 void registerInstructionLine(instructionLineObject *il);

/* 
 * @descriptoin after allocating memory, the data need to be clear from garbage 
 */
void registerDataLine(data_line *dl);

#endif
