#include "../src/second_preim_48.h"
#include "../tools/tools.h"

#define NB_TESTS 5
#define NB_FIND 100

void test_em(void)
{   
    printf("Testing find_exp_mess..\n");
    float average = 0.0f;

    for (unsigned i = 0; i < NB_TESTS; i++)
    {   
        printf("Test %d/%d: ", i+1, NB_TESTS);
        uint32_t m1[4];
        uint32_t m2[4];

        uint64_t start = clock();
        find_exp_mess(m1, m2);
        uint64_t end = clock();

        uint64_t h1 = cs48_dm(m1, IV);
        uint64_t h2 = get_cs48_dm_fp(m2);

        if (h1 == h2){
            printf("PASSED in %lu ticks (%fs)\n", end - start, ((float)(end - start))/1000000);
        } else {
            printf("FAILED \n");
        }

        average += (float)(end - start);
    }

    printf("Average time: %fs\n", average / (NB_TESTS * 1000000));
    printf("\n");
}

void find_c1_c2(){

    printf("Finding c1 and c2..\n");
    
    float c1 = 0.0f;
    float c2 = 0.0f;

    uint64_t start = 0.0f;
    uint64_t end = 0.0f;
    
    uint32_t m1[4];
    uint32_t m2[4];
    
    uint64_t N = 14000000;
    const float load_factor = 0.75;
    uint64_t nb_buckets = (uint64_t)(((float)N) / load_factor);
    
    for (unsigned i = 0; i < NB_FIND; i++)
    {   
        printf("Measure n°%d..\n", i+1);
        struct Hashmap *hash = init_hash(nb_buckets);
        rdm_init();

        start = clock();
        // Step 1: Generate random first-block messages, storing their chaining values
        for (unsigned j = 0; j < N; j++) {
            rdm_block(m1);
            uint64_t h = cs48_dm(m1, IV);
            add_hash(hash, h, m1);
        }
        end = clock();
        c1 += (float)(end - start);

        start = clock();
        // Step 2: Compute fixed points for random messages until a match is found
        for (unsigned j = 0; j < 10000; j++) {
            rdm_block(m2);
            uint64_t h_fp = get_cs48_dm_fp(m2);

            // Check if this fixed point matches any chaining value
            if (get_hash(hash, h_fp, m1)) {
                break;  // Collision found
            }
        }
        end = clock();
        c2 += (float)(end - start);

        free_hash(hash);
    }

    printf("Average time for c1: %fs\n", c1 / (N*NB_FIND));
    printf("Average time for c2: %fs\n", c2 / (10000*NB_FIND));
}

void compare_perf()
{
    printf("Comparing performances depending on the value of N..\n");

    unsigned N = 10000000;

    for (unsigned i = 0; i < 20; i++)
    {
        printf("Measure n°%d/20..\n", i+1);
        config_N(N);
        
        float mean = 0.0f;
        for (unsigned j = 0; j < NB_TESTS; j++)
        {   
            uint32_t m1[4];
            uint32_t m2[4];
            
            uint64_t start = clock();
            find_exp_mess(m1, m2);
            uint64_t end = clock();
            mean += (float)(end - start);
        }

        mean /= NB_TESTS;
        printf("Average time for N=%d: %fs\n", N, mean/1000000);
        N += 1000000;
    }
}

int main(){
    test_em();
    //find_c1_c2();
    //compare_perf();
    return 0;
}