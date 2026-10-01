// crible.c : recherche de f1 (6 variables) compatible avec f0 donnee, pour
// f = f0 + x7*(f0+f1) de degre 6 en 7 variables et de linearite <= LIN.
// Methode : f1 = (p,q), p sur x6=0, q sur x6=1 (5 variables chacune).
//   1) crible de p (2^32, code de Gray, mise a jour incrementale du Walsh)
//      avec |P(a')| <= B(a') = (beta(a',0)+beta(a',1))/2, beta = LIN-|W0|
//   2) jointure des survivants : |P+Q|<=beta(a',0), |P-Q|<=beta(a',1),
//      wt(p)+wt(q) impair (f1 de poids impair => g=f0+f1 de degre <=5)
// Compilation : gcc -O3 -march=native -o crible crible.c
// Usage       : ./crible <f0 en hexa 64 bits, bit x = f0(x)> [LIN=16]
// Convention  : x = (x1..x6), x1 = bit 0, x6 = bit 5 ; a.x = parite(a&x).
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct { uint32_t f; int8_t w[32]; } item;
static int8_t R[32][32];          // R[x][a] = 2*(-1)^{a.x}

int main(int argc, char **argv) {
  if (argc < 2) { fprintf(stderr, "usage: %s f0_hex [LIN]\n", argv[0]); return 1; }
  uint64_t f0 = strtoull(argv[1], 0, 16);
  int LIN = argc > 2 ? atoi(argv[2]) : 16;
  if (__builtin_popcountll(f0) % 2 == 0) { fprintf(stderr, "f0 de poids pair: hors cas A\n"); return 1; }

  int W0[64], beta[64];
  for (int a = 0; a < 64; a++) {
    int s = 0;
    for (int x = 0; x < 64; x++)
      s += ((__builtin_popcount(a & x) + (f0 >> x)) & 1) ? -1 : 1;
    W0[a] = s; beta[a] = LIN - abs(s);
  }
  int8_t B[32];
  for (int a = 0; a < 32; a++) {
    int b0 = beta[a], b1 = beta[a + 32];
    if (b0 < 0 || b1 < 0) { printf("f0 de linearite > %d : aucune solution\n", LIN); return 0; }
    B[a] = (b0 + b1) / 2;
  }
  for (int x = 0; x < 32; x++)
    for (int a = 0; a < 32; a++)
      R[x][a] = (__builtin_popcount(a & x) & 1) ? -2 : 2;

  // ---- etape 1 : crible de p ----
  size_t cap = 1 << 20, n = 0, MAXN = 30000000;
  item *L = malloc(cap * sizeof(item));
  int8_t w[32] = {0}; w[0] = 32;
  uint32_t p = 0;
  for (uint64_t i = 0; ; i++) {
    if (i > 0) {
      int x = __builtin_ctzll(i);
      int s = ((p >> x) & 1) ? -1 : 1;
      for (int a = 0; a < 32; a++) w[a] -= s * R[x][a];
      p ^= 1u << x;
    }
    int bad = 0;
    for (int a = 0; a < 32; a++) bad |= (abs(w[a]) > B[a]);
    if (!bad) {
      if (n == MAXN) { fprintf(stderr, "trop de survivants (> %zu)\n", MAXN); return 2; }
      if (n == cap) { cap *= 2; L = realloc(L, cap * sizeof(item)); }
      L[n].f = p; memcpy(L[n].w, w, 32); n++;
    }
    if (i == 0xFFFFFFFFull) break;
  }
  fprintf(stderr, "survivants p : %zu\n", n);

  // ---- etape 2 : jointure ----
  int ord[32];                       // points tries du plus contraint au moins contraint
  for (int a = 0; a < 32; a++) ord[a] = a;
  for (int i = 0; i < 32; i++) for (int j = i + 1; j < 32; j++)
    if (B[ord[j]] < B[ord[i]]) { int t = ord[i]; ord[i] = ord[j]; ord[j] = t; }

  long nsol = 0;
  for (size_t i = 0; i < n; i++) {
    int pi = __builtin_popcount(L[i].f) & 1;
    for (size_t j = 0; j < n; j++) {
      if (((__builtin_popcount(L[j].f) & 1) ^ pi) == 0) continue;
      int ok = 1;
      for (int k = 0; k < 32 && ok; k++) {
        int a = ord[k];
        int P = L[i].w[a], Q = L[j].w[a];
        if (abs(P + Q) > beta[a] || abs(P - Q) > beta[a + 32]) ok = 0;
      }
      if (ok) {
        uint64_t f1 = (uint64_t)L[i].f | ((uint64_t)L[j].f << 32);
        printf("SOLUTION f1=%016llx g=%016llx\n",
               (unsigned long long)f1, (unsigned long long)(f0 ^ f1));
        nsol++;
      }
    }
  }
  printf("%ld solution(s) pour f0=%016llx (LIN=%d)\n", nsol, (unsigned long long)f0, LIN);
  return 0;
}
