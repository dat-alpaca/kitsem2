#include <stdio.h>
#include <stdlib.h>

typedef int T_Seed;
#define MY_DEFAULT_SEED 2026
#define SeedTypeTrans(S) atoi(S)
#define RandInit(S) srand(S)
#define Rand(I) ((double)rand() / ((double)RAND_MAX + 1.0) * 4.0)

void usage(char *);
void generate(int, char *, T_Seed, int);

void usage(char *cmd) {
  fprintf(stderr, "Usage:  %s [-n] [-s seed] nitem outfile\n", cmd);
  exit(1);
}

int main(int argc, char *argv[]) {
  char *cmd, *fname = NULL;
  int flag_nitem = 0, nitem = 0;
  T_Seed seed = MY_DEFAULT_SEED;

  cmd = *argv++;
  while (--argc > 0) {
    if ((*argv)[0] != '-')
      break;
    switch ((*argv)[1]) {
    case 'n':
      flag_nitem = 1;
      break;
    case 's':
      seed = SeedTypeTrans(*++argv);
      argc--;
      break;
    default:
      usage(cmd);
      break;
    }
    argv++;
  }
  if (argc == 2) {
    if ((nitem = atoi(argv[0])) <= 0) {
      fprintf(stderr, "Illegal number of items.\n");
      usage(cmd);
    }
    fname = argv[1];
  } else {
    usage(cmd);
  }

  generate(nitem, fname, seed, flag_nitem);

  return 0;
}

void generate(int nitem, char *fname, T_Seed seed, int nflag) {
  FILE *fp;
  double r;
  int i;

  if ((fp = fopen(fname, "w")) == NULL) {
    fprintf(stderr, "Unable to open %s\n", fname);
    exit(1);
  }

  if (nflag) {
    fprintf(fp, "%d\n", nitem);
  }

  RandInit(seed);
  for (i = 0; i < nitem; i++) {
    r = Rand(i);
    fprintf(fp, "%f\t%d\tMake_Aquatan_Great_Again_%f\n", r, nitem - i, 5.0 - r);
  }

  fclose(fp);
}