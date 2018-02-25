
#include "bit.h"
#include <stdint.h>

uint32_t bit_next_pow2(uint32_t n)
{
    n--;
    
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    
    n++;
    
    return n;
}

uint32_t bit_pow2_greater_or_equal(uint32_t n)
{
    return bit_is_pow2(n) ? (n) : bit_next_pow2(n);
}
