#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include "option.h"

// ############################# Variables ############################# //
typedef unsigned char* boole;

int ffdimen = 6;
int ffsize  = 64;

typedef u_int64_t word;
#define one ((word)1)
#define WT(f) (__builtin_popcountll(f[0])+__builtin_popcountll(f[1]))

typedef struct {
  int dim;
  boole* G;
  int* pivot;
}code;

int binomial(int k, int m){
  if (k==0){
    return 1;
  }

  int step1 = m - k + 1;
  int step0;
  for(int i = 1 ; i < k; i++){
    step0 = step1;
    step1 = step0 * (m-k+1+i)/(i+1);
  }
  return step1;
}

int rmdimen(int k, int m){
  int somme = 0;
  for(int i = 0 ; i < k+1 ; i++ ){
    somme = somme + binomial(i,m);
  }
  return somme;
}

int bstdimen(int s, int t, int m){
  int somme = 0;
  for(int i = s ; i < t+1 ; i++ ){
    somme = somme + binomial(i,m);
  }
  return somme;
}


code initCode(int dim){
  code res;
  res.dim = dim;
  res.G  = (boole *) calloc(dim, sizeof(boole));
  res.pivot = (int* ) calloc(dim, sizeof(int));
  for(int i = 0; i < dim; i++){
    res.G[i] = (boole) calloc(dim, sizeof(boole));
  }
  return res;
}

void freeCode(code c){
  for(int i =0; i < c.dim ; i++){
    free(c.G[i]);
  }
  free(c.G);
  free(c.pivot);
}

int wt(int u) {
    int res = 0;
    for (; u > 0; u = u>>1) res += (u & 1);
    return res;
}

boole getBoole(){
  boole res = calloc(ffsize, sizeof(unsigned char));
  return res;
}

code RM(int k, int m){ 
  code res = initCode(rmdimen(k,m));
  int u ;
  int numLigne = 0;
  for(u = 0; u <ffsize; u++){
    if (wt(u)<= k){
      int x;
      for(x=0; x<ffsize; x++){
        res.G[numLigne][x] = (u&x)==u;
      }
      numLigne = numLigne + 1;
    }
  }
  assert(numLigne == res.dim);
  return res;
}

code B(int s, int t, int m){
  code res = initCode(bstdimen(s,t,m));
  int u ;
  int numLigne = 0;
  for(u = 0; u <ffsize; u++){
    if ((s <= wt(u)) && (wt(u)<= t)){
      int x;
      for(x=0; x<ffsize; x++){
        res.G[numLigne][x] = (u&x)==u;
      }
      numLigne = numLigne + 1;
    }
  }
  assert(numLigne == res.dim);
  return res;
}

void addBoole(boole v, boole ligne){
  for (int i = 0; i < ffsize; i++){
    v[i] = v[i]^ligne[i];
  }
}

int wtBoole(boole v){
  int res = 0;
  for (int i = 0; i < ffsize; i++){
    res  =  res + v[i];
  }
  return res;
}

int distance (code C){
  int score = ffsize;
  boole vec = getBoole(); //longueur ffsize uchar f[ffsize];
  u_int64_t limite = 1;
  limite = limite  << C.dim ;
  u_int64_t v=1;
  limite = ((u_int64_t)1) << C.dim;
  int i;

    while (v<limite){
    i = __builtin_ctz(v);
    //int p = bit_a_changer(v); //codeGrey ou builtin gcc ?
    addBoole(vec, C.G[i]); // on ajoute dans vec  ( i ou p , version 1 ou 2)
    int w = wtBoole(vec);
    if (w<score){
      score = w;
    }
    v =  v + 1;
  }
  free(vec);
  return score;
}

int estimation (boole f, code C, int seuil){
  u_int64_t limite = 1;
  limite = limite  << C.dim ;
  u_int64_t v=1;
  limite = ((u_int64_t)1) << C.dim;
  int i;
  int w;
  int res = wtBoole(f);

  while (v<limite){
    i = __builtin_ctzll(v);
    addBoole(f, C.G[i]);
    w = wtBoole(f);

    if (res<seuil){
      return 0;
    }
    if (w<res){
      res = w;
    }
    v =  v + 1;
  }
  return res;
}

/** parametre s = "abc+db" (on enlève hdr avec la commande sed)
    On aurait pu utiliser strtok() **/
boole strToBoole(char* s){

  boole res = getBoole();
  int u;
  while(*s){//tant que jsuis pas en fin de chaine
    u = 0;
    while (isalpha(*s)){//tant que jsuis pas au '+'
      u |= 1<< (*s-'a');
      s++;
    }
    for(int x = 0; x<ffsize; x++){
      res[x] ^= (x&u)==u; //je rajoute 'u'
    }

    if (*s){s++;} //pour sauter le '+'
  }

  return res;

}

void anf(boole f, int q){
  if(q==1) return;
  anf(f, q/2);
  anf(&f[q/2], q/2);
  for(int x=0; x<q/2; x++){
    f[x+q/2] ^= f[x];
  }
}

// il nomme cette fonction 'panf'
void printANF(boole f){
  int flag = 0;
  anf(f,ffsize);
  printf("\n");
  for(int u=0; u<ffsize; u++){
    if(f[u]){
      if(flag==0){flag=1;}
      else{
        printf("+");
      }
      for(int i=0; i<ffdimen; i++){
        if(u&(1<<i)){
          printf("%c",'a'+i);
        }
      }
    }

  }
anf(f,ffsize);
}

void pivotage(code c){
    int pivot;
    for(int i = 0; i<c.dim; i++){
        do{
            pivot = rand() % ffsize;
        }
        while(c.G[i][pivot] != 1);
        c.pivot[i] = pivot;
        for(int j = i+1; j<c.dim; j++){
            if(c.G[j][pivot] == 1){
                addBoole(c.G[j], c.G[i]);
            }
        }
    }
}

/*
    Si return 0 = c'est mort,
    Sinon return une probabilité que ça soit correct.

  */
int probabiliste (code c, boole f, int tour, int seuil)
{

  int j,wt;
  int score = ffsize;
  while (tour-- > 0){
    pivotage(c);
    for(int i=0; i<c.dim;i++){
      j=c.pivot[i];
      if (f[j]) addBoole(f, c.G[i]);
    }
    wt = wtBoole(f);
    if (wt < seuil) return 0;
    if (wt < score) score = wt;
  }
  return score;
}

void probfile(char* fsrc, char* fout, code c,int tour, int seuil){
    FILE *src = fopen(fsrc, "r");
    FILE *out = fopen(fout, "w");

    char ligne[1024];

    boole f;
    int compteur = 0;
    int val;

    while(fgets(ligne,1024,src)){

    f = strToBoole(ligne);

      val = probabiliste(c, f, tour, seuil);
      if(val){
        compteur++;
        fputs(ligne, out);
        printf("%s\tVAL =%d\n",ligne,val );
      }
  }

  free(f);
  freeCode(c);
  printf("COMPTEUR FINAL = %d\n",compteur);
  fclose(src);
  fclose(out);

}

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

void printWord(word* w){
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
  printANF(f);
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

int estimationWordPlus(word* f, word** C, word** W, int dim_C, int dim_W, int seuil,
  u_int64_t start, u_int64_t nb_tour){
  int score = estimationWord(f,C, dim_C, seuil);
  u_int64_t limite = start + nb_tour;
  u_int64_t vec = start;
  int e;
  initStart(f, W, start);

  while (vec < limite && score > 0) {
    int i = __builtin_ctzll(vec);
    f[0] ^= W[i][0];
    f[1] ^= W[i][1];
    e =  estimationWord(f,C, dim_C, seuil);
    if (e > score){
      score = e;
    }
    if (e >= seuil){
      printWord(f);
    }
    vec = vec + 1;
  }
  return score;
}

