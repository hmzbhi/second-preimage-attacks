#include "../src/second_preim_48.h"
#include "../tools/tools.h"

#define NB_KEYS 10

int test(){
    /* generate some random keys and plaintexts (4 for the key and 2 for the plaintext)*/
    uint32_t keys_plain[NB_KEYS][6];
    gen_rdm((uint8_t *)keys_plain, NB_KEYS*6*sizeof(uint32_t));

    /* test speck48_96_inv (& 0xFFFFFF is used to work with 24 bits)*/
    for (int i = 0; i < NB_KEYS; i++){
        uint32_t k[4] = {keys_plain[i][0] & 0xFFFFFF, keys_plain[i][1] & 0xFFFFFF, keys_plain[i][2] & 0xFFFFFF, keys_plain[i][3] & 0xFFFFFF};
        uint32_t p[2] = {keys_plain[i][4] & 0xFFFFFF, keys_plain[i][5] & 0xFFFFFF};
        uint32_t c[2];
        speck48_96(k, p, c);

        uint32_t p2[2];
        speck48_96_inv(k, c, p2);
        printf("- Test n°%d: \n", i+1);
        printf("key: {%06X %06X %06X %06X}\n", k[0], k[1], k[2], k[3]);
        printf("plaintext: {%06X %06X}\n", p[0], p[1]);

        if (p[0] == p2[0] && p[1] == p2[1]){
            printf("Test n° %d PASSED\n", i+1);
        } else {
            printf("Test n° %d FAILED\n", i+1);
            return 1;
        }
    }
    printf("All tests PASSED\n");
    printf("\n");
    return 0;
}

int main(){
    printf("Testing speck48_96_inv..\n");
    test();
}