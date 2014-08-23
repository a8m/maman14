/*
 * =========================================================================================
 * name        : maman_14/hashtable.h
 * author      : Ariel Mashraki, Osnat Izic
 * =========================================================================================
 */

typedef struct stringNodeList {
    struct stringNodeList *next; /* next node */
    char *name; /* name */
    char *defn; /* replacement string */
} stringLinkedListNode;

typedef struct intNodeList { 
    struct intNodeList *next; /* next node */
    char *name; /* name */
    int defn; /* replacement int */
} intLinkedListNode;

#define HASHSIZE 200

stringLinkedListNode *registerStringToHashTab(char *name, char *defn, stringLinkedListNode *hashtab[]);

stringLinkedListNode *fetchStringFromHashTab(char *s, stringLinkedListNode *hashtab[]);

intLinkedListNode *registerIntToHashTab(char *name, int defn, intLinkedListNode *hashtab[]);

intLinkedListNode *fetchIntFromHashTab(char *s, intLinkedListNode *hashtab[]);
