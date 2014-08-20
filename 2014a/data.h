#define COMB_WIDTH 2
#define DREG_WIDTH 3
#define DADDR_WIDTH 2
#define SREG_WIDTH 3
#define SADDR_WIDTH 2
#define OPCODE_WIDTH 4
#define TYPE_WIDTH 1
#define RSVD_WIDTH 3

#define COMB_OFFSET 0
#define DREG_OFFSET 2
#define DADDR_OFFSET 5
#define SREG_OFFSET 7
#define SADDR_OFFSET 10
#define OPCODE_OFFSET 12
#define TYPE_OFFSET 16
#define RSVD_OFFSET 17

/* type for 20 bits data */
typedef struct dline {
    unsigned int data : 20;
} data_line;

/* type for an instruction line (using bit fields) */
typedef struct instrline {
    unsigned int comb      : COMB_WIDTH;
    unsigned int dest_reg  : DREG_WIDTH;
    unsigned int dest_addr : DADDR_WIDTH;
    unsigned int src_reg   : SREG_WIDTH;
    unsigned int src_addr  : SADDR_WIDTH;
    unsigned int opcode    : OPCODE_WIDTH;
    unsigned int type      : TYPE_WIDTH;
    unsigned int rsvd      : RSVD_WIDTH;
} instruction_line;

/* type for a whole line, with relevant fields helping the parsing */
typedef struct cline {
    char *line;
    instruction_line *instruction;
    unsigned char repeat;
    unsigned int line_number;
    char *src_opr;
    char *dest_opr;
    char done;
} code_line;


/* this function returning data_line containing the char in the argument */
data_line char2data(char ch);
/* this function returning data_line containing the (boolean) instruction line in the argument */
data_line bline2data(instruction_line bl);
/* this function returning data_line containing the int in the argument with the sign bit */
data_line num2data(int num);
/* this function clearing the values of an instruction line */
void init_instruction_line(instruction_line *il);
/* after allocating memory, the data need to be clear from garbage */
void init_data_line(data_line *dl);
