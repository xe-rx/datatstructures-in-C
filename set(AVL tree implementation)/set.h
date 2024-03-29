#ifndef SET_H
#define SET_H

struct set;

/* Return a pointer to a new empty set, or return NULL on failure.
 * If the turbo flag is set to 1 the set will use the fast tree
 * implementation. If turbo is 0 a regular BST is used. */
struct set *set_init(int turbo);

/* Insert num into set s. Return 0 for success and -1 for failure.
 * When item is already in the set s, do nothing and return 1. */
int set_insert(struct set *s, int num);

/* Lookup num in set s. Return 1 if item is found, return 0
 * otherwise. */
int set_find(struct set *s, int num);

/* Remove num from set s. Return 0 is the item is successfully removed
 * and return 1 otherwise. */
int set_remove(struct set *s, int num);

/* Free all memory associated with set s. */
void set_cleanup(struct set *s);

/* Print all the items in set s in sorted order to stdout. Print one
 * item per line.
 *
 * For debugging purposes this function can also call tree_dot() to
 * print a graphical representation of the tree. */
void set_print(const struct set *s);

/* Perform sanity checks on the data structure that implements the set
 * s. Return 0 if the checks pass, return an error code otherwise.
 *
 * This function is optional and can be implemented to help debugging.
 * It should only print to stderr, not to stdout. */
int set_verify(const struct set *s);

#endif /* SET_H */
