/* Name: M. Bellari
   UvAnetID: 14675218
   Course: BSc Informatica

    This file consists of all the hash functions.
*/
#include "limits.h"
#include "hash_func.h"


/* Do not edit this function, as it used in testing too
 * Add you own hash functions with different headers instead. */
unsigned long hash_too_simple(const unsigned char *str) {
    return (unsigned long) *str;
}

/* djb2 algorithm, attributed to Daniel J. Bernstein. */
unsigned long hash_djb2 (const unsigned char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = hash * 33 + (unsigned long)c;
    }

    return hash;
}

/* Multiplcation algorithm making use of the golden ratio.
*   Source: https://softwareengineering.stackexchange.com/questions/402542/where
*           -do-magic-hashing-constants-like-0x9e3779b9-and-0x9e3779b1-come-from
*/
unsigned long hash_multiplication(const unsigned char *str) {
    double A = 0.6180339887;
    double hash = 0;

    while (*str) {
        hash = (hash * 256 + *str) * A;
        str++;
    }

    return (unsigned long)(hash * UINT_MAX);
}

/* A hash function for strings. It mixes each character into the hash
 * by rotating and blending with arithmetic operations. */
unsigned long hash_original(const unsigned char *str) {
    unsigned long hash = 0;
    int counter = 1;
    while (*str) {
        hash = (hash << 4) | (hash >> (sizeof(hash) * 8 - 4));
        hash += ((unsigned long)(*str) * (unsigned long)counter) ^ (hash >> 3);
        counter = ((counter + *str) % 19) + 1;

        str++;
    }

    return hash;
}
