#include "../interface/newPUWeight.h"

PUWTool::PUWTool(){
  return;
}
PUWTool::~PUWTool(){
  return;
}

void PUWTool::newPUWeight(double PUWeight_, double &puweight,double &puweightUP,double &puweightDOWN){
  double *npuProbs = 0;
  unsigned int nPUMax = 50;
  double npu_mix_2016_25ns_SpringMC_PUScenarioV1[50] = {0.000829312873542,0.00124276120498,0.00339329181587,0.00408224735376,0.00383036590008,0.00659159288946,0.00816022734493,0.00943640833116,0.0137777376066,0.017059392038,0.0213193035468,0.0247343174676,0.0280848773878,0.0323308476564,0.0370394341409,0.0456917721191,0.0558762890594,0.0576956187107,0.0625325287017,0.0591603758776,0.0656650815128,0.0678329011676,0.0625142146389,0.0548068448797,0.0503893295063,0.040209818868,0.0374446988111,0.0299661572042,0.0272024759921,0.0219328403791,0.0179586571619,0.0142926728247,0.00839941654725,0.00522366397213,0.00224457976761,0.000779274977993,0.000197066585944,7.16031761328e-05,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
  npuProbs = npu_mix_2016_25ns_SpringMC_PUScenarioV1;
  TFile file("/afs/cern.ch/work/j/jthomasw/private/IHEP/CMSSW_8_0_24_patch1/src/BSMFramework/BSM3G_TNT_Maker/data/PUReweight/PileUpReweighting2016.root", "READ");
  TH1* h = NULL;
  file.GetObject("pileup",h);
  h->SetDirectory(0);
  file.Close();
  std::vector<double> result(nPUMax,0.);
  double s = 0.;
  TFile fileNEW(  "SF/MyDataPileupHistogram_69200.root",   "READ");
  TFile fileNEWUp("SF/MyDataPileupHistogram_69200Up.root", "READ");
  TFile fileNEWDo("SF/MyDataPileupHistogram_69200Do.root", "READ");
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
