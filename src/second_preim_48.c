#include "second_preim_48.h"
#include "xoshiro.h"

void speck48_96(const uint32_t k[4], const uint32_t p[2], uint32_t c[2])
{
	uint32_t rk[23];
	uint32_t ell[3] = {k[2], k[1], k[0]};

	rk[0] = k[3];

	c[0] = p[0];
	c[1] = p[1];

	/* full key schedule */
	for (unsigned i = 0; i < 22; i++)
	{
		uint32_t new_ell = ((ROTL24_16(ell[0]) + rk[i]) ^ i) & 0xFFFFFF; // addition (+) is done mod 2**24
		rk[i+1] = ROTL24_3(rk[i]) ^ new_ell;
		ell[0] = ell[1];
		ell[1] = ell[2];
		ell[2] = new_ell;
	}

	for (unsigned i = 0; i < 23; i++)
	{
		c[0] = ((ROTL24_16(c[0]) + c[1]) ^ rk[i]) & 0xFFFFFF;
		c[1] = ROTL24_3(c[1]) ^ c[0];
	}

	return;
}

void speck48_96_inv(const uint32_t k[4], const uint32_t c[2], uint32_t p[2])
{
	uint32_t rk[23];
	uint32_t ell[3] = {k[2], k[1], k[0]};

	rk[0] = k[3];

	p[0] = c[0];
	p[1] = c[1];

	/* full key schedule */
	for (unsigned i = 0; i < 22; i++)
	{
		uint32_t new_ell = ((ROTL24_16(ell[0]) + rk[i]) ^ i) & 0xFFFFFF; // addition (+) is done mod 2**24
		rk[i+1] = ROTL24_3(rk[i]) ^ new_ell;
		ell[0] = ell[1];
		ell[1] = ell[2];
		ell[2] = new_ell;
	}

	for (int i = 22; i >= 0; i--)
	{
		p[1] = ROTL24_21(p[0] ^ p[1]);
		p[0] = ROTL24_8(((p[0] ^ rk[i]) - p[1]) & 0xFFFFFF);
	}
}

/* The Davies-Meyer compression function based on speck48_96,
 * using an XOR feedforward
 * The input/output chaining value is given on the 48 low bits of a single 64-bit word,
 * whose 24 lower bits are set to the low half of the "plaintext"/"ciphertext" (p[0]/c[0])
 * and whose 24 higher bits are set to the high half (p[1]/c[1])
 */
uint64_t cs48_dm(const uint32_t m[4], const uint64_t h)
{
	uint32_t p[2] = {h & 0xFFFFFF, (h >> 24) & 0xFFFFFF};
	uint32_t c[2];

	speck48_96(m, p, c);

	c[0] ^= p[0];
	c[1] ^= p[1];

	return ((uint64_t)c[1] << 24) | (uint64_t)c[0];

}

/* Assumes message length is fourlen * four blocks of 24 bits, each stored as the low bits of 32-bit words
 * fourlen is stored on 48 bits (as the 48 low bits of a 64-bit word)
 * when padding is included, simply adds one block (96 bits) of padding with fourlen and zeros on higher bits
 * (The fourlen * four blocks must be considered “full”, otherwise trivial collisions are possible)
 */
uint64_t hs48(const uint32_t *m, uint64_t fourlen, int padding, int verbose)
{
	uint64_t h = IV;
	const uint32_t *mp = m;

	for (uint64_t i = 0; i < fourlen; i++)
	{
		h = cs48_dm(mp, h);
		if (verbose)
			printf("@%lu : %06X %06X %06X %06X => %06lX\n", i, mp[0], mp[1], mp[2], mp[3], h);
		mp += 4;
	}
	if (padding)
	{
		uint32_t pad[4];
		pad[0] = fourlen & 0xFFFFFF;
		pad[1] = (fourlen >> 24) & 0xFFFFFF;
		pad[2] = 0;
		pad[3] = 0;
		h = cs48_dm(pad, h);
		if (verbose)
			printf("@%lu : %06X %06X %06X %06X => %06lX\n", fourlen, pad[0], pad[1], pad[2], pad[3], h);
	}

	return h;
}

/* Computes the unique fixed-point for cs48_dm for the message m */
uint64_t get_cs48_dm_fp(uint32_t m[4])
{
	/* we need a h s.t cs48_dm(m, h) = h,thus speck48_96(m, h) = 0 and  
	we have speck48_96_inv(m, 0) = h */
	uint32_t p[2] = {0, 0};
	uint32_t c[2] = {0, 0};

	speck48_96_inv(m, c, p);
	return ((uint64_t)p[1] << 24) | (uint64_t)p[0];
}

/* Hashmap 
 * init_hash() To initialize my hashmap
 * add_hash() To add an element in my hashmap
 * get_hash() To get an element in the hashmap
 * free_hash() To free the memory allocated for my hashmap
*/

struct Hashmap* init_hash(uint64_t nb_buckets) 
{	
	struct Hashmap *hash = malloc(sizeof(struct Hashmap));
	if (!hash) 
	{
		fprintf(stderr, "Failed to allocate memory for Hashmap structure.\n");
		exit(EXIT_FAILURE);
	}
	// Initialize the mask and fill_mask used for efficient indexing
	uint64_t hash_mask = 0;
	uint64_t fill_mask = 0;

	// This sets hash_mask to have 1's from the MSB of nb_buckets downward
	for (int i = 63; i>= 0; i--)
	{
		if ((!fill_mask) && (nb_buckets & ((uint64_t)1 << i)))
		{
			fill_mask = 1;
		}
		if (fill_mask)
		{
			hash_mask = hash_mask | ((uint64_t)1 << i);
		}
	}

	// Set the number of buckets as a power of two for faster indexing
	hash->hash_mask = hash_mask;
	hash->bucket_count = hash_mask+1;
	// Allocate memory for the bucket array, initialized to NULL
	hash->buckets = calloc(hash->bucket_count, sizeof(struct HashNode*));
	if (!hash->buckets) 
	{
		fprintf(stderr, "Failed to allocate memory for buckets.\n");
		free(hash);
		exit(EXIT_FAILURE);
	}

	return hash;
}

void add_hash(struct Hashmap* hash, uint64_t k, uint32_t v[4]) 
{
	int64_t bucket = k & (hash->hash_mask);
	struct HashNode *new_node = malloc(sizeof(struct HashNode));
	if (!new_node) 
	{
		fprintf(stderr, "Failed to allocate memory for HashNode structure.\n");
		exit(EXIT_FAILURE);
	}
	
	new_node->k = k;
	memcpy(new_node->v, v, 4*sizeof(uint32_t));
	new_node->next_node = hash->buckets[bucket];
	hash->buckets[bucket] = new_node;
}

int get_hash(struct Hashmap* hash, uint64_t k, uint32_t v[4]) 
{
	uint64_t bucket = k & hash->hash_mask;
	struct HashNode *node = hash->buckets[bucket];
	
	while (node != NULL)
	{
		if (node->k == k)
		{
			memcpy(v, node->v, 4*sizeof(uint32_t));
			return 1;
		}
		node = node->next_node;
	}

	return 0;
}

void free_hash(struct Hashmap* hash) 
{
	for (unsigned i = 0; i < hash->bucket_count; i++)
	{
		struct HashNode *node = hash->buckets[i];
		while (node != NULL)
		{
			struct HashNode *next = node->next_node;
			free(node);
			node = next;
		}
	}

	free(hash->buckets);
	free(hash);
}

void rdm_block(uint32_t m[4])
{
	uint64_t a = xoshiro256starstar_random_unsafe();
	uint64_t b = xoshiro256starstar_random_unsafe();

	m[0] = a & 0xFFFFFF;
	m[1] = (a >> 24) & 0xFFFFFF;

	m[2] = b & 0xFFFFFF;
	m[3] = (b >> 24) & 0xFFFFFF;
}

/* Finds a two-block expandable message for hs48, using a fixed-point
 * That is, computes m1, m2 s.t. hs48_nopad(m1||m2) = hs48_nopad(m1||m2^*),
 * where hs48_nopad is hs48 with no padding */
void find_exp_mess(uint32_t m1[4], uint32_t m2[4]) 
{
    
	uint64_t N = 15000000;
    const float load_factor = 0.75;
    uint64_t nb_buckets = (uint64_t)(((float)N) / load_factor);
    
	struct Hashmap *hash = init_hash(nb_buckets);

    __my_little_xoshiro256starstar_unseeded_init();

    // Step 1: Generate random first-block messages, storing their chaining values
    for (unsigned i = 0; i < N; i++) {
        rdm_block(m1);
        uint64_t h = cs48_dm(m1, IV);
        add_hash(hash, h, m1);
    }

    // Step 2: Compute fixed points for random messages until a match is found
    while (1) {
        rdm_block(m2);
        uint64_t h_fp = get_cs48_dm_fp(m2);

        // Check if this fixed point matches any chaining value
        if (get_hash(hash, h_fp, m1)) {
            break;  // Collision found
        }
    }

    free_hash(hash);
}
