#ifndef UTILS_HSM_H
#define UTILS_HSM_H

#ifndef HIGH_BIT_MASK
    #define HIGH_BIT_MASK (0x1 << 31)
#endif

int catch_sum_overflow(const unsigned * a, const unsigned * b);
unsigned has_high_bit(const unsigned * a);
void unsigned_int_to_string(char *const s, const int num, const int NUM_SIZE);
#endif