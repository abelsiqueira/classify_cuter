#include <iostream>
extern "C" {
#include "cuter.h"
}

int MAINENTRY () {
  char fname[10] = "OUTSDIF.d";
  long int nvar = 0, ncon = 0, amax = 0;
  long int nmax, mmax;
  long int funit = 42, ierr = 0, fout = 6;

  FORTRAN_OPEN ((&funit), fname, (&ierr));
  CDIMEN ((&funit), (&nvar), (&ncon));

  nmax = nvar;
  double x[nvar], bl[nvar], bu[nvar];

  if (ncon == 0) {
    USETUP ((&funit), (&fout), (&nvar), x, bl, bu, (&nmax));
  } else {
    mmax = ncon;
    double y[ncon], cl[ncon], cu[ncon];
    long int equatn[ncon], linear[ncon];
    long int efirst = 0, lfirst = 0, nvfirst = 0;

    CSETUP ((&funit), (&fout), (&nvar), (&ncon), x, bl, bu, (&nmax), equatn, linear, y, cl, cu, (&mmax), (&efirst), (&lfirst), (&nvfirst));
    CDIMSJ ((&amax));
    amax *= 2;
  }

  FORTRAN_CLOSE ((&funit), (&ierr));

  return 0;

}
