#include <stdio.h>
#include<string.h>
#include <ctype.h>
#include <stdlib.h>
#include "hash.h"

int sameString(char *s1, char *s2) {
    int i, j;
    size_t size1 = strlen(s1);
    size_t size2 = strlen(s2);
    int exact = 1;
    if (size1 != size2) {
        return 0;
    }
    for (i = 0; i < size1; i++) {
        int found = 0;
        for (j = 0; j < size2; j++) {
            if (tolower(s1[i]) == tolower(s2[j])) {
                found = 1;
                if (i != j) {
                    exact = 0;
                }
            }
        }
        if (found == 0) {
            return 0;
        }
    }

    if (exact == 1) {
        return 0;
    }
    return 1;
}

void anagram(char *w, hashtable_t *table) {

    unsigned short hash = create_hash(w);

    printf("Finding all Anagrams for %s\n", w);
    entry_t *entry = table->values[hash];

    int i = 0;
    while (entry != NULL) {
        if ((sameString(w, entry->value) == 1) &&
            (strcasecmp(w, entry->value) != 0)) {
            printf("%d. %s\n", ++i, entry->value);
        }
        entry = entry->next;
    }

    if (i == 0) {
        printf("None found.\n");
    }
    return;
}

void scrabble(char *w, char l, int pos, hashtable_t *table) {

    unsigned short hash = create_hash(w);

    printf("Finding all possible scrabble values for %s with %c at %d\n", w, l, pos);
    entry_t *entry = table->values[hash];

    int i = 0;
    while (entry != NULL) {
        if ((sameString(w, entry->value) == 1) &&
            (strcasecmp(w, entry->value) != 0) &&
            ((tolower(entry->value[pos]) == tolower(l)))) {
            printf("%d. %s\n", ++i, entry->value);
        }
        entry = entry->next;
    }
    if (i == 0) {
        printf("None found.\n");
    }
    return;
}

int main(int argc, char *argv[]) {
    FILE *dict = fopen("/usr/share/dict/words", "r");

    if (dict == NULL) {
        printf("Error occurred. Dictionary was null.\n");
    }

    char word[128];
    hashtable_t *table = create_ht();

    while (fgets(word, sizeof(word), dict) != NULL) {
        unsigned short hash = create_hash(word);
        entry_t *e = create_newpair(hash, word);
        insert(table, e);
    }
    //free(word);
    switch (argc) {
        case 2: {
            char *option = argv[1];
            if (strcmp("-a", option) == 0) { // if Anagram
                anagram("Amir", table);
            } else {
                scrabble("Amir", 'm', 0, table);
            }
            break;
        }
        case 3: {
            char *option = argv[1];
            if (strcmp("-a", option) == 0) { // if Anagram
                anagram(argv[2], table);
            } else { //Scrabble
                scrabble("Amir", 'm', 0, table);
            }
            break;
        }
        case 5: {
            char *option = argv[1];
            char letter = argv[3][0];
            int position = atoi(argv[4]);
            if (strcmp("-a", option) == 0) { // if Anagram
                anagram(argv[2], table);
            } else { //Scrabble
                scrabble(argv[2], letter, position, table);
            }
            break;
        }
        default:
            anagram("Amir", table);
            scrabble("Amir", 'm', 0, table);
            break;
    }
    return 0;
}