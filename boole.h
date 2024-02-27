#include <stdio.h>

typedef unsigned char* boole;

extern int ffdimen;
extern int ffsize ;

int wt(int u);
int binomial(int k, int m);
int rmdimen(int k, int m);
int bstdimen(int s, int t, int m);
void printANF(boole f);

void panfnum(int num, boole f);
boole  loadBoole(  FILE *src, int * num );

int wtBoole( boole f );
boole getBoole( void );
boole strtoboole(char* s);
void projboole( int s, int t, boole f );
