#ifndef JETFUNCS_H
#define JETFUNCS_H

#include "TLorentzVector.h"
#include "TVector.h"
#include <vector>
using namespace std;

void convert_jets_to_TLVs(vector<vector<double>> jets, vector<TLorentzVector> &vect_of_jet_TLVs);
void vect_of_tagged_TLVs(vector<vector<double>> jets, vector<double> jetCSV, vector<TLorentzVector> &vect_of_btag_TLVs);

#endif
