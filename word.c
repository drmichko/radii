#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include "boole.h"
#include "code.h"
#include "word.h"



/* permet de modifier f, pour qu'il correspond au bon état à l'etape start*/
void initStart(word* f, word** W, int start){
  int j=0;
  start = start ^ (start>>1); //Tips pour code Gray par rapport a un "n"
  while (start){
    if (start & 1){
      f[0] ^= W[j][0];
      f[1] ^= W[j][1];
      //addBoole(f, C.G[j]);
    }
    start = start >> 1;
    j++;
  }
}

/* Permet de faire B(3,5,7) en le décomposant en B(3,4,7) + B(5,5,7) */
/*int estimationPlus(boole f, code C, int seuil, int start, int nb_tour){


  int score = estimation(f,C,seuil);
  code W = B(5,5,7);
  initStart(f, W, start);

  int i, e;
  int limite = start + nb_tour;
  u_int64_t v = (u_int64_t) start;

  while ((v < limite) && (score > 0)){
    i = __builtin_ctz(v);
    addBoole(f, W.G[i]);
    e =  estimation(f,C,seuil);
    if (e > score){
      score = e;
    }
    if (e >= seuil){
      printANF(f);
    }
    v = v + 1;
  }
  freeCode(W);
  return score;
}*/

void zip(word* w, boole f){
  int i;
  w[0] = 0;
  w[1] = 0;
  for(i=0; i<64; i++){
    if(f[i]){
      w[0] ^= one << i ;
    }
    if(f[i+64]){
      w[1] ^= one << i;
    }
  }
}

void printWord( int v, word* w){
  int i;
  boole f = getBoole();
  for(i=0; i<64; i++){
    if(  w[0] & (one << i) ){
      f[i] = 1;
    }
    if(w[1] & (one << i)){
      f[i+64] = 1;
    }
  }
  panfnum(v, f);
  free(f);
}

word** pack(code C){
  word** res = calloc(C.dim, sizeof(word*));
  for (int i=0; i < C.dim; i++){
    res[i] = calloc(2, sizeof(word));
    zip(res[i], C.G[i]);
  }
  return res;
}

int estimationWord (word* f, word** C, int dim_C, int seuil){
  u_int64_t limite = 1;
  limite = limite  << dim_C;
  u_int64_t v=1;
  limite = ((u_int64_t)1) << dim_C;
  int i;
  int w;
  int res = WT(f);
  if ( res < seuil ) 
	  return res;
  while (v<limite){
    i = __builtin_ctzll(v);
    f[0] ^= C[i][0];
    f[1] ^= C[i][1];
    w = WT(f);

    if (w<seuil){
      return 0;
    }
    if (w<res){
      res = w;
    }
    v =  v + 1;
  }
  return res;
}
