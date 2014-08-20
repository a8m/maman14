#define MAX_ARR_SIZE 1000
#define LINE_OFFSET 99
#define MAX_FILENAME 20
#define PAD 1
#define NO_PAD 0
#define BASE 6

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

/* this function is doing the first parsing to the line, and leaving for the second parsing to check for the symbols addresses and parsing the operands */
int first_parsing(code_line *file, int num_of_lines);
/* this function is doing the second parsing for the line, resolving the symbols and parsing the operands */
int second_parsing(code_line *file, int num_of_lines, char *module_name);
/* this function is called only in start of day, and loading all the opcodes into the hashtable */
void init_op_table();
