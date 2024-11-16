#include "../src/second_preim_48.h"
#include "../tools/tools.h"

#define NB_TESTS 20

void test_attack_perf()
{   
    float mean = 0.0f;

    for (unsigned i = 0; i < NB_TESTS; i++)
    {
        printf("- Measure n°%d/20..\n", i+1);
        
        uint64_t start = clock();
        attack();
        uint64_t end = clock();
        
        mean += (float)(end - start);
        printf("Time: %fs\n", (float)(end - start)/1000000);
    }

    printf("Average time: %fs\n", mean / (NB_TESTS*1000000));
}


int main(){
    attack();
    test_attack_perf();
    return 0;
}