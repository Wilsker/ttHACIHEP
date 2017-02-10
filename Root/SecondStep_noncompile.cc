#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include "TVector.h"
#include <iostream>
#include "TMVA/Reader.h"
#include <vector>
#include <map>
#include <math.h>
#include "TTH/CommonClassifier/interface/MEMClassifier.h"
#include "TTH/CommonClassifier/interface/BlrBDTClassifier.h"
#include "TStopwatch.h"
using namespace std;

void newPUWeight(double PUWeight_, double &puweight,double &puweightUP,double &puweightDOWN){
  double *npuProbs = 0;
  unsigned int nPUMax = 50;
  double npu_mix_2016_25ns_SpringMC_PUScenarioV1[50] = {0.000829312873542,0.00124276120498,0.00339329181587,0.00408224735376,0.00383036590008,0.00659159288946,0.00816022734493,0.00943640833116,0.0137777376066,0.017059392038,0.0213193035468,0.0247343174676,0.0280848773878,0.0323308476564,0.0370394341409,0.0456917721191,0.0558762890594,0.0576956187107,0.0625325287017,0.0591603758776,0.0656650815128,0.0678329011676,0.0625142146389,0.0548068448797,0.0503893295063,0.040209818868,0.0374446988111,0.0299661572042,0.0272024759921,0.0219328403791,0.0179586571619,0.0142926728247,0.00839941654725,0.00522366397213,0.00224457976761,0.000779274977993,0.000197066585944,7.16031761328e-05,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
  npuProbs = npu_mix_2016_25ns_SpringMC_PUScenarioV1;
  TFile file("/afs/cern.ch/work/j/jthomasw/private/IHEP/CMSSW_8_0_8_patch1/src/TTH/BSMFramework/BSM3G_TNT_Maker/data/PUReweight/PileUpReweighting2016.root", "READ");
  TH1* h = NULL;
  file.GetObject("pileup",h);
  h->SetDirectory(0);
  file.Close();
  std::vector<double> result(nPUMax,0.);
  double s = 0.;
  TFile fileNEW(  "../SF/MyDataPileupHistogram_69200.root",   "READ");
  TFile fileNEWUp("../SF/MyDataPileupHistogram_69200Up.root", "READ");
  TFile fileNEWDo("../SF/MyDataPileupHistogram_69200Do.root", "READ");
  TH1* hNEW   = NULL;
  TH1* hNEWUp = NULL;
  TH1* hNEWDo = NULL;
  fileNEW.GetObject("pileup",hNEW);
  fileNEWUp.GetObject("pileup",hNEWUp);
  fileNEWDo.GetObject("pileup",hNEWDo);
  hNEW->SetDirectory(0);
  hNEWUp->SetDirectory(0);
  hNEWDo->SetDirectory(0);
  fileNEW.Close();
  fileNEWUp.Close();
  fileNEWDo.Close();
  std::vector<double> resultNEW(nPUMax,0.);
  std::vector<double> resultNEWUp(nPUMax,0.);
  std::vector<double> resultNEWDo(nPUMax,0.);
  double sNEW   = 0.;
  double sNEWUp = 0.;
  double sNEWDo = 0.;
  for(unsigned int npu = 0; npu < nPUMax; ++npu) {
    const double npu_estimated = h->GetBinContent(h->GetXaxis()->FindBin(npu));
    result[npu] = npu_estimated / npuProbs[npu];
    s += npu_estimated;
    const double npu_estimatedNEW   = hNEW  ->GetBinContent(hNEW  ->GetXaxis()->FindBin(npu));
    const double npu_estimatedNEWUp = hNEWUp->GetBinContent(hNEWUp->GetXaxis()->FindBin(npu));
    const double npu_estimatedNEWDo = hNEWDo->GetBinContent(hNEWDo->GetXaxis()->FindBin(npu));
    resultNEW[npu]   = npu_estimatedNEW   / npuProbs[npu];
    resultNEWUp[npu] = npu_estimatedNEWUp / npuProbs[npu];
    resultNEWDo[npu] = npu_estimatedNEWDo / npuProbs[npu];
    sNEW   += npu_estimatedNEW;
    sNEWUp += npu_estimatedNEWUp;
    sNEWDo += npu_estimatedNEWDo;
  }
  unsigned int NPU = -1;
  for(unsigned int npu = 0; npu < nPUMax; ++npu) {
    result[npu] /= s;
    resultNEW[npu]   /= sNEW;
    resultNEWUp[npu] /= sNEWUp;
    resultNEWDo[npu] /= sNEWDo;
    if(result[npu]==PUWeight_) NPU = npu;
  }
  puweight     = resultNEW[NPU];
  puweightUP   = resultNEWUp[NPU];
  puweightDOWN = resultNEWDo[NPU];
}

pair<double,double> Electron_ID(double pt, double eta){
  double sfval = 1;
  double sferr = 1;
  if(pt<=20){//10<pt && pt<=20)
    if(-2.5<eta && eta<=-2){sfval = 0.83737; sferr = 0.0237874;}
    if(-2<eta && eta<=-1.566){sfval = 0.799714; sferr = 0.0141407;}
    if(-1.566<eta && eta<=-1.444){sfval = 0.951583; sferr = 0.0776466;}
    if(-1.444<eta && eta<=-0.8){sfval = 0.914992; sferr = 0.0352903;}
    if(-0.8<eta && eta<=0){sfval = 0.90313; sferr = 0.0272266;}
    if(0<eta && eta<=0.8){sfval = 0.927622; sferr = 0.0268911;}
    if(0.8<eta && eta<=1.444){sfval = 0.937206; sferr = 0.0351763;}
    if(1.444<eta && eta<=1.566){sfval = 0.968401; sferr = 0.0774164;}
    if(1.566<eta && eta<=2){sfval = 0.842647; sferr = 0.0138921;}
    if(2<eta && eta<=2.5){sfval = 0.850575; sferr = 0.0239472;}
  }
  if(20<pt && pt<=30){
    if(-2.5<eta && eta<=-2){sfval = 0.862958; sferr = 0.0132085;}
    if(-2<eta && eta<=-1.566){sfval = 0.871007; sferr = 0.0132405;}
    if(-1.566<eta && eta<=-1.444){sfval = 0.890744; sferr = 0.0517097;}
    if(-1.444<eta && eta<=-0.8){sfval = 0.923476; sferr = 0.0209732;}
    if(-0.8<eta && eta<=0){sfval = 0.9199; sferr = 0.0168032;}
    if(0<eta && eta<=0.8){sfval = 0.944931; sferr = 0.0168032;}
    if(0.8<eta && eta<=1.444){sfval = 0.919166; sferr = 0.0209278;}
    if(1.444<eta && eta<=1.566){sfval = 0.865356; sferr = 0.0515486;}
    if(1.566<eta && eta<=2){sfval = 0.866417; sferr = 0.0131643;}
    if(2<eta && eta<=2.5){sfval = 0.891856; sferr = 0.0132896;}
  }
  if(30<pt && pt<=40){
    if(-2.5<eta && eta<=-2){sfval = 0.91492; sferr = 0.00831223;}
    if(-2<eta && eta<=-1.566){sfval = 0.916188; sferr = 0.00481392;}
    if(-1.566<eta && eta<=-1.444){sfval = 0.920253; sferr = 0.0121675;}
    if(-1.444<eta && eta<=-0.8){sfval = 0.94881; sferr = 0.00803681;}
    if(-0.8<eta && eta<=0){sfval = 0.940904; sferr = 0.00442778;}
    if(0<eta && eta<=0.8){sfval = 0.961494; sferr = 0.00442778;}
    if(0.8<eta && eta<=1.444){sfval = 0.944844; sferr = 0.00803681;}
    if(1.444<eta && eta<=1.566){sfval = 0.918471; sferr = 0.0120133;}
    if(1.566<eta && eta<=2){sfval = 0.919262; sferr = 0.00481392;}
    if(2<eta && eta<=2.5){sfval = 0.927007; sferr = 0.00837403;}
  }
  if(40<pt && pt<=50){
    if(-2.5<eta && eta<=-2){sfval = 0.929825; sferr = 0.00558629;}
    if(-2<eta && eta<=-1.566){sfval = 0.938957; sferr = 0.0032966;}
    if(-1.566<eta && eta<=-1.444){sfval = 0.953162; sferr = 0.00758102;}
    if(-1.444<eta && eta<=-0.8){sfval = 0.95553; sferr = 0.00501427;}
    if(-0.8<eta && eta<=0){sfval = 0.950728; sferr = 0.0044236;}
    if(0<eta && eta<=0.8){sfval = 0.965051; sferr = 0.0044236;}
    if(0.8<eta && eta<=1.444){sfval = 0.954181; sferr = 0.00501427;}
    if(1.444<eta && eta<=1.566){sfval = 0.932322; sferr = 0.0074618;}
    if(1.566<eta && eta<=2){sfval = 0.943333; sferr = 0.00314095;}
    if(2<eta && eta<=2.5){sfval = 0.943155; sferr = 0.00558629;}
  }
  if(pt>50){//50<pt && pt<=200)
    if(-2.5<eta && eta<=-2){sfval = 0.936; sferr = 0.00652911;}
    if(-2<eta && eta<=-1.566){sfval = 0.949946; sferr = 0.00353559;}
    if(-1.566<eta && eta<=-1.444){sfval = 0.965438; sferr = 0.0081386;}
    if(-1.444<eta && eta<=-0.8){sfval = 0.957494; sferr = 0.00381244;}
    if(-0.8<eta && eta<=0){sfval = 0.956954; sferr = 0.00695617;}
    if(0<eta && eta<=0.8){sfval = 0.971143; sferr = 0.00695617;}
    if(0.8<eta && eta<=1.444){sfval = 0.963842; sferr = 0.00381244;}
    if(1.444<eta && eta<=1.566){sfval = 0.943936; sferr = 0.0081386;}
    if(1.566<eta && eta<=2){sfval = 0.958425; sferr = 0.00353559;}
    if(2<eta && eta<=2.5){sfval = 0.954442; sferr = 0.00619668;}
  }
  return make_pair(sfval,sferr);
}

pair<double,double> Electron_GsfSF(double eta){
  double sfval = 1;
  double sferr = 1;
  if(-2.5<eta && eta<=-2.4){sfval = 1.17034; sferr = 0.00966552;}
  if(-2.4<eta && eta<=-2.3){sfval = 1.00852; sferr = 0.0120181;}
  if(-2.3<eta && eta<=-2.2){sfval = 1.01047; sferr = 0.0085874;}
  if(-2.2<eta && eta<=-2){sfval = 1.00519; sferr = 0.00814729;}
  if(-2<eta && eta<=-1.8){sfval = 0.997932; sferr = 0.00754439;}
  if(-1.8<eta && eta<=-1.63){sfval = 0.991701; sferr = 0.00761482;}
  if(-1.63<eta && eta<=-1.566){sfval = 0.986486; sferr = 0.00699237;}
  if(-1.566<eta && eta<=-1.444){sfval = 0.961582; sferr = 0.0185147;}
  if(-1.444<eta && eta<=-1.2){sfval = 0.986667; sferr = 0.00602468;}
  if(-1.2<eta && eta<=-1){sfval = 0.977505; sferr = 0.00696244;}
  if(-1<eta && eta<=-0.6){sfval = 0.969388; sferr = 0.00597084;}
  if(-0.6<eta && eta<=-0.4){sfval = 0.966361; sferr = 0.00662906;}
  if(-0.4<eta && eta<=-0.2){sfval = 0.963303; sferr = 0.00634912;}
  if(-0.2<eta && eta<=0){sfval = 0.96; sferr = 0.00656714;}
  if(0<eta && eta<=0.2){sfval = 0.966189; sferr = 0.00656714;}
  if(0.2<eta && eta<=0.4){sfval = 0.979633; sferr = 0.00634912;}
  if(0.4<eta && eta<=0.6){sfval = 0.976578; sferr = 0.00662906;}
  if(0.6<eta && eta<=1){sfval = 0.980652; sferr = 0.00597084;}
  if(1<eta && eta<=1.2){sfval = 0.986735; sferr = 0.00696244;}
  if(1.2<eta && eta<=1.444){sfval = 0.98668; sferr = 0.00602468;}
  if(1.444<eta && eta<=1.566){sfval = 0.970721; sferr = 0.0185147;}
  if(1.566<eta && eta<=1.63){sfval = 0.989669; sferr = 0.00699237;}
  if(1.63<eta && eta<=1.8){sfval = 0.995872; sferr = 0.00783568;}
  if(1.8<eta && eta<=2){sfval = 0.989733; sferr = 0.007487;}
  if(2<eta && eta<=2.2){sfval = 0.994861; sferr = 0.00819214;}
  if(2.2<eta && eta<=2.3){sfval = 0.992769; sferr = 0.00850434;}
  if(2.3<eta && eta<=2.4){sfval = 0.966632; sferr = 0.0119341;}
  if(2.4<eta && eta<=2.5){sfval = 0.884021; sferr = 0.00953672;}
  return make_pair(sfval,sferr);
}

pair<double,double> Muon_IDSF(double pt, double eta){
  double sfval = 1;
  double sferr = 1;
  if(pt<=25)//20<pt && pt<=25)
    {
      if(0<fabs(eta) && fabs(eta)<=0.9){sfval = 0.958376; sferr = 0.00256685;}
      if(0.9<fabs(eta) && fabs(eta)<=1.2){sfval = 0.969183; sferr = 0.00375759;}
      if(1.2<fabs(eta) && fabs(eta)<=2.1){sfval = 0.991418; sferr = 0.00197292;}
      if(2.1<fabs(eta) && fabs(eta)<=2.4){sfval = 0.985294; sferr = 0.00368382;}
    }
  if(25<pt && pt<=30){
    if(0<fabs(eta) && fabs(eta)<=0.9){sfval = 0.970763; sferr = 0.00126508;}
    if(0.9<fabs(eta) && fabs(eta)<=1.2){sfval = 0.966866; sferr = 0.00206616;}
    if(1.2<fabs(eta) && fabs(eta)<=2.1){sfval = 0.989645; sferr = 0.00109714;}
    if(2.1<fabs(eta) && fabs(eta)<=2.4){sfval = 0.975853; sferr = 0.00230061;}
  }
  if(30<pt && pt<=40){
    if(0<fabs(eta) && fabs(eta)<=0.9){sfval = 0.975881; sferr = 0.000481227;}
    if(0.9<fabs(eta) && fabs(eta)<=1.2){sfval = 0.971573; sferr = 0.000830174;}
    if(1.2<fabs(eta) && fabs(eta)<=2.1){sfval = 0.990198; sferr = 0.000450168;}
    if(2.1<fabs(eta) && fabs(eta)<=2.4){sfval = 0.973918; sferr = 0.00113783;}
  }
  if(40<pt && pt<=50){
    if(0<fabs(eta) && fabs(eta)<=0.9){sfval = 0.976182; sferr = 0.000387079;}
    if(0.9<fabs(eta) && fabs(eta)<=1.2){sfval = 0.970849; sferr = 0.000627464;}
    if(1.2<fabs(eta) && fabs(eta)<=2.1){sfval = 0.990848; sferr = 0.000200121;}
    if(2.1<fabs(eta) && fabs(eta)<=2.4){sfval = 0.969894; sferr = 0.00108924;}
  }
  if(50<pt && pt<=60){
    if(0<fabs(eta) && fabs(eta)<=0.9){sfval = 0.972266; sferr = 0.000999986;}
    if(0.9<fabs(eta) && fabs(eta)<=1.2){sfval = 0.971497; sferr = 0.00167629;}
    if(1.2<fabs(eta) && fabs(eta)<=2.1){sfval = 0.991589; sferr = 0.00115025;}
    if(2.1<fabs(eta) && fabs(eta)<=2.4){sfval = 0.981064; sferr = 0.00354674;}
  }
  if(60<pt && pt<=100){
    if(0<fabs(eta) && fabs(eta)<=0.9){sfval = 0.973523; sferr = 0.00201625;}
    if(0.9<fabs(eta) && fabs(eta)<=1.2){sfval = 0.968216; sferr = 0.00347208;}
    if(1.2<fabs(eta) && fabs(eta)<=2.1){sfval = 0.989786; sferr = 0.00262609;}
    if(2.1<fabs(eta) && fabs(eta)<=2.4){sfval = 0.975218; sferr = 0.00763513;}
  }
  if(pt>100)//100<pt && pt<=200)
    {
      if(0<fabs(eta) && fabs(eta)<=0.9){sfval = 0.98764; sferr = 0.0160819;}
      if(0.9<fabs(eta) && fabs(eta)<=1.2){sfval = 1.02714; sferr = 0.0224793;}
      if(1.2<fabs(eta) && fabs(eta)<=2.1){sfval = 1.01349; sferr = 0.00944602;}
      if(2.1<fabs(eta) && fabs(eta)<=2.4){sfval = 0.91808; sferr = 0.0796353;}
    }
  return make_pair(sfval,sferr);
}

pair<double,double> Muon_IsoSF(double pt, double eta){
  double sfval = 1;
  double sferr = 1;
  if(pt<=25)//20<pt && pt<=25)
    {
      if(0<fabs(eta) && fabs(eta)<=0.9){sfval = 0.982323; sferr = 0.00298554;}
      if(0.9<fabs(eta) && fabs(eta)<=1.2){sfval = 0.986009; sferr = 0.00456872;}
      if(1.2<fabs(eta) && fabs(eta)<=2.1){sfval = 0.986859; sferr = 0.00220422;}
      if(2.1<fabs(eta) && fabs(eta)<=2.4){sfval = 0.983397; sferr = 0.00381719;}
    }
  if(25<pt && pt<=30){
    if(0<fabs(eta) && fabs(eta)<=0.9){sfval = 0.988605; sferr = 0.00166344;}
    if(0.9<fabs(eta) && fabs(eta)<=1.2){sfval = 0.994709; sferr = 0.00289202;}
    if(1.2<fabs(eta) && fabs(eta)<=2.1){sfval = 0.993867; sferr = 0.00140074;}
    if(2.1<fabs(eta) && fabs(eta)<=2.4){sfval = 0.987529; sferr = 0.00237643;}
  }
  if(30<pt && pt<=40){
    if(0<fabs(eta) && fabs(eta)<=0.9){sfval = 0.992043; sferr = 0.000580556;}
    if(0.9<fabs(eta) && fabs(eta)<=1.2){sfval = 0.99809; sferr = 0.00106105;}
    if(1.2<fabs(eta) && fabs(eta)<=2.1){sfval = 0.997524; sferr = 0.000582381;}
    if(2.1<fabs(eta) && fabs(eta)<=2.4){sfval = 0.993539; sferr = 0.00101822;}
  }
  if(40<pt && pt<=50){
    if(0<fabs(eta) && fabs(eta)<=0.9){sfval = 0.994218; sferr = 0.000327784;}
    if(0.9<fabs(eta) && fabs(eta)<=1.2){sfval = 0.997873; sferr = 0.000448171;}
    if(1.2<fabs(eta) && fabs(eta)<=2.1){sfval = 0.99795; sferr = 0.000246853;}
    if(2.1<fabs(eta) && fabs(eta)<=2.4){sfval = 0.996392; sferr = 0.000394046;}
  }
  if(50<pt && pt<=60){
    if(0<fabs(eta) && fabs(eta)<=0.9){sfval = 0.996457; sferr = 0.000556209;}
    if(0.9<fabs(eta) && fabs(eta)<=1.2){sfval = 0.999352; sferr = 0.000926904;}
    if(1.2<fabs(eta) && fabs(eta)<=2.1){sfval = 0.999048; sferr = 0.000520563;}
    if(2.1<fabs(eta) && fabs(eta)<=2.4){sfval = 0.997946; sferr = 0.00108252;}
  }
  if(60<pt && pt<=100){
    if(0<fabs(eta) && fabs(eta)<=0.9){sfval = 0.999023; sferr = 0.000703851;}
    if(0.9<fabs(eta) && fabs(eta)<=1.2){sfval = 0.999509; sferr = 0.00110414;}
    if(1.2<fabs(eta) && fabs(eta)<=2.1){sfval = 0.998433; sferr = 0.000669639;}
    if(2.1<fabs(eta) && fabs(eta)<=2.4){sfval = 1.00073; sferr = 0.00153644;}
  }
  if(pt>100)//100<pt && pt<=200)
    {
      if(0<fabs(eta) && fabs(eta)<=0.9){sfval = 1.00007; sferr = 0.00222242;}
      if(0.9<fabs(eta) && fabs(eta)<=1.2){sfval = 0.995709; sferr = 0.00424566;}
      if(1.2<fabs(eta) && fabs(eta)<=2.1){sfval = 1.00571; sferr = 0.00281406;}
      if(2.1<fabs(eta) && fabs(eta)<=2.4){sfval = 0.99858; sferr = 0.00587265;}
    }
  return make_pair(sfval,sferr);
}

pair<double,double> Muon_TrkSF(double eta){
  double sfval = 1;
  double sferr = 1;
  const double x[10] = {-2.23090284685015616e+00, -1.82699807131713809e+00, -1.34607008919971971e+00, -8.43046106233286685e-01, -2.97940760993056608e-01, 2.98252730786497722e-01, 8.43136487245492505e-01, 1.34752802994490373e+00, 1.82701299844882636e+00, 2.23329880623762422e+00};
  const double y[10] = {9.82399009186853522e-01, 9.91746789037933008e-01, 9.95944961092376846e-01, 9.93413142541369476e-01, 9.91460688530866996e-01, 9.94680143661991423e-01, 9.96666389348924819e-01, 9.94933892427240618e-01, 9.91186607207322878e-01, 9.76811919457875155e-01};
  TGraph* tg = new TGraph(10,x,y);
  sfval = tg->Eval(eta);
  return make_pair(sfval,sferr);
}

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

void getSp(TLorentzVector lepton, TLorentzVector met, vector<TLorentzVector> jets, double &Aplanarity, double &Sphericity) {
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

void getFox(vector<TLorentzVector> jets, double &h0, double &h1, double &h2, double &h3, double &h4){
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

double getBestHiggsMass(TLorentzVector lepton, TLorentzVector met, vector<TLorentzVector> jets, vector<double> btag, double &minChi, double &dRbb, TLorentzVector &bjet1, TLorentzVector &bjet2, vector<TLorentzVector> loose_jets, vector<double> loose_btag){
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

double getBestHiggsMass2(TLorentzVector lepton, TLorentzVector &met, vector<TLorentzVector> jets, vector<double> btag, double &minChi, double &dRbb, TLorentzVector &bjet1, TLorentzVector &bjet2, double &chi2lepW, double &chi2leptop, double &chi2hadW, double &chi2hadtop, double &mass_lepW, double &mass_leptop, double &mass_hadW, double &mass_hadtop, TLorentzVector &toplep, TLorentzVector &tophad){
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

double study_tops_bb_syst(double MET, double METphi, TLorentzVector &metv, TLorentzVector lepton, vector<vector<double>> jets, vector<double> csv, double &minChi, double &chi2lepW, double &chi2leptop, double &chi2hadW, double &chi2hadtop, double &mass_lepW, double &mass_leptop, double &mass_hadW, double &mass_hadtop, double &dRbb, double &testquant1, double &testquant2, double &testquant3, double &testquant4, double &testquant5, double &testquant6, double &testquant7, TLorentzVector &b1, TLorentzVector &b2){
  double pi = 3.14;
  metv.SetPtEtaPhiE(MET,0.,METphi,MET);
  vector<TLorentzVector> jet_TLVs;
  convert_jets_to_TLVs(jets, jet_TLVs);
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

double pt_E_ratio_jets(vector<vector<double>> jets){
  double ratio = 0.;
  double ptsum = 0.;
  double Esum = 0.;
  vector<TLorentzVector> jetvect;
  convert_jets_to_TLVs(jets,jetvect);
  for (int i=0; i<int(jetvect.size()); i++){
    ptsum += jetvect[i].Pt();
    Esum += jetvect[i].E();
  }
  ratio = ptsum / Esum;
  return ratio;
}

double get_jet_jet_etamax (vector<vector<double>> jets){
  vector<TLorentzVector> thejets;
  convert_jets_to_TLVs(jets, thejets);
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

double get_jet_tag_etamax (vector<vector<double>> jets, vector<double> jetCSV){
  vector<TLorentzVector> thejets;
  convert_jets_to_TLVs(jets, thejets);
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
  vect_of_tagged_TLVs(jets, jetCSV, thetags);
  for (int k=0; k<int(thetags.size()); k++){
    deta = fabs(fabs(thetags[k].Eta())-avgval);
    if(deta>etamax)etamax=deta;
  }
  return etamax;
}

double get_tag_tag_etamax (vector<vector<double>> jets, vector<double> jetCSV){
  vector<TLorentzVector> thetags;
  vect_of_tagged_TLVs(jets, jetCSV, thetags);
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

void SecondStep_noncompile(){
  TStopwatch t;
  t.Start();
  cout << "Stopwatch started" << endl;
  MEMClassifier mem;
  int sample = 1;
  const char * Input = "/afs/cern.ch/work/j/jthomasw/private/IHEP/CMSSW_8_0_24_patch1/src/TTH/ttHACIHEP_v2/input/ttHbb_MC.root";
  const char * Output = "output.root";

  //Get old file, old tree and set top branch address
  TFile *oldfile = TFile::Open(Input);
  TTree *evtree = (TTree*)oldfile->Get("TNT/evtree");
  evtree->SetBranchStatus("*",0);
  evtree->SetBranchStatus("eventnum",1);
  //evtree->SetBranchStatus("eventnumnegative",1);
  TTree *oldtree = (TTree*)oldfile->Get("TNT/BOOM");
  oldtree->SetBranchStatus("*",0);
  oldtree->SetBranchStatus("Jet_pt",1);
  oldtree->SetBranchStatus("Jet_Uncorr_pt",1);
  oldtree->SetBranchStatus("Jet_eta",1);
  oldtree->SetBranchStatus("Jet_phi",1);
  oldtree->SetBranchStatus("Jet_mass",1);
  oldtree->SetBranchStatus("Jet_JesSF",1);
  oldtree->SetBranchStatus("Jet_JerSF",1);
  oldtree->SetBranchStatus("Jet_JesSFup",1);
  oldtree->SetBranchStatus("Jet_JerSFup",1);
  oldtree->SetBranchStatus("Jet_JesSFdown",1);
  oldtree->SetBranchStatus("Jet_JerSFdown",1);
  oldtree->SetBranchStatus("Jet_newpfCombinedInclusiveSecondaryVertexV2BJetTags",1);
  oldtree->SetBranchStatus("Jet_neutralHadEnergyFraction",1);
  oldtree->SetBranchStatus("Jet_chargedEmEnergyFraction",1);
  oldtree->SetBranchStatus("Jet_neutralEmEnergyFraction",1);
  oldtree->SetBranchStatus("Jet_numberOfConstituents",1);
  oldtree->SetBranchStatus("Jet_chargedHadronEnergyFraction",1);
  oldtree->SetBranchStatus("Jet_chargedMultiplicity",1);
  oldtree->SetBranchStatus("BJetness_isSingleLepton",1);
  oldtree->SetBranchStatus("BJetness_isDoubleLepton",1);
  oldtree->SetBranchStatus("BJetness_ngenbh",1);
  oldtree->SetBranchStatus("BJetness_ngenbt",1);
  oldtree->SetBranchStatus("BJetness_ngenb",1);
  oldtree->SetBranchStatus("BJetness_ngenc",1);
  oldtree->SetBranchStatus("BJetness_partonFlavour",1);
  oldtree->SetBranchStatus("BJetness_hadronFlavour",1);
  oldtree->SetBranchStatus("BJetness_numjet",1);
  oldtree->SetBranchStatus("BJetness_jetpt",1);
  oldtree->SetBranchStatus("BJetness_jeteta",1);
  oldtree->SetBranchStatus("BJetness_jetphi",1);
  oldtree->SetBranchStatus("BJetness_jetenergy",1);
  oldtree->SetBranchStatus("BJetness_jetcsv",1);
  oldtree->SetBranchStatus("BJetness_pfJetProbabilityBJetTags",1);
  oldtree->SetBranchStatus("BJetness_pfCombinedMVAV2BJetTags",1);
  oldtree->SetBranchStatus("BJetness_pfCombinedCvsLJetTags",1);
  oldtree->SetBranchStatus("BJetness_pfCombinedCvsBJetTags",1);
  oldtree->SetBranchStatus("BJetness_pt",1);
  oldtree->SetBranchStatus("BJetness_eta",1);
  oldtree->SetBranchStatus("BJetness_phi",1);
  oldtree->SetBranchStatus("BJetness_en",1);
  oldtree->SetBranchStatus("BJetness_ptOVen",1);
  oldtree->SetBranchStatus("BJetness_jetschpvass",1);
  oldtree->SetBranchStatus("BJetness_jetschfrompv",1);
  oldtree->SetBranchStatus("BJetness_jetschip3dval",1);
  oldtree->SetBranchStatus("BJetness_jetschip3dsig",1);
  oldtree->SetBranchStatus("BJetness_jetschip2dval",1);
  oldtree->SetBranchStatus("BJetness_jetschip2dsig",1);
  oldtree->SetBranchStatus("BJetness_jetschisgoodtrk",1);
  oldtree->SetBranchStatus("BJetness_jetschtrkpur",1);
  oldtree->SetBranchStatus("BJetness_jetschpt",1);
  oldtree->SetBranchStatus("BJetness_jetschen",1);
  oldtree->SetBranchStatus("BJetness_num_pdgid_eles",1);
  oldtree->SetBranchStatus("BJetness_num_soft_eles",1);
  oldtree->SetBranchStatus("BJetness_num_vetonoipnoiso_eles",1);
  oldtree->SetBranchStatus("BJetness_num_loosenoipnoiso_eles",1);
  oldtree->SetBranchStatus("BJetness_num_veto_eles",1);
  oldtree->SetBranchStatus("BJetness_num_loose_eles",1);
  oldtree->SetBranchStatus("BJetness_num_medium_eles",1);
  oldtree->SetBranchStatus("BJetness_num_tight_eles",1);
  oldtree->SetBranchStatus("BJetness_num_mvatrig_eles",1);
  oldtree->SetBranchStatus("BJetness_num_mvanontrig_eles",1);
  oldtree->SetBranchStatus("BJetness_num_mvatrigwp90_eles",1);
  oldtree->SetBranchStatus("BJetness_num_mvanontrigwp90_eles",1);
  oldtree->SetBranchStatus("BJetness_num_heep_eles",1);
  oldtree->SetBranchStatus("BJetness_num_pdgid_mus",1);
  oldtree->SetBranchStatus("BJetness_num_loose_mus",1);
  oldtree->SetBranchStatus("BJetness_num_soft_mus",1);
  oldtree->SetBranchStatus("BJetness_num_medium_mus",1);
  oldtree->SetBranchStatus("BJetness_num_tight_mus",1);
  oldtree->SetBranchStatus("BJetness_num_highpt_mus",1);
  oldtree->SetBranchStatus("BJetness_num_POGisGood_mus",1);
  oldtree->SetBranchStatus("BJetness_numjettrks",1);
  oldtree->SetBranchStatus("BJetness_numjettrkspv",1);
  oldtree->SetBranchStatus("BJetness_numjettrksnopv",1);
//  oldtree->SetBranchStatus("BJetness_npvTrkOVcollTrk",1);
  oldtree->SetBranchStatus("BJetness_pvTrkOVcollTrk",1);
  oldtree->SetBranchStatus("BJetness_npvTrkOVpvTrk",1);
  oldtree->SetBranchStatus("BJetness_npvPtOVcollPt",1);
  oldtree->SetBranchStatus("BJetness_pvPtOVcollPt",1);
  oldtree->SetBranchStatus("BJetness_npvPtOVpvPt",1);
  oldtree->SetBranchStatus("BJetness_avprel",1);
  oldtree->SetBranchStatus("BJetness_avppar",1);
  oldtree->SetBranchStatus("BJetness_avetarel",1);
  oldtree->SetBranchStatus("BJetness_avetapar",1);
  oldtree->SetBranchStatus("BJetness_avdr",1);
  oldtree->SetBranchStatus("BJetness_avpreljetpt",1);
  oldtree->SetBranchStatus("BJetness_avpreljeten",1);
  oldtree->SetBranchStatus("BJetness_avpparjetpt",1);
  oldtree->SetBranchStatus("BJetness_avpparjeten",1);
  oldtree->SetBranchStatus("BJetness_avnum2v",1);
  oldtree->SetBranchStatus("BJetness_avnumno2v",1);
  oldtree->SetBranchStatus("BJetness_avdca3d2t",1);
  oldtree->SetBranchStatus("BJetness_avdca3dno2t",1);
  oldtree->SetBranchStatus("BJetness_avdca3d",1);
  oldtree->SetBranchStatus("BJetness_avdca2d2t",1);
  oldtree->SetBranchStatus("BJetness_avdca2dno2t",1);
  oldtree->SetBranchStatus("BJetness_avdca2d",1);
  oldtree->SetBranchStatus("BJetness_chi2",1);
  oldtree->SetBranchStatus("BJetness_avip3d_val",1);
  oldtree->SetBranchStatus("BJetness_avip3d_sig",1);
  oldtree->SetBranchStatus("BJetness_avsip3d_val",1);
  oldtree->SetBranchStatus("BJetness_avsip3d_sig",1);
  oldtree->SetBranchStatus("BJetness_numip3dpos",1);
  oldtree->SetBranchStatus("BJetness_numip3dneg",1);
  oldtree->SetBranchStatus("BJetness_avip2d_val",1);
  oldtree->SetBranchStatus("BJetness_avip2d_sig",1);
  oldtree->SetBranchStatus("BJetness_avsip2d_val",1);
  oldtree->SetBranchStatus("BJetness_avsip2d_sig",1);
  oldtree->SetBranchStatus("BJetness_numip2dpos",1);
  oldtree->SetBranchStatus("BJetness_numip2dneg",1);
  oldtree->SetBranchStatus("BJetness_avip1d_val",1);
  oldtree->SetBranchStatus("BJetness_avip1d_sig",1);
  oldtree->SetBranchStatus("BJetness_avsip1d_val",1);
  oldtree->SetBranchStatus("BJetness_avsip1d_sig",1);
  oldtree->SetBranchStatus("BJetnessFV_isSingleLepton",1);
  oldtree->SetBranchStatus("BJetnessFV_isDoubleLepton",1);
  oldtree->SetBranchStatus("BJetnessFV_jetcsv",1);
  oldtree->SetBranchStatus("BJetnessFV_pfJetProbabilityBJetTags",1);
  oldtree->SetBranchStatus("BJetnessFV_pfCombinedMVAV2BJetTags",1);
  oldtree->SetBranchStatus("BJetnessFV_num_leps",1);
  oldtree->SetBranchStatus("BJetnessFV_npvTrkOVcollTrk",1);
  oldtree->SetBranchStatus("BJetnessFV_avip3d_val",1);
  oldtree->SetBranchStatus("BJetnessFV_avip3d_sig",1);
  oldtree->SetBranchStatus("BJetnessFV_avsip3d_sig",1);
  oldtree->SetBranchStatus("BJetnessFV_avip1d_sig",1);
  oldtree->SetBranchStatus("bWeight",1);
  oldtree->SetBranchStatus("bWeightLFup",1);
  oldtree->SetBranchStatus("bWeightLFdown",1);
  oldtree->SetBranchStatus("bWeightHFup",1);
  oldtree->SetBranchStatus("bWeightHFdown",1);
  oldtree->SetBranchStatus("bWeightHFStats1up",1);
  oldtree->SetBranchStatus("bWeightHFStats1down",1);
  oldtree->SetBranchStatus("bWeightLFStats1up",1);
  oldtree->SetBranchStatus("bWeightLFStats1down",1);
  oldtree->SetBranchStatus("bWeightHFStats2up",1);
  oldtree->SetBranchStatus("bWeightHFStats2down",1);
  oldtree->SetBranchStatus("bWeightLFStats2up",1);
  oldtree->SetBranchStatus("bWeightLFStats2down",1);
  oldtree->SetBranchStatus("bWeightCErr1up",1);
  oldtree->SetBranchStatus("bWeightCErr1down",1);
  oldtree->SetBranchStatus("bWeightCErr2up",1);
  oldtree->SetBranchStatus("bWeightCErr2down",1);
  oldtree->SetBranchStatus("bWeightJESup",1);
  oldtree->SetBranchStatus("bWeightJESdown",1);
  oldtree->SetBranchStatus("patElectron_pt",1);
  oldtree->SetBranchStatus("patElectron_eta",1);
  oldtree->SetBranchStatus("patElectron_phi",1);
  oldtree->SetBranchStatus("patElectron_energy",1);
  oldtree->SetBranchStatus("patElectron_SCeta",1);
  oldtree->SetBranchStatus("patElectron_passConversionVeto",1);
  oldtree->SetBranchStatus("patElectron_inCrack",1);
  oldtree->SetBranchStatus("patElectron_isPassMvatrig",1);
  oldtree->SetBranchStatus("patElectron_isPassMvanontrig",1);
  oldtree->SetBranchStatus("patElectron_full5x5_sigmaIetaIeta",1);
  oldtree->SetBranchStatus("patElectron_hOverE",1);
  oldtree->SetBranchStatus("patElectron_ecalPFClusterIso",1);
  oldtree->SetBranchStatus("patElectron_hcalPFClusterIso",1);
  oldtree->SetBranchStatus("patElectron_isolPtTracks",1);
  oldtree->SetBranchStatus("patElectron_dEtaIn",1);
  oldtree->SetBranchStatus("patElectron_dPhiIn",1);
  oldtree->SetBranchStatus("patElectron_relIsoRhoEA",1);
  oldtree->SetBranchStatus("Muon_pt",1);
  oldtree->SetBranchStatus("Muon_eta",1);
  oldtree->SetBranchStatus("Muon_phi",1);
  oldtree->SetBranchStatus("Muon_energy",1);
  oldtree->SetBranchStatus("Muon_charge",1);
  oldtree->SetBranchStatus("Muon_tight",1);
  oldtree->SetBranchStatus("Muon_TrackerIso",1);
  oldtree->SetBranchStatus("Muon_isHighPt",1);
  oldtree->SetBranchStatus("Muon_relIsoDeltaBetaR04",1);
  if(sample==0 || sample==2){
    oldtree->SetBranchStatus("HLT_IsoMu22",1);
    oldtree->SetBranchStatus("HLT_IsoTkMu22",1);
    oldtree->SetBranchStatus("HLT_Ele27_eta2p1_WPTight_Gsf",1);
  }
  if(sample==1 || sample==2){
    oldtree->SetBranchStatus("ttHFCategory",1);
    oldtree->SetBranchStatus("Gen_pt",1);
    oldtree->SetBranchStatus("Gen_eta",1);
    oldtree->SetBranchStatus("Gen_phi",1);
    oldtree->SetBranchStatus("Gen_pdg_id",1);
    oldtree->SetBranchStatus("Gen_motherpdg_id",1);
  }
  oldtree->SetBranchStatus("EVENT_Q2tthbbWeightUp",1);
  oldtree->SetBranchStatus("EVENT_Q2tthbbWeightDown",1);
  oldtree->SetBranchStatus("EVENT_PDFtthbbWeightUp",1);
  oldtree->SetBranchStatus("EVENT_PDFtthbbWeightDown",1);
  oldtree->SetBranchStatus("EVENT_event",1);
  oldtree->SetBranchStatus("EVENT_run",1);
  oldtree->SetBranchStatus("EVENT_lumiBlock",1);
  oldtree->SetBranchStatus("PUWeight",1);
  oldtree->SetBranchStatus("Met_type1PF_pt",1);
  oldtree->SetBranchStatus("Met_type1PF_px",1);
  oldtree->SetBranchStatus("Met_type1PF_py",1);
  oldtree->SetBranchStatus("Met_type1PF_phi",1);

  //Create a new file + a clone of old tree in new file
  TFile *newfile = new TFile(Output,"recreate");
  TTree *newtree = oldtree->CloneTree(0);
  TTree *newevtree = evtree->CloneTree();

  //NEW VARIABLES
  double BDT_=-99;                TBranch *BDT =newtree->Branch("BDT",&BDT_,"BDT/D");
  double puweight_=1;             TBranch *puweight     =newtree->Branch("puweight",     &puweight_,     "puweight/D");
  double puweightUP_=1;           TBranch *puweightUP   =newtree->Branch("puweightUP",   &puweightUP_,   "puweightUP/D");
  double puweightDOWN_=1;         TBranch *puweightDOWN =newtree->Branch("puweightDOWN", &puweightDOWN_, "puweightDOWN/D");
  double Muon_IDSFval_=1;         TBranch *Muon_IDSFval =newtree->Branch("Muon_IDSFval", &Muon_IDSFval_, "Muon_IDSFval/D");
  double Muon_IsoSFval_=1;        TBranch *Muon_IsoSFval=newtree->Branch("Muon_IsoSFval",&Muon_IsoSFval_,"Muon_IsoSFval/D");
  double Muon_TrkSFval_=1;        TBranch *Muon_TrkSFval=newtree->Branch("Muon_TrkSFval",&Muon_TrkSFval_,"Muon_TrkSFval/D");
  double Muon_IDSFerr_=1;         TBranch *Muon_IDSFerr =newtree->Branch("Muon_IDSFerr", &Muon_IDSFerr_, "Muon_IDSFerr/D");
  double Muon_IsoSFerr_=1;        TBranch *Muon_IsoSFerr=newtree->Branch("Muon_IsoSFerr",&Muon_IsoSFerr_,"Muon_IsoSFerr/D");
  double Muon_TrkSFerr_=1;        TBranch *Muon_TrkSFerr=newtree->Branch("Muon_TrkSFerr",&Muon_TrkSFerr_,"Muon_TrkSFerr/D");
  double Electron_IDSFval_=1;     TBranch *Electron_IDSFval =newtree->Branch("Electron_IDSFval", &Electron_IDSFval_, "Electron_IDSFval/D");
  double Electron_GsfSFval_=1;    TBranch *Electron_GsfSFval=newtree->Branch("Electron_GsfSFval",&Electron_GsfSFval_,"Electron_GsfSFval/D");
  double Electron_IDSFerr_=1;     TBranch *Electron_IDSFerr =newtree->Branch("Electron_IDSFerr", &Electron_IDSFerr_, "Electron_IDSFerr/D");
  double Electron_GsfSFerr_=1;    TBranch *Electron_GsfSFerr=newtree->Branch("Electron_GsfSFerr",&Electron_GsfSFerr_,"Electron_GsfSFerr/D");
  int type_=-99;                  TBranch *type =newtree->Branch("type", &type_, "type/I");
  int ttHGenCategory_=-99;        TBranch *ttHGenCategory =newtree->Branch("ttHGenCategory", &ttHGenCategory_, "ttHGenCategory/I");
  int NumberOfJets_=-99;          TBranch *NumberOfJets =newtree->Branch("NumberOfJets",&NumberOfJets_,"NumberOfJets/I");
  int NumberOfBJets_=-99;         TBranch *NumberOfBJets=newtree->Branch("NumberOfBJets",&NumberOfBJets_,"NumberOfBJets/I");
  double all_sum_pt_with_met_=-99;TBranch *all_sum_pt_with_met=newtree->Branch("all_sum_pt_with_met",&all_sum_pt_with_met_,"all_sum_pt_with_met/D");
  double aplanarity_=-99;         TBranch *aplanarity=newtree->Branch("aplanarity",&aplanarity_,"aplanarity/D");
  double avg_btag_disc_btags_=-99;TBranch *avg_btag_disc_btags=newtree->Branch("avg_btag_disc_btags",&avg_btag_disc_btags_,"avg_btag_disc_btags/D");
  double avg_dr_tagged_jets_=-99; TBranch *avg_dr_tagged_jets=newtree->Branch("avg_dr_tagged_jets",&avg_dr_tagged_jets_,"avg_dr_tagged_jets/D");
  double best_higgs_mass_=-99;    TBranch *best_higgs_mass=newtree->Branch("best_higgs_mass",&best_higgs_mass_,"best_higgs_mass/D");
  double dEta_fn_=-99;            TBranch *dEta_fn=newtree->Branch("dEta_fn",&dEta_fn_,"dEta_fn/D");
  double fifth_highest_CSV_=-99;  TBranch *fifth_highest_CSV=newtree->Branch("fifth_highest_CSV",&fifth_highest_CSV_,"fifth_highest_CSV/D");
  double first_jet_pt_=-99;       TBranch *first_jet_pt=newtree->Branch("first_jet_pt",&first_jet_pt_,"first_jet_pt/D");
  double fourth_highest_btag_=-99;TBranch *fourth_highest_btag=newtree->Branch("fourth_highest_btag",&fourth_highest_btag_,"fourth_highest_btag/D");
  double fourth_jet_pt_=-99;      TBranch *fourth_jet_pt=newtree->Branch("fourth_jet_pt",&fourth_jet_pt_,"fourth_jet_pt/D");
  double h0_=-99;                 TBranch *h0=newtree->Branch("h0",&h0_,"h0/D");
  double h1_=-99;                 TBranch *h1=newtree->Branch("h1",&h1_,"h1/D");
  double h2_=-99;                 TBranch *h2=newtree->Branch("h2",&h2_,"h2/D");
  double h3_=-99;                 TBranch *h3=newtree->Branch("h3",&h3_,"h3/D");
  double HT_=-99;                 TBranch *HT=newtree->Branch("HT",&HT_,"HT/D");
  double lowest_btag_=-99;        TBranch *lowest_btag=newtree->Branch("lowest_btag",&lowest_btag_,"lowest_btag/D");
  double M3_=-99;                 TBranch *M3=newtree->Branch("M3",&M3_,"M3/D");
  double maxeta_jet_jet_=-99;     TBranch *maxeta_jet_jet=newtree->Branch("maxeta_jet_jet",&maxeta_jet_jet_,"maxeta_jet_jet/D");
  double maxeta_jet_tag_=-99;     TBranch *maxeta_jet_tag=newtree->Branch("maxeta_jet_tag",&maxeta_jet_tag_,"maxeta_jet_tag/D");
  double maxeta_tag_tag_=-99;     TBranch *maxeta_tag_tag=newtree->Branch("maxeta_tag_tag",&maxeta_tag_tag_,"maxeta_tag_tag/D");
  double min_dr_tagged_jets_=-99; TBranch *min_dr_tagged_jets=newtree->Branch("min_dr_tagged_jets",&min_dr_tagged_jets_,"min_dr_tagged_jets/D");
  double MET_=-99;                TBranch *MET=newtree->Branch("MET",&MET_,"MET/D");
  double MHT_=-99;                TBranch *MHT=newtree->Branch("MHT",&MHT_,"MHT/D");
  double Mlb_=-99;                TBranch *Mlb=newtree->Branch("Mlb",&Mlb_,"Mlb/D");
  double second_highest_btag_=-99;TBranch *second_highest_btag=newtree->Branch("second_highest_btag",&second_highest_btag_,"second_highest_btag/D");
  double second_jet_pt_=-99;      TBranch *second_jet_pt=newtree->Branch("second_jet_pt",&second_jet_pt_,"second_jet_pt/D");
  double sphericity_=-99;         TBranch *sphericity=newtree->Branch("sphericity",&sphericity_,"sphericity/D");
  double third_highest_btag_=-99; TBranch *third_highest_btag=newtree->Branch("third_highest_btag",&third_highest_btag_,"third_highest_btag/D");
  double third_jet_pt_=-99;       TBranch *third_jet_pt=newtree->Branch("third_jet_pt",&third_jet_pt_,"third_jet_pt/D");
  double Evt_CSV_Average_=-99;    TBranch *Evt_CSV_Average=newtree->Branch("Evt_CSV_Average",&Evt_CSV_Average_,"Evt_CSV_Average/D");
  double blr_=-99;                TBranch *blr=newtree->Branch("blr",&blr_,"blr/D");
  double Evt_Deta_JetsAverage_=-99;
  double closest_tagged_dijet_mass_=-99;
  double dev_from_avg_disc_btags_=-99;
  double dr_between_lep_and_closest_jet_=-99;
  double invariant_mass_of_everything_=-99;
  double pt_all_jets_over_E_all_jets_=-99;
  double tagged_dijet_mass_closest_to_125_=-99;
  TBranch *Evt_Deta_JetsAverage=newtree->Branch("Evt_Deta_JetsAverage",&Evt_Deta_JetsAverage_,"Evt_Deta_JetsAverage/D");
  TBranch *closest_tagged_dijet_mass=newtree->Branch("closest_tagged_dijet_mass",&closest_tagged_dijet_mass_,"closest_tagged_dijet_mass/D");
  TBranch *dev_from_avg_disc_btags=newtree->Branch("dev_from_avg_disc_btags",&dev_from_avg_disc_btags_,"dev_from_avg_disc_btags/D");
  TBranch *dr_between_lep_and_closest_jet=newtree->Branch("dr_between_lep_and_closest_jet",&dr_between_lep_and_closest_jet_,"dr_between_lep_and_closest_jet/D");
  TBranch *invariant_mass_of_everything=newtree->Branch("invariant_mass_of_everything",&invariant_mass_of_everything_,"invariant_mass_of_everything/D");
  TBranch *pt_all_jets_over_E_all_jets=newtree->Branch("pt_all_jets_over_E_all_jets",&pt_all_jets_over_E_all_jets_,"pt_all_jets_over_E_all_jets/D");
  TBranch *tagged_dijet_mass_closest_to_125=newtree->Branch("tagged_dijet_mass_closest_to_125",&tagged_dijet_mass_closest_to_125_,"tagged_dijet_mass_closest_to_125/D");

  //MUONS LOAD
  vector<double>* Muon_pt=0;
  vector<double>* Muon_eta=0;
  vector<double>* Muon_phi=0;
  vector<double>* Muon_energy=0;
  vector<double>* Muon_relIsoDeltaBetaR04=0;
  vector<int>* Muon_tight=0;
  TBranch *b_Muon_pt,*b_Muon_eta,*b_Muon_phi,*b_Muon_energy,*b_Muon_relIsoDeltaBetaR04,*b_Muon_tight;
  oldtree->SetBranchAddress("Muon_pt", &Muon_pt, &b_Muon_pt);
  oldtree->SetBranchAddress("Muon_eta",&Muon_eta,&b_Muon_eta);
  oldtree->SetBranchAddress("Muon_phi",&Muon_phi,&b_Muon_phi);
  oldtree->SetBranchAddress("Muon_energy",&Muon_energy,&b_Muon_energy);
  oldtree->SetBranchAddress("Muon_relIsoDeltaBetaR04",&Muon_relIsoDeltaBetaR04,&b_Muon_relIsoDeltaBetaR04);
  oldtree->SetBranchAddress("Muon_tight",&Muon_tight,&b_Muon_tight);

  //ELECTRON LOAD
  vector<double>* patElectron_passConversionVeto=0;
  vector<double>* patElectron_inCrack=0;
  vector<double>* patElectron_pt=0;
  vector<double>* patElectron_eta=0;
  vector<double>* patElectron_SCeta=0;
  vector<double>* patElectron_phi=0;
  vector<double>* patElectron_energy=0;
  vector<double>* patElectron_full5x5_sigmaIetaIeta=0;
  vector<double>* patElectron_hOverE=0;
  vector<double>* patElectron_ecalPFClusterIso=0;
  vector<double>* patElectron_hcalPFClusterIso=0;
  vector<double>* patElectron_isolPtTracks=0;
  vector<double>* patElectron_dEtaIn=0;
  vector<double>* patElectron_dPhiIn=0;
  vector<double>* patElectron_relIsoRhoEA=0;
  vector<int> *patElectron_isPassMvatrig=0;
  vector<int> *patElectron_isPassMvanontrig=0;
  TBranch *b_patElectron_pt,*b_patElectron_eta,*b_patElectron_SCeta,*b_patElectron_phi,*b_patElectron_energy,*b_patElectron_isPassMvatrig,*b_patElectron_isPassMvanontrig,*b_patElectron_inCrack,*b_patElectron_passConversionVeto,*b_patElectron_full5x5_sigmaIetaIeta,*b_patElectron_hOverE,*b_patElectron_ecalPFClusterIso,*b_patElectron_hcalPFClusterIso,*b_patElectron_isolPtTracks,*b_patElectron_dEtaIn,*b_patElectron_dPhiIn,*b_patElectron_relIsoRhoEA;
  oldtree->SetBranchAddress("patElectron_pt", &patElectron_pt, &b_patElectron_pt);
  oldtree->SetBranchAddress("patElectron_eta", &patElectron_eta, &b_patElectron_eta);
  oldtree->SetBranchAddress("patElectron_SCeta", &patElectron_SCeta, &b_patElectron_SCeta);
  oldtree->SetBranchAddress("patElectron_phi", &patElectron_phi, &b_patElectron_phi);
  oldtree->SetBranchAddress("patElectron_energy", &patElectron_energy, &b_patElectron_energy);
  oldtree->SetBranchAddress("patElectron_inCrack", &patElectron_inCrack, &b_patElectron_inCrack);
  oldtree->SetBranchAddress("patElectron_passConversionVeto", &patElectron_passConversionVeto, &b_patElectron_passConversionVeto);
  oldtree->SetBranchAddress("patElectron_isPassMvatrig",&patElectron_isPassMvatrig,&b_patElectron_isPassMvatrig);
  oldtree->SetBranchAddress("patElectron_isPassMvanontrig",&patElectron_isPassMvanontrig,&b_patElectron_isPassMvanontrig);
  oldtree->SetBranchAddress("patElectron_full5x5_sigmaIetaIeta",&patElectron_full5x5_sigmaIetaIeta,&b_patElectron_full5x5_sigmaIetaIeta);
  oldtree->SetBranchAddress("patElectron_hOverE",&patElectron_hOverE,&b_patElectron_hOverE);
  oldtree->SetBranchAddress("patElectron_ecalPFClusterIso",&patElectron_ecalPFClusterIso,&b_patElectron_ecalPFClusterIso);
  oldtree->SetBranchAddress("patElectron_hcalPFClusterIso",&patElectron_hcalPFClusterIso,&b_patElectron_hcalPFClusterIso);
  oldtree->SetBranchAddress("patElectron_isolPtTracks",&patElectron_isolPtTracks,&b_patElectron_isolPtTracks);
  oldtree->SetBranchAddress("patElectron_dEtaIn",&patElectron_dEtaIn,&b_patElectron_dEtaIn);
  oldtree->SetBranchAddress("patElectron_dPhiIn",&patElectron_dPhiIn,&b_patElectron_dPhiIn);
  oldtree->SetBranchAddress("patElectron_relIsoRhoEA",&patElectron_relIsoRhoEA,&b_patElectron_relIsoRhoEA);

  //JET LOAD
  vector<double>* Jet_pt=0;
  vector<double>* Jet_Uncorr_pt=0;
  vector<double>* Jet_eta=0;
  vector<double>* Jet_phi=0;
  vector<double>* Jet_newpfCombinedInclusiveSecondaryVertexV2BJetTags=0;
  vector<double>* Jet_neutralHadEnergyFraction=0;
  vector<double>* Jet_chargedEmEnergyFraction=0;
  vector<double>* Jet_neutralEmEnergyFraction=0;
  vector<double>* Jet_chargedHadronEnergyFraction=0;
  vector<double>* Jet_numberOfConstituents=0;
  vector<double>* Jet_chargedMultiplicity=0;
  vector<double>* Jet_mass=0;
  vector<double>* Jet_JesSF=0;
  vector<double>* Jet_JerSF=0;
  vector<double>* Jet_JesSFup=0;
  vector<double>* Jet_JerSFup=0;
  vector<double>* Jet_JesSFdown=0;
  vector<double>* Jet_JerSFdown=0;
  TBranch *b_Jet_pt,*b_Jet_Uncorr_pt,*b_Jet_eta,*b_Jet_phi,*b_Jet_neutralHadEnergyFraction,*b_Jet_chargedEmEnergyFraction,*b_Jet_neutralEmEnergyFraction,*b_Jet_mass,*b_Jet_JesSF,*b_Jet_JesSFup,*b_Jet_JesSFdown;
  TBranch *b_Jet_newpfCombinedInclusiveSecondaryVertexV2BJetTags,*b_Jet_numberOfConstituents,*b_Jet_chargedHadronEnergyFraction,*b_Jet_chargedMultiplicity,*b_Jet_JerSF,*b_Jet_JerSFup,*b_Jet_JerSFdown;
  oldtree->SetBranchAddress("Jet_pt", &Jet_pt, &b_Jet_pt);
  oldtree->SetBranchAddress("Jet_Uncorr_pt", &Jet_Uncorr_pt, &b_Jet_Uncorr_pt);
  oldtree->SetBranchAddress("Jet_eta", &Jet_eta, &b_Jet_eta);
  oldtree->SetBranchAddress("Jet_phi", &Jet_phi, &b_Jet_phi);
  oldtree->SetBranchAddress("Jet_newpfCombinedInclusiveSecondaryVertexV2BJetTags", &Jet_newpfCombinedInclusiveSecondaryVertexV2BJetTags, &b_Jet_newpfCombinedInclusiveSecondaryVertexV2BJetTags);
  oldtree->SetBranchAddress("Jet_neutralHadEnergyFraction", &Jet_neutralHadEnergyFraction, &b_Jet_neutralHadEnergyFraction);
  oldtree->SetBranchAddress("Jet_chargedEmEnergyFraction", &Jet_chargedEmEnergyFraction, &b_Jet_chargedEmEnergyFraction);
  oldtree->SetBranchAddress("Jet_neutralEmEnergyFraction", &Jet_neutralEmEnergyFraction, &b_Jet_neutralEmEnergyFraction);
  oldtree->SetBranchAddress("Jet_numberOfConstituents", &Jet_numberOfConstituents, &b_Jet_numberOfConstituents);
  oldtree->SetBranchAddress("Jet_chargedHadronEnergyFraction", &Jet_chargedHadronEnergyFraction, &b_Jet_chargedHadronEnergyFraction);
  oldtree->SetBranchAddress("Jet_chargedMultiplicity", &Jet_chargedMultiplicity, &b_Jet_chargedMultiplicity);
  oldtree->SetBranchAddress("Jet_mass", &Jet_mass, &b_Jet_mass);
  oldtree->SetBranchAddress("Jet_JesSF",&Jet_JesSF,&b_Jet_JesSF);
  oldtree->SetBranchAddress("Jet_JerSF",&Jet_JerSF,&b_Jet_JerSF);
  oldtree->SetBranchAddress("Jet_JesSFup",&Jet_JesSFup,&b_Jet_JesSFup);
  oldtree->SetBranchAddress("Jet_JerSFup",&Jet_JerSFup,&b_Jet_JerSFup);
  oldtree->SetBranchAddress("Jet_JesSFdown",&Jet_JesSFdown,&b_Jet_JesSFdown);
  oldtree->SetBranchAddress("Jet_JerSFdown",&Jet_JerSFdown,&b_Jet_JerSFdown);

  //BJETNESS LOAD
  int *BJetness_isSingleLepton, *BJetness_isDoubleLepton, *BJetness_ngenbh, *BJetness_ngenbt, *BJetness_ngenb, *BJetness_ngenc;
  int *BJetnessFV_isSingleLepton, *BJetnessFV_isDoubleLepton;
  vector<double>* BJetness_partonFlavour=0;
  vector<double>* BJetness_hadronFlavour=0;
  vector<double>* BJetness_numjet=0;
  vector<double>* BJetness_jetpt=0;
  vector<double>* BJetness_jeteta=0;
  vector<double>* BJetness_jetphi=0;
  vector<double>* BJetness_jetenergy=0;
  vector<double>* BJetness_jetcsv=0;
  vector<double>* BJetness_pfJetProbabilityBJetTags=0;
  vector<double>* BJetness_pfCombinedMVAV2BJetTags=0;
  vector<double>* BJetness_pfCombinedCvsLJetTags=0;
  vector<double>* BJetness_pfCombinedCvsBJetTags=0;
  vector<double>* BJetness_pt=0;
  vector<double>* BJetness_eta=0;
  vector<double>* BJetness_phi=0;
  vector<double>* BJetness_en=0;
  vector<double>* BJetness_ptOVen=0;
  vector<double>* BJetness_jetschpvass=0;
  vector<double>* BJetness_jetschfrompv=0;
  vector<double>* BJetness_jetschip3dval=0;
  vector<double>* BJetness_jetschip3dsig=0;
  vector<double>* BJetness_jetschip2dval=0;
  vector<double>* BJetness_jetschip2dsig=0;
  vector<double>* BJetness_jetschisgoodtrk=0;
  vector<double>* BJetness_jetschtrkpur=0;
  vector<double>* BJetness_jetschpt=0;
  vector<double>* BJetness_jetschen=0;
  vector<double>* BJetness_num_pdgid_eles=0;
  vector<double>* BJetness_num_soft_eles=0;
  vector<double>* BJetness_num_vetonoipnoiso_eles=0;
  vector<double>* BJetness_num_loosenoipnoiso_eles=0;
  vector<double>* BJetness_num_veto_eles=0;
  vector<double>* BJetness_num_loose_eles=0;
  vector<double>* BJetness_num_medium_eles=0;
  vector<double>* BJetness_num_tight_eles=0;
  vector<double>* BJetness_num_mvatrig_eles=0;
  vector<double>* BJetness_num_mvanontrig_eles=0;
  vector<double>* BJetness_num_mvatrigwp90_eles=0;
  vector<double>* BJetness_num_mvanontrigwp90_eles=0;
  vector<double>* BJetness_num_heep_eles=0;
  vector<double>* BJetness_num_pdgid_mus=0;
  vector<double>* BJetness_num_loose_mus=0;
  vector<double>* BJetness_num_soft_mus=0;
  vector<double>* BJetness_num_medium_mus=0;
  vector<double>* BJetness_num_tight_mus=0;
  vector<double>* BJetness_num_highpt_mus=0;
  vector<double>* BJetness_num_POGisGood_mus=0;
  vector<double>* BJetness_numjettrks=0;
  vector<double>* BJetness_numjettrkspv=0;
  vector<double>* BJetness_numjettrksnopv=0;
//  vector<double>* BJetness_npvTrkOVcollTrk=0;
  vector<double>* BJetness_pvTrkOVcollTrk=0;
  vector<double>* BJetness_npvTrkOVpvTrk=0;
  vector<double>* BJetness_npvPtOVcollPt=0;
  vector<double>* BJetness_pvPtOVcollPt=0;
  vector<double>* BJetness_npvPtOVpvPt=0;
  vector<double>* BJetness_avprel=0;
  vector<double>* BJetness_avppar=0;
  vector<double>* BJetness_avetarel=0;
  vector<double>* BJetness_avetapar=0;
  vector<double>* BJetness_avdr=0;
  vector<double>* BJetness_avpreljetpt=0;
  vector<double>* BJetness_avpreljeten=0;
  vector<double>* BJetness_avpparjetpt=0;
  vector<double>* BJetness_avpparjeten=0;
  vector<double>* BJetness_avnum2v=0;
  vector<double>* BJetness_avnumno2v=0;
  vector<double>* BJetness_avdca3d2t=0;
  vector<double>* BJetness_avdca3dno2t=0;
  vector<double>* BJetness_avdca3d=0;
  vector<double>* BJetness_avdca2d2t=0;
  vector<double>* BJetness_avdca2dno2t=0;
  vector<double>* BJetness_avdca2d=0;
  vector<double>* BJetness_chi2=0;
  vector<double>* BJetness_avip3d_val=0;
  vector<double>* BJetness_avip3d_sig=0;
  vector<double>* BJetness_avsip3d_val=0;
  vector<double>* BJetness_avsip3d_sig=0;
  vector<double>* BJetness_numip3dpos=0;
  vector<double>* BJetness_numip3dneg=0;
  vector<double>* BJetness_avip2d_val=0;
  vector<double>* BJetness_avip2d_sig=0;
  vector<double>* BJetness_avsip2d_val=0;
  vector<double>* BJetness_avsip2d_sig=0;
  vector<double>* BJetness_numip2dpos=0;
  vector<double>* BJetness_numip2dneg=0;
  vector<double>* BJetness_avip1d_val=0;
  vector<double>* BJetness_avip1d_sig=0;
  vector<double>* BJetness_avsip1d_val=0;
  vector<double>* BJetness_avsip1d_sig=0;
  vector<double>* BJetnessFV_jetcsv=0;
  vector<double>* BJetnessFV_pfJetProbabilityBJetTags=0;
  vector<double>* BJetnessFV_pfCombinedMVAV2BJetTags=0;
  vector<double>* BJetnessFV_num_leps=0;
  vector<double>* BJetnessFV_npvTrkOVcollTrk=0;
  vector<double>* BJetnessFV_avip3d_val=0;
  vector<double>* BJetnessFV_avip3d_sig=0;
  vector<double>* BJetnessFV_avsip3d_sig=0;
  vector<double>* BJetnessFV_avip1d_sig=0;
  double bWeight, bWeightLFup, bWeightLFdown, bWeightHFup, bWeightHFdown, bWeightHFStats1up, bWeightHFStats1down, bWeightLFStats1up, bWeightLFStats1down, bWeightHFStats2up, bWeightHFStats2down, bWeightLFStats2up, bWeightLFStats2down, bWeightCErr1up, bWeightCErr1down, bWeightCErr2up, bWeightCErr2down, bWeightJESup, bWeightJESdown;
  TBranch *b_BJetness_isSingleLepton, *b_BJetness_isDoubleLepton, *b_BJetness_ngenbh, *b_BJetness_ngenbt, *b_BJetness_ngenb, *b_BJetness_ngenc, *b_BJetness_partonFlavour, *b_BJetness_hadronFlavour, *b_BJetness_numjet, *b_BJetness_jetpt, *b_BJetness_jeteta, *b_BJetness_jetphi, *b_BJetness_jetenergy, *b_BJetness_jetcsv, *b_BJetness_pfJetProbabilityBJetTags, *b_BJetness_pfCombinedMVAV2BJetTags, *b_BJetness_pfCombinedCvsLJetTags, *b_BJetness_pfCombinedCvsBJetTags, *b_BJetness_pt, *b_BJetness_eta, *b_BJetness_phi, *b_BJetness_en, *b_BJetness_ptOVen, *b_BJetness_jetschpvass, *b_BJetness_jetschfrompv, *b_BJetness_jetschip3dval, *b_BJetness_jetschip3dsig, *b_BJetness_jetschip2dval, *b_BJetness_jetschip2dsig, *b_BJetness_jetschisgoodtrk, *b_BJetness_jetschtrkpur, *b_BJetness_jetschpt, *b_BJetness_jetschen, *b_BJetness_num_pdgid_eles, *b_BJetness_num_soft_eles, *b_BJetness_num_vetonoipnoiso_eles, *b_BJetness_num_loosenoipnoiso_eles, *b_BJetness_num_veto_eles, *b_BJetness_num_loose_eles, *b_BJetness_num_medium_eles, *b_BJetness_num_tight_eles, *b_BJetness_num_mvatrig_eles, *b_BJetness_num_mvanontrig_eles, *b_BJetness_num_mvatrigwp90_eles, *b_BJetness_num_mvanontrigwp90_eles, *b_BJetness_num_heep_eles, *b_BJetness_num_pdgid_mus, *b_BJetness_num_loose_mus, *b_BJetness_num_soft_mus, *b_BJetness_num_medium_mus, *b_BJetness_num_tight_mus, *b_BJetness_num_highpt_mus, *b_BJetness_num_POGisGood_mus, *b_BJetness_numjettrks, *b_BJetness_numjettrkspv, *b_BJetness_numjettrksnopv, *b_BJetness_pvTrkOVcollTrk, *b_BJetness_npvTrkOVpvTrk, *b_BJetness_npvPtOVcollPt, *b_BJetness_pvPtOVcollPt, *b_BJetness_npvPtOVpvPt, *b_BJetness_avprel, *b_BJetness_avppar, *b_BJetness_avetarel, *b_BJetness_avetapar, *b_BJetness_avdr, *b_BJetness_avpreljetpt, *b_BJetness_avpreljeten, *b_BJetness_avpparjetpt, *b_BJetness_avpparjeten, *b_BJetness_avnum2v, *b_BJetness_avnumno2v, *b_BJetness_avdca3d2t, *b_BJetness_avdca3dno2t, *b_BJetness_avdca3d, *b_BJetness_avdca2d2t, *b_BJetness_avdca2dno2t, *b_BJetness_avdca2d, *b_BJetness_chi2, *b_BJetness_avip3d_val, *b_BJetness_avip3d_sig, *b_BJetness_avsip3d_val, *b_BJetness_avsip3d_sig, *b_BJetness_numip3dpos, *b_BJetness_numip3dneg, *b_BJetness_avip2d_val, *b_BJetness_avip2d_sig, *b_BJetness_avsip2d_val, *b_BJetness_avsip2d_sig, *b_BJetness_numip2dpos, *b_BJetness_numip2dneg, *b_BJetness_avip1d_val, *b_BJetness_avip1d_sig, *b_BJetness_avsip1d_val, *b_BJetness_avsip1d_sig, *b_BJetnessFV_isSingleLepton, *b_BJetnessFV_isDoubleLepton, *b_BJetnessFV_jetcsv, *b_BJetnessFV_pfJetProbabilityBJetTags, *b_BJetnessFV_pfCombinedMVAV2BJetTags, *b_BJetnessFV_num_leps, *b_BJetnessFV_npvTrkOVcollTrk, *b_BJetnessFV_avip3d_val, *b_BJetnessFV_avip3d_sig, *b_BJetnessFV_avsip3d_sig, *b_BJetnessFV_avip1d_sig, *b_bWeight, *b_bWeightLFup, *b_bWeightLFdown, *b_bWeightHFup, *b_bWeightHFdown, *b_bWeightHFStats1up, *b_bWeightHFStats1down, *b_bWeightLFStats1up, *b_bWeightLFStats1down, *b_bWeightHFStats2up, *b_bWeightHFStats2down, *b_bWeightLFStats2up, *b_bWeightLFStats2down, *b_bWeightCErr1up, *b_bWeightCErr1down, *b_bWeightCErr2up, *b_bWeightCErr2down, *b_bWeightJESup, *b_bWeightJESdown;// *b_BJetness_npvTrkOVcollTrk
  oldtree->SetBranchAddress("BJetness_isSingleLepton",&BJetness_isSingleLepton,&b_BJetness_isSingleLepton);
  oldtree->SetBranchAddress("BJetness_isDoubleLepton",&BJetness_isDoubleLepton,&b_BJetness_isDoubleLepton);
  oldtree->SetBranchAddress("BJetness_ngenbh",&BJetness_ngenbh,&b_BJetness_ngenbh);
  oldtree->SetBranchAddress("BJetness_ngenbt",&BJetness_ngenbt,&b_BJetness_ngenbt);
  oldtree->SetBranchAddress("BJetness_ngenb",&BJetness_ngenb,&b_BJetness_ngenb);
  oldtree->SetBranchAddress("BJetness_ngenc",&BJetness_ngenc,&b_BJetness_ngenc);
  oldtree->SetBranchAddress("BJetness_partonFlavour",&BJetness_partonFlavour,&b_BJetness_partonFlavour);
  oldtree->SetBranchAddress("BJetness_hadronFlavour",&BJetness_hadronFlavour,&b_BJetness_hadronFlavour);
  oldtree->SetBranchAddress("BJetness_numjet",&BJetness_numjet,&b_BJetness_numjet);
  oldtree->SetBranchAddress("BJetness_jetpt",&BJetness_jetpt,&b_BJetness_jetpt);
  oldtree->SetBranchAddress("BJetness_jeteta",&BJetness_jeteta,&b_BJetness_jeteta);
  oldtree->SetBranchAddress("BJetness_jetphi",&BJetness_jetphi,&b_BJetness_jetphi);
  oldtree->SetBranchAddress("BJetness_jetenergy",&BJetness_jetenergy,&b_BJetness_jetenergy);
  oldtree->SetBranchAddress("BJetness_jetcsv",&BJetness_jetcsv,&b_BJetness_jetcsv);
  oldtree->SetBranchAddress("BJetness_pfJetProbabilityBJetTags",&BJetness_pfJetProbabilityBJetTags,&b_BJetness_pfJetProbabilityBJetTags);
  oldtree->SetBranchAddress("BJetness_pfCombinedMVAV2BJetTags",&BJetness_pfCombinedMVAV2BJetTags,&b_BJetness_pfCombinedMVAV2BJetTags);
  oldtree->SetBranchAddress("BJetness_pfCombinedCvsLJetTags",&BJetness_pfCombinedCvsLJetTags,&b_BJetness_pfCombinedCvsLJetTags);
  oldtree->SetBranchAddress("BJetness_pfCombinedCvsBJetTags",&BJetness_pfCombinedCvsBJetTags,&b_BJetness_pfCombinedCvsBJetTags);
  oldtree->SetBranchAddress("BJetness_pt",&BJetness_pt,&b_BJetness_pt);
  oldtree->SetBranchAddress("BJetness_eta",&BJetness_eta,&b_BJetness_eta);
  oldtree->SetBranchAddress("BJetness_phi",&BJetness_phi,&b_BJetness_phi);
  oldtree->SetBranchAddress("BJetness_en",&BJetness_en,&b_BJetness_en);
  oldtree->SetBranchAddress("BJetness_ptOVen",&BJetness_ptOVen,&b_BJetness_ptOVen);
  oldtree->SetBranchAddress("BJetness_jetschpvass",&BJetness_jetschpvass,&b_BJetness_jetschpvass);
  oldtree->SetBranchAddress("BJetness_jetschfrompv",&BJetness_jetschfrompv,&b_BJetness_jetschfrompv);
  oldtree->SetBranchAddress("BJetness_jetschip3dval",&BJetness_jetschip3dval,&b_BJetness_jetschip3dval);
  oldtree->SetBranchAddress("BJetness_jetschip3dsig",&BJetness_jetschip3dsig,&b_BJetness_jetschip3dsig);
  oldtree->SetBranchAddress("BJetness_jetschip2dval",&BJetness_jetschip2dval,&b_BJetness_jetschip2dval);
  oldtree->SetBranchAddress("BJetness_jetschip2dsig",&BJetness_jetschip2dsig,&b_BJetness_jetschip2dsig);
  oldtree->SetBranchAddress("BJetness_jetschisgoodtrk",&BJetness_jetschisgoodtrk,&b_BJetness_jetschisgoodtrk);
  oldtree->SetBranchAddress("BJetness_jetschtrkpur",&BJetness_jetschtrkpur,&b_BJetness_jetschtrkpur);
  oldtree->SetBranchAddress("BJetness_jetschpt",&BJetness_jetschpt,&b_BJetness_jetschpt);
  oldtree->SetBranchAddress("BJetness_jetschen",&BJetness_jetschen,&b_BJetness_jetschen);
  oldtree->SetBranchAddress("BJetness_num_pdgid_eles",&BJetness_num_pdgid_eles,&b_BJetness_num_pdgid_eles);
  oldtree->SetBranchAddress("BJetness_num_soft_eles",&BJetness_num_soft_eles,&b_BJetness_num_soft_eles);
  oldtree->SetBranchAddress("BJetness_num_vetonoipnoiso_eles",&BJetness_num_vetonoipnoiso_eles,&b_BJetness_num_vetonoipnoiso_eles);
  oldtree->SetBranchAddress("BJetness_num_loosenoipnoiso_eles",&BJetness_num_loosenoipnoiso_eles,&b_BJetness_num_loosenoipnoiso_eles);
  oldtree->SetBranchAddress("BJetness_num_veto_eles",&BJetness_num_veto_eles,&b_BJetness_num_veto_eles);
  oldtree->SetBranchAddress("BJetness_num_loose_eles",&BJetness_num_loose_eles,&b_BJetness_num_loose_eles);
  oldtree->SetBranchAddress("BJetness_num_medium_eles",&BJetness_num_medium_eles,&b_BJetness_num_medium_eles);
  oldtree->SetBranchAddress("BJetness_num_tight_eles",&BJetness_num_tight_eles,&b_BJetness_num_tight_eles);
  oldtree->SetBranchAddress("BJetness_num_mvatrig_eles",&BJetness_num_mvatrig_eles,&b_BJetness_num_mvatrig_eles);
  oldtree->SetBranchAddress("BJetness_num_mvanontrig_eles",&BJetness_num_mvanontrig_eles,&b_BJetness_num_mvanontrig_eles);
  oldtree->SetBranchAddress("BJetness_num_mvatrigwp90_eles",&BJetness_num_mvatrigwp90_eles,&b_BJetness_num_mvatrigwp90_eles);
  oldtree->SetBranchAddress("BJetness_num_mvanontrigwp90_eles",&BJetness_num_mvanontrigwp90_eles,&b_BJetness_num_mvanontrigwp90_eles);
  oldtree->SetBranchAddress("BJetness_num_heep_eles",&BJetness_num_heep_eles,&b_BJetness_num_heep_eles);
  oldtree->SetBranchAddress("BJetness_num_pdgid_mus",&BJetness_num_pdgid_mus,&b_BJetness_num_pdgid_mus);
  oldtree->SetBranchAddress("BJetness_num_loose_mus",&BJetness_num_loose_mus,&b_BJetness_num_loose_mus);
  oldtree->SetBranchAddress("BJetness_num_soft_mus",&BJetness_num_soft_mus,&b_BJetness_num_soft_mus);
  oldtree->SetBranchAddress("BJetness_num_medium_mus",&BJetness_num_medium_mus,&b_BJetness_num_medium_mus);
  oldtree->SetBranchAddress("BJetness_num_tight_mus",&BJetness_num_tight_mus,&b_BJetness_num_tight_mus);
  oldtree->SetBranchAddress("BJetness_num_highpt_mus",&BJetness_num_highpt_mus,&b_BJetness_num_highpt_mus);
  oldtree->SetBranchAddress("BJetness_num_POGisGood_mus",&BJetness_num_POGisGood_mus,&b_BJetness_num_POGisGood_mus);
  oldtree->SetBranchAddress("BJetness_numjettrks",&BJetness_numjettrks,&b_BJetness_numjettrks);
  oldtree->SetBranchAddress("BJetness_numjettrkspv",&BJetness_numjettrkspv,&b_BJetness_numjettrkspv);
  oldtree->SetBranchAddress("BJetness_numjettrksnopv",&BJetness_numjettrksnopv,&b_BJetness_numjettrksnopv);
//  oldtree->SetBranchAddress("BJetness_npvTrkOVcollTrk",&BJetness_npvTrkOVcollTrk,&b_BJetness_npvTrkOVcollTrk);
  oldtree->SetBranchAddress("BJetness_pvTrkOVcollTrk",&BJetness_pvTrkOVcollTrk,&b_BJetness_pvTrkOVcollTrk);
  oldtree->SetBranchAddress("BJetness_npvTrkOVpvTrk",&BJetness_npvTrkOVpvTrk,&b_BJetness_npvTrkOVpvTrk);
  oldtree->SetBranchAddress("BJetness_npvPtOVcollPt",&BJetness_npvPtOVcollPt,&b_BJetness_npvPtOVcollPt);
  oldtree->SetBranchAddress("BJetness_pvPtOVcollPt",&BJetness_pvPtOVcollPt,&b_BJetness_pvPtOVcollPt);
  oldtree->SetBranchAddress("BJetness_npvPtOVpvPt",&BJetness_npvPtOVpvPt,&b_BJetness_npvPtOVpvPt);
  oldtree->SetBranchAddress("BJetness_avprel",&BJetness_avprel,&b_BJetness_avprel);
  oldtree->SetBranchAddress("BJetness_avppar",&BJetness_avppar,&b_BJetness_avppar);
  oldtree->SetBranchAddress("BJetness_avetarel",&BJetness_avetarel,&b_BJetness_avetarel);
  oldtree->SetBranchAddress("BJetness_avetapar",&BJetness_avetapar,&b_BJetness_avetapar);
  oldtree->SetBranchAddress("BJetness_avdr",&BJetness_avdr,&b_BJetness_avdr);
  oldtree->SetBranchAddress("BJetness_avpreljetpt",&BJetness_avpreljetpt,&b_BJetness_avpreljetpt);
  oldtree->SetBranchAddress("BJetness_avpreljeten",&BJetness_avpreljeten,&b_BJetness_avpreljeten);
  oldtree->SetBranchAddress("BJetness_avpparjetpt",&BJetness_avpparjetpt,&b_BJetness_avpparjetpt);
  oldtree->SetBranchAddress("BJetness_avpparjeten",&BJetness_avpparjeten,&b_BJetness_avpparjeten);
  oldtree->SetBranchAddress("BJetness_avnum2v",&BJetness_avnum2v,&b_BJetness_avnum2v);
  oldtree->SetBranchAddress("BJetness_avnumno2v",&BJetness_avnumno2v,&b_BJetness_avnumno2v);
  oldtree->SetBranchAddress("BJetness_avdca3d2t",&BJetness_avdca3d2t,&b_BJetness_avdca3d2t);
  oldtree->SetBranchAddress("BJetness_avdca3dno2t",&BJetness_avdca3dno2t,&b_BJetness_avdca3dno2t);
  oldtree->SetBranchAddress("BJetness_avdca3d",&BJetness_avdca3d,&b_BJetness_avdca3d);
  oldtree->SetBranchAddress("BJetness_avdca2d2t",&BJetness_avdca2d2t,&b_BJetness_avdca2d2t);
  oldtree->SetBranchAddress("BJetness_avdca2dno2t",&BJetness_avdca2dno2t,&b_BJetness_avdca2dno2t);
  oldtree->SetBranchAddress("BJetness_avdca2d",&BJetness_avdca2d,&b_BJetness_avdca2d);
  oldtree->SetBranchAddress("BJetness_chi2",&BJetness_chi2,&b_BJetness_chi2);
  oldtree->SetBranchAddress("BJetness_avip3d_val",&BJetness_avip3d_val,&b_BJetness_avip3d_val);
  oldtree->SetBranchAddress("BJetness_avip3d_sig",&BJetness_avip3d_sig,&b_BJetness_avip3d_sig);
  oldtree->SetBranchAddress("BJetness_avsip3d_val",&BJetness_avsip3d_val,&b_BJetness_avsip3d_val);
  oldtree->SetBranchAddress("BJetness_avsip3d_sig",&BJetness_avsip3d_sig,&b_BJetness_avsip3d_sig);
  oldtree->SetBranchAddress("BJetness_numip3dpos",&BJetness_numip3dpos,&b_BJetness_numip3dpos);
  oldtree->SetBranchAddress("BJetness_numip3dneg",&BJetness_numip3dneg,&b_BJetness_numip3dneg);
  oldtree->SetBranchAddress("BJetness_avip2d_val",&BJetness_avip2d_val,&b_BJetness_avip2d_val);
  oldtree->SetBranchAddress("BJetness_avip2d_sig",&BJetness_avip2d_sig,&b_BJetness_avip2d_sig);
  oldtree->SetBranchAddress("BJetness_avsip2d_val",&BJetness_avsip2d_val,&b_BJetness_avsip2d_val);
  oldtree->SetBranchAddress("BJetness_avsip2d_sig",&BJetness_avsip2d_sig,&b_BJetness_avsip2d_sig);
  oldtree->SetBranchAddress("BJetness_numip2dpos",&BJetness_numip2dpos,&b_BJetness_numip2dpos);
  oldtree->SetBranchAddress("BJetness_numip2dneg",&BJetness_numip2dneg,&b_BJetness_numip2dneg);
  oldtree->SetBranchAddress("BJetness_avip1d_val",&BJetness_avip1d_val,&b_BJetness_avip1d_val);
  oldtree->SetBranchAddress("BJetness_avip1d_sig",&BJetness_avip1d_sig,&b_BJetness_avip1d_sig);
  oldtree->SetBranchAddress("BJetness_avsip1d_val",&BJetness_avsip1d_val,&b_BJetness_avsip1d_val);
  oldtree->SetBranchAddress("BJetness_avsip1d_sig",&BJetness_avsip1d_sig,&b_BJetness_avsip1d_sig);
  oldtree->SetBranchAddress("BJetnessFV_isSingleLepton",&BJetnessFV_isSingleLepton,&b_BJetnessFV_isSingleLepton);
  oldtree->SetBranchAddress("BJetnessFV_isDoubleLepton",&BJetnessFV_isDoubleLepton,&b_BJetnessFV_isDoubleLepton);
  oldtree->SetBranchAddress("BJetnessFV_jetcsv",&BJetnessFV_jetcsv,&b_BJetnessFV_jetcsv);
  oldtree->SetBranchAddress("BJetnessFV_pfJetProbabilityBJetTags",&BJetnessFV_pfJetProbabilityBJetTags,&b_BJetnessFV_pfJetProbabilityBJetTags);
  oldtree->SetBranchAddress("BJetnessFV_pfCombinedMVAV2BJetTags",&BJetnessFV_pfCombinedMVAV2BJetTags,&b_BJetnessFV_pfCombinedMVAV2BJetTags);
  oldtree->SetBranchAddress("BJetnessFV_num_leps",&BJetnessFV_num_leps,&b_BJetnessFV_num_leps);
  oldtree->SetBranchAddress("BJetnessFV_npvTrkOVcollTrk",&BJetnessFV_npvTrkOVcollTrk,&b_BJetnessFV_npvTrkOVcollTrk);
  oldtree->SetBranchAddress("BJetnessFV_avip3d_val",&BJetnessFV_avip3d_val,&b_BJetnessFV_avip3d_val);
  oldtree->SetBranchAddress("BJetnessFV_avip3d_sig",&BJetnessFV_avip3d_sig,&b_BJetnessFV_avip3d_sig);
  oldtree->SetBranchAddress("BJetnessFV_avsip3d_sig",&BJetnessFV_avsip3d_sig,&b_BJetnessFV_avsip3d_sig);
  oldtree->SetBranchAddress("BJetnessFV_avip1d_sig",&BJetnessFV_avip1d_sig,&b_BJetnessFV_avip1d_sig);
  oldtree->SetBranchAddress("bWeight",&bWeight,&b_bWeight);
  oldtree->SetBranchAddress("bWeightLFup",&bWeightLFup,&b_bWeightLFup);
  oldtree->SetBranchAddress("bWeightLFdown",&bWeightLFdown,&b_bWeightLFdown);
  oldtree->SetBranchAddress("bWeightHFup",&bWeightHFup,&b_bWeightHFup);
  oldtree->SetBranchAddress("bWeightHFdown",&bWeightHFdown,&b_bWeightHFdown);
  oldtree->SetBranchAddress("bWeightHFStats1up",&bWeightHFStats1up,&b_bWeightHFStats1up);
  oldtree->SetBranchAddress("bWeightHFStats1down",&bWeightHFStats1down,&b_bWeightHFStats1down);
  oldtree->SetBranchAddress("bWeightLFStats1up",&bWeightLFStats1up,&b_bWeightLFStats1up);
  oldtree->SetBranchAddress("bWeightLFStats1down",&bWeightLFStats1down,&b_bWeightLFStats1down);
  oldtree->SetBranchAddress("bWeightHFStats2up",&bWeightHFStats2up,&b_bWeightHFStats2up);
  oldtree->SetBranchAddress("bWeightHFStats2down",&bWeightHFStats2down,&b_bWeightHFStats2down);
  oldtree->SetBranchAddress("bWeightLFStats2up",&bWeightLFStats2up,&b_bWeightLFStats2up);
  oldtree->SetBranchAddress("bWeightLFStats2down",&bWeightLFStats2down,&b_bWeightLFStats2down);
  oldtree->SetBranchAddress("bWeightCErr1up",&bWeightCErr1up,&b_bWeightCErr1up);
  oldtree->SetBranchAddress("bWeightCErr1down",&bWeightCErr1down,&b_bWeightCErr1down);
  oldtree->SetBranchAddress("bWeightCErr2up",&bWeightCErr2up,&b_bWeightCErr2up);
  oldtree->SetBranchAddress("bWeightCErr2down",&bWeightCErr2down,&b_bWeightCErr2down);
  oldtree->SetBranchAddress("bWeightJESup",&bWeightJESup,&b_bWeightJESup);
  oldtree->SetBranchAddress("bWeightJESdown",&bWeightJESdown,&b_bWeightJESdown);

  //OTHERS LOAD
  int HLT_IsoMu22=0;
  int HLT_IsoTkMu22=0;
  int HLT_Ele27_eta2p1_WPTight_Gsf=0;
  int ttHFCategory=0;
  int EVENT_event,EVENT_run,EVENT_lumiBlock;
  double EVENT_Q2tthbbWeightUp,EVENT_Q2tthbbWeightDown,EVENT_PDFtthbbWeightUp,EVENT_PDFtthbbWeightDown,PUWeight;
  double Met_type1PF_pt = -99.;
  double Met_type1PF_px = -99.;
  double Met_type1PF_py = -99.;
  double Met_type1PF_phi = -99.;
  vector<double>* Gen_pt=0;
  vector<double>* Gen_eta=0;
  vector<double>* Gen_phi=0;
  vector<int>* Gen_motherpdg_id=0;
  vector<int>* Gen_pdg_id=0;
  TBranch *b_HLT_IsoMu22,*b_HLT_IsoTkMu22,*b_HLT_Ele27_eta2p1_WPTight_Gsf,*b_ttHFCategory,*b_EVENT_event,*b_EVENT_run,*b_EVENT_lumiBlock,*b_Met_type1PF_pt,*b_Met_type1PF_px,*b_Met_type1PF_py,*b_Met_type1PF_phi,*b_EVENT_Q2tthbbWeightUp,*b_EVENT_Q2tthbbWeightDown,*b_EVENT_PDFtthbbWeightUp,*b_EVENT_PDFtthbbWeightDown,*b_PUWeight,*b_Gen_pt,*b_Gen_eta,*b_Gen_phi,*b_Gen_pdg_id,*b_Gen_motherpdg_id;
  if(sample==0 || sample==2){
    oldtree->SetBranchAddress("HLT_IsoMu22",&HLT_IsoMu22,&b_HLT_IsoMu22);
    oldtree->SetBranchAddress("HLT_IsoTkMu22",&HLT_IsoTkMu22,&b_HLT_IsoTkMu22);
    oldtree->SetBranchAddress("HLT_Ele27_eta2p1_WPTight_Gsf",&HLT_Ele27_eta2p1_WPTight_Gsf,&b_HLT_Ele27_eta2p1_WPTight_Gsf);
  }
  if(sample==1 || sample==2){
    oldtree->SetBranchAddress("ttHFCategory",&ttHFCategory,&b_ttHFCategory);
    oldtree->SetBranchAddress("Gen_pt",&Gen_pt,&b_Gen_pt);
    oldtree->SetBranchAddress("Gen_eta",&Gen_eta,&b_Gen_eta);
    oldtree->SetBranchAddress("Gen_phi",&Gen_phi,&b_Gen_phi);
    oldtree->SetBranchAddress("Gen_pdg_id",&Gen_pdg_id,&b_Gen_pdg_id);
    oldtree->SetBranchAddress("Gen_motherpdg_id",&Gen_motherpdg_id,&b_Gen_motherpdg_id);
  }
  oldtree->SetBranchAddress("EVENT_Q2tthbbWeightUp",&EVENT_Q2tthbbWeightUp,&b_EVENT_Q2tthbbWeightUp);
  oldtree->SetBranchAddress("EVENT_Q2tthbbWeightDown",&EVENT_Q2tthbbWeightDown,&b_EVENT_Q2tthbbWeightDown);
  oldtree->SetBranchAddress("EVENT_PDFtthbbWeightUp",&EVENT_PDFtthbbWeightUp,&b_EVENT_PDFtthbbWeightUp);
  oldtree->SetBranchAddress("EVENT_PDFtthbbWeightDown",&EVENT_PDFtthbbWeightDown,&b_EVENT_PDFtthbbWeightDown);
  oldtree->SetBranchAddress("EVENT_event",&EVENT_event,&b_EVENT_event);
  oldtree->SetBranchAddress("EVENT_run",&EVENT_run,&b_EVENT_run);
  oldtree->SetBranchAddress("EVENT_lumiBlock",&EVENT_lumiBlock,&b_EVENT_lumiBlock);
  oldtree->SetBranchAddress("PUWeight",&PUWeight,&b_PUWeight);
  oldtree->SetBranchAddress("Met_type1PF_pt",&Met_type1PF_pt,&b_Met_type1PF_pt);
  oldtree->SetBranchAddress("Met_type1PF_px",&Met_type1PF_px,&b_Met_type1PF_px);
  oldtree->SetBranchAddress("Met_type1PF_py",&Met_type1PF_py,&b_Met_type1PF_py);
  oldtree->SetBranchAddress("Met_type1PF_phi",&Met_type1PF_phi,&b_Met_type1PF_phi);

  Int_t nentries = (Int_t)oldtree->GetEntries();
  for (Int_t i=0;i<nentries; i++) {//if(i==100) break;
    cout << "Entry: " << i << endl;
    Long64_t tentry = oldtree->LoadTree(i);
    b_Muon_pt->GetEntry(tentry);
    b_Muon_eta->GetEntry(tentry);
    b_Muon_phi->GetEntry(tentry);
    b_Muon_energy->GetEntry(tentry);
    b_Muon_relIsoDeltaBetaR04->GetEntry(tentry);
    b_Muon_tight->GetEntry(tentry);
    b_patElectron_pt->GetEntry(tentry);
    b_patElectron_eta->GetEntry(tentry);
    b_patElectron_SCeta->GetEntry(tentry);
    b_patElectron_phi->GetEntry(tentry);
    b_patElectron_energy->GetEntry(tentry);
    b_patElectron_inCrack->GetEntry(tentry);
    b_patElectron_passConversionVeto->GetEntry(tentry);
    b_patElectron_isPassMvatrig->GetEntry(tentry);
    b_patElectron_isPassMvanontrig->GetEntry(tentry);
    b_patElectron_full5x5_sigmaIetaIeta->GetEntry(tentry);
    b_patElectron_hOverE->GetEntry(tentry);
    b_patElectron_ecalPFClusterIso->GetEntry(tentry);
    b_patElectron_hcalPFClusterIso->GetEntry(tentry);
    b_patElectron_isolPtTracks->GetEntry(tentry);
    b_patElectron_dEtaIn->GetEntry(tentry);
    b_patElectron_dPhiIn->GetEntry(tentry);
    b_patElectron_relIsoRhoEA->GetEntry(tentry);
    b_Jet_pt->GetEntry(tentry);
    b_Jet_Uncorr_pt->GetEntry(tentry);
    b_Jet_eta->GetEntry(tentry);
    b_Jet_phi->GetEntry(tentry);
    b_Jet_newpfCombinedInclusiveSecondaryVertexV2BJetTags->GetEntry(tentry);
    b_Jet_neutralHadEnergyFraction->GetEntry(tentry);
    b_Jet_chargedEmEnergyFraction->GetEntry(tentry);
    b_Jet_neutralEmEnergyFraction->GetEntry(tentry);
    b_Jet_numberOfConstituents->GetEntry(tentry);
    b_Jet_chargedHadronEnergyFraction->GetEntry(tentry);
    b_Jet_chargedMultiplicity->GetEntry(tentry);
    b_Jet_mass->GetEntry(tentry);
    b_Jet_JesSF->GetEntry(tentry);
    b_Jet_JerSF->GetEntry(tentry);
    b_Jet_JesSFup->GetEntry(tentry);
    b_Jet_JerSFup->GetEntry(tentry);
    b_Jet_JesSFdown->GetEntry(tentry);
    b_Jet_JerSFdown->GetEntry(tentry);
    b_BJetness_isSingleLepton->GetEntry(tentry);
    b_BJetness_isDoubleLepton->GetEntry(tentry);
    b_BJetness_ngenbh->GetEntry(tentry);
    b_BJetness_ngenbt->GetEntry(tentry);
    b_BJetness_ngenb->GetEntry(tentry);
    b_BJetness_ngenc->GetEntry(tentry);
    b_BJetness_partonFlavour->GetEntry(tentry);
    b_BJetness_hadronFlavour->GetEntry(tentry);
    b_BJetness_numjet->GetEntry(tentry);
    b_BJetness_jetpt->GetEntry(tentry);
    b_BJetness_jeteta->GetEntry(tentry);
    b_BJetness_jetphi->GetEntry(tentry);
    b_BJetness_jetenergy->GetEntry(tentry);
    b_BJetness_jetcsv->GetEntry(tentry);
    b_BJetness_pfJetProbabilityBJetTags->GetEntry(tentry);
    b_BJetness_pfCombinedMVAV2BJetTags->GetEntry(tentry);
    b_BJetness_pfCombinedCvsLJetTags->GetEntry(tentry);
    b_BJetness_pfCombinedCvsBJetTags->GetEntry(tentry);
    b_BJetness_pt->GetEntry(tentry);
    b_BJetness_eta->GetEntry(tentry);
    b_BJetness_phi->GetEntry(tentry);
    b_BJetness_en->GetEntry(tentry);
    b_BJetness_ptOVen->GetEntry(tentry);
    b_BJetness_jetschpvass->GetEntry(tentry);
    b_BJetness_jetschfrompv->GetEntry(tentry);
    b_BJetness_jetschip3dval->GetEntry(tentry);
    b_BJetness_jetschip3dsig->GetEntry(tentry);
    b_BJetness_jetschip2dval->GetEntry(tentry);
    b_BJetness_jetschip2dsig->GetEntry(tentry);
    b_BJetness_jetschisgoodtrk->GetEntry(tentry);
    b_BJetness_jetschtrkpur->GetEntry(tentry);
    b_BJetness_jetschpt->GetEntry(tentry);
    b_BJetness_jetschen->GetEntry(tentry);
    b_BJetness_num_pdgid_eles->GetEntry(tentry);
    b_BJetness_num_soft_eles->GetEntry(tentry);
    b_BJetness_num_vetonoipnoiso_eles->GetEntry(tentry);
    b_BJetness_num_loosenoipnoiso_eles->GetEntry(tentry);
    b_BJetness_num_veto_eles->GetEntry(tentry);
    b_BJetness_num_loose_eles->GetEntry(tentry);
    b_BJetness_num_medium_eles->GetEntry(tentry);
    b_BJetness_num_tight_eles->GetEntry(tentry);
    b_BJetness_num_mvatrig_eles->GetEntry(tentry);
    b_BJetness_num_mvanontrig_eles->GetEntry(tentry);
    b_BJetness_num_mvatrigwp90_eles->GetEntry(tentry);
    b_BJetness_num_mvanontrigwp90_eles->GetEntry(tentry);
    b_BJetness_num_heep_eles->GetEntry(tentry);
    b_BJetness_num_pdgid_mus->GetEntry(tentry);
    b_BJetness_num_loose_mus->GetEntry(tentry);
    b_BJetness_num_soft_mus->GetEntry(tentry);
    b_BJetness_num_medium_mus->GetEntry(tentry);
    b_BJetness_num_tight_mus->GetEntry(tentry);
    b_BJetness_num_highpt_mus->GetEntry(tentry);
    b_BJetness_num_POGisGood_mus->GetEntry(tentry);
    b_BJetness_numjettrks->GetEntry(tentry);
    b_BJetness_numjettrkspv->GetEntry(tentry);
    b_BJetness_numjettrksnopv->GetEntry(tentry);
  //  b_BJetness_npvTrkOVcollTrk->GetEntry(tentry);
    b_BJetness_pvTrkOVcollTrk->GetEntry(tentry);
    b_BJetness_npvTrkOVpvTrk->GetEntry(tentry);
    b_BJetness_npvPtOVcollPt->GetEntry(tentry);
    b_BJetness_pvPtOVcollPt->GetEntry(tentry);
    b_BJetness_npvPtOVpvPt->GetEntry(tentry);
    b_BJetness_avprel->GetEntry(tentry);
    b_BJetness_avppar->GetEntry(tentry);
    b_BJetness_avetarel->GetEntry(tentry);
    b_BJetness_avetapar->GetEntry(tentry);
    b_BJetness_avdr->GetEntry(tentry);
    b_BJetness_avpreljetpt->GetEntry(tentry);
    b_BJetness_avpreljeten->GetEntry(tentry);
    b_BJetness_avpparjetpt->GetEntry(tentry);
    b_BJetness_avpparjeten->GetEntry(tentry);
    b_BJetness_avnum2v->GetEntry(tentry);
    b_BJetness_avnumno2v->GetEntry(tentry);
    b_BJetness_avdca3d2t->GetEntry(tentry);
    b_BJetness_avdca3dno2t->GetEntry(tentry);
    b_BJetness_avdca3d->GetEntry(tentry);
    b_BJetness_avdca2d2t->GetEntry(tentry);
    b_BJetness_avdca2dno2t->GetEntry(tentry);
    b_BJetness_avdca2d->GetEntry(tentry);
    b_BJetness_chi2->GetEntry(tentry);
    b_BJetness_avip3d_val->GetEntry(tentry);
    b_BJetness_avip3d_sig->GetEntry(tentry);
    b_BJetness_avsip3d_val->GetEntry(tentry);
    b_BJetness_avsip3d_sig->GetEntry(tentry);
    b_BJetness_numip3dpos->GetEntry(tentry);
    b_BJetness_numip3dneg->GetEntry(tentry);
    b_BJetness_avip2d_val->GetEntry(tentry);
    b_BJetness_avip2d_sig->GetEntry(tentry);
    b_BJetness_avsip2d_val->GetEntry(tentry);
    b_BJetness_avsip2d_sig->GetEntry(tentry);
    b_BJetness_numip2dpos->GetEntry(tentry);
    b_BJetness_numip2dneg->GetEntry(tentry);
    b_BJetness_avip1d_val->GetEntry(tentry);
    b_BJetness_avip1d_sig->GetEntry(tentry);
    b_BJetness_avsip1d_val->GetEntry(tentry);
    b_BJetness_avsip1d_sig->GetEntry(tentry);
    b_BJetnessFV_isSingleLepton->GetEntry(tentry);
    b_BJetnessFV_isDoubleLepton->GetEntry(tentry);
    b_BJetnessFV_jetcsv->GetEntry(tentry);
    b_BJetnessFV_pfJetProbabilityBJetTags->GetEntry(tentry);
    b_BJetnessFV_pfCombinedMVAV2BJetTags->GetEntry(tentry);
    b_BJetnessFV_num_leps->GetEntry(tentry);
    b_BJetnessFV_npvTrkOVcollTrk->GetEntry(tentry);
    b_BJetnessFV_avip3d_val->GetEntry(tentry);
    b_BJetnessFV_avip3d_sig->GetEntry(tentry);
    b_BJetnessFV_avsip3d_sig->GetEntry(tentry);
    b_BJetnessFV_avip1d_sig->GetEntry(tentry);
    b_bWeight->GetEntry(tentry);
    b_bWeightLFup->GetEntry(tentry);
    b_bWeightLFdown->GetEntry(tentry);
    b_bWeightHFup->GetEntry(tentry);
    b_bWeightHFdown->GetEntry(tentry);
    b_bWeightHFStats1up->GetEntry(tentry);
    b_bWeightHFStats1down->GetEntry(tentry);
    b_bWeightLFStats1up->GetEntry(tentry);
    b_bWeightLFStats1down->GetEntry(tentry);
    b_bWeightHFStats2up->GetEntry(tentry);
    b_bWeightHFStats2down->GetEntry(tentry);
    b_bWeightLFStats2up->GetEntry(tentry);
    b_bWeightLFStats2down->GetEntry(tentry);
    b_bWeightCErr1up->GetEntry(tentry);
    b_bWeightCErr1down->GetEntry(tentry);
    b_bWeightCErr2up->GetEntry(tentry);
    b_bWeightCErr2down->GetEntry(tentry);
    b_bWeightJESup->GetEntry(tentry);
    b_bWeightJESdown->GetEntry(tentry);
    if(sample==0 || sample==2){
      b_HLT_IsoMu22->GetEntry(tentry);
      b_HLT_IsoTkMu22->GetEntry(tentry);
      b_HLT_Ele27_eta2p1_WPTight_Gsf->GetEntry(tentry);
    }
    if(sample==1 || sample==2){
      b_ttHFCategory->GetEntry(tentry);
      b_Gen_pt->GetEntry(tentry);
      b_Gen_eta->GetEntry(tentry);
      b_Gen_phi->GetEntry(tentry);
      b_Gen_pdg_id->GetEntry(tentry);
      b_Gen_motherpdg_id->GetEntry(tentry);
    }
    b_EVENT_Q2tthbbWeightUp->GetEntry(tentry);
    b_EVENT_Q2tthbbWeightDown->GetEntry(tentry);
    b_EVENT_PDFtthbbWeightUp->GetEntry(tentry);
    b_EVENT_PDFtthbbWeightDown->GetEntry(tentry);
    b_EVENT_event->GetEntry(tentry);
    b_EVENT_run->GetEntry(tentry);
    b_EVENT_lumiBlock->GetEntry(tentry);
    b_PUWeight->GetEntry(tentry);
    b_Met_type1PF_pt->GetEntry(tentry);
    b_Met_type1PF_px->GetEntry(tentry);
    b_Met_type1PF_py->GetEntry(tentry);
    b_Met_type1PF_phi->GetEntry(tentry);

    //ttH_hbb ttH_hcc ttH_hww ttH_hzz ttH_htt ttH_hgg ttH_hgluglu ttH_hzg
    int b_from_H = 0; int c_from_H = 0; int w_from_H = 0; int z_from_H = 0; int t_from_H = 0; int g_from_H = 0; int glu_from_H = 0;
    for (UInt_t j = 0; j < Gen_pt->size(); ++j) {
      //cout<<j<<" "<<Gen_pdg_id->at(j)<<" "<<Gen_motherpdg_id->at(j)<<" "<<Gen_pt->at(j)<<endl;
      if(abs(Gen_motherpdg_id->at(j))==25 && abs(Gen_pdg_id->at(j))== 5) b_from_H = b_from_H + 1;;
      if(abs(Gen_motherpdg_id->at(j))==25 && abs(Gen_pdg_id->at(j))== 4) c_from_H = c_from_H + 1;;
      if(abs(Gen_motherpdg_id->at(j))==25 && abs(Gen_pdg_id->at(j))==24) w_from_H = w_from_H + 1;;
      if(abs(Gen_motherpdg_id->at(j))==25 && abs(Gen_pdg_id->at(j))==23) z_from_H = z_from_H + 1;;
      if(abs(Gen_motherpdg_id->at(j))==25 && abs(Gen_pdg_id->at(j))==15) t_from_H = t_from_H + 1;;
      if(abs(Gen_motherpdg_id->at(j))==25 && abs(Gen_pdg_id->at(j))==22) g_from_H = g_from_H + 1;;
      if(abs(Gen_motherpdg_id->at(j))==25 && abs(Gen_pdg_id->at(j))==21) glu_from_H = glu_from_H + 1;;
    }
    int ttHGenCategory_def=0;
    if(b_from_H==2) ttHGenCategory_def=1;
    if(c_from_H==2) ttHGenCategory_def=2;
    if(w_from_H==2) ttHGenCategory_def=3;
    if(z_from_H==2) ttHGenCategory_def=4;
    if(t_from_H==2) ttHGenCategory_def=5;
    if(g_from_H==2) ttHGenCategory_def=6;
    if(glu_from_H==2) ttHGenCategory_def=7;
    if(z_from_H==1 && g_from_H==1) ttHGenCategory_def=8;
    ttHGenCategory_ = ttHGenCategory_def;

    //LEPTON SELECTION - MUON
    vector<double> SelMuon_pt,SelMuon_eta,SelMuon_phi,SelMuon_iso,SelMuon_energy;
    for (UInt_t j = 0; j < Muon_pt->size(); ++j){
      if(!(Muon_pt->at(j)>15))                     continue;
      if(!(fabs(Muon_eta->at(j))<2.4))             continue;
      if(!(Muon_tight->at(j)==1))                  continue;
      if(!(Muon_relIsoDeltaBetaR04->at(j)<0.25))   continue;
      SelMuon_pt.push_back(Muon_pt->at(j));
      SelMuon_eta.push_back(Muon_eta->at(j));
      SelMuon_phi.push_back(Muon_phi->at(j));
      SelMuon_energy.push_back(Muon_energy->at(j));
      SelMuon_iso.push_back(Muon_relIsoDeltaBetaR04->at(j));
    }
    //LEPTON SELECTION - ELECTRON
    vector<double> SelElectronMVA_pt,SelElectronMVA_eta,SelElectronMVA_phi,SelElectronMVA_energy,SelElectronMVA_iso;
    for (UInt_t j = 0; j < patElectron_pt->size(); ++j){
      if(!(patElectron_pt->at(j)>15))               continue;
      if(!(fabs(patElectron_eta->at(j))<2.4))	    continue;
      if(!(patElectron_inCrack->at(j)==0))	    continue;
      if(!(patElectron_isPassMvanontrig->at(j)==1)) continue;
      if(!(patElectron_relIsoRhoEA->at(j)<0.15))    continue;
      SelElectronMVA_pt.push_back(patElectron_pt->at(j));
      SelElectronMVA_eta.push_back(patElectron_eta->at(j));
      SelElectronMVA_phi.push_back(patElectron_phi->at(j));
      SelElectronMVA_energy.push_back(patElectron_energy->at(j));
      SelElectronMVA_iso.push_back(patElectron_relIsoRhoEA->at(j));
    }

    //JET SELECTION
    vector<double> SelJet_pt,SelJet_eta,SelJet_phi,SelJet_mass,SelJet_newpfCombinedInclusiveSecondaryVertexV2BJetTags;
    vector<double> SelTightJet_pt,SelTightJet_eta,SelTightJet_phi,SelTightJet_mass,SelTightJet_newpfCombinedInclusiveSecondaryVertexV2BJetTags;
    for (UInt_t j = 0; j < Jet_pt->size(); ++j){
      double jet_pt = Jet_Uncorr_pt->at(j)*Jet_JesSF->at(j)*Jet_JerSF->at(j);
      if(!(jet_pt>20)) continue;
      if(!(fabs(Jet_eta->at(j))<2.4)) continue;
      if(!(Jet_neutralHadEnergyFraction->at(j)<0.99)) continue;
      if(!(Jet_chargedEmEnergyFraction->at(j)<0.99)) continue;
      if(!(Jet_neutralEmEnergyFraction->at(j)<0.99)) continue;
      if(!(Jet_numberOfConstituents->at(j)>1)) continue;
      if(!(Jet_chargedHadronEnergyFraction->at(j)>0.0)) continue;
      if(!(Jet_chargedMultiplicity->at(j)>0.0)) continue;
      bool deltaRJetLepBoolean = true;
      for (UInt_t k = 0; k < SelMuon_pt.size(); ++k){
        float deltaEta = SelMuon_eta[k]-Jet_eta->at(j);
        float deltaPhi = fabs(SelMuon_phi[k]-Jet_phi->at(j));
        if(deltaPhi>TMath::Pi()) deltaPhi = 2*TMath::Pi() - deltaPhi;
        if(sqrt(deltaEta*deltaEta + deltaPhi*deltaPhi)<0.4) deltaRJetLepBoolean=false;
      }
      for (UInt_t k = 0; k < SelElectronMVA_pt.size(); ++k){
        float deltaEta = SelElectronMVA_eta[k]-Jet_eta->at(j);
        float deltaPhi = fabs(SelElectronMVA_phi[k]-Jet_phi->at(j));
        if(deltaPhi>TMath::Pi()) deltaPhi = 2*TMath::Pi() - deltaPhi;
        if(sqrt(deltaEta*deltaEta + deltaPhi*deltaPhi)<0.4) deltaRJetLepBoolean=false;
      }
      if(deltaRJetLepBoolean){
        SelJet_pt.push_back(Jet_pt->at(j));
        SelJet_eta.push_back(Jet_eta->at(j));
        SelJet_phi.push_back(Jet_phi->at(j));
        SelJet_mass.push_back(Jet_mass->at(j));
        SelJet_newpfCombinedInclusiveSecondaryVertexV2BJetTags.push_back(Jet_newpfCombinedInclusiveSecondaryVertexV2BJetTags->at(j));
        if(!(jet_pt>30)) continue;
        SelTightJet_pt.push_back(Jet_pt->at(j));
        SelTightJet_eta.push_back(Jet_eta->at(j));
        SelTightJet_phi.push_back(Jet_phi->at(j));
        SelTightJet_mass.push_back(Jet_mass->at(j));
        SelTightJet_newpfCombinedInclusiveSecondaryVertexV2BJetTags.push_back(Jet_newpfCombinedInclusiveSecondaryVertexV2BJetTags->at(j));
      }
    }
    int nBCSVM_DL = 0;
    for(UInt_t k = 0; k < SelJet_pt.size(); ++k){
      if(SelJet_newpfCombinedInclusiveSecondaryVertexV2BJetTags[k]>0.80) nBCSVM_DL=nBCSVM_DL+1;
    }
    int nBCSVM_SL = 0;
    for(UInt_t k = 0; k < SelTightJet_pt.size(); ++k){
      if(SelTightJet_newpfCombinedInclusiveSecondaryVertexV2BJetTags[k]>0.80) nBCSVM_SL=nBCSVM_SL+1;
    }

    //EVENT SELECTION - tth SKIMMING
    bool MUON = false;
    bool ELECTRON = false;
    if(sample==0 || sample==2){
      if(SelMuon_pt.size()==1&&SelMuon_pt[0]>25&&fabs(SelMuon_eta[0])<2.1&&SelMuon_iso[0]<0.15&&(HLT_IsoMu22==1 || HLT_IsoTkMu22==1)) MUON=true;
      if(SelElectronMVA_pt.size()==1&&SelElectronMVA_pt[0]>30&&fabs(SelElectronMVA_eta[0])<2.1&&HLT_Ele27_eta2p1_WPTight_Gsf==1) ELECTRON=true;
    } else {
      if(SelMuon_pt.size()==1&&SelMuon_pt[0]>25&&fabs(SelMuon_eta[0])<2.1&&SelMuon_iso[0]<0.15) MUON=true;
      if(SelElectronMVA_pt.size()==1&&SelElectronMVA_pt[0]>30&&fabs(SelElectronMVA_eta[0])<2.1) ELECTRON=true;
    }
    if(!(MUON || ELECTRON)) continue;
    if(!(SelMuon_pt.size()+SelElectronMVA_pt.size()==1)) continue;
    if(!(SelTightJet_pt.size()>=4&&nBCSVM_SL>=2)) continue;
    if(!((SelTightJet_pt.size()>5&&nBCSVM_SL>1) || (SelTightJet_pt.size()==5&&nBCSVM_SL>3) || (SelTightJet_pt.size()==4&&nBCSVM_SL>2))) continue;

    //BDT VARIABLES
    std::vector<TLorentzVector> selectedLeptonP4;
    std::vector<TLorentzVector> selectedJetP4;
    std::vector<double> selectedJetCSV;
    std::vector<TLorentzVector> looseSelectedJetP4;
    std::vector<double> looseSelectedJetCSV;
    for(unsigned int j=0; j<SelMuon_pt.size(); j++){
      TLorentzVector prov; prov.SetPtEtaPhiE(SelMuon_pt[j],SelMuon_eta[j],SelMuon_phi[j],SelMuon_energy[j]);
      selectedLeptonP4.push_back(prov);
    }
    for(unsigned int j=0; j<SelElectronMVA_pt.size(); j++){
      TLorentzVector prov; prov.SetPtEtaPhiE(SelElectronMVA_pt[j],SelElectronMVA_eta[j],SelElectronMVA_phi[j],SelElectronMVA_energy[j]);
      selectedLeptonP4.push_back(prov);
    }
    for(unsigned int j=0; j<SelTightJet_pt.size(); j++){
      TLorentzVector prov; prov.SetPtEtaPhiM(SelTightJet_pt[j],SelTightJet_eta[j],SelTightJet_phi[j],SelTightJet_mass[j]);
      selectedJetP4.push_back(prov);
      selectedJetCSV.push_back(SelTightJet_newpfCombinedInclusiveSecondaryVertexV2BJetTags[j]);
    }
    for(unsigned int j=0; j<SelJet_pt.size(); j++){
      TLorentzVector prov; prov.SetPtEtaPhiM(SelJet_pt[j],SelJet_eta[j],SelJet_phi[j],SelJet_mass[j]);
      looseSelectedJetP4.push_back(prov);
      looseSelectedJetCSV.push_back(SelJet_newpfCombinedInclusiveSecondaryVertexV2BJetTags[j]);
    }
    TLorentzVector metP4;metP4.SetPtEtaPhiM(Met_type1PF_pt,0.,Met_type1PF_phi,0.);
    std::vector<double> selectedJetCSV_fixed;
    for(unsigned int j=0; j<selectedJetCSV.size(); j++){
      double tag=selectedJetCSV[j];
      if (tag<0)     tag=-.1;
      else if(tag>1) tag=1.;
      selectedJetCSV_fixed.push_back(tag);
    }
    vector<double> sortedCSV=selectedJetCSV_fixed;
    std::sort(sortedCSV.begin(),sortedCSV.end());
    std::vector<TLorentzVector> selectedTaggedJetP4;
    for(unsigned int j=0; j<selectedJetP4.size(); j++){
      if(selectedJetCSV_fixed[j]>0.80){
        selectedTaggedJetP4.push_back(selectedJetP4[j]);
      }
    }
    double Aplanarity=-99.;
    double Sphericity=-99.;
    getSp(selectedLeptonP4[0],metP4,selectedJetP4,Aplanarity,Sphericity);
    double H0,H1,H2,H3,H4;
    getFox(selectedJetP4,H0,H1,H2,H3,H4);
    double minChi,dRbb;
    TLorentzVector bjet1,bjet2;
    double bestHiggsMass = getBestHiggsMass(selectedLeptonP4[0],metP4,selectedJetP4,selectedJetCSV_fixed,minChi,dRbb,bjet1,bjet2,looseSelectedJetP4,looseSelectedJetCSV);
    TLorentzVector dummy_metv;
    double minChiStudy, chi2lepW, chi2leptop, chi2hadW, chi2hadtop, mass_lepW, mass_leptop, mass_hadW, mass_hadtop, dRbbStudy, testquant1, testquant2, testquant3, testquant4, testquant5, testquant6, testquant7;
    TLorentzVector b1,b2;
    vector< vector<double> > jets_vvdouble;
    for(auto jet=selectedJetP4.begin();jet!=selectedJetP4.end(); jet++){
      vector<double> pxpypzE;
      pxpypzE.push_back(jet->Px());
      pxpypzE.push_back(jet->Py());
      pxpypzE.push_back(jet->Pz());
      pxpypzE.push_back(jet->E());
      jets_vvdouble.push_back(pxpypzE);
    }
    study_tops_bb_syst(metP4.Pt(), metP4.Phi(), dummy_metv, selectedLeptonP4[0], jets_vvdouble, selectedJetCSV_fixed, minChiStudy, chi2lepW, chi2leptop, chi2hadW, chi2hadtop, mass_lepW, mass_leptop, mass_hadW, mass_hadtop, dRbbStudy, testquant1, testquant2, testquant3, testquant4, testquant5, testquant6, testquant7, b1, b2);
    double DEta_fn=testquant6;
    double pt_E_ratio = pt_E_ratio_jets(jets_vvdouble);
    double jet_jet_etamax = get_jet_jet_etamax (jets_vvdouble);
    double jet_tag_etamax = get_jet_tag_etamax (jets_vvdouble,selectedJetCSV_fixed);
    double tag_tag_etamax = get_tag_tag_etamax (jets_vvdouble,selectedJetCSV_fixed);
    double sum_pt_jets=0;
    double Dr_between_lep_and_closest_jet=99;
    double mht_px=0;
    double mht_py=0;
    TLorentzVector p4_of_everything=selectedLeptonP4[0];
    p4_of_everything+=metP4;
    for(auto jetvec = selectedJetP4.begin() ; jetvec != selectedJetP4.end(); ++jetvec){
      if(Dr_between_lep_and_closest_jet>selectedLeptonP4[0].DeltaR(*jetvec)) Dr_between_lep_and_closest_jet=selectedLeptonP4[0].DeltaR(*jetvec);
      sum_pt_jets += jetvec->Pt();
      mht_px += jetvec->Px();
      mht_py += jetvec->Py();
      p4_of_everything += *jetvec;
    }
    mht_px+=selectedLeptonP4[0].Px();
    mht_py+=selectedLeptonP4[0].Py();
    double mass_of_everything=p4_of_everything.M();
    double sum_pt_wo_met=sum_pt_jets+selectedLeptonP4[0].Pt();
    double sum_pt_with_met=metP4.Pt()+sum_pt_wo_met;
    double mht=sqrt( mht_px*mht_px + mht_py*mht_py );
    double mlb=0;
    double minDr_for_Mlb=999.;
    for(auto tagged_jet=selectedTaggedJetP4.begin();tagged_jet!=selectedTaggedJetP4.end();tagged_jet++){
      double drLep=selectedLeptonP4[0].DeltaR(*tagged_jet);
      if(drLep<minDr_for_Mlb){
	minDr_for_Mlb=drLep;
	TLorentzVector prov = (selectedLeptonP4[0]+*tagged_jet);
	mlb=prov.M();
      }
    }
    double Closest_tagged_dijet_mass=-99;
    double minDrTagged=99;
    double sumDrTagged=0;
    int npairs=0;
    double Tagged_dijet_mass_closest_to_125=-99;
    for(auto tagged_jet1=selectedTaggedJetP4.begin();tagged_jet1!=selectedTaggedJetP4.end();tagged_jet1++){
      for(auto tagged_jet2=tagged_jet1+1;tagged_jet2!=selectedTaggedJetP4.end();tagged_jet2++){
	double dr=tagged_jet1->DeltaR(*tagged_jet2);
	TLorentzVector prov = (*tagged_jet1+*tagged_jet2);
	double m = prov.M();
	sumDrTagged+=dr;
	npairs++;
	if(dr<minDrTagged){
	  minDrTagged=dr;
	  Closest_tagged_dijet_mass=m;
	}
	if(fabs(Tagged_dijet_mass_closest_to_125-125)>fabs(m-125)){
	  Tagged_dijet_mass_closest_to_125=m;
	}
      }
    }
    double avgDrTagged=-1;
    if(npairs!=0) avgDrTagged=sumDrTagged/npairs;
    double m3 = -1.;
    double maxpt_for_m3=-1;
    for(auto itJetVec1 = selectedJetP4.begin() ; itJetVec1 != selectedJetP4.end(); ++itJetVec1){
      for(auto itJetVec2 = itJetVec1+1 ; itJetVec2 != selectedJetP4.end(); ++itJetVec2){
	for(auto itJetVec3 = itJetVec2+1 ; itJetVec3 != selectedJetP4.end(); ++itJetVec3){
	  TLorentzVector m3vec = *itJetVec1 + *itJetVec2 + *itJetVec3;
	  if(m3vec.Pt() > maxpt_for_m3){
	    maxpt_for_m3 = m3vec.Pt();
	    m3 = m3vec.M();
	  }
	}
      }
    }
    double detaJetsAverage = 0;
    int nPairsJets = 0;
    for(auto itJetVec1 = selectedJetP4.begin() ; itJetVec1 != selectedJetP4.end(); ++itJetVec1){
      for(auto itJetVec2 = itJetVec1+1 ; itJetVec2 != selectedJetP4.end(); ++itJetVec2){
	detaJetsAverage += fabs(itJetVec1->Eta()-itJetVec2->Eta());
	nPairsJets++;
      }
    }
    if(nPairsJets > 0){
      detaJetsAverage /= (double) nPairsJets;
    }
    double averageCSV_tagged = 0;
    double averageCSV_all = 0;
    double Lowest_btag=99;
    int njets=selectedJetP4.size();
    int ntags=0;
    for(auto itCSV = selectedJetCSV_fixed.begin() ; itCSV != selectedJetCSV_fixed.end(); ++itCSV){
      if(*itCSV>0) averageCSV_all += *itCSV;
      if(*itCSV<0.80) continue;
      if(*itCSV<Lowest_btag) Lowest_btag=*itCSV;
      if(*itCSV>0) averageCSV_tagged += *itCSV;
      ntags++;
    }
    if(ntags>0) averageCSV_tagged /= ntags;
    else averageCSV_tagged=0;
    if(selectedJetCSV_fixed.size()>0) averageCSV_all /= selectedJetCSV_fixed.size();
    else averageCSV_all=0;
    if(Lowest_btag>90) Lowest_btag=-1;
    double csvDev = 0;
    for(auto itCSV = selectedJetCSV_fixed.begin() ; itCSV != selectedJetCSV_fixed.end(); ++itCSV){
      if(*itCSV<0.80) continue;
      csvDev += pow(*itCSV - averageCSV_tagged,2);
    }
    if(ntags>0) csvDev /= ntags;
    else csvDev=-1.;
    double Fifth_highest_CSV=njets>4?sortedCSV[njets-5]:-1.;
    double First_jet_pt=selectedJetP4.size()>0?selectedJetP4[0].Pt():-99;
    double Fourth_highest_btag=njets>3?sortedCSV[njets-4]:-1.;
    double Fourth_jet_pt=selectedJetP4.size()>3?selectedJetP4[3].Pt():-99;
    double met=metP4.Pt();
    double Second_highest_btag=njets>1?sortedCSV[njets-2]:-1.;
    double Second_jet_pt=selectedJetP4.size()>1?selectedJetP4[1].Pt():-99;
    double Third_highest_btag=njets>2?sortedCSV[njets-3]:-1.;
    double Third_jet_pt=selectedJetP4.size()>2?selectedJetP4[2].Pt():-99;
    //cout<<H0<<" "<<H1<<" "<<H2<<" "<<H3<<" "<<Aplanarity<<" "<<Sphericity<<" "<<bestHiggsMass<<" "<<DEta_fn<<" "<<pt_E_ratio<<" "<<jet_jet_etamax<<" "<<jet_tag_etamax<<" "<<tag_tag_etamax<<" "<<Dr_between_lep_and_closest_jet<<" "<<sum_pt_with_met<<" "<<mht<<" "<<mass_of_everything<<" "<<detaJetsAverage<<" "<<m3<<" "<<avgDrTagged<<" "<<minDrTagged<<" "<<averageCSV_tagged<<" "<<Closest_tagged_dijet_mass<<" "<<csvDev<<" "<<sum_pt_jets<<" "<<Lowest_btag<<" "<<averageCSV_all<<" "<<Fifth_highest_CSV<<" "<<First_jet_pt<<" "<<Fourth_highest_btag<<" "<<Fourth_jet_pt<<" "<<met<<" "<<mlb<<" "<<Second_highest_btag<<" "<<Second_jet_pt<<" "<<Tagged_dijet_mass_closest_to_125<<" "<<Third_highest_btag<<" "<<Third_jet_pt<<endl;

    //btagging likelihood ratio
    double eth_blr=-1;
    std::vector<unsigned int> out_best_perm;
    double out_P_4b=-1;
    double out_P_2b=-1;
    eth_blr=mem.GetBTagLikelihoodRatio(selectedJetP4,selectedJetCSV,out_best_perm,out_P_4b,out_P_2b);

    //BDT
    BlrBDTClassifier bdt;
    int bJetness_num_soft_leps = 1;//(int)BJetness_num_soft_leps[0];
    double bJetness_avip3d_val = 0.2;//BJetness_avip3d_val[0];
    auto result = bdt.GetBDTOutput(
				   selectedLeptonP4,
				   selectedJetP4,
				   selectedJetCSV,
				   looseSelectedJetP4,
				   looseSelectedJetCSV,
				   metP4,
				   eth_blr
				   //bJetness_num_soft_leps,
				   //bJetness_avip3d_val
				   );
    //std::cout << "================================" << std::endl;
    //std::cout << "bdtoutput=" << result << std::endl;
    //std::cout << "bdtcategory=" << bdt.GetCategoryOfLastEvaluation() << ": "<<SelTightJet_pt.size()<<" "<<nBCSVM_SL<<endl;

    if(!(eth_blr>=0.0 || eth_blr<0.0)) eth_blr=0.001;
    eth_blr = TMath::Log(eth_blr/(1-eth_blr));

    //NEW VARIABLES
    if(sample!=0){
      newPUWeight(PUWeight, puweight_,puweightUP_,puweightDOWN_);
      if(MUON==true && ELECTRON==false){
        type_ = 0;
        Muon_IDSFval_ = Muon_IDSF( SelMuon_pt[0],SelMuon_eta[0]).first;
        Muon_IsoSFval_= Muon_IsoSF(SelMuon_pt[0],SelMuon_eta[0]).first;
        Muon_TrkSFval_= Muon_TrkSF(SelMuon_eta[0]).first;
        Muon_IDSFerr_ = Muon_IDSF( SelMuon_pt[0],SelMuon_eta[0]).second;
        Muon_IsoSFerr_= Muon_IsoSF(SelMuon_pt[0],SelMuon_eta[0]).second;
        Muon_TrkSFerr_= Muon_TrkSF(SelMuon_eta[0]).second;
        Electron_IDSFval_ = 1.0;
        Electron_IDSFerr_ = 0.0;
        Electron_GsfSFval_= 1.0;
        Electron_GsfSFerr_= 0.0;
      } else if(MUON==false && ELECTRON==true){
        type_ = 1;
        Muon_IDSFval_ = 1.0;
        Muon_IsoSFval_= 1.0;
        Muon_TrkSFval_= 1.0;
        Muon_IDSFerr_ = 0.0;
        Muon_IsoSFerr_= 0.0;
        Muon_TrkSFerr_= 0.0;
        Electron_IDSFval_ = Electron_ID(SelElectronMVA_pt[0],SelElectronMVA_eta[0]).first;
        Electron_IDSFerr_ = Electron_ID(SelElectronMVA_pt[0],SelElectronMVA_eta[0]).second;
        Electron_GsfSFval_= Electron_GsfSF(SelElectronMVA_eta[0]).first;
        Electron_GsfSFerr_= Electron_GsfSF(SelElectronMVA_eta[0]).second;
      } else {
        type_ = 2;
      }
    } else {
      Muon_IDSFval_ = 1.0;
      Muon_IsoSFval_= 1.0;
      Muon_TrkSFval_= 1.0;
      Muon_IDSFerr_ = 0.0;
      Muon_IsoSFerr_= 0.0;
      Muon_TrkSFerr_= 0.0;
      Electron_IDSFval_ = 1.0;
      Electron_IDSFerr_ = 0.0;
      Electron_GsfSFval_= 1.0;
      Electron_GsfSFerr_= 0.0;
      if(MUON==true && ELECTRON==false)      type_ = 0;
      else if(MUON==false && ELECTRON==true) type_ = 1;
      else                                   type_ = 2;
    }
    BDT_ = result;
    NumberOfJets_  = SelTightJet_pt.size();
    NumberOfBJets_ = nBCSVM_SL;
    all_sum_pt_with_met_ = sum_pt_with_met;
    aplanarity_ = Aplanarity;
    avg_btag_disc_btags_ = averageCSV_tagged;
    avg_dr_tagged_jets_ = avgDrTagged;
    best_higgs_mass_ = bestHiggsMass;
    closest_tagged_dijet_mass_ = Closest_tagged_dijet_mass;
    dEta_fn_ = DEta_fn;
    dev_from_avg_disc_btags_ = csvDev;
    dr_between_lep_and_closest_jet_ = Dr_between_lep_and_closest_jet;
    fifth_highest_CSV_ = Fifth_highest_CSV;
    first_jet_pt_ = First_jet_pt;
    fourth_highest_btag_ = Fourth_highest_btag;
    fourth_jet_pt_ = Fourth_jet_pt;
    h0_ = H0;
    h1_ = H1;
    h2_ = H2;
    h3_ = H3;
    HT_ = sum_pt_jets;
    invariant_mass_of_everything_ = mass_of_everything;
    lowest_btag_ = Lowest_btag;
    M3_ = m3;
    maxeta_jet_jet_ = jet_jet_etamax;
    maxeta_jet_tag_ = jet_tag_etamax;
    maxeta_tag_tag_ = tag_tag_etamax;
    min_dr_tagged_jets_ = minDrTagged;
    MET_ = metP4.Pt();
    MHT_ = mht;
    Mlb_ = mlb;
    pt_all_jets_over_E_all_jets_ = pt_E_ratio;
    second_highest_btag_ = Second_highest_btag;
    second_jet_pt_ = Second_jet_pt;
    sphericity_ = Sphericity;
    tagged_dijet_mass_closest_to_125_ = Tagged_dijet_mass_closest_to_125;
    third_highest_btag_ = Third_highest_btag;
    third_jet_pt_ = Third_jet_pt;
    Evt_CSV_Average_ = averageCSV_all;
    Evt_Deta_JetsAverage_ = detaJetsAverage;
    blr_ = eth_blr;

    newtree->Fill();
  }
  newtree->Print();
  newtree->AutoSave();
  newevtree->Print();
  newevtree->AutoSave();
  delete oldfile;
  delete newfile;
  t.Stop();
  t.Print();
}
