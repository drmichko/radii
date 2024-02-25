#include <stdint.h>

typedef u_int64_t word;

#define one ((word)1)
#define WT(f) (__builtin_popcountll(f[0])+__builtin_popcountll(f[1]))

int estimationWord (word* f, word** C, int dim_C, int seuil);
void initStart(word* f, word** W, int start);
word** pack(code C);
void printWord(int r, word* w);
void zip(word* w, boole f);
