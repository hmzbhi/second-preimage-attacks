#include "../src/second_preim_48.h"

/* Test against EP 2013/404, App. C */
bool test_vector_okay()
{
    uint32_t k[4] = {0x1a1918, 0x121110, 0x0a0908, 0x020100};
    uint32_t p[2] = {0x6d2073, 0x696874};
    uint32_t c[2];
    speck48_96(k, p, c);
    printf("%X %X\n", c[0], c[1]);

    return (c[0] == 0x735E10) && (c[1] == 0xB6445D);
}

void test(){

}

int main(){
    test();
    return 0;
}