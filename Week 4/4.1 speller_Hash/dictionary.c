// Implements a dictionary's functionality

#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 26

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Represents a hash table
node *hashtable[N];

// Words count
int WordsCount = 0;

// Hashes word to a number between 0 and 25, inclusive, based on its first letter
unsigned int hash(const char *word)
{
    return tolower(word[0]) - 'a';
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char buffer[LENGTH + 1];

    // Insert words into hash table
    while (fscanf(file, "%s", buffer) != EOF)
    {
        // Give memory for  new node
        // For understandting: NewNode has the same structure as node (that we declare in the beggining)
        node *NewNode = malloc(sizeof(node));

        // Check if memory is available
        if (NewNode == NULL)
        {
            return false;
        }
        // copy word from buffer to node
        strcpy(NewNode->word, buffer);

        // As it can be last word in list, so pointer should be determined
        // Otherwise you can have segmentetion fault and/or problems with valgrind
        NewNode->next = NULL;

        // Get index of hashtable
        int index = hash(NewNode->word);

        // Set head for hashtable
        node *head = hashtable[index];

        // If it's first node of the current index
        if (head == NULL)
        {

            hashtable[index] = NewNode;
            // Counting number of our words
            WordsCount++;
        }
        // Not first node of the current index
        else
        {
            // The general idea is to insert each new node in front of previous one

            // Set pointer to the beggining of the list
            NewNode->next = hashtable[index];
            // Now our NewNode become new beggining of the list
            hashtable[index] = NewNode;
            // Counting number of our words
            WordsCount++;
        }
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return WordsCount;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Define word lenght
    int L = strlen(word);

    // Set buffer
    char buffer[L + 1];

    // Make all characters to lowercase
    for (int i = 0; i < L; i++)
    {
        buffer[i] = tolower(word[i]);
    }
    // Don't forget to add \0 to the end of the string
    buffer[L] = '\0';

    // Set cursor to the beggining of the linked list
    int index = hash(buffer);
    node *cursor = hashtable[index];

    while (cursor != NULL)
    {
        // Compare string from hashtable and string from text (changed to lowercase)
        if (strcasecmp(cursor->word, buffer) == 0)
        {
            return true;
        }
        else
        {
        // Jump to the next node
        cursor = cursor->next;
        }
    }
    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // For runnung through all indexes of the hashtable
    for (int i = 0; i < N; i++)
    {
        // Set cursor to the beggining of the list
        node *head = hashtable[i];
        node *cursor = head;


        // Untill we reach end of the list
        while (cursor != NULL)
        {
            // Set tmp node that will unload memory after cursor go to next node
            node *tmp = cursor;
            // Cursor jumps to next node
            cursor = cursor->next;
            // Unloading memory
            free(tmp);
        }
    }
    return true;
}
