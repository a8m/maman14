
#include "hashtable.h"
#include <stdlib.h>
#include <string.h>



/*lookup: look for s in hashtab */

hash_node *lookup(char *s, hash_node *hashtab)
{
    hash_node *np;
    for (np = hashtab ; np != NULL; np = np->next)
        if (strcmp(s, np->name) == 0)
            return np; /* found */
    return NULL; /* not found */
    }
    
    
    
/*insert: insert node to the list */


hash_node *insert(char *s, int def ,hash_node *hashtab)
{
	 if(lookup(s,hashtab) == NULL){
		hash_node *np = (hash_node*) malloc(sizeof(hush_node));

		while(np->next){
			np = np->next;
 		}

		strcpy(np->name, s);
		np-> def = def;
		np->next = NULL;
		
	 	}
	/*
	else
	{
	eror massege (?)
	}
*/




 }


/*
 * =========================================================================================
 * name        : maman_14/hash_table.c
 * author      : Ariel Mashraki, Osnat Izic
 * email       :
 * description :
 * =========================================================================================
 */
