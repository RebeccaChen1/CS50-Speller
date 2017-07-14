/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

#define NUM_BUCKET 100000

// creates node struct
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// initialize array for hash
node *dic[NUM_BUCKET];

// number of total words
int wordcount = 0;

// hash from http://stackoverflow.com/questions/7666509/hash-function-for-string
unsigned long
hash(char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
    {
        /* hash * 33 + c */
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // new array for each word
    char new_word[strlen(word) + 1];
    // turns every char in the word to lowercase b/c dictionary is in lowercase
    for(int i = 0, n = strlen(word); i < n; i++)
    {
        new_word[i] = tolower(word[i]);
    }
    // ends each word
    new_word[strlen(word)] = '\0';
    // number of the bucket
    int i = hash(new_word) % NUM_BUCKET;
    // creates pointer to point at bucket
    node *ptr = dic[i];
    // interates through the bucket checking for the word
    while(ptr != NULL)
    {
        if (strcmp(ptr->word, new_word) == 0)
        {
            return true;
        }
        
        ptr = ptr->next;
    }
    
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // opens file
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        printf("cannot read\n");
        return false;
    }
    // creates temp array
    char temp[LENGTH + 1];
    
    // runs until there are no more words
    while(fscanf(dict, "%s", temp) != -1)
    {
        // gets index of bucket
        int i = hash(temp) % NUM_BUCKET;
        // creates temp pointer to bucket at i
        node *temp_ptr = dic[i];
        // mallocs sizeof node and sets word to point at it
        node *word = malloc(sizeof(node));
        // copies temp into word of word
        strcpy(word->word, temp);
        // if no word in the bucket yet, add word
        if (temp_ptr == NULL)
        {
            dic[i] = word; 
        }
        // else adds word to front of bucket
        else
        {
            word->next = temp_ptr;
            dic[i] = word;
           
        }
        // increases number of words
        wordcount++;
        
    }
    fclose(dict);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return wordcount;
}
 
/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // goes through buckets
    for(int i = 0; i < NUM_BUCKET; i++)
    {
        node *ptr = dic[i];
        // frees each node in a bucket
        while(ptr != NULL)
        {
        
            node *temp = ptr->next;
            free(ptr);
            ptr = temp;
        }
        
    }
    
    return true;
}


