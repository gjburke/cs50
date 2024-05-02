// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 1000;

// Hash table
node *table[N];

//My own declarations
bool unloadLinkedList(node *ptr);

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    //Get the index of what the word would be
    int hashInd = hash(word);
    //Get a pointer to the node at that index
    node *checkNode = table[hashInd];
    //Ierate over every node
    while (checkNode) {
        //Get the word at the node
        char *str = checkNode->word;
        //char *newStr = cleanString(str);
        //Check if it is the word
        if (strcasecmp(str, word) == 0) {
            return true;
        }
        checkNode = checkNode->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{

    //Crazy hash going
    int total = 0;
    for (int i = 0; i < strlen(word); i++) {
        total += tolower(word[i]);
    }
    return total % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    //Read the content from dictionary
    FILE *f = fopen(dictionary, "r");
    //Check if opening file doesn't work
    if (!f) {
        fclose(f);
        return false;
    }
    //Find the length of the dictionary string then put the pointer back to the start
    int dictLen = 0;
    char c;
    while (fread(&c, sizeof(char), 1, f)) {
        dictLen++;
    }
    fseek(f, 0, SEEK_SET);
    //Instantiate the dictionary as a string
    char *dict = malloc((dictLen + 1) *sizeof(char));
    int idx = 0;
    while (fread(&c, sizeof(char), 1, f)) {
        dict[idx] = c;
        idx++;
    }
    //Close file we are done loading
    fclose(f);

    //Load each word into hash table
    int start = 0;
    for (int i = 0; i < dictLen; i++) {
        if (dict[i] == '\n') {
            //Calcullate the word length and malloc for that size
            int wordLen = i - start;
            char *word = malloc((LENGTH + 1) * sizeof(char));
            //Get the string and set it equal to temp variable word
            for (int j = 0; j < wordLen; j++) {
                word[j] = dict[start + j];
            }
            word[wordLen] = '\0';
            //Hash the word to find its place in the hash table
            int hashInd = hash(word);
            //Create new node and insert at that index
            node *newNode = malloc(sizeof(node));
            newNode->next = table[hashInd];
            table[hashInd] = newNode;
            strcpy(newNode->word, word);
            free(word);
            //Set the starting index to be the next character
            start = i + 1;
        }
    }
    free(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    int count = 0;
    for (int i = 0; i < N; i++) {
        node *nodePtr = table[i];
        while (nodePtr) {
            if (strlen(nodePtr->word) != 0) {
                count++;
            }
            nodePtr = nodePtr->next;
        }
    }
    return count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    bool unloaded = false;
    for (int i = 0; i < N; i++) {
        node *nodePtr = table[i];
        unloaded = unloadLinkedList(nodePtr);
    }
    return unloaded;
}

//Unload a linked list
bool unloadLinkedList(node *ptr) {

    if (ptr == NULL) {
        return true;
    }

    unloadLinkedList(ptr->next);

    free(ptr);

    return true;
}
