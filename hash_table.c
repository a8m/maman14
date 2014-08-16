
#include "common.h"
#include "hash_table.h"

/* hash: form hash value for string s */
unsigned int hash(char *s)
{
    unsigned int hashval;
    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;
}

/* string_lookup: look for s in hashtab */
string_hash_node *string_lookup(char *s, string_hash_node *hashtab[])
{
    string_hash_node *np;
    for (np = hashtab[hash(s)]; np != NULL; np = np->next)
        if (strcmp(s, np->name) == 0)
            return np; /* found */
    return NULL; /* not found */
}

/* int_lookup: look for s in hashtab */
int_hash_node *int_lookup(char *s, int_hash_node *hashtab[])
{
    int_hash_node *np;
    for (np = hashtab[hash(s)]; np != NULL; np = np->next)
        if (strcmp(s, np->name) == 0)
            return np; /* found */
    return NULL; /* not found */
}

/* string_install: put (name, defn) in hashtab */
string_hash_node *string_install(char *name, char *defn, string_hash_node *hashtab[])
{
    string_hash_node *np;
    unsigned int hashval;
    if ((np = string_lookup(name, hashtab)) == NULL) { /* not found */
        np = (string_hash_node *) malloc(sizeof(*np));
        if (np == NULL || (np->name = strdup(name)) == NULL)
            return NULL;
        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    } else /* already there */
        free((void *) np->defn); /*free previous defn */
    if ((np->defn = strdup(defn)) == NULL)
        return NULL;
    return np;
}

/* int_install: put (name, defn) in hashtab */
int_hash_node *int_install(char *name, int defn, int_hash_node *hashtab[])
{
    int_hash_node *np;
    unsigned int hashval;
    if ((np = int_lookup(name, hashtab)) == NULL) { /* not found */
        np = (int_hash_node *) malloc(sizeof(*np));
        if (np == NULL || (np->name = strdup(name)) == NULL)
            return NULL;
        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    }
    np->defn = defn;
    return np;
}

