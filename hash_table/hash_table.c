/* Name: M. Bellari
   UvAnetID: 14675218
   Course: BSc Informatica

    This file defines a hash table for storing strings as keys and arrays of
    integers as values. It includes functions to initialize the table, insert
    key-value pairs, lookup values by key, calculate load factor, delete keys,
    and clean up the table.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "array.h"
#include "hash_table.h"
#include "hash_func.h"

#define ARRAY_SIZE 100

typedef unsigned long longtype;
struct table {
    /* The (simple) array used to index the table */
    struct node **array;
    /* The function used for computing the hash values in this table */
    unsigned long (*hash_func)(const unsigned char *);
    /* Maximum load factor after which the table array should be resized */
    double max_load_factor;
    /* Capacity of the array used to index the table */
    unsigned long capacity;
    /* Current number of elements stored in the table */
    unsigned long load;
};

/* Note: This struct should be a *strong* hint to a specific type of hash table
 * You may implement other options, if you can build them in such a way they
 * pass all tests. However, the other options are generally harder to code. */
struct node {
    /* The string of characters that is the key for this node */
    char *key;
    /* A resizing array, containing the all the integer values for this key */
    struct array *value;
    /* Next pointer */
    struct node *next;
};

/* Missing comment */
int resize_and_rehash(struct table *t);

struct table *table_init(unsigned long capacity,
                         double max_load_factor,
                         unsigned long (*hash_func)(const unsigned char *)) {

    /* Note: The 'array' member of struct table is a pointer to a block of
     * memory that contains pointers to struct nodes. Make sure that the struct
     * node pointers in this block are all set to NULL. A good way to do this is
     * with the calloc() function. Check the manual page 'man calloc' for
     * its usage. */

    struct table *table = malloc(sizeof(struct table));
    if (table == NULL) {
        return NULL;
    }

    table->array = calloc(capacity, sizeof(struct node *));
    if (table->array == NULL) {
        free(table);
        return NULL;
    }

    table->capacity = capacity;
    table->max_load_factor = max_load_factor;
    table->hash_func = hash_func;
    table->load = 0;

    return table;
}

struct node *node_init(const char *key) {
    if (key == NULL) {
        return NULL;
    }

    struct node *new_node = malloc(sizeof(struct node));
    if (new_node == NULL) {
        return NULL;
    }

    new_node->key = malloc(strlen(key) + 1);
    if (new_node->key == NULL) {
        free(new_node);
        return NULL;
    }

    strcpy(new_node->key, key);

    new_node->value = array_init(ARRAY_SIZE);
    if (new_node->value == NULL) {
        free(new_node->key);
        free(new_node);
        return NULL;
    }

    new_node->next = NULL;

    return new_node;
}



int table_insert(struct table *t, const char *key, int value) {
    if (key == NULL || t == NULL) {
        return 1;
    }

    longtype hash = t->hash_func((const unsigned char *)key);
    longtype index = hash % t->capacity;

    struct array *existing_array = table_lookup(t, key);
    if (existing_array != NULL) {
        if (array_append(existing_array, value) != 0) {
            return 1;
        }
        return 0;
    }

    struct node *new_node = node_init(key);
    if (new_node == NULL) {
        return 1;
    } else if (array_append(new_node->value, value) == 1) {
        array_cleanup(new_node->value);
        free(new_node->key);
        free(new_node);
        return 1;
    }

    new_node->next = t->array[index];
    t->array[index] = new_node;

    t->load++;
    if (((double)t->load / (double)t->capacity) >= t->max_load_factor) {
        resize_and_rehash(t);
    }

    return 0;
}

struct array *table_lookup(const struct table *t, const char *key) {
    if (t == NULL || key == NULL) {
        return NULL;
    }

    longtype hash = t->hash_func((const unsigned char *)key);
    longtype index = hash % t->capacity;

    struct node *current = t->array[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }

    return NULL;
}


double table_load_factor(const struct table *t) {
    if (t == NULL) {
        return -1.0;
    }
    return (double)t->load / (double)t->capacity;
}

int table_delete(struct table *t, const char *key) {
    if (t == NULL || key == NULL) {
        return -1;
    }

    longtype hash = t->hash_func((const unsigned char *)key);
    longtype index = hash % t->capacity;

    struct node *current = t->array[index];
    struct node *prev = NULL;

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (prev == NULL) {
                t->array[index] = current->next;
            } else {
                prev->next = current->next;
            }

            array_cleanup(current->value);
            free(current->key);
            free(current);

            t->load--;

            return 0;
        }

        prev = current;
        current = current->next;
    }

    return 1;
}

void table_cleanup(struct table *t) {
    if (t == NULL) {
        return;
    }

    for (longtype i = 0; i < t->capacity; i++) {
        struct node *current = t->array[i];
        while (current != NULL) {
            struct node *temp = current;
            current = current->next;

            array_cleanup(temp->value);
            free(temp->key);
            free(temp);
        }
    }

    free(t->array);

    free(t);
}

int resize_and_rehash(struct table *t) {
    if (t == NULL) {
        return -1;
    }

    longtype new_capacity = t->capacity * 2;
    struct node **new_array = calloc(new_capacity, sizeof(struct node *));
    if (new_array == NULL) {
        return -1;
    }

    for (longtype i = 0; i < t->capacity; i++) {
        struct node *current = t->array[i];
        while (current != NULL) {
            longtype new_index = t->hash_func((const unsigned char *)current->key)
                                                 % new_capacity;

            struct node *temp = current;
            current = current->next;
            temp->next = new_array[new_index];
            new_array[new_index] = temp;
        }
    }

    free(t->array);
    t->array = new_array;
    t->capacity = new_capacity;


    return 0;
}
