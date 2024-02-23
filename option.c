#include "option.h"
#include "assert.h"

int optdim = 2;
int optvar = 6;
char *direct = "/tmp";
char *pid = "pid";
int optbent = 0;
int optsum = 0;
int Bmin = 0, Bmax = 0;
int Smin = 0, Smax = 0;
int optarget = 0;
int Tmin = 0, Tmax = 0;
int module = 1;
int job = 0;
int optwrite = 0;
int verb = 0;
int force = 0;
int optJ  = 0;
int depth  = 0;
int tour  = 0;
int optX = 0;
int optcom = 0;
extern int level;
char Oname[64];
int option(int argc, char *argv[])
{
    int opt;
    char Bname[16] = { 0 };
    char Sname[16] = { 0 };
    char Tname[16] = { 0 };
    while ((opt = getopt(argc, argv, "wvd:j:m:B:S:p:T:l:fJ:D:t:XC")) != -1) {
	switch (opt) {
	case 'C' :
            optcom  = 1;
            break;
	case 'D' :
            depth  = atoi(optarg);
            break;
	case 'B':
	    if (2 > sscanf(optarg, "%d:%d", &Bmin, &Bmax))
		Bmax = Bmin;
	    optbent = 1;
	    sprintf(Bname, "-B%s", optarg);
	    break;
	case 'T':
	    if (2 > sscanf(optarg, "%d:%d", &Tmin, &Tmax))
		Tmax = Tmin;
	    optarget = 1;
	    sprintf(Tname, "-T%s", optarg);
	    break;
	case 'S':
	    if (2 > sscanf(optarg, "%d:%d", &Smin, &Smax))
		Smax = Smin;
	    optsum = 1;
	    sprintf(Sname, "-S%s", optarg);
	    break;
	case 'd':
	    direct = strdup(optarg);
	    break;
	case 'f':
	    force++;
	    break;
	case 'X':
	    optX = 1;
	    break;
	case 'p':
	    pid = strdup(optarg);
	    break;
	case 'w':
	    optwrite = 1;
	    break;
	case 'j':
	    job = atoi(optarg);;
	    break;
	case 'J':
	    optJ = atoi(optarg);;
	    break;
	case 'k':
	    optdim = atoi(optarg);;
	    break;
	case 'm':
	    optvar = atoi(optarg);;
	    break;
	case 't':
	    tour = atoi(optarg);
	    break;
	case 'v':
	    verb++;
	    break;
	default:		/* '?' */
	    fprintf(stderr, "Usage: %s [-a anf ] [-r log iter]\n",
		    argv[0]);
	    exit(EXIT_FAILURE);
	}
    }
    return optind;
}























