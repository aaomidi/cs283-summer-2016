#include <stdio.h>
#include<string.h>
#include <ctype.h>
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

int main() {
    FILE *dict = fopen("/usr/share/dict/words", "r");
    //FILE *dict = fopen("D:\\Dropbox\\College\\Summer 2016\\CS283\\Project\\Assignments\\01\\words", "r");

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

    char *w = "Amir";
    unsigned short hash = create_hash(w);
    printf("%d\n", hash);
    entry_t *entry = table->values[hash];
    if (entry == NULL) {
        printf("None found.\n");
        return 0;
    }
    while (entry != NULL) {
        if (sameString(w, entry->value) == 1 && (strcasecmp(w, entry->value) != 0)) {
            printf("%s\n", entry->value);
        }
        entry = entry->next;
    }
    return 0;
}