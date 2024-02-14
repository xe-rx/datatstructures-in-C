/*
 Name: Marouan Bellari
 SSID: 14675218

 This file implements a BST and AVL tree based on the turbo value in the tree
 struct. It handles initializing the tree, adding nodes and deleting them while
 keeping the AVL properties in check through balancing.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "tree.h"

/* I implemented a print function that is commented out in the cleanup,
*  which prints to stderr so it does not interfere with the checks, if
*  you run it it displays the balance of every node withinin the range of -1 to 1.
*  Had to do this because graphviz and time did not work for me and I
*  needed a way to check my AVL! :) */
#define PLACEHOLDER_BALANCE 99

struct tree {
    struct node *root;
    int turbo;
};

struct node {
    int data;
    int balance;
    struct node *lhs;
    struct node *rhs;
};

typedef struct node node;

/*  Recursively iterates through the tree and prints the balance factor of
 *  every node*/
void print_balance_factors(node *n) {
    if (n == NULL) {
        return;
    }

    print_balance_factors(n->lhs);
    fprintf(stderr, "balance factor: %d\n", n->balance);

    print_balance_factors(n->rhs);
}

/* Handles the case of an empty tree, if not; it calls the print balance*/
void print_tree_balance_factors(struct tree *t) {
    if (t == NULL) {
        fprintf(stderr, "tree empty?\n");
        return;
    }

    print_balance_factors(t->root);
}

/* Helper function: Allocate a new tree node and initialise it with
 * the given parameters. Return a pointer to the new node or NULL on
 * failure. */
static node *make_node(int data) {
    struct node *new_node = malloc(sizeof(struct node));
    if (new_node == NULL) {
        return NULL;
    }

    new_node->rhs = NULL;
    new_node->lhs = NULL;
    new_node->data = data;
    new_node->balance = PLACEHOLDER_BALANCE;

    return new_node;
}

/* Calculates the height of a node recursively. Returns -1 on failure and
 * the correct height when succesful. */
int node_height(struct node *n) {
    if (n == NULL) {
        return -1;
    }
    int left_height = node_height(n->lhs);
    int right_height = node_height(n->rhs);
    return (left_height > right_height ? left_height : right_height) + 1;
}

/*  This function rotates the subtree to the right to maintain AVL properties.
*   returns new (sub)tree root on succes and NULL on failure. Updates balance
*   after succesful rotation. */
static node *rotate_right(struct node *n) {
    if (n == NULL || n->lhs == NULL) {
        return NULL;
    }

    struct node *n_left = n->lhs;
    struct node *right_n_left = n_left->rhs;

    n_left->rhs = n;
    n->lhs = right_n_left;

    int left_height = node_height(n->lhs);
    int right_height = node_height(n->rhs);
    n->balance = left_height - right_height;

    left_height = node_height(n_left->lhs);
    right_height = node_height(n_left->rhs);
    n_left->balance = left_height - right_height;

    return n_left;
}

/*  This function rotates the subtree to the left to maintain AVL properties.
*   returns new (sub)tree root on succes and NULL on failure. Updates balance
*   after succesful rotation. */
static node *rotate_left(struct node *n) {
    if (n == NULL || n->rhs == NULL) {
        return NULL;
    }

    struct node *n_right = n->rhs;
    struct node *left_n_right = n_right->lhs;

    n_right->lhs = n;
    n->rhs = left_n_right;

    int left_height = node_height(n->lhs);
    int right_height = node_height(n->rhs);
    n->balance = left_height - right_height;

    left_height = node_height(n_right->lhs);
    right_height = node_height(n_right->rhs);
    n_right->balance = left_height - right_height;

    return n_right;
}

/* Function serves two purposes:
 * (1) Calculates the balance factor of a node.
 * (2) Updates the balanec factor in the node struct.
 *
 * Returns the balance factor on succes and 0 on failure.
*/
int balance_factor(struct node *n) {
    if (n == NULL) {
        return 0;
    }
    n->balance = node_height(n->lhs) - node_height(n->rhs);
    return n->balance;
}

/* Balances a given node in the tree. Calculates the balance factor and performs
 * rotations (left or right) as needed to maintain tree balance. Recursively
 * to left and right children. Returns the balanced node. */
struct node *balance_node(struct node *node) {
    if (node == NULL) {
        return NULL;
    }

    node->balance = balance_factor(node);

    if (node->balance > 1) {
        if (balance_factor(node->lhs) >= 0) {
            return rotate_right(node);
        } else {
            node->lhs = rotate_left(node->lhs);
            return rotate_right(node);
        }
    } else if (node->balance < -1) {
        if (balance_factor(node->rhs) <= 0) {
            return rotate_left(node);
        } else {
            node->rhs = rotate_right(node->rhs);
            return rotate_left(node);
        }
    }

    node->lhs = balance_node(node->lhs);
    node->rhs = balance_node(node->rhs);

    return node;
}

struct tree *tree_init(int turbo) {
    struct tree *tree = malloc(sizeof(struct tree));
    if (tree == NULL) {
        return NULL;
    }

    tree->root = NULL;
    tree->turbo = 1;

    return tree;
}

int tree_insert(struct tree *tree, int data) {
    if (tree == NULL) {
        return -1;
    }

    struct node *insert = make_node(data);
    if (tree->root == NULL) {
        tree->root = insert;
        return 0;
    }

    struct node *curr = tree->root;

    /* This loop is used to find the correct insertion point for the node;
    *  The data that needs to be inserted gets compared with data existing
    *  In the tree, due to the BST properties we can decide whether we have to
    *  iterate left or right until we reach the end of the tree which is when
    *  curr's child-pointers == NULL. If teh inserted data turns out to be a dupe
    *  it gets trashed.
    *
    *  Ik heb voor het eerst minpunten gekregen op mijn vorige opdracht voor het
    *  missen van comments boven belangrijke while-loops, vandaar deze! Weet
    * niet of het nou wel of niet moet.
    *  */
    while (1) {
        if (insert->data > curr->data) {
            if (curr->rhs == NULL) {
                curr->rhs = insert;
                break;
            }
            curr = curr->rhs;
        } else if (insert->data < curr->data) {
            if (curr->lhs == NULL) {
                curr->lhs = insert;
                break;
            }
            curr = curr->lhs;
        } else if (insert->data == curr->data) {
            free(insert);
            return 1;
        }
    }

    if (tree->turbo != 0) {
        tree->root = balance_node(tree->root);
    }
    return 0;
}

/* Recursively iterates through the tree until it finds the data in a node that
*  matches the data thats being looked for. Returns 0 on failure and 1 on succes
*/
int find_loop(struct node *node, int data) {
    if (node == NULL) {
        return 0;
    }

    if (node->data == data) {
        return 1;
    } else if (data < node->data) {
        return find_loop(node->lhs, data);
    } else {
        return find_loop(node->rhs, data);
    }
}

/* Calls find_loop until data is found, returns 1 on succes and 0 on failure. */
int tree_find(struct tree *tree, int data) {
    if (tree == NULL) {
        return 0;
    }
    return find_loop(tree->root, data);
}

/* This function handles the removal of the found node. If the node has one or
 *  no children, it directly removes the node and adjusts the parent's link. If
 *  the node has two children, it finds the in-order successor (the smallest
 * node in the right subtree), swaps the data, and then removes the successor
 * node. After removal, it checks if balancing is needed. */
int handle_removal(struct tree *tree, struct node **search_ptr,
                    struct node *search_parent) {
    struct node *search = *search_ptr;

    if (search->lhs == NULL || search->rhs == NULL) {
        struct node *new_child = (search->lhs != NULL) ? search->lhs : search->rhs;

        if (search_parent == NULL) {
            tree->root = new_child;
        } else if (search_parent->lhs == search) {
            search_parent->lhs = new_child;
        } else {
            search_parent->rhs = new_child;
        }
        free(search);
    } else {
        struct node *successor = search->rhs;
        struct node *successor_parent = search;

        while (successor->lhs != NULL) {
            successor_parent = successor;
            successor = successor->lhs;
        }

        search->data = successor->data;

        if (successor_parent != search) {
            successor_parent->lhs = successor->rhs;
        } else {
            search->rhs = successor->rhs;
        }

        free(successor);
    }

    if (tree->turbo != 0) {
        tree->root = balance_node(tree->root);
    }
    return 0;
}

/* This function searches for the node with the given data to be removed. It
 * traverses the tree, comparing the target data with each node's data. Based on
 * the comparison, it moves either left or right in the tree. When it finds the
 * matching node, it calls handle_removal to delete it. If the node isnt foud,
 * the function returns 1. */
int tree_remove(struct tree *tree, int data) {
    if (tree == NULL || tree_find(tree, data) == 0) {
        return 1;
    }

    struct node **search_ptr = &(tree->root);
    struct node *search_parent = NULL;
    while (*search_ptr != NULL) {
        if (data > (*search_ptr)->data) {
            search_parent = *search_ptr;
            search_ptr = &((*search_ptr)->rhs);
        } else if (data < (*search_ptr)->data) {
            search_parent = *search_ptr;
            search_ptr = &((*search_ptr)->lhs);
        } else {
            return handle_removal(tree, search_ptr, search_parent);
        }
    }
    return 1;
}

/* Recursively prints all the data in the tree. From left to right. */
void print_loop(struct node *node) {
    if (node == NULL) {
        return;
    }

    print_loop(node->lhs);
    printf("%d\n", node->data);
    print_loop(node->rhs);
}

void tree_print(const struct tree *tree) {
    if (tree == NULL) {
        return;
    }

    print_loop(tree->root);
}

/* Recursively frees all the nodes in the tree. */
void node_cleanup(struct node *node) {
    if (node == NULL) {
        return;
    }

    node_cleanup(node->lhs);
    node_cleanup(node->rhs);

    free(node);
}

void tree_cleanup(struct tree *tree) {
    if (tree == NULL) {
        return;
    }

    print_tree_balance_factors(tree);
    node_cleanup(tree->root);
    free(tree);
}

/* Unique id's for numbering nodes in dot format. */
static int global_node_counter = 0;

static int print_tree_dot_r(node *root, FILE *dotf) {
    int left_id, right_id, my_id = global_node_counter++;

    if (root == NULL) {
        fprintf(dotf, "    %d [shape=point];\n", my_id);
        return my_id;
    }

    fprintf(dotf, "    %d [color=%s label=\"%d\"]\n", my_id, "black", root->data);

    left_id = print_tree_dot_r(root->lhs, dotf);
    fprintf(dotf, "    %d -> %d [label=\"l\"]\n", my_id, left_id);

    right_id = print_tree_dot_r(root->rhs, dotf);
    fprintf(dotf, "    %d -> %d [label=\"r\"]\n", my_id, right_id);

    return my_id;
}

void tree_dot(const struct tree *tree, const char *filename) {
    node *root = tree->root;
    global_node_counter = 0;
    FILE *dotf = fopen(filename, "w");
    if (!dotf) {
        printf("error opening file: %s\n", filename);
        exit(1);
    }
    fprintf(dotf, "digraph {\n");
    if (root) {
        print_tree_dot_r(root, dotf);
    }
    fprintf(dotf, "}\n");
    fclose(dotf);
}
