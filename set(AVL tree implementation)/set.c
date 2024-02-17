/*
 * This file implements a set based on a BST (or AVL) tree, it handles initializiation,
 * cleanup, printing the values of the set to stdout, adding "nodes" and removing
 * them.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "set.h"
#include "tree.h"

struct set {
    int *members;
    int length;
    struct tree *tree;
};

struct set *set_init(int turbo) {
    struct set *s = malloc(sizeof(struct set));
    if (s == NULL) {
        return NULL;
    }

    s->tree = tree_init(turbo);
    if (s->tree == NULL) {
        free(s);
        return NULL;
    }

    return s;
}

int set_insert(struct set *s, int num) {
    if (s == NULL) {
        return -1;
    } else if (tree_find(s->tree, num) == 1) {
        return 1;
    } else if (tree_insert(s->tree, num) == 0) {
        return 0;
    }

    return -1;
}

int set_find(struct set *s, int num) {
    if (s == NULL || tree_find(s->tree, num) == 0) {
        return 0;
    }

    return 1;
}

int set_remove(struct set *s, int num) {
    if (s == NULL || tree_remove(s->tree, num) == 1) {
        return 1;
    }

    return 0;
}

void set_cleanup(struct set *s) {
    if (s == NULL) {
        return;
    }

    tree_dot(s->tree, "tree.dot");
    tree_cleanup(s->tree);
    free(s);
}

void set_print(const struct set *s) {
    if (s == NULL) {
        return;
    }

    tree_print(s->tree);
}

int set_verify(const struct set *s) {
    /* Chose not to implement because it was optional, when I remove it
    it gives me a "clang error: linker command failed with exit code 1"
    This is why its still present in the code. */
    return 0;
}
