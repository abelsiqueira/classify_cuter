#include <iostream>
#include <fstream>
extern "C" {
#include "cuter.h"
}

using namespace std;

int MAINENTRY () {
  char fname[10] = "OUTSDIF.d";
  long int nvar = 0, ncon = 0;
  long int nmax, mmax;
  long int funit = 42, ierr = 0, fout = 6;
  char pname[11];

  bool has_eq = false;
  bool has_ineq = false;
  bool has_upper = false;
  bool has_lower = false;
  bool is_linear = true;

  FORTRAN_OPEN ((&funit), fname, (&ierr));
  CDIMEN ((&funit), (&nvar), (&ncon));

  nmax = nvar;
  double x[nvar], bl[nvar], bu[nvar];

  if (ncon == 0) {
    USETUP ((&funit), (&fout), (&nvar), x, bl, bu, (&nmax));
    char vnames[11*nvar];
    UNAMES (&nvar, pname, vnames);
  } else {
    mmax = ncon;
    double y[ncon], cl[ncon], cu[ncon];
    long int equatn[ncon], linear[ncon];
    long int efirst = 0, lfirst = 0, nvfirst = 0;

    CSETUP ((&funit), (&fout), (&nvar), (&ncon), x, bl, bu, (&nmax), equatn, linear, y, cl, cu, (&mmax), (&efirst), (&lfirst), (&nvfirst));

    char vnames[nvar*11], gnames[ncon*11];
    CNAMES (&nvar, &ncon, pname, vnames, gnames);

    for (int i = 0; i < ncon; i++) {
      if (equatn[i])
        has_eq = true;
      else
        has_ineq = true;
      if (!linear[i])
        is_linear = false;
      if (has_eq && has_ineq && !is_linear)
        break;
    }
  }
  for (int i = 0; i < nvar; i++) {
    if (bl[i] > -1e10)
      has_lower = true;
    if (bu[i] < 1e10)
      has_upper = true;
    if (has_lower && has_upper)
      break;
  }

  string outfile("classification.");
  if (has_eq && has_ineq)
    outfile += "gencon";
  else if (has_eq)
    outfile += "equ";
  else if (has_ineq)
    outfile += "ineq";
  else
    outfile += "unc";
  outfile += ".";

  if (has_upper && has_lower)
    outfile += "box";
  else if (has_upper)
    outfile += "upper";
  else if (has_lower)
    outfile += "lower";
  else
    outfile += "free";

  if (has_eq || has_ineq) {
    outfile += ".";
    outfile += (is_linear ? "linear" : "nonlin");
  }
  pname[10] = 0;

  ofstream file(outfile.c_str(), ios_base::app);
  file << pname << endl;
  file.close();
        
  FORTRAN_CLOSE ((&funit), (&ierr));

  return 0;

}
