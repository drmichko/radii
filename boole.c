#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include "option.h"

typedef unsigned char* boole;

int ffdimen = 6;
int ffsize  = 64;



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


int bstdimen(int s, int t, int m){
  int somme = 0;
  for(int i = s ; i < t+1 ; i++ ){
    somme = somme + binomial(i,m);
  }
  return somme;
}



int wt(int u) {
    int res = 0;
    for (; u > 0; u = u>>1) res += (u & 1);
    return res;
}

boole getBoole( void ){
  boole res = calloc(ffsize, sizeof(unsigned char));
  return res;
}

void addBoole(boole v, boole ligne){
  for (int i = 0; i < ffsize; i++){
    v[i] = v[i] ^ ligne[i];
  }
}

int wtBoole(boole v){
  int res = 0;
  for (int i = 0; i < ffsize; i++){
    res  =  res + v[i];
  }
  return res;
}


boole strToBoole(char* s){

  boole res = getBoole();
  int u;
  while(  *s  ){ 
    u = 0;
    while (isalpha( *s )){ 
      u |= 1<< (*s - 'a' );
      s++;
    }
    for(int x = 0; x<ffsize; x++){
      res[x] ^= (x&u)==u; 
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


