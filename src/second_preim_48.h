#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define ROTL24_16(x) ((((x) << 16) ^ ((x) >> 8)) & 0xFFFFFF)
#define ROTL24_3(x) ((((x) << 3) ^ ((x) >> 21)) & 0xFFFFFF)

#define ROTL24_8(x) ((((x) << 8) ^ ((x) >> 16)) & 0xFFFFFF)
#define ROTL24_21(x) ((((x) << 21) ^ ((x) >> 3)) & 0xFFFFFF)

#define IV 0x010203040506ULL 

/*
 * the 96-bit key is stored in four 24-bit chunks in the low bits of k[0]...k[3]
 * the 48-bit plaintext is stored in two 24-bit chunks in the low bits of p[0], p[1]
 * the 48-bit ciphertext is written similarly in c
 */
void speck48_96(const uint32_t k[4], const uint32_t p[2], uint32_t c[2]);

/* Hashmap Structure */

/* Hashmap */
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

struct Hashmap* hashmap_init(uint64_t nb_buckets);

void add_hash(struct Hashmap* hash, uint64_t k, uint32_t v[4]);

int get_hash(struct Hashmap* hash, uint64_t k, uint32_t v[4]);

void free_hash(struct Hashmap* hash);


/* the inverse cipher */
void speck48_96_inv(const uint32_t k[4], const uint32_t c[2], uint32_t p[2]);

uint64_t cs48_dm(const uint32_t m[4], const uint64_t h);

uint64_t hs48(const uint32_t *m, uint64_t fourlen, int padding, int verbose);

uint64_t get_cs48_dm_fp(uint32_t m[4]);

void rdm_block(uint32_t m[4]);

void find_exp_mess(uint32_t m1[4], uint32_t m2[4]);

void attack(void);

