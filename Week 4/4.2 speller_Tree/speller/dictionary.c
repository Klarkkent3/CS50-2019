// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

// Represents number of children for each node in a trie
#define N 27

// Represents a node in a trie
typedef struct node
{
    bool is_word;
    struct node *children[N];
}
node;

// Represents a trie
node *root = NULL;

// Words counter
int WordCount = 0;

void free_node(node *tmp);

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{

    // Initialize triee
    root = calloc(1, sizeof(node));
    root->is_word = false;

    // Index for word arrays
    int index = 0;

    // Buffer for a word
    char word[LENGTH + 1];

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Insert words into trie
    while (fscanf(file, "%s", word) != EOF)
    {
        // Temporary pointer
        node *tmp = root;

        // Iterate through the loop and convert all leters to index (0-25), apostrophes to 26, or break the loop if we reach end of word.
        for (int i = 0; i < strlen(word) + 1; i++)
        {
            if (word[i] == '\0')
            {
                    tmp->is_word = true;
                    WordCount++;
                    break;
            }
            else if (word[i] == '\'')
            {
                index = 26;
            }
            else if (islower(word[i]))
            {
                index = word[i] - 'a';
            }

            // If node with letter not yet created
            if (tmp->children[index] == NULL)
            {
                // Create new node
                node *N_node = calloc(1, sizeof(node));
                N_node->is_word = false;
                // Point children to new node
                tmp->children[index] = N_node;
                // Set temporary pointer to New_Node
                tmp = N_node;


            }
            // If there is already pointer, jump to next node
            else
            {
                tmp = tmp->children[index];
            }
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
    return WordCount;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Temporary poiner
    node *tmp = root;

    // Index for word arrays
    int index = 0;

    // Iterate through the loop and convert all leters to index (0-25), apostrophes to 26.
    for (int i = 0; i < strlen(word) + 1; i++)
    {
        if (word[i] == '\0')
        {
            if (tmp->is_word == true)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        if (word[i] == '\'')
        {
            index = 26;
        }
        else if (isupper(word[i]))
        {
            index = word[i] - 'A';
        }
        else if (islower(word[i]))
        {
            index = word[i] - 'a';
        }

        // If the key of this word is not existed
        if (tmp->children[index] == NULL)
        {
           return false;
        }
        // If there is already pointer, jump to next node
        else
        {
            tmp = tmp->children[index];
        }
    }
    return false;
}

void free_node(node *tmp)
{
    for (int i = 0; i < 27; i++)
    {
        if (tmp->children[i] != NULL)
        {
            free_node(tmp->children[i]);
        }
    }
    free(tmp);
    return;
}



// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    node *tmp = root;

    if (tmp != NULL)
    {
        if (root != NULL)
        {
            free_node(root);
        }
        return true;
    }
    return false;
}
