#include <stdio.h>
#include <kv.h>
#include <string.h>


int main() {
    kv_t *table = kv_init(1024);
    printf("%p\n", table);
    printf("%ld\n", table->capacity);

    kv_put(table, "hehe", "haha");
    kv_put(table, "lala", "hoho");
    kv_put(table, "john", "kek");
    kv_put(table, "test1", "test");
    kv_put(table, "name", "alice");

    
    for (int i = 0; i < table->capacity; i++) {
        if (table->entries[i].key) {
            printf("[%d] %s: %s\n",
                i, 
                table->entries[i].key,
                table->entries[i].value);
        }
    }

    char *val = kv_get(table, "heheg");

    if (val)
    printf("%s\n", val);
    else
    printf("not found\n");

    kv_free(table);
}