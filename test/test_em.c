#include "../src/second_preim_48.h"
#include "../tools/tools.h"

int test_em(void){
    uint32_t m1[4];
    uint32_t m2[4];

    uint64_t start = clock();
    find_exp_mess(m1, m2);
    uint64_t end = clock();

    uint64_t h1 = cs48_dm(m1, IV);
    uint64_t h2 = get_cs48_dm_fp(m2);

    if (h1 == h2){
        printf("PASSED in %lu ticks\n", end - start);
        printf("\n");
        return 0;
    }
    printf("FAILED \n");
    printf("\n");
    return 1;
}

int main(){
    printf("Testing find_exp_mess..\n");
    test_em();
}