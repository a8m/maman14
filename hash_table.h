/*
 * =========================================================================================
 * name        : maman_14/hash_table.h
 * author      : Ariel Mashraki, Osnat Izic
 * email       :
 * description :
 * =========================================================================================
 */

/* >>>>>>>>>>>>>>>>>>>>>>>> DEFINE OP ACTIONS >>>>>>>>>>>>>>>>>>>>>>>> */
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
/* >>>>>>>>>>>>>>>>>>>>>>>>>>> END DEFINE >>>>>>>>>>>>>>>>>>>>>>>>>>> */

typedef struct node {
    struct node *next; /* pointer */
    char *name; /* defined name */
    int *defn; /* decimal adress */
} hash_node;
