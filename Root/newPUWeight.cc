#include "../interface/newPUWeight.h"

PUWTool::PUWTool(){
  return;
}
PUWTool::~PUWTool(){
  return;
}

void PUWTool::newPUWeight(double PUWeight_, double &puweight,double &puweightUP,double &puweightDOWN){
  double *npuProbs = 0;

  //------------ Moriond17 PU SCENARIO ------------//
  unsigned int nPUMax = 0;
  nPUMax =75;//# of bins in pile-up distributions
  //Array of normalisation factors for MC.
  double npu_Moriond17Scenario[nPUMax] = {1.78653e-05 ,2.56602e-05 ,5.27857e-05 ,8.88954e-05 ,0.000109362 ,0.000140973 ,0.000240998 ,0.00071209 ,0.00130121 ,0.00245255 ,0.00502589 ,0.00919534 ,0.0146697 ,0.0204126 ,0.0267586 ,0.0337697 ,0.0401478 ,0.0450159 ,0.0490577 ,0.0524855 ,0.0548159 ,0.0559937 ,0.0554468 ,0.0537687 ,0.0512055 ,0.0476713 ,0.0435312 ,0.0393107 ,0.0349812 ,0.0307413 ,0.0272425 ,0.0237115 ,0.0208329 ,0.0182459 ,0.0160712 ,0.0142498 ,0.012804 ,0.011571 ,0.010547 ,0.00959489 ,0.00891718 ,0.00829292 ,0.0076195 ,0.0069806 ,0.0062025 ,0.00546581 ,0.00484127 ,0.00407168 ,0.00337681 ,0.00269893 ,0.00212473 ,0.00160208 ,0.00117884 ,0.000859662 ,0.000569085 ,0.000365431 ,0.000243565 ,0.00015688 ,9.88128e-05 ,6.53783e-05 ,3.73924e-05 ,2.61382e-05 ,2.0307e-05 ,1.73032e-05 ,1.435e-05 ,1.36486e-05 ,1.35555e-05 ,1.37491e-05 ,1.34255e-05 ,1.33987e-05 ,1.34061e-05 ,1.34211e-05 ,1.34177e-05 ,1.32959e-05 ,1.33287e-05};
  npuProbs = npu_Moriond17Scenario;
  //------------------------------------------//

  // Data pileup histogram for current Moriond17 dataset 14th Feb 2017.
  TFile file("/publicfs/cms/user/joshuha/ttHACIHEP/SF/PileUpReweightingMoriond17.root", "READ");
  //TFile file("SF/PileUpReweightingMoriond17.root", "READ");
  TH1* h = NULL;
  file.GetObject("pileup",h);
  h->SetDirectory(0);
  file.Close();
  std::vector<double> result(nPUMax,0.);
  double s = 0.;
  // Histograms used to scale to new lumi (in case Data increases and dont want to run all MC again to get one weight.)
  // Currently, nominal the same as PileUpReweightingMoriond17.root
  // Systematic variations scale luminosity up/down by 5%
  // New histograms can be calculated using procedure on twiki:
  //              https://twiki.cern.ch/twiki/bin/view/CMS/PileupJSONFileforData#Pileup_JSON_Files_For_Run_II
  TFile fileNEW("/publicfs/cms/user/joshuha/ttHACIHEP/SF/MyDataPileupHistogram_69200.root", "READ");
  TFile fileNEWUp("/publicfs/cms/user/joshuha/ttHACIHEP/SF/MyDataPileupHistogram_69200Up.root", "READ");
  TFile fileNEWDo("/publicfs/cms/user/joshuha/ttHACIHEP/SF/MyDataPileupHistogram_69200Do.root", "READ");
  //TFile fileNEW("SF/MyDataPileupHistogram_69200.root", "READ");
  //TFile fileNEWUp("SF/MyDataPileupHistogram_69200Up.root", "READ");
  //TFile fileNEWDo("SF/MyDataPileupHistogram_69200Do.root", "READ");
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
    result[npu] = npu_estimated / npuProbs[npu]; // Data (un-normalised) / MC (normalised)
    s += npu_estimated;//Cumulative sum of all bins in data pileup
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
    result[npu] /= s;// Normalises data.
    resultNEW[npu]   /= sNEW;
    resultNEWUp[npu] /= sNEWUp;
    resultNEWDo[npu] /= sNEWDo;
    if(result[npu]==PUWeight_) NPU = npu;
  }
  puweight     = resultNEW[NPU];
  puweightUP   = resultNEWUp[NPU];
  puweightDOWN = resultNEWDo[NPU];
}
