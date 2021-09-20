#ifndef UTILS_HSM
#define UTILS_HSM

#ifndef HIGH_BIT_MASK
    #define HIGH_BIT_MASK (0x1 << 31)
#endif

int catch_sum_overflow(const unsigned * a, const unsigned * b);
unsigned has_high_bit(const unsigned * a);

#endif