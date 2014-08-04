

#ifndef PARSER_H_
#define PARSER_H_

/* >>>>>>>>>>>>>>>>>>>>>> INSTRUCTION BIT WIDTH >>>>>>>>>>>>>>>>>>>> */
#define COMB_WIDTH 2
#define DREG_WIDTH 3    /*destination register*/
#define DADDR_WIDTH 2   /*destination address*/
#define SREG_WIDTH 3    /*source register*/
#define SADDR_WIDTH 2   /*source address*/
#define OPCODE_WIDTH 4
#define TYPE_WIDTH 1
#define RSVD_WIDTH 3

/* >>>>>>>>>>>>>>> INSTRUCTION LINE (USING BIT FIELDS) >>>>>>>>>>>>> */
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

/* >>>>>>>>>>>>>>>>>>>>>>> 20 BITS DATA LINE >>>>>>>>>>>>>>>>>>>>>>> */
typedef struct dline {
    unsigned int data : 20;
} data_line;

/* >>>>>>>>>>>>>>>>>>>>>> TYPE FOR A WHOLE LINE >>>>>>>>>>>>>>>>>>>> */
typedef struct cline {
    char *line;
    instruction_line *instruction;
    unsigned char repeat;
    unsigned int line_number;
    char *src_opr;
    char *dest_opr;
    char done;
} code_line;




#endif
