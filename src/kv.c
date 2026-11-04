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

// Gets a key-value pair from a database db.
//
// params:
//  - db: a pointer to the db
//  - key: a pointer to the key value
//
// returns: a pointer to the key, returns
// NULL if not found
char *kv_get(kv_t *db, char *key) {
    if (!db || !key)  return NULL;

    size_t idx = hash(key, db->capacity);

    for (int i = 0; i < db->capacity -1; i++) {
        size_t real_idx = (idx + i) % db->capacity;

        kv_entry_t *entry = &db->entries[real_idx];

        // key doesn't exist, return NULL
        if (entry->key == NULL) {
            return NULL;
        }

        // found entry and keys match
        if (entry->key && 
            entry->key != TOMBSTONE && 
            !strcmp(entry->key, key)) {
            return entry->value;
        }
    }

    return NULL;
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
int kv_put(kv_t *db, char *key, char *value) {
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
                // return real_idx;
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
            // return real_idx;    
            return 0;
        }
    }

    // the db is fully occupied
    return -2;

}

// Deletes a key value entry from db.
//
// params:
//  - db: a pointer to the db
//  - key: a pointer to the key value
//
// returns: the  index of the deletion
// or -1 if not found
int kv_delete(kv_t *db, char *key) {
    if (!db || !key)  return -1;

    size_t idx = hash(key, db->capacity);

    for (int i = 0; i < db->capacity -1; i++) {
        size_t real_idx = (idx + i) % db->capacity;

        kv_entry_t *entry = &db->entries[real_idx];

        if (entry->key == NULL) {
            return -1;
        }

        if (entry->key && 
            entry->key != TOMBSTONE && 
            !strcmp(entry->key, key)) {
                free(entry->key);
                free(entry->value);
                db->count--;
                entry->key = TOMBSTONE;
                entry->value = NULL;

                return real_idx;
        }
    }

    return -1;
}

// Frees all memory used by db
//
// params:
//  - db: a pointer to the db
// returns: 0 on success, -1 on failure
int kv_free(kv_t *db) {
    if (!db) return -1;

    for (int i = 0; i < db->capacity -1; i++) {
        kv_entry_t *e = &db->entries[i];

        if (e->key && e->key != TOMBSTONE) {
            free(e->key);
            free(e->value);
            e->key = NULL;
            e->value = NULL;
            db->count--;
        }
    }

    free(db->entries);
    free(db);

    return 0;
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
