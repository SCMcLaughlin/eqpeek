
#ifndef BIT_H
#define BIT_H

#include <stdint.h>

#define bit_is_pow2(n) ((n) && (((n) & ((n) - 1)) == 0))
#define bit_is_pow2_or_zero(n) ((n == 0) || (((n) & ((n) - 1)) == 0))

uint32_t bit_next_pow2(uint32_t n);
uint32_t bit_pow2_greater_or_equal(uint32_t n);

#endif/*BIT_H*/
