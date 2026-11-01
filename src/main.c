#include <stdio.h>
#include <kv.h>

int main() {
    kv_t *table = kv_init(3);

    printf("Pointer: %p\nSize: %ld\nCapacity: %li\n", table, sizeof(table), table->capacity);
}