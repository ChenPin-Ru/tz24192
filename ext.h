#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>

#define DEFAULT_TABLE_SIZE 1024

// Hash table node structure
typedef struct dict {
    char* word;          // Store the word
    int freq;            // Word frequency
    struct dict* next;   // Linked list for collision resolution
    struct dict** table;        // Array of pointers to nodes
    int size;            // Size of hash table
} dict;


// Function declarations
// Hash function
unsigned int dict_function(const char* str); 
dict* dict_init(void);                              
bool dict_addword(dict* d, const char* word);    
dict* dict_spell(dict* d, const char* word);        
int dict_wordcount(dict* d);          
int dict_mostcommon(dict* d);                      
void dict_free(dict** d);                             
void test(void);  
