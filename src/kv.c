#include <string.h>
#include <stdlib.h>
#include <kv.h>

int hash(char *val, size_t capacity) {
    size_t hash = 0x13371337deadbeef;

    while(*val) {

        hash ^= *val;
        hash = hash << 8;
        hash += *val;

        val++;
    }

    return hash % capacity;
}

// Inserts a key-value pair into a database db.
//
// params:
//  - db: A pointer to the db
//  - key: A pointer to the key
//  - value: A pointer to the value
//
// returns: The index of the key, otherwise on
// error returns -1, on not found returns -2
int kv_put(kv_t *db, const char *key, const char *value) {
    if (!db || !key || !value)  return -1;

    size_t idx = hash(key, db->capacity);

    for (int i = 0; i < db->capacity -1; i++) {

        size_t real_idx = (idx + i) % db->capacity;
        kv_entry_t *entry = &db->entries[real_idx];

        // key already set, updating
        if (entry->key && 
                entry->key != TOMBSTONE && 
                !strcmp(entry->key, key)) {
                char *newval = strdup(value);
                if (!newval) {
                    free(newval);
                    return -1;
                };
                entry->value = newval;
                //return real_idx;
                return 0;
            }
        
        // land in empty, NULL, or tombstoned slot
        if (!entry->key || entry->key == TOMBSTONE) {
            char *newval = strdup(value);
            char *newkey = strdup(key);
            if (!newval || !newkey) {
                free(newval);
                free(newkey);
                return -1;
            };
            entry->key = newkey;
            entry->value = newval;
            db->count++;
            //return real_idx;    
            return 0;
        }
    }

    // the db is fully occupied
    return -2;

}

// Initialises an empty kv store.
//
// params:
//  - capacity: The capacity of the store.
//
// returns: A pointer to the initialised table,
// otherwise on error returns NULL
kv_t *kv_init(size_t capacity) {
    if (capacity == 0) {
        return NULL;
    }

    kv_t *table = malloc(sizeof(kv_t));
    if (table == NULL) {
        return NULL;
    }

    table->capacity = capacity;
    table->count = 0;

    table->entries = calloc(capacity, sizeof(kv_entry_t));
    if (table->entries == NULL) {
        free(table);
        return NULL;
    }

    return table;
}
