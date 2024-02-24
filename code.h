#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include "boole.h"


typedef struct {
  int dim;
  boole* G;
  int* pivot;
}code;


code initCode(int dim);
void freeCode(code c);
code RM(int k, int m);
code B(int s, int t, int m);
void addBoole(boole v, boole ligne);
void pivotage(code c);


int estimation (boole f, code C, int seuil);

int probabiliste (code c, boole f, int tour, int seuil);
