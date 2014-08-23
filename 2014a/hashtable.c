/*
 * =========================================================================================
 * name        : maman_14/hashtable.c
 * author      : Ariel Mashraki, Osnat Izic
 * =========================================================================================
 */
 
#include "hashtable.h"
#include <stdlib.h>
#include <string.h>
#include "common.h"

/* hash: form hash value for string s */
unsigned int hash(char *s)
{
    unsigned int hashval;
    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;
}

/* fetchStringFromHashTab: look for s in hashtab */
string_hash_node *fetchStringFromHashTab(char *s, string_hash_node *hashtab[])
{
    string_hash_node *np;
    for (np = hashtab[hash(s)]; np != NULL; np = np->next)
        if (strcmp(s, np->name) == 0)
            return np; /* found */
    return NULL; /* not found */
}

/* fetchIntFromHashTab: look for s in hashtab */
int_hash_node *fetchIntFromHashTab(char *s, int_hash_node *hashtab[])
{
    int_hash_node *np;
    for (np = hashtab[hash(s)]; np != NULL; np = np->next)
        if (strcmp(s, np->name) == 0)
            return np; /* found */
    return NULL; /* not found */
}

/* registerStringToHashTab: put (name, defn) in hashtab */
string_hash_node *registerStringToHashTab(char *name, char *defn, string_hash_node *hashtab[])
{
    string_hash_node *np;
    unsigned int hashval;
    if ((np = fetchStringFromHashTab(name, hashtab)) == NULL) { /* not found */
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

/* registerIntToHashTab: put (name, defn) in hashtab */
int_hash_node *registerIntToHashTab(char *name, int defn, int_hash_node *hashtab[])
{
    int_hash_node *np;
    unsigned int hashval;
    if ((np = fetchIntFromHashTab(name, hashtab)) == NULL) { /* not found */
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

