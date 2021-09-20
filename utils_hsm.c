#include <stdio.h>
#include "utils_hsm.h"

int catch_sum_overflow(const unsigned * a, const unsigned * b){
    unsigned a_high = has_high_bit(a);
    unsigned b_high = has_high_bit(b);
    unsigned c = *a + *b;
    unsigned c_high = has_high_bit(&c);
    if(a_high && b_high && !(c_high)){
        return 1;
    }
    if( !(a_high) && !(b_high) && c_high){
        return 1;
    }
    return 0;
}
unsigned has_high_bit(const unsigned * a){
    if(*a&HIGH_BIT_MASK){
        return 1;
    }
    return 0;
}