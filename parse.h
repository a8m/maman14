/*
 * =========================================================================================
 * name        : maman_14/parse.h
 * author      : Ariel Mashraki, Osnat Izic
 * =========================================================================================
 */

#ifndef PARSE_H_
#define PARSE_H_

/* 
 * @description Do the first parsing to the line, 
 * and leaving for the second parsing to check for the symbols addresses and parsing the operands 
 * @param *file {codeLineObject}
 * @param numberOfLine {int}
 */
int firstPhase(codeLineObject *file, int numberOfLine);
/* 
 * @description Do the second parsing for the line, resolving the symbols and parsing the operands
 * @param *file {codeLineObject}
 * @param numberOfLine {int}
 * @param file_path {String} argv argument
 */
 int secondPhase(codeLineObject *file, int numberOfLine, char *filePath);
/* 
 * @description Register the operations code to hash table
 */
void registerOperations();

/*
 * Checks whether code_line have symbol
 * if there, return pointer to it, else null
 * @param code_line {String}
 * @return String/NULL
 */
char *fetchSymbol(codeLineObject *codeLine);

/*
 * @description parse codeLine, throw errors if needed
 * @param codeLine {codeLineObject}
 */
void parseData(codeLineObject *codeLine);

/*
 * @description parse .string in codeLine, throw errors if needed
 * @param codeLine {codeLineObject}
 */
void parseString(codeLineObject *codeLine);

/*
 * @description parse .entry in codeLine, throw errors if needed
 * @param codeLine {codeLineObject}
 */
void parseEntry(codeLineObject *codeLine);

/*
 * @description parse .extern in codeLine, throw errors if needed
 * @param codeLine {codeLineObject}
 */
void parseExtern(codeLineObject *codeLine);

/*
 * @description checkExpOperands
 * @param *s {String}
 * @return char
 */
char checkExpOperands(char *s);

/*
 * @description parsing instruction on first phase
 * @param *codeLine {codeLineObject}
 */
void parsingInstruction(codeLineObject *codeLine);

/*
 * @description parsing command on first phase,
 * use parseString, parseData, parseExtern, parseEntry helpers
 * @param *codeLine {codeLineObject}
 */
void parsingCmd(codeLineObject *codeLine, char *symbol);

/*
 * @description parse operand
 * @param *op {String}
 * @param addr {int}
 * @param *obj {FILE}
 * @param *ext {FILE}
 * @param *lineCount {*int}
 * @param orgLineNum {int}
 */
void parsingOperand(char *op, int addr, FILE *obj, FILE *ext, int *lineCount, int orgLineNum);

/*
 * @description check for address
 * @param *instLine {instructionLineObject}
 * @param lineNumber {int}
 */
void checkForAdress(instructionLineObject *instLine, int lineNumber);

#endif

