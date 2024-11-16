#ifndef SECOND_PREIM_48_H
#define SECOND_PREIM_48_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

// Rotation macros for 24-bit integers
#define ROTL24_16(x) ((((x) << 16) ^ ((x) >> 8)) & 0xFFFFFF)
#define ROTL24_3(x) ((((x) << 3) ^ ((x) >> 21)) & 0xFFFFFF)

#define ROTL24_8(x) ((((x) << 8) ^ ((x) >> 16)) & 0xFFFFFF)
#define ROTL24_21(x) ((((x) << 21) ^ ((x) >> 3)) & 0xFFFFFF)


// Initial vector for hash computation
#define IV 0x010203040506ULL 

/**
 * @brief Encrypts a 48-bit plaintext using the Speck48/96 cipher.
 *
 * @param k The 96-bit key split into four 24-bit integers (low bits of k[0] to k[3]).
 * @param p The 48-bit plaintext split into two 24-bit integers (low bits of p[0] and p[1]).
 * @param c The 48-bit ciphertext written into two 24-bit integers (low bits of c[0] and c[1]).
 */

void speck48_96(const uint32_t k[4], const uint32_t p[2], uint32_t c[2]);


/**
 * @brief Decrypts a 48-bit ciphertext using the Speck48/96 cipher.
 *
 * @param k The 96-bit key split into four 24-bit integers (low bits of k[0] to k[3]).
 * @param c The 48-bit ciphertext split into two 24-bit integers (low bits of c[0] and c[1]).
 * @param p The 48-bit plaintext is written into two 24-bit integers (low bits of p[0] and p[1]).
 */

void speck48_96_inv(const uint32_t k[4], const uint32_t c[2], uint32_t p[2]);


/**
 * @brief Davies-Meyer compression function based on Speck48/96.
 *
 * @param m The 96-bit message split into four 24-bit integers.
 * @param h The 48-bit chaining value represented as the low 48 bits of a 64-bit word.
 * @return The new 48-bit chaining value after applying the compression function.
 */

uint64_t cs48_dm(const uint32_t m[4], const uint64_t h);


/**
 * @brief Computes the hash of a message using the cs48_dm compression function.
 *
 * @param m The message split into 96-bit blocks (each block has four 24-bit integers).
 * @param fourlen The number of 96-bit blocks in the message.
 * @param padding Whether to include padding (1 for true, 0 for false).
 * @param verbose Whether to output intermediate steps (1 for true, 0 for false).
 * @return The 48-bit hash represented as the low 48 bits of a 64-bit word.
 */

uint64_t hs48(const uint32_t *m, uint64_t fourlen, int padding, int verbose);


/**
 * @brief Computes the unique fixed-point for the cs48_dm compression function for a given message block.
 *
 * @param m The 96-bit message block split into four 24-bit integers.
 * @return The fixed-point 48-bit chaining value.
 */

uint64_t get_cs48_dm_fp(uint32_t m[4]);


/**
 * @brief Generates a random 96-bit block using the Xoshiro256** random number generator.
 *
 * @param m The generated block is written into four 24-bit integers (low bits of m[0] to m[3]).
 */

void rdm_block(uint32_t m[4]);


/**
 * @brief Initializes the Xoshiro256** random number generator with a default seed.
 */

void rdm_init();


/**
 * @brief Configures the number of iterations for the expandable message search.
 *
 * @param N The number of iterations to use in the `find_exp_mess` function.
 */

void config_N(unsigned N);


/**
 * @brief Finds a two-block expandable message for the hs48 hash function.
 *
 * This computes two 96-bit blocks \( m1 \) and \( m2 \) such that:
 * \( hs48(m1 || m2) = hs48(m1 || m2^*) \), where \( m2^* \) is a compressed version of \( m2 \).
 *
 * @param m1 The first block of the expandable message (output).
 * @param m2 The second block of the expandable message (output).
 */

void find_exp_mess(uint32_t m1[4], uint32_t m2[4]);


/**
 * @brief Executes the attack to find a second preimage for a given message.
 *
 * The function computes a new message that produces the same hash as the original
 * message but differs in content. It uses expandable messages and fixed-point collisions.
 */

void attack(void);


/* Hashmap Structure */
struct HashNode
{
  uint64_t k;
  uint32_t v[4];
  struct HashNode *next_node;
};

struct Hashmap
{
  struct HashNode **buckets;
  uint64_t bucket_count;
  uint64_t hash_mask;
};


/**
 * @brief Initializes a hashmap to store chaining values for collision search.
 *
 * @param nb_buckets The number of buckets in the hashmap.
 * @return A pointer to the initialized hashmap structure.
 */

struct Hashmap* init_hash(uint64_t nb_buckets);


/**
 * @brief Adds a key-value pair to the hashmap.
 *
 * @param hash The hashmap to add the key-value pair to.
 * @param k The key (48-bit chaining value).
 * @param v The value (96-bit block split into four 24-bit integers).
 */

void add_hash(struct Hashmap* hash, uint64_t k, uint32_t v[4]);


/**
 * @brief Retrieves a value from the hashmap for a given key.
 *
 * @param hash The hashmap to search.
 * @param k The key to search for (48-bit chaining value).
 * @param v The output value associated with the key (96-bit block).
 * @return 1 if the key is found, 0 otherwise.
 */

int get_hash(struct Hashmap* hash, uint64_t k, uint32_t v[4]);


/**
 * @brief Frees the memory allocated for a hashmap.
 *
 * @param hash The hashmap to free.
 */

void free_hash(struct Hashmap* hash);



#endif // SECOND_PREIM_48_H

