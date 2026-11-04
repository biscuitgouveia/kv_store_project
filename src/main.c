#include <stdio.h>
#include <kv.h>

int main() {
    kv_t *table = kv_init(1024);

    printf("Pointer: %p\nSize: %ld\nCapacity: %li\n", table, sizeof(table), table->capacity);

    kv_put(table, "hehe", "haha");
    kv_put(table, "hehe", "hoho");
    kv_put(table, "lala", "hehe");

    // for (int i = 0; i < table->capacity; i++) {
    //     if (table->entries[i].key) {
    //         printf("[%d] %s: %s\n",
    //             i, 
    //             table->entries[i].key, 
    //             table->entries[i].value);
    //     }
    // }

    char *val = kv_get(table, "hehe");
    char *val2 = kv_get(table, "lala");
    char *val3 = kv_get(table, "this_doesn't_exist");

    printf("%s\n%s\n%s\n", val, val2, val3);

    kv_delete(table, "hehe");
    val = NULL;
    val = kv_get(table, "hehe");
    printf("%s\n%s\n%s\n", val, val2, val3);
}