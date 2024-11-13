#include "tools.h"

void gen_rdm(uint8_t* data, size_t size)
{
    int urandom = open("/dev/urandom", O_RDONLY);
    if (urandom < 0)
    {
    printf("ERROR: could not open /dev/urandom\n");
    exit(1);
    }

    size_t nb_bytes = read(urandom, data, size);
    if (nb_bytes != size)
    {
    printf("ERROR: could not read %zu bytes from /dev/urandom\n", size);
    exit(1);
    }
}