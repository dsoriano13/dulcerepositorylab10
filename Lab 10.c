#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

struct Trie {
    struct Trie *children[ALPHABET_SIZE];
    int count;
};

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word) {
    struct Trie *current = pTrie;
    int len = strlen(word);

    for (int i = 0; i < len; i++) {
        int index = word[i] - 'a';
        if (!current->children[index]) {
            current->children[index] = (struct Trie *)malloc(sizeof(struct Trie));
            if (!current->children[index]) {
                // Failed to allocate memory for the new node
                printf("Memory allocation error\n");
                return;
            }
            // Initialize the new node
            current->children[index]->count = 0;
            for (int j = 0; j < ALPHABET_SIZE; j++) {
                current->children[index]->children[j] = NULL;
            }
        }
        current = current->children[index];
    }
    // Mark the last node as a leaf and increment its count
    current->count++;
}

// Computes the number of occurrences of the word
int numberOfOccurrences(struct Trie *pTrie, char *word) {
    struct Trie *current = pTrie;
    int len = strlen(word);

    for (int i = 0; i < len; i++) {
        int index = word[i] - 'a';
        if (!current->children[index]) {
            return 0; // Word not found
        }
        current = current->children[index];
    }
    return current->count;
}

// Deallocate the trie structure
void deallocateTrie(struct Trie *pTrie) {
    if (!pTrie) {
        return;
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        deallocateTrie(pTrie->children[i]);
    }
    free(pTrie);
}

// Initializes a trie structure
struct Trie *createTrie() {
    struct Trie *pTrie = (struct Trie *)malloc(sizeof(struct Trie));
    if (pTrie) {
        pTrie->count = 0;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            pTrie->children[i] = NULL;
        }
    }
    return pTrie;
}

// This function will return the number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file: %s\n", filename);
        exit(1);
    }

    int count = 0;
    char word[100]; // Assuming max word length is 100

    while (fscanf(file, "%s", word) != EOF) {
        pInWords[count] = strdup(word);
        count++;
    }

    fclose(file);
    return count;
}

int main(void) {
    char *inWords[256];

    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i) {
        printf("%s\n", inWords[i]);
    }

    struct Trie *pTrie = createTrie();
    if (!pTrie) {
        printf("Error creating Trie\n");
        return 1;
    }

    for (int i = 0; i < numWords; i++) {
        insert(pTrie, inWords[i]);
    }

    // Words to check for occurrences
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    deallocateTrie(pTrie);
    return 0;
}
