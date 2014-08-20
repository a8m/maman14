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

/* string_install: put (name, defn) in hashtab */
string_hash_node *string_install(char *name, char *defn, string_hash_node *hashtab[]);
/* string_lookup: look for s in hashtab */
string_hash_node *string_lookup(char *s, string_hash_node *hashtab[]);
/* int_install: put (name, defn) in hashtab */
int_hash_node *int_install(char *name, int defn, int_hash_node *hashtab[]);
/* int_lookup: look for s in hashtab */
int_hash_node *int_lookup(char *s, int_hash_node *hashtab[]);
