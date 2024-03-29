#ifndef TREE_H
#define TREE_H

struct tree;

/* Return a pointer to a new tree. Return NULL on failure.
 * If the turbo flag is 0 the tree is initialized as a regular
 * binary search tree. If the turbo flag is 1 the tree is initialized
 * as a fast balanced binary search tree. */
struct tree *tree_init(int turbo);

/* Insert data into the tree. Return 0 if successful and -1 for
 * failure. If the data is already in the tree it remains unchanged
 * and 1 is returned. */
int tree_insert(struct tree *tree, int num);

/* Find data in the tree. Return 1 if the tree contains the data or
 * 0 if the data is not found. */
int tree_find(struct tree *tree, int num);

/* Remove data from the tree. Return 0 if successful.  If the data is
 * not found 1 is returned. */
int tree_remove(struct tree *tree, int num);

/* Print the contents of the tree in sorted order to stdout. Print one
 * item per line. */
void tree_print(const struct tree *tree);

/* Free all memory associated with the tree. */
void tree_cleanup(struct tree *tree);

/* Check properties of the BST and balacing properties if turbo is
 * set. Return 0 if the checks pass, return an error code  otherwise.
 *
 * This function is optional can be implemented to  for debugging.
 * It should only print to stderr, not to stdout. */
int tree_check(const struct tree *tree);

/* Print a dot representation of the tree to filename. */
void tree_dot(const struct tree *tree, const char *filename);

#endif /* TREE_H */
