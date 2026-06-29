#include <kv.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define TOMBSTONE ((char *)0x1)

size_t hash(const char *val, size_t capacity) {
    
    if (capacity == 0) return 0;

    size_t hash = 0x13371337deadbeef;

    while(*val) {

        hash ^= *val;
        hash = hash << 8;
        hash += *val;

        val++;
    }

    return hash % capacity;
}

char *kv_get(kv_t *db, const char *key) {
    if (!db || !key) return NULL;

    size_t idx = hash(key, db->capacity);

    for (size_t i = 0; i < db->capacity; i++) {
        size_t real_idx = (idx + i) % db->capacity;

        kv_entry_t *entry = &db->entries[real_idx];

        if (entry->key == NULL) {
            return NULL;
        }

        if (entry->key
            && entry->key != (void*)TOMBSTONE
            && !strcmp(entry->key, key)) {
            return entry->value;
        }
    }
    return NULL;
}

int kv_put(kv_t *db, char *key, char *value) {
    if (!db || !db->entries || !key || !value) return -1;

    size_t idx = hash(key, db->capacity);

    for (size_t i = 0; i < db->capacity; i++) {

        size_t real_idx = (idx + i) % db->capacity;

        kv_entry_t *entry = &db->entries[real_idx];

        if (entry->key 
            && entry->key != (void*)TOMBSTONE 
            && !strcmp(entry->key, key)) {
                char *newval = strdup(value);
                if (!newval) return -1;
                free(entry->value);
                entry->value = newval;
                return 0;
            }

        if (!entry->key || entry->key == (void*)TOMBSTONE) {
            char *newval = strdup(value);
            char *newkey = strdup(key);
            if (!newval || !newkey)  {
                free(newkey);
                free(newval);
                return -1;
            }
            entry->value = newval;
            entry->key = newkey;
            db->count++;
            return 0;
        }
    }

    // db is occupied
    return -2;

}

kv_t *kv_init(size_t capacity) {
    if (capacity == 0) return NULL;

    kv_t *table = malloc(sizeof(kv_t));

    if (table == NULL) {
        return NULL;
    }

    table->capacity = capacity; 
    table->count = 0;

    table->entries = calloc(capacity, sizeof(*table->entries));
    if (table->entries == NULL) {
        free(table);
        return NULL;
    }

    return table;
}

void kv_free(kv_t *db)
{
    if (db == NULL)
        return;

    for (size_t i = 0; i < db->capacity; i++) {
        if (db->entries[i].key &&
            db->entries[i].key != TOMBSTONE) {
            free(db->entries[i].key);
            free(db->entries[i].value);
        }
    }

    free(db->entries);
    free(db);
}