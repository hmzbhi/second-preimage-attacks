#include "../src/second_preim_48.h"
#include "../tools/tools.h"

int test(){
    uint64_t h = IV;
    uint32_t m[4] = {0, 1, 2, 3};

    uint64_t res = cs48_dm(m, h);
    if (res == 0x5DFD97183F91ULL){
        printf("Test PASSED\n");
        printf("\n");
        return 0;
    } else {
        printf("Test FAILED\n");
        printf("\n");
        return 1;
    }
}

int main(){
    printf("Testing cs48_dm..\n");
    test();
}