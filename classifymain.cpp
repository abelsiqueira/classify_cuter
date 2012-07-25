/* Copyright 2012 Abel Soares Siqueira
 *
 * Classify Cuter
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
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
  bool has_fixed = false;

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
    if (bl[i] == bu[i])
      has_fixed = true;
    if (has_lower && has_upper && has_fixed)
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

  if (has_fixed)
    outfile += ".fixed";

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
