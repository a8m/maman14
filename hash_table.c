
#include "common.h"
#include "hash_table.h"

/*lookup: look for s in hashtab */

hash_node *lookup(char *s, hash_node *hashtab)
{
    hash_node *np;
    for (np = hashtab ; np != NULL; np = np->next)
        if (strcmp(s, np->name) == 0)
            return np; /* found */
    return NULL; /* not found */
    }
    
    
    
/*insert: insert node to the list
hash_node *insert(char *s, int def ,hash_node *hashtab){

	hash_node *np = lookup(s,hashtab);
	
		 if(np == NULL){
			hash_node *np = (hash_node*) malloc(sizeof(hash_node));

		while(np->next){
			np = np->next;
 		}

		strcpy(np->name, s);
		np->defn = def;
		np->next = NULL;
		
	 	}
    return np;

}*/
/* else - if already exist?*/
