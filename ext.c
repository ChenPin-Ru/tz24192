#include "ext.h"
#define DEFAULT_TABLE_SIZE 1024

// Function declarations
unsigned int dict_function(const char* str);          
dict* dict_init(void);
int strcasecmp_custom(const char* s1, const char* s2);                                
bool dict_addword(dict* d, const char* word);         
dict* dict_spell(dict* d, const char* word);          
int dict_wordcount(dict* d);                         
int dict_mostcommon(dict* d);                         
void dict_free(dict** d);                             
void test(void);  

// 0. Hash function
unsigned int dict_function(const char* str){
    unsigned int hash = 5381;
    while (*str != '\0') {
        hash = ((hash << 5) + hash) + tolower(*str); // djb2 algorithm
        str++;
    }
    return hash;
}

// 1. Initialize the hash table
dict* dict_init(void){
    dict* d = (dict*)malloc(sizeof(dict));
    if (d == NULL) {
        return NULL;
    }
    
    d->size = DEFAULT_TABLE_SIZE;
    d->table = (dict**)calloc(d->size, sizeof(dict*));
    
    return d;
}

// helper function
int strcasecmp_custom(const char* s1, const char* s2) {
    while (*s1 && *s2) {
        int c1 = tolower((unsigned char)*s1);
        int c2 = tolower((unsigned char)*s2);
        if (c1 != c2) {
            return c1 - c2;
        }
        s1++;
        s2++;
    }
    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}

// 2. Add a word to the hash table
bool dict_addword(dict* d, const char* word){
    if (d == NULL || word == NULL){
       return false;
    }
 
    unsigned int idx = dict_function(word) % d->size;
    dict* current = d->table[idx];
 
    // Check if word already exists
    while (current) {
        if (strcasecmp_custom(current->word, word) == 0) {
            current->freq++;
            return false; // Word already exists
        }
        current = current->next;
    }
 
    // Create new node
    dict* new_node = (dict*)malloc(sizeof(dict));
    size_t len = strlen(word) + 1;
    new_node->word = (char*)malloc(len);
    if (new_node->word == NULL) {
        free(new_node);
        return false;
    }
    strcpy(new_node->word, word);
   
    new_node->freq = 1;
    new_node->next = d->table[idx];
    d->table[idx] = new_node;
   
    return true;
}

// 3. Check if a word exists and return its node
dict* dict_spell(dict* d, const char* word){
    if (d == NULL || word == NULL){
       return NULL;
    }
 
    unsigned int idx = dict_function(word) % d->size;
    dict* current = d->table[idx];
 
    while (current) {
        if (strcasecmp_custom(current->word, word) == 0) {
            return current; // Word found
        }
        current = current->next;
    }
    return NULL; // Word not found
}

// 4. Count total words (including frequency)
int dict_wordcount(dict* d){
    if (d == NULL){ 
        return 0;
    }

    int count = 0;
    for (int i = 0; i < d->size; i++) {
        dict* current = d->table[i];
        while (current) {
            count += current->freq;
            current = current->next;
        }
    }
    return count;
}

// 5. Find the most common word's frequency
int dict_mostcommon(dict* d){
    if (!d){ 
        return 0;
    }

    int max_freq = 0;
    for (int i = 0; i < d->size; i++) {
        dict* current = d->table[i];
        while (current) {
            if (current->freq > max_freq) {
                max_freq = current->freq;
            }
            current = current->next;
        }
    }
    return max_freq;
}

// 6. Free the hash table
void dict_free(dict** d){
    if (!d || !*d){
        return;
    }

    for (int i = 0; i < (*d)->size; i++) {
        dict* current = (*d)->table[i];
        while (current) {
            dict* temp = current;
            current = current->next;
            free(temp->word);
            free(temp);
        }
    }
    
    free((*d)->table);
    free(*d);
    *d = NULL;
}

// 7. test function
void test(void) {
    
    dict* d = NULL;
    d = dict_init();
    
    // Add words to the hash table
    dict_addword(d, "car");
    dict_addword(d, "cart");
    dict_addword(d, "part");

    // Check word count
    assert(dict_wordcount(d) == 3);

    // Check most common frequency
    dict_addword(d, "cart");
    assert(dict_mostcommon(d) == 2);

    // Spell check
    assert(dict_spell(d, "cart") != NULL);
    assert(dict_spell(d, "car") != NULL);
    assert(dict_spell(d, "unknown") == NULL);

    // Free memory
    dict_free(&d);
}


