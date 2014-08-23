/*
 * =========================================================================================
 * name        : maman_14/hashtable.h
 * author      : Ariel Mashraki, Osnat Izic
 * =========================================================================================
 */

typedef struct string_nlist { /* table entry: */
    struct string_nlist *next; /* next entry in chain */
    char *name; /* defined name */
    char *defn; /* replacement text */
} string_hash_node;

typedef struct int_nlist { /* table entry: */
    struct int_nlist *next; /* next entry in chain */
    char *name; /* defined name */
    int defn; /* replacement number */
} int_hash_node;

#define HASHSIZE 200

/* registerStringToHashTab: put (name, defn) in hashtab */
string_hash_node *registerStringToHashTab(char *name, char *defn, string_hash_node *hashtab[]);
/* fetchStringFromHashTab: look for s in hashtab */
string_hash_node *fetchStringFromHashTab(char *s, string_hash_node *hashtab[]);
/* registerIntToHashTab: put (name, defn) in hashtab */
int_hash_node *registerIntToHashTab(char *name, int defn, int_hash_node *hashtab[]);
/* fetchIntFromHashTab: look for s in hashtab */
int_hash_node *fetchIntFromHashTab(char *s, int_hash_node *hashtab[]);
