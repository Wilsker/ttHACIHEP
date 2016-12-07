#ifndef MUFUNCS_H
#define MUFUNCS_H

#include <vector>
using namespace std;
#include "TGraph.h"
#include <math.h>

class muFuncs {

public:
  pair<double,double> Muon_IDSF(double pt, double eta);
  pair<double,double> Muon_IsoSF(double pt, double eta);
  pair<double,double> Muon_TrkSF(double eta);

private:

};
#endif
