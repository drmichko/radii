#ifndef OPTION_H
#define OPTION_H
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>


extern int optdim;
extern int optvar;

extern int optbent;
extern int optsum;
extern int optarget;
extern int Bmin, Bmax;
extern int Smin, Smax;
extern int Tmin, Tmax;
extern int module;
extern int job;
extern int optwrite;
extern int verb;
extern int optJ;
extern int depth;
extern int tour;
int option(int argc, char *argv[]);
FILE * destin( char * prefixe, char *suffixe, int l  );
extern char* pid;
#define TXT 1
#define DAT 2

int filetype( char *fn );

#endif
