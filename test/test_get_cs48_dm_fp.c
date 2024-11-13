#include "../src/second_preim_48.h"
#include "../tools/tools.h"

#define NB_TESTS 10

int test(){

    uint32_t m[4];
    
    for (int i = 0; i < NB_TESTS; i++){

        printf("- Test n°%d\n", i+1);
        gen_rdm((uint8_t*)m, 16);
        for (int j = 0; j < 4; j++){
            m[j] &= 0xFFFFFF;
        }

        printf("message: %06X %06X %06X %06X\n", m[0], m[1], m[2], m[3]);

        uint64_t fp = get_cs48_dm_fp(m);
        uint64_t f = cs48_dm(m, fp);

        if (f == fp){
            printf("Test n°%d PASSED\n", i+1);
        } else {
            printf("Test n°%d FAILED\n", i+1);
            return 1;
        }
    }
    printf("\n");
    return 0;
}

int main(){
    printf("Testing cs48_dm..\n");
    test();
}