/*
 * Implements a dynamic array structure.
 */

#include <stdlib.h>

#include "array.h"

struct array {
    void **data;
    long size;
    long capacity;
};

struct array *array_init(long initial_capacity) {
    struct array *a = malloc(sizeof(struct array));
    if (a == NULL) {
        return NULL;
    }
    if (initial_capacity == 0) {
        initial_capacity = 1;
    }
    a->data = (void **) malloc((unsigned long) initial_capacity * sizeof(void *));
    if (a->data == NULL) {
        free(a);
        return NULL;
    }
    a->size = 0;
    a->capacity = initial_capacity;
    return a;
}

void array_cleanup(struct array *a, void (*free_func)(void *)) {
    if (a == NULL) {
        return;
    }

    if (free_func == NULL) {
        free_func = free;
    }

    void *e;
    while ((e = array_pop(a))) {
        free_func(e);
    }

    free(a->data);
    free(a);
}

int array_set(struct array *a, long int index, void *p) {
    if (a == NULL || index < 0 || index >= a->size) {
        return -1;
    }

    a->data[index] = p;
    return 0;
}

void *array_get(const struct array *a, long int index) {
    if (a == NULL || index < 0 || index >= a->size) {
        return NULL;
    } else {
        return a->data[index];
    }
}

int array_append(struct array *a, void *p) {
    if (a == NULL) {
        return -1;
    }

    if (a->size == a->capacity) {
        long new_capacity = (a->capacity + 1) * 2;
        void **new = realloc(a->data, (unsigned long) new_capacity * sizeof(void *));
        if (new == NULL) {
            return -1;
        }
        a->capacity = new_capacity;
        a->data = new;
    }

    a->size++;
    return array_set(a, a->size - 1, p);
}

void *array_pop(struct array *a) {
    if (a == NULL || a->size == 0) {
        return NULL;
    }

    void *last = a->data[a->size - 1];
    a->size--;
    return last;
}

long int array_size(const struct array *a) {
    if (a == NULL) {
        return -1;
    }

    return a->size;
}
