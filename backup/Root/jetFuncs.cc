#include "../interface/jetFuncs.h"

void convert_jets_to_TLVs(vector<vector<double>> jets, vector<TLorentzVector> &vect_of_jet_TLVs){
  TLorentzVector jet;
  int nJets = jets.size();
  for(int i=0;i<nJets;i++){
    jet.SetPxPyPzE(jets[i][0],jets[i][1],jets[i][2],jets[i][3]);
    vect_of_jet_TLVs.push_back(jet);
  }
}

void vect_of_tagged_TLVs(vector<vector<double>> jets, vector<double> jetCSV, vector<TLorentzVector> &vect_of_btag_TLVs){
  TLorentzVector tagged_jet;
  int nJets = jets.size();
  double btagCut = 0.80;
  for(int i=0;i<nJets;i++){
    if (jetCSV[i]>btagCut){
      tagged_jet.SetPxPyPzE(jets[i][0],jets[i][1],jets[i][2],jets[i][3]);
      vect_of_btag_TLVs.push_back(tagged_jet);
    }
  }
}
