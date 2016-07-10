//
// Created by amir on 2016-07-09.
//

#ifndef INC_01_HASH_H
#define INC_01_HASH_H

struct entry_s {
    short key;
    char *value;
    struct entry_s *next;
};
typedef struct entry_s entry_t;

struct hashtable_s {
    entry_t **values;
};
typedef struct hashtable_s hashtable_t;

hashtable_t *create_ht();
entry_t *create_newpair(short key, char *value);
void insert(hashtable_t *table, entry_t *entry);
unsigned short create_hash(char *string);
#endif //INC_01_HASH_H
