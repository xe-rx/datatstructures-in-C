/*
 Name: Marouan Bellari
 SSID: 14675218

 Manages a  heap-based priority queue, with functions for initialisation,
  popping, insertion etc. It also uses a dynamically sized array and custom
  compare function.
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"
#include "prioq.h"

#define SIZE_ARRAY 100

typedef long int longtype;

/* prioq.h, prioq_init comment applies. Same goes for the rest of the funcs.*/
static struct heap *heap_init(int (*compare)(const void *, const void *)) {

    struct heap *h = malloc(sizeof(struct heap));
    if (h == NULL) {
        return NULL;
    }

    h->array = array_init(SIZE_ARRAY);
    if (h->array == NULL) {
        free(h);
        return NULL;
    }

    h->compare = compare;
    return h;
}

prioq *prioq_init(int (*compare)(const void *, const void *)) {
    prioq *q = heap_init(compare);
    if (q == NULL) {
        return NULL;
    }

    return q;
}

long int prioq_size(const prioq *q) {
    if (q == NULL || q->array == NULL) {
        return -1;
    }

    return array_size(q->array);
}

static int heap_cleanup(struct heap *h, void free_func(void *)) {
    if (h == NULL || h->array == NULL) {
        return -1;
    }

    array_cleanup(h->array, free_func);
    free(h);
    return 0;
}

int prioq_cleanup(prioq *q, void free_func(void *)) {
    if (q == NULL || q->array == NULL) {
        return -1;
    }

    heap_cleanup(q, free_func);
    return 0;
}

static int heap_insert(struct heap *h, void *p) {
    if (h == NULL || p == NULL) {
        return -1;
    }

    if (array_append(h->array, p) != 0) {
        return -1;
    }

    longtype root = 0;
    longtype inserted_node = array_size(h->array) - 1;
    longtype parent_node = (inserted_node - 1) / 2;

    while (inserted_node > root && h->compare(array_get(h->array, inserted_node),
             array_get(h->array, parent_node)) < 0) {
        void *temp = array_get(h->array, inserted_node);


        void *inserted_ptr = array_get(h->array, inserted_node);
        void *parent_ptr = array_get(h->array, parent_node);
        if (inserted_ptr == NULL || parent_ptr == NULL) {
            return -1;
        }

        array_set(h->array, inserted_node, array_get(h->array, parent_node));
        array_set(h->array, parent_node, temp);

        inserted_node = parent_node;
        parent_node = (inserted_node - 1) / 2;
    }

    return 0;
}

int prioq_insert(prioq *q, void *p) {
    if (q == NULL || p == NULL) {
        return -1;
    }

    heap_insert(q, p);
    return 0;
}

static void *heap_pop(struct heap *h) {
    if (h == NULL || array_size(h->array) == 0) {
        return NULL;
    }

    void *root_node = array_get(h->array, 0);
    void *last_node = array_pop(h->array);

    if (array_size(h->array) > 0) {
        array_set(h->array, 0, last_node);

        longtype index = 0;
        while (1) {
            longtype child_left = index * 2 + 1;
            longtype child_right = index * 2 + 2;
            longtype child_smallest = index;
            if (child_left < array_size(h->array)) {
                void *childLeftValue = array_get(h->array, child_left);
                if (childLeftValue != NULL && h->compare(childLeftValue, array_get(h->array, index)) < 0) {
                    child_smallest = child_left;
                }
            }

            if (child_right < array_size(h->array)) {
                void *childRightValue = array_get(h->array, child_right);
                if (childRightValue != NULL && h->compare(childRightValue, array_get(h->array, child_smallest)) < 0) {
                    child_smallest = child_right;
                }
            }

            if (child_smallest != index) {
                void *temp = array_get(h->array, index);
                array_set(h->array, index, array_get(h->array, child_smallest));
                array_set(h->array, child_smallest, temp);
                index = child_smallest;
            } else {
                break;
            }
        }
    }

    return root_node;
}

void *prioq_pop(prioq *q) {
    if (q == NULL) {
        return NULL;
    }

    return heap_pop(q);
}

