#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

struct stack {
    int *collection;
    size_t capacity;
    size_t size;
    size_t max_size;
    size_t total_push;
    size_t total_pop;
};

/* Initializes a stack with the specified capacity. The function allocates
    memory for the stack structure on the heap.

   capacity: The maximum number of (in this case) ints that the stack can hold.

   Returns: A pointer to the newly initialized stack or NULL if memory
   allocation fails.
*/
struct stack *stack_init(size_t capacity) {
    if (capacity <= 0) return NULL;

    struct stack *s = malloc(sizeof(struct stack));
    if (s == NULL) return NULL;

    s->collection = malloc(sizeof(int) * capacity);
    if (s->collection == NULL) {
        free(s);
        return NULL;
    }

    s->capacity = capacity;
    s->size = 0;
    s->max_size = 0;
    s->total_push = 0;
    s->total_pop = 0;

    return s;
}

/* Cleans up the stack and its associated resources. This function frees the
    memory allocated for the stack's storage and the stack itself.

   s: A pointer to the stack to be cleaned up.
*/
void stack_cleanup(struct stack *s) {
    if (s == NULL) {
        return;
    }
    free(s->collection);
    free(s);
}

/* Prints the current statistics of the stack, such as the number of push
    and pop operations performed, and the maximum size reached by the stack.

   s: A pointer to the stack whose statistics are to be printed.
*/
void stack_stats(const struct stack *s) {
    if (s == NULL) {
        return;
    }
    fprintf(stderr, "stats %zu %zu %zu\n",
            s->total_push, s->total_pop, s->max_size);
}

/* Doubles the capacity of the stack. If memory
    allocation fails, an error is reported.

   s: A pointer to the stack to resize.

   Returns: 1 if the resizing is successful, or 0 if an error occurs.
*/
int stack_resize(struct stack *s) {
    if (s == NULL) {
        return 0;
    }

    size_t new_capacity = (s->capacity * 2) + 1;

    int *new_collection;
    new_collection = realloc(s->collection, sizeof(int) * new_capacity);
    if (new_collection == NULL) {
        return 0;
    }

    s->collection = new_collection;
    s->capacity = new_capacity;
 
    return 1;
}

/* Pushes an element onto the stack. If the stack is not full, the element is
    added to the top and the stack's size is incremented. If the push operation
    results in the stack size reaching a new maximum, the max_size attribute is
    updated. The stack size is one ahead of the actual index 'size'.

   s: A pointer to the stack.
   c: The element to be pushed onto the stack.

   Returns: 0 if the element is successfully pushed, or 1 if the stack is full.
*/
int stack_push(struct stack *s, int c) {
    if (s == NULL) {
        return 1;
    }
    if (s->size > s->capacity) {
        stack_resize(s);
    }

    if (s->size < s->capacity) {
        s->collection[s->size] = c;
        s->size++;
        s->total_push++;
    if (s->size > s->max_size) {
            s->max_size = s->size;
    }
        return 0;
    } else {
        return 1;
    }
}

/* Pops an element from the top of the stack. If the stack is not empty,
    the top element is removed, the stack's size is decremented, and the
    element is returned. If the stack is empty, an error is reported.

   s: A pointer to the stack.

   Returns: The popped element, or -1 if the stack is empty.
*/
int stack_pop(struct stack *s) {
    if (s == NULL) {
        return -1;
    }
    if (s->size > 0) {
        int popped;
        s->size--;
        popped = s->collection[s->size];
        s->total_pop++;
        return popped;
    } else {
        return -1;
    }
}

/* Peeks at the top element of the stack without removing it. If the stack is
    not empty, the top element is returned. If the stack is empty, an error is
    reported.

   s: A pointer to the stack.

   Returns: The element at the top of the stack, or -1 if the stack is empty.
*/
int stack_peek(const struct stack *s) {
    if (s == NULL) {
        return -1;
    }
    if (s->size > 0) {
        return s->collection[s->size - 1];
    } else {
        return -1;
    }
}

/* Checks whether the stack is empty. The function evaluates the stack's size
    attribute to determine if the stack contains any elements.

   s: A pointer to the stack.

   Returns: 1 if the stack is empty, 0 if it is not, or -1 if the pointer is
   NULL.
*/
int stack_empty(const struct stack *s) {
    if (s == NULL) {
        return -1;
    }
    if (!(s->size > 0)) {
        return 1;
    } else if (s->size > 0) {
        return 0;
    } else {
        return -1;
    }
}

/* Retrieves the current size of the stack, indicating how many elements
    are on the stack.

   s: A pointer to the stack.

   Returns: The number of elements currently on the stack.
*/
size_t stack_size(const struct stack *s) {
    if (s == NULL) {
        return 0;
    }
    return s->size;
}
