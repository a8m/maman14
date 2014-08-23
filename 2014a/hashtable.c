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

/* 
 * @description look for node in a stringlinkedList 
 * @param s {String} name of node
 * @param table to looking for
 * @return NULL or Node
 */
 stringLinkedListNode *fetchStringFromHashTab(char *s, stringLinkedListNode *hashtab[])
{
    stringLinkedListNode *np;
    for (np = hashtab[hash(s)]; np != NULL; np = np->next)
        if (strcmp(s, np->name) == 0)
            return np; /* found */
    return NULL; /* not found */
}

/* 
 * @description look for node in a intlinkedList 
 * @param s {String} name of node
 * @param table to looking for
 * @return NULL or Node
 */
intLinkedListNode *fetchIntFromHashTab(char *s, intLinkedListNode *hashtab[])
{
    intLinkedListNode *np;
    for (np = hashtab[hash(s)]; np != NULL; np = np->next) {
        if (strcmp(s, np->name) == 0)
            return np; /* found node*/
    }
    return NULL; /* not found node*/
}

/* 
 * @description insert node in stringlinkedList(hashTable)
 * @param name {String}
 * @param defn {char}
 * @param stringLinkedList
 */
stringLinkedListNode *registerStringToHashTab(char *name, char *defn, stringLinkedListNode *hashtab[])
{
    stringLinkedListNode *np;
    unsigned int hashval;
    if ((np = fetchStringFromHashTab(name, hashtab)) == NULL) { /* not found */
        np = (stringLinkedListNode *) malloc(sizeof(*np));
        if (np == NULL || (np->name = strCopy(name)) == NULL)
            return NULL;
        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    } else /* already there */
        free((void *) np->defn); /*free previous defn */
    if ((np->defn = strCopy(defn)) == NULL)
        return NULL;
    return np;
}

/* 
 * @description insert node in intlinkedList(hashTable)
 * @param name {String}
 * @param defn {int}
 * @param intLinkedList
 */
 intLinkedListNode *registerIntToHashTab(char *name, int defn, intLinkedListNode *hashtab[])
{
    intLinkedListNode *np;
    unsigned int hashval;
    if ((np = fetchIntFromHashTab(name, hashtab)) == NULL) { /* not found */
        np = (intLinkedListNode *) malloc(sizeof(*np));
        if (np == NULL || (np->name = strCopy(name)) == NULL)
            return NULL;
        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    }
    np->defn = defn;
    return np;
}

