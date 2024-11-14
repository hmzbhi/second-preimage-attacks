#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


/**
 * @brief Generates random bytes to data by reading from /dev/urandom.
 * 
 * @param data Pointer to the buffer where the random key will be stored.
 * @param size Number of random bytes to generate.
 */
void gen_rdm(uint8_t* data, size_t size);