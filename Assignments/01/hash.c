#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "hash.h"
#include <string.h>

hashtable_t *create_ht() {
    hashtable_t *hashtable = NULL;
    hashtable = (hashtable_t *) malloc(sizeof(hashtable_t));
    if (hashtable == NULL) {
        printf("Something bad happened hashtable1");
        exit(0);
    }
    hashtable->values = malloc(sizeof(entry_t *) * USHRT_MAX);
    if (hashtable->values == NULL) {
        printf("Something bad happened hashtable2\n");
        exit(0);
    }
    int i = 0;
    for (; i < USHRT_MAX; i++) {
        hashtable->values[i] = NULL; // because C is annoying
    }
    return hashtable;
}

entry_t *create_newpair(short key, char *value) {
    entry_t *entry = (entry_t *) malloc(sizeof(entry_t));
    if (entry == NULL) {
        printf("Something bad happened newpair");
        exit(0);
    }
    entry->key = key;
    entry->value = strdup(value);
    entry->next = NULL;

    return entry;
}

void insert(hashtable_t *table, entry_t *entry) {
    short inx = entry->key;

    entry_t *next = NULL;
    entry_t *previous = NULL;

    next = table->values[inx];
    if (next == NULL) {
        table->values[inx] = entry;
        return;
    }

    while (next != NULL) {
        previous = next;
        next = next->next;
    }

    previous->next = entry;
}

unsigned short create_hash(char *string) {
    int i = 0;
    unsigned short res = 0;
    for (; i < strlen(string); i++) {
        unsigned int val = (unsigned int) string[i];
        if (val > 64 && val < 91) { // Between A-Z
            val -= 64;
            res += val;

        } else if (val > 96 && val < 123) { // Between a-z
            val -= 96;
            res += val;
        }

    }
    return res;
}

char *cleanstring(char *s) {
    int i = 0;
    char *string = strdup(s);

    for (; i < strlen(string); i++) {
        if (string[i] == '\n') {
            string[i] = '\0';
            break;
        }
    }
    return string;
}