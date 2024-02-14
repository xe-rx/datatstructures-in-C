
/* Example hash function with terrible performance */
unsigned long hash_too_simple(const unsigned char *str);

/* djb2 algorithm, attributed to Daniel J. Bernstein. */
unsigned long hash_djb2 (const unsigned char *str);

/* Multiplcation algorithm making use of the golden ratio.
*   Source: https://softwareengineering.stackexchange.com/questions/402542/where
*           -do-magic-hashing-constants-like-0x9e3779b9-and-0x9e3779b1-come-from
*/
unsigned long hash_multiplication(const unsigned char *str);

/* A hash function for strings. It mixes each character into the hash
 * by rotating and blending with arithmetic operations. */
unsigned long hash_original(const unsigned char *str);
