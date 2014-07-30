/*
 * =========================================================================================
 * name        : maman_14/hash_table.h
 * author      : Ariel Mashraki, Osnat Izic
 * email       :
 * description :
 * =========================================================================================
 */

typedef struct node {
    struct node *next; /* pointer */
    char *name; /* defined name */
    char *defn; /* decimal adress */
} hash_node;

