#ifndef BDTVARCALCULATOR_H
#define BDTVARCALCULATOR_H

#include "TMath.h"
#include "TLorentzVector.h"
#include "TVector.h"
#include <iostream>
#include <vector>
#include <map>
#include <math.h>
#include "jetFuncs.h"
using namespace std;

class bdtVarCalculator {
public:

  void getSp(TLorentzVector lepton, TLorentzVector met, vector<TLorentzVector> jets, double &Aplanarity, double &Sphericity);
  double getBestHiggsMass(TLorentzVector lepton, TLorentzVector met, vector<TLorentzVector> jets, vector<double> btag, double &minChi, double &dRbb, TLorentzVector &bjet1, TLorentzVector &bjet2, vector<TLorentzVector> loose_jets, vector<double> loose_btag);
  double getBestHiggsMass2(TLorentzVector lepton, TLorentzVector &met, vector<TLorentzVector> jets, vector<double> btag, double &minChi, double &dRbb, TLorentzVector &bjet1, TLorentzVector &bjet2, double &chi2lepW, double &chi2leptop, double &chi2hadW, double &chi2hadtop, double &mass_lepW, double &mass_leptop, double &mass_hadW, double &mass_hadtop, TLorentzVector &toplep, TLorentzVector &tophad);
  double study_tops_bb_syst(double MET, double METphi, TLorentzVector &metv, TLorentzVector lepton, vector<vector<double>> jets, vector<double> csv, double &minChi, double &chi2lepW, double &chi2leptop, double &chi2hadW, double &chi2hadtop, double &mass_lepW, double &mass_leptop, double &mass_hadW, double &mass_hadtop, double &dRbb, double &testquant1, double &testquant2, double &testquant3, double &testquant4, double &testquant5, double &testquant6, double &testquant7, TLorentzVector &b1, TLorentzVector &b2);
  double pt_E_ratio_jets(vector<vector<double>> jets);
  double get_jet_jet_etamax (vector<vector<double>> jets);
  double get_jet_tag_etamax (vector<vector<double>> jets, vector<double> jetCSV);
  double get_tag_tag_etamax (vector<vector<double>> jets, vector<double> jetCSV);
  void getFox(vector<TLorentzVector> jets, double &h0, double &h1, double &h2, double &h3, double &h4);

private:

};

#endif
