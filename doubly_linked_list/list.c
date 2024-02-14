#include "list.h"

/*
 * Start by adding the definitions for the list and node structs. You may
 * implement any of the Linked List versions discussed in the lecture, which
 * have some different trade-offs for the functions you will need to write.
 *
 * Note: The function prototypes in list.h assume the most basic Singly Linked
 * List. If you implement some other version, you may not need all of the function
 * arguments for all of the described functions. This will produce a warning,
 * which you can suppress by adding a simple if-statement to check the value
 * of the unused parameter.
 *
 * Also, do not forget to add any required includes at the top of your file.
 */

/* Defined return variable */
#define UNDEFINED -3

/* New type for all integers used in list.c */
typedef int keytype;

struct node {
    keytype data;
    struct node* next;
    struct node* prev;
} node;

struct list {
    struct node* head;
    struct node* tail;
} list;


/* Creates a new linked list and returns a pointer to it.
 * Returns NULL on failure. */
struct list *list_init(void) {
    struct list *l = malloc(sizeof(struct list));
    if (l == NULL) {
        return NULL;
    }

    l->head = NULL;
    l->tail = NULL;

    return l;
}


/* Creates a new node that contains the number num and returns a pointer to
 * it.  Returns NULL on failure. */
struct node *list_new_node(int num) {
    struct node *n = malloc(sizeof(node));
    if (n != NULL) {
        n->next = n->prev = NULL;
        n->data = num;
        return n;
    } else {
        return NULL;
    }
}


/* Returns a pointer to the first node of the list L or NULL if list is empty. */
struct node *list_head(const struct list *l) {
    if (l == NULL) {
        return NULL;
    }

    return l->head;
}

/* Returns a pointer to the node after node N.
 * Return NULL if N is NULL or if N is the last node in the list. */
struct node *list_next(const struct node *n) {
    if (n == NULL || n->next == NULL) {
        return NULL;
    }

    return n->next;
}


/* Inserts node N at the front of list L.
 * Returns 0 if N was successfully inserted, 1 otherwise. */
int list_add_front(struct list *l, struct node *n) {
    if (l == NULL || n == NULL) {
        return 1;
    }

    n->next = l->head;
    n->prev = NULL;

    if (l->head != NULL) {
        l->head->prev = n;
    } else {
        l->tail = n;
    }
    l->head = n;

    return 0;
}

/* Returns a pointer to the last node of the list L or NULL if list is empty. */
struct node *list_tail(const struct list *l) {
    if (l == NULL) {
        return NULL;
    }
    return l->tail;
}

/* Returns a pointer to the node before node N in the list L, or returns NULL
 * if N is the first node in the list or if N is not in the list at all. */
struct node *list_prev(const struct list *l, const struct node *n) {
    if (l == NULL || n == NULL || list_node_present(l, n) == 0 || n == l->head) {
        return NULL;
    } else {
        return n->prev;
    }
}

/* Appends node N at the back of list L.
 * Returns 0 if N was successfully appended, 1 otherwise. */
int list_add_back(struct list *l, struct node *n) {
    if (l == NULL || n == NULL) {
        return 1;
    }

    if (l->head == NULL && l->tail == NULL) {
        l->head = n;
        l->tail = n;
        n->next = n->prev = NULL;
    } else {
        n->prev = l->tail;
        l->tail->next = n;
        n->next = NULL;
        l->tail = n;
    }

    return 0;
}

/* Returns the value stored in the list node N. If N is NULL the return
 * value is not defined. */
int list_node_get_value(const struct node *n) {
    if (n == NULL) {
        return UNDEFINED;
    }

    return n->data;
}

/* Set the value of node N to VALUE.
 * Returns 0 if successful, 1 otherwise. */
int list_node_set_value(struct node *n, int value) {
    if (n == NULL) {
        return 1;
    }

    n->data = value;
    return 0;
}

/* Unlink node N from list L. After unlinking, the list L contains no pointers
 * to the node N and N contains no pointers to nodes in L.
 * Returns 0 if N was successfully unlinked from list L, or 1 otherwise */
int list_unlink_node(struct list *l, struct node *n) {
    if (n == NULL || l == NULL || list_node_present(l, n) == 0) {
        return 1;
    }

    if (n == l->head) {
        l->head = n->next;
        if (l->head != NULL) {
            l->head->prev = NULL;
        }
    }

    if (n == l->tail) {
        l->tail = n->prev;
        if (l->tail != NULL) {
            l->tail->next = NULL;
        }
    }

    if (n->prev != NULL) {
        n->prev->next = n->next;
    }
    if (n->next != NULL) {
        n->next->prev = n->prev;
    }

    n->prev = NULL;
    n->next = NULL;

    return 0;
}


/* Frees node N. */
void list_free_node(struct node *n) {
    if (n != NULL) {
        free(n);
    }
}


/* Cleans up entire list L data structure.
 * Returns 0 if successful, 1 otherwise. */
int list_cleanup(struct list *l) {
    if (l == NULL) {
        return 1;
    }

    struct node* curr = l->head;
    while (curr != NULL) {
        struct node* temp = curr->next;
        free(curr);
        curr = temp;
    }

    free(l);
    return 0;
}


/* Returns 1 if node N is present in list L and 0 if N is not present
 * in L. Returns -1 if an error occured. */
int list_node_present(const struct list *l, const struct node *n) {
    if (n == NULL || l == NULL) {
        return -1;
    }

    for (struct node* curr = l->head; curr != NULL; curr = curr->next) {
        if (curr->next == n->next && curr->prev == n->prev && curr->data == n->data) {
            return 1;
        }
    }
    return 0;
}


/* Inserts node N after node M in list L.
 * Fails if node M is not in the list L or if node N is already in list L.
 * Returns 0 if N was successfully inserted, or 1 otherwise. */
int list_insert_after(struct list *l, struct node *n, struct node *m) {
    if (l == NULL || n == NULL || m == NULL) {
        return 1;
    }

    if (list_node_present(l, m) == 0 || list_node_present(l, n) == 1) {
        return 1;
    }

    n->next = m->next;
    n->prev = m;
    m->next = n;

    if (n->next != NULL) {
        n->next->prev = n;
    } else {
        l->tail = n;
    }

    return 0;
}


/* Inserts node N before node M in list L.
 * Fails if node M is not in the list L or if node N is already in list L.
 * Returns 0 if N was successfully inserted, or 1 otherwise. */
int list_insert_before(struct list *l, struct node *n, struct node *m) {
    if (l == NULL || n == NULL || m == NULL) {
        return 1;
    }

    if (list_node_present(l, m) == 0 || list_node_present(l, n) == 1) {
        return 1;
    }

    n->next = m;
    n->prev = m->prev;
    if (m->prev != NULL) {
        m->prev->next = n;
    } else {
        l->head = n;
    }
    m->prev = n;

    return 0;
}

/* Returns the length of list L, or 0 if L is the NULL pointer */
size_t list_length(const struct list *l) {
    if (l == NULL) {
        return 0;
    }

    size_t size_list = 0;
    for (struct node* curr = l->head; curr != NULL; curr = curr->next) {
        size_list++;
    }
    return size_list;
}

/* Returns a pointer to the i^th node of list L or NULL if there is no i^th
 * element in list L. */
struct node *list_get_ith(const struct list *l, size_t i) {
    if (l == NULL || list_length(l) <= i) {
        return NULL;
    }

    struct node* curr = l->head;
    for (size_t j = 0; j < i; j++) {
        curr = curr->next;
    }
    return curr;
}

/* Cuts list L into 2 lists, with node N being the last node in the first half
 * and all nodes after nodes N are part to the second half, in the same
 * order they were in in the original list.  Modifies list L to only contain
 * the first half and creates a new list for the second half. Returns a
 * pointer to the second half if successfully cut and NULL otherwise. */
struct list *list_cut_after(struct list *l, struct node *n) {
    if (l == NULL || n == NULL || list_node_present(l, n) == 0) {
        return NULL;
    }

    struct list *l_second = malloc(sizeof(struct list));
    if (l_second == NULL) {
        return NULL;
    }

    if (n->next == NULL) {
        l_second->head = l_second->tail = NULL;
        l->tail = n;
    } else {
        l_second->head = n->next;
        l_second->tail = l->tail;
        l_second->head->prev = NULL;

        l->tail = n;
        n->next = NULL;
    }

    return l_second;
}

