#include <stdlib.h>

#include "array.h"

typedef unsigned long longtype;

struct array {
    longtype size;
    longtype capacity;
    int *data;
};

struct array *array_init(unsigned long initial_capacity) {
    struct array *new_array = malloc(sizeof(struct array));
    if (new_array == NULL) {
        return NULL;
    }

    new_array->data = malloc(initial_capacity * sizeof(int));
    if (new_array->data == NULL) {
        free(new_array);
        return NULL;
    }
    new_array->size = 0;
    new_array->capacity = initial_capacity;

    return new_array;
}

int array_get(const struct array *a, unsigned long index) {
    if (a == NULL || index >= a->size) {
        return -1;
    }

    return a->data[index];
}

int array_append(struct array *a, int elem) {
    if (a == NULL) {
        return 1;
    }

    if (a->size == a->capacity) {
        longtype new_capacity = a->capacity * 2;
        int *new_data = realloc(a->data, new_capacity * sizeof(int));
        if (new_data == NULL) {
            return 1;
        }

        a->data = new_data;
        a->capacity = new_capacity;
    }

    a->data[a->size] = elem;
    a->size++;

    return 0;
}

unsigned long array_size(const struct array *a) {
    return a->size;
}

void array_cleanup(struct array *a) {
    if (a == NULL) {
        return;
    }

    free(a->data);
    free(a);
}
