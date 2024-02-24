#include <stdint.h>

typedef u_int64_t word;

#define one ((word)1)
#define WT(f) (__builtin_popcountll(f[0])+__builtin_popcountll(f[1]))




