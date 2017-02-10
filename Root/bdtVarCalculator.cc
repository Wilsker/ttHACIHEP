#include "../interface/bdtVarCalculator.h"

bdtVarCalculator::bdtVarCalculator(){
  return;
}

bdtVarCalculator::~bdtVarCalculator(){
  return;
}

void bdtVarCalculator::getSp(TLorentzVector lepton, TLorentzVector met, vector<TLorentzVector> jets, double &Aplanarity, double &Sphericity) {
  int nJets = int(jets.size());
  double mxx = lepton.Px()*lepton.Px() + met.Px()*met.Px();
  double myy = lepton.Py()*lepton.Py() + met.Py()*met.Py();
  double mzz = lepton.Pz()*lepton.Pz() + met.Pz()*met.Pz();
  double mxy = lepton.Px()*lepton.Py() + met.Px()*met.Py();
  double mxz = lepton.Px()*lepton.Pz() + met.Px()*met.Pz();
  double myz = lepton.Py()*lepton.Pz() + met.Py()*met.Pz();
  for (int i=0; i<nJets; i++) {
    mxx += jets[i].Px()*jets[i].Px();
    myy += jets[i].Py()*jets[i].Py();
    mzz += jets[i].Pz()*jets[i].Pz();
    mxy += jets[i].Px()*jets[i].Py();
    mxz += jets[i].Px()*jets[i].Pz();
    myz += jets[i].Py()*jets[i].Pz();
  }
  double sum = mxx + myy + mzz;
  mxx /= sum;
  myy /= sum;
  mzz /= sum;
  mxy /= sum;
  mxz /= sum;
  myz /= sum;
  TMatrix tensor(3,3);
  tensor(0,0) = mxx;
  tensor(1,1) = myy;
  tensor(2,2) = mzz;
  tensor(0,1) = mxy;
  tensor(1,0) = mxy;
  tensor(0,2) = mxz;
  tensor(2,0) = mxz;
  tensor(1,2) = myz;
  tensor(2,1) = myz;
  TVector eigenval(3);
  tensor.EigenVectors(eigenval);
  Sphericity = 3.0*(eigenval(1)+eigenval(2))/2.0;
  Aplanarity = 3.0*eigenval(2)/2.0;
}

void bdtVarCalculator::getFox(vector<TLorentzVector> jets, double &h0, double &h1, double &h2, double &h3, double &h4){
  int visObjects = int(jets.size());
  double eVis = 0.0;
  for (int i=0; i<visObjects; i++) {
    eVis += jets[i].E();
  }
  h0 = 0.0;
  h1 = 0.0;
  h2 = 0.0;
  h3 = 0.0;
  h4 = 0.0;
  for (int i=0; i<visObjects-1; i++) {
    for (int j=i+1; j<visObjects; j++) {
      double costh = cos(jets[i].Angle(jets[j].Vect()));
      double p0 = 1.0;
      double p1 = costh;
      double p2 = 0.5*(3.0*costh*costh - 1.0);
      double p3 = 0.5*(5.0*costh*costh - 3.0*costh);
      double p4 = 0.125*(35.0*costh*costh*costh*costh - 30.0*costh*costh + 3.0);
      double pipj = jets[i].P()*jets[j].P();
      h0 += (pipj/(eVis*eVis))*p0;
      h1 += (pipj/(eVis*eVis))*p1;
      h2 += (pipj/(eVis*eVis))*p2;
      h3 += (pipj/(eVis*eVis))*p3;
      h4 += (pipj/(eVis*eVis))*p4;
    }
  }
}

double bdtVarCalculator::getBestHiggsMass(TLorentzVector lepton, TLorentzVector met, vector<TLorentzVector> jets, vector<double> btag, double &minChi, double &dRbb, TLorentzVector &bjet1, TLorentzVector &bjet2, vector<TLorentzVector> loose_jets, vector<double> loose_btag){
  if( jets.size()<6 && loose_jets.size()>0 ){
    jets.push_back( loose_jets[0] );
    btag.push_back( loose_btag[0] );
  }
  int nJets = int(jets.size());
  double chi_top_lep=10000;
  double chi_top_had=10000;
  double chi_W_had=10000;
  minChi = 1000000;
  dRbb = 1000000;
  double btagCut = 0.814;
  double W_mass = 80.0;
  double top_mass = 172.5;
  double sigma_hadW   = 12.77;
  double sigma_hadTop = 18.9;
  double sigma_lepTop = 32.91;
  double metPz[2];
  double chi=999999;
  //stuff to find:
  double higgs_mass_high_energy=0;
  int nBtags = 0;
  for(int i=0;i<nJets;i++){
    if(btag[i]>btagCut) nBtags++;
  }
  int nUntags = nJets-nBtags;
  double lowest_btag = 99.;
  double second_lowest_btag = 999.;
  int ind_lowest_btag = 999;
  int ind_second_lowest_btag = 999;
  if( nJets>=6 && nBtags>=4 ){
    if( nUntags<2 ){
      for(int i=0;i<nJets;i++){
	if( btag[i]<lowest_btag ){
	  second_lowest_btag = lowest_btag;
	  ind_second_lowest_btag = ind_lowest_btag;
	  lowest_btag = btag[i];
	  ind_lowest_btag = i;
	}
	else if( btag[i]<second_lowest_btag ){
	  second_lowest_btag = btag[i];
	  ind_second_lowest_btag = i;
	}
      }
    }
  }
  //Handle 6j3t.
  int ind_promoted_btag = 999;
  if( nJets>=6 && nBtags==3 ){
    for(int i=0;i<nJets;i++){
      int rank = 0;
      for(int j=0;j<nJets;j++){
	if( btag[j] > btag[i] ){
	  rank++;
	}
      }
      if( rank == 3 ) ind_promoted_btag = i;
    }
  }
  // First get the neutrino z
  double energyLep = lepton.E();
  double a = (W_mass*W_mass/(2.0*energyLep)) + (lepton.Px()*met.Px() + lepton.Py()*met.Py())/energyLep;
  double radical = (2.0*lepton.Pz()*a/energyLep)*(2.0*lepton.Pz()*a/energyLep);
  radical = radical - 4.0*(1.0 - (lepton.Pz()/energyLep)*(lepton.Pz()/energyLep))*(met.Px()*met.Px() + met.Py()*met.Py()- a*a);
  if (radical < 0.0) radical = 0.0;
  metPz[0] = (lepton.Pz()*a/energyLep) + 0.5*sqrt(radical);
  metPz[0] = metPz[0] / (1.0 - (lepton.Pz()/energyLep)*(lepton.Pz()/energyLep));
  metPz[1] = (lepton.Pz()*a/energyLep) - 0.5*sqrt(radical);
  metPz[1] = metPz[1] / (1.0 - (lepton.Pz()/energyLep)*(lepton.Pz()/energyLep));
  // Loop over all jets, both Pz, calcaulte chi-square
  TLorentzVector metNew;
  for( int ipznu=0; ipznu<2; ipznu++ ){
    metNew.SetXYZM(met.Px(),met.Py(),metPz[ipznu],0.0); //neutrino has mass 0
    //with b-tag info
    if( (nJets>=6 && nBtags>=4) || (nJets>=6 && nBtags==3) ){
      vector<TLorentzVector> not_b_tagged,b_tagged;
      //fill not_b_tagged and b_tagged
      for( int i=0;i<nJets;i++ ){
	if( (btag[i]>btagCut && i!=ind_second_lowest_btag && i!=ind_lowest_btag) || (i==ind_promoted_btag) ) b_tagged.push_back(jets[i]);
	else not_b_tagged.push_back(jets[i]);
      }
      for( int i=0; i<int(b_tagged.size()); i++ ){
	TLorentzVector W_lep=metNew+lepton; //used for histogram drawing only
	TLorentzVector top_lep=metNew+lepton+b_tagged.at(i);
	chi_top_lep=pow((top_lep.M()-top_mass)/sigma_lepTop,2);
	for( int j=0; j<int(not_b_tagged.size()); j++ ){
	  for( int k=0; k<int(not_b_tagged.size()); k++ ){
	    if( j!=k ){
	      TLorentzVector W_had=not_b_tagged.at(j)+not_b_tagged.at(k);
	      chi_W_had=pow((W_had.M()-W_mass)/sigma_hadW,2);
	      for( int l=0; l<int(b_tagged.size()); l++ ){
		if( l!=i ){
		  TLorentzVector top_had=W_had+b_tagged.at(l);
		  chi_top_had=pow((top_had.M()-top_mass)/sigma_hadTop,2);
		  chi=chi_top_lep+chi_W_had+chi_top_had;
		  //accept the lowest chi
		  if( chi<minChi ){
		    minChi=chi;
		    TLorentzVector H2;
		    int numH2Constituents=0;
		    TLorentzVector bBest[2];
		    for( int m=0; m<int(b_tagged.size()); m++ ){
		      if( m!=i && m!=l && numH2Constituents<2 ){
			bBest[numH2Constituents] = b_tagged.at(m);
			numH2Constituents++;
			H2+=b_tagged.at(m);
		      }
		    }
		    dRbb = bBest[0].DeltaR( bBest[1] );
		    higgs_mass_high_energy=H2.M();
		    bjet1 = bBest[0];
		    bjet2 = bBest[1];
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }
  return higgs_mass_high_energy;
}

double bdtVarCalculator::getBestHiggsMass2(TLorentzVector lepton, TLorentzVector &met, vector<TLorentzVector> jets, vector<double> btag, double &minChi, double &dRbb, TLorentzVector &bjet1, TLorentzVector &bjet2, double &chi2lepW, double &chi2leptop, double &chi2hadW, double &chi2hadtop, double &mass_lepW, double &mass_leptop, double &mass_hadW, double &mass_hadtop, TLorentzVector &toplep, TLorentzVector &tophad){
  int nJets = int(jets.size());
  double pfmet_px=met.Px(), pfmet_py=met.Py();
  double chi_top_lep=10000;
  double chi_top_had=10000;
  double chi_W_had=10000;
  minChi = 1000000;
  dRbb = 1000000;
  double btagCut = 0.80;
  double W_mass = 80.0;
  double top_mass = 172.5;
  double sigma_hadW   = 12.77;
  double sigma_hadTop = 18.9;
  //double sigma_lepTop = 32.91;
  double sigma_lepTop = 18.9;
  /// more initializitions
  bjet1.SetPxPyPzE(1.,1.,1.,2.);
  bjet2.SetPxPyPzE(1.,1.,1.,2.);
  //  chi2lepW = 0.;
  //  chi2leptop = 0.;
  //  chi2hadtop = 0.;
  mass_lepW = 0.;
  mass_leptop = 0.;
  mass_hadW = 0.;
  mass_hadtop = 0.;
  toplep.SetPxPyPzE(1.,1.,1.,2.);
  tophad.SetPxPyPzE(1.,1.,1.,2.);
  double metPz[2];
  double chi=999999;
  //stuff to find:
  double higgs_mass_high_energy=0;
  int nBtags = 0;
  for(int i=0;i<nJets;i++){
    if(btag[i]>btagCut) nBtags++;
  }
  int nUntags = nJets-nBtags;
  double lowest_btag = 99.;
  double second_lowest_btag = 999.;
  int ind_lowest_btag = 999;
  int ind_second_lowest_btag = 999;
  vector<double> btag_sorted = btag;
  if( nJets>=6 && nBtags>=4 ){
    if( nUntags<2 ){
      for(int i=0;i<nJets;i++){
	if( btag[i]<lowest_btag ){
	  second_lowest_btag = lowest_btag;
	  ind_second_lowest_btag = ind_lowest_btag;
	  lowest_btag = btag[i];
	  ind_lowest_btag = i;
	}
	else if( btag[i]<second_lowest_btag ){
	  second_lowest_btag = btag[i];
	  ind_second_lowest_btag = i;
	}
      }
    }
  }
  //Handle 6j3t.
  int ind_promoted_btag = 999;
  if( nJets>=6 && nBtags==3 ){
    for(int i=0;i<nJets;i++){
      int rank = 0;
      for(int j=0;j<nJets;j++){
	if( btag[j] > btag[i] ){
	  rank++;
	}
      }
      if( rank == 3 ) ind_promoted_btag = i;
    }
  }
  // First get the neutrino z
  double energyLep = lepton.E();
  double a = (W_mass*W_mass/(2.0*energyLep)) + (lepton.Px()*met.Px() + lepton.Py()*met.Py())/energyLep;
  double radical = (2.0*lepton.Pz()*a/energyLep)*(2.0*lepton.Pz()*a/energyLep);
  radical = radical - 4.0*(1.0 - (lepton.Pz()/energyLep)*(lepton.Pz()/energyLep))*(met.Px()*met.Px() + met.Py()*met.Py()- a*a);
  bool imaginary = false;
  if (radical < 0.0)
    {
      imaginary=true;
    }
  if(imaginary)
    {
      radical=-1.0;
      double value=.001;
      while(true)
	{
	  met.SetPxPyPzE(pfmet_px,pfmet_py,0.0,sqrt(pow(pfmet_px,2)+pow(pfmet_py,2)));
	  a = (W_mass*W_mass/(2.0*energyLep)) + (lepton.Px()*met.Px() + lepton.Py()*met.Py())/energyLep;
	  radical = (2.0*lepton.Pz()*a/energyLep)*(2.0*lepton.Pz()*a/energyLep);
	  radical = radical - 4.0*(1.0 - (lepton.Pz()/energyLep)*(lepton.Pz()/energyLep))*(met.Px()*met.Px() + met.Py()*met.Py()- a*a);
	  if(radical>=0)
	    break;
	  pfmet_px-=pfmet_px*value;
	  pfmet_py-=pfmet_py*value;
	}
    }
  metPz[0] = (lepton.Pz()*a/energyLep) + 0.5*sqrt(radical);
  metPz[0] = metPz[0] / (1.0 - (lepton.Pz()/energyLep)*(lepton.Pz()/energyLep));
  metPz[1] = (lepton.Pz()*a/energyLep) - 0.5*sqrt(radical);
  metPz[1] = metPz[1] / (1.0 - (lepton.Pz()/energyLep)*(lepton.Pz()/energyLep));
  // Loop over all jets, both Pz, calcaulte chi-square
  TLorentzVector metNew;
  for( int ipznu=0; ipznu<2; ipznu++ ){
    metNew.SetXYZM(met.Px(),met.Py(),metPz[ipznu],0.0); //neutrino has mass 0
    //with b-tag info
    if(( nJets>=6 && nBtags>=4 )||( nJets>=6 && nBtags==3 )){
      vector<TLorentzVector> not_b_tagged,b_tagged;
      //fill not_b_tagged and b_tagged
      for( int i=0;i<nJets;i++ ){
	if( (btag[i]>btagCut && i!=ind_second_lowest_btag && i!=ind_lowest_btag) || (i==ind_promoted_btag) ) b_tagged.push_back(jets[i]);
	else not_b_tagged.push_back(jets[i]);
      }
      for( int i=0; i<int(b_tagged.size()); i++ ){
	TLorentzVector W_lep=metNew+lepton; //used for histogram drawing only
	TLorentzVector top_lep=metNew+lepton+b_tagged.at(i);
	chi_top_lep=pow((top_lep.M()-top_mass)/sigma_lepTop,2);
	for( int j=0; j<int(not_b_tagged.size()); j++ ){
	  for( int k=0; k<int(not_b_tagged.size()); k++ ){
	    if( j!=k ){
	      TLorentzVector W_had=not_b_tagged.at(j)+not_b_tagged.at(k);
	      chi_W_had=pow((W_had.M()-W_mass)/sigma_hadW,2);
	      for( int l=0; l<int(b_tagged.size()); l++ ){
		if( l!=i ){
		  TLorentzVector top_had=W_had+b_tagged.at(l);
		  chi_top_had=pow((top_had.M()-top_mass)/sigma_hadTop,2);
		  chi=chi_top_lep+chi_W_had+chi_top_had;
		  //accept the lowest chi
		  if( chi<minChi ){
		    minChi=chi;
		    TLorentzVector H2;
		    int numH2Constituents=0;
		    TLorentzVector bBest[2];
		    for( int m=0; m<int(b_tagged.size()); m++ ){
		      if( m!=i && m!=l && numH2Constituents<2 ){
			bBest[numH2Constituents] = b_tagged.at(m);
			numH2Constituents++;
			H2+=b_tagged.at(m);
		      }
		    }
		    dRbb = bBest[0].DeltaR( bBest[1] );
		    higgs_mass_high_energy=H2.M();
		    bjet1 = bBest[0];
		    bjet2 = bBest[1];
		    mass_lepW = W_mass;
		    mass_leptop = top_lep.M();
		    mass_hadW = W_had.M();
		    mass_hadtop = top_had.M();
		    toplep = top_lep;
		    tophad = top_had;
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }
  chi2lepW = 0.;
  chi2leptop = chi_top_lep;
  chi2hadtop = chi_top_had;
  chi2hadW = chi_W_had;
  return higgs_mass_high_energy;
}

double bdtVarCalculator::study_tops_bb_syst(double MET, double METphi, TLorentzVector &metv, TLorentzVector lepton, vector<vector<double>> jets, vector<double> csv, double &minChi, double &chi2lepW, double &chi2leptop, double &chi2hadW, double &chi2hadtop, double &mass_lepW, double &mass_leptop, double &mass_hadW, double &mass_hadtop, double &dRbb, double &testquant1, double &testquant2, double &testquant3, double &testquant4, double &testquant5, double &testquant6, double &testquant7, TLorentzVector &b1, TLorentzVector &b2){
  double pi = 3.14;
  metv.SetPtEtaPhiE(MET,0.,METphi,MET);
  vector<TLorentzVector> jet_TLVs;
  jetFuncs jetF;//Probably want to declare this as global!<<<<<<<<<<<<<< Josh
  jetF.convert_jets_to_TLVs(jets, jet_TLVs);
  TLorentzVector bjet1;
  TLorentzVector bjet2;
  TLorentzVector leptop;
  TLorentzVector hadtop;
  double bhm = getBestHiggsMass2(lepton, metv, jet_TLVs, csv, minChi, dRbb, bjet1, bjet2, chi2lepW, chi2leptop, chi2hadW, chi2hadtop, mass_lepW, mass_leptop, mass_hadW, mass_hadtop, leptop, hadtop); // Jon T. version 2
  b1 = bjet1;
  b2 = bjet2;
  TLorentzVector bsyst = bjet1+bjet2;
  TLorentzVector topsyst = leptop+hadtop;
  double dphihad = bsyst.DeltaPhi(hadtop);
  double dphilep = bsyst.DeltaPhi(leptop);
  testquant1 = bsyst.Eta() - leptop.Eta();
  testquant2 = bsyst.Eta() - hadtop.Eta();
  testquant3 = fabs((dphilep - pi)*(dphilep + pi)) + pow(dphihad,2);
  testquant3 = sqrt(testquant3 / (2.0*pow(pi,2)));
  testquant4 = bsyst.Eta();
  testquant5 = (hadtop.Eta() + leptop.Eta())/2;
  testquant6 = sqrt(fabs((bsyst.Eta() - leptop.Eta())*(bsyst.Eta() - hadtop.Eta())));
  testquant7 = bsyst.Angle(topsyst.Vect());
  return bhm;
}

double bdtVarCalculator::pt_E_ratio_jets(vector<vector<double>> jets){
  double ratio = 0.;
  double ptsum = 0.;
  double Esum = 0.;
  vector<TLorentzVector> jetvect;
  jetFuncs jetF;//Probably want to declare this as global!<<<<<<<<<<<<<< Josh
  jetF.convert_jets_to_TLVs(jets,jetvect);
  for (int i=0; i<int(jetvect.size()); i++){
    ptsum += jetvect[i].Pt();
    Esum += jetvect[i].E();
  }
  ratio = ptsum / Esum;
  return ratio;
}

double bdtVarCalculator::get_jet_jet_etamax (vector<vector<double>> jets){
  vector<TLorentzVector> thejets;
  jetFuncs jetF;//Probably want to declare this as global!<<<<<<<<<<<<<< Josh
  jetF.convert_jets_to_TLVs(jets, thejets);
  int count=0;
  double avgval=0.;
  for (int i=0; i<int(thejets.size()); i++){
    avgval += fabs(thejets[i].Eta());
    count++;
  }
  avgval /= count;
  double deta = 0.;
  double etamax=-1.;
  for (int k=0; k<int(thejets.size()); k++){
    deta = fabs(fabs(thejets[k].Eta())-avgval);
    if(deta>etamax)etamax = deta;
  }
  return etamax;
}

double bdtVarCalculator::get_jet_tag_etamax (vector<vector<double>> jets, vector<double> jetCSV){
  vector<TLorentzVector> thejets;
  jetFuncs jetF;//Probably want to declare this as global!<<<<<<<<<<<<<< Josh
  jetF.convert_jets_to_TLVs(jets, thejets);
  int count=0;
  double avgval=0.;
  for (int i=0; i<int(thejets.size()); i++){
    avgval += fabs(thejets[i].Eta());
    count++;
  }
  avgval /= count;
  double deta = 0.;
  double etamax=0.;
  vector<TLorentzVector> thetags;
  jetF.vect_of_tagged_TLVs(jets, jetCSV, thetags);
  for (int k=0; k<int(thetags.size()); k++){
    deta = fabs(fabs(thetags[k].Eta())-avgval);
    if(deta>etamax)etamax=deta;
  }
  return etamax;
}

double bdtVarCalculator::get_tag_tag_etamax (vector<vector<double>> jets, vector<double> jetCSV){
  vector<TLorentzVector> thetags;
  jetFuncs jetF;
  jetF.vect_of_tagged_TLVs(jets, jetCSV, thetags);
  int count=0;
  double avgval=0.;
  for (int i=0; i<int(thetags.size()); i++){
    avgval += fabs(thetags[i].Eta());
    count++;
  }
  avgval /= count;
  double deta = 0.;
  double etamax=0.;
  for (int k=0; k<int(thetags.size()); k++){
    deta = fabs(fabs(thetags[k].Eta())-avgval);
    if(deta>etamax)etamax=deta;
  }
  return etamax;
}
