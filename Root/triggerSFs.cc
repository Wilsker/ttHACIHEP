
#include "../interface/triggerSFs.h"

triggerSFs::triggerSFs(){
  return;
}

triggerSFs::~triggerSFs(){
  return;
}

double triggerSFs::getEventTriggerSF(bool is_el, bool is_mu, double lepton_pt, double lepton_eta){
  double trigger_SF_value = 1;
  int X;
  int Y;
  int X1;
  int Y1;
  int X2;
  int Y2;

  if(is_el==true && is_mu==false){
    const char* eleTrigSF_file_path = "/publicfs/cms/user/joshuha/ttHACIHEP/SF/eleTrig_SF.root";
    //const char* eleTrigSF_file_path = "SF/eleTrig_SF.root";
    const char* eleTrigSF_hist_path =  "h_eleTrig_SF";
    TFile* eleTrigSF_file = TFile::Open(eleTrigSF_file_path);
    if(!eleTrigSF_file->IsOpen()) throw "Error : Unable to open electron trigger SF file!";
    TH1F* eleTrig_SF_hist = (TH1F*)eleTrigSF_file->Get(eleTrigSF_hist_path);

    //Find the pt bin into which a lepton of the given pt would fall.
    X = -99;
    Y = -99;
    for(int i =1; i<eleTrig_SF_hist->GetXaxis()->GetNbins()+2; i++){
      //If lepton pt lower than current bins low edge, use previous bin.
      if(lepton_pt<eleTrig_SF_hist->GetXaxis()->GetBinLowEdge(i)){
        X=i-1;
        break;
      }
    }
    //If electron pt is larger than the largest bin assign to the highest bin.
    if(lepton_pt>=eleTrig_SF_hist->GetXaxis()->GetBinLowEdge(eleTrig_SF_hist->GetXaxis()->GetNbins()+1)){
      X=(eleTrig_SF_hist->GetXaxis()->GetNbins()+1)-1;
    }

    //Find the eta bin into which a lepton of given eta would fall.
    for(int i = 1; i<eleTrig_SF_hist->GetYaxis()->GetNbins()+2; i++){
      //If lepton eta lower than current bins low edge, use previous bin.
      if(lepton_eta<eleTrig_SF_hist->GetYaxis()->GetBinLowEdge(i)){
        Y=i-1;
        break;
      }
    }
    trigger_SF_value = eleTrig_SF_hist->GetBinContent(X,Y);
    eleTrigSF_file->Close();
  }

  else if(is_mu==true && is_el==false){
    const char* muonTrigSF_file_path = "/publicfs/cms/user/joshuha/ttHACIHEP/SF/SingleMuonTrigger_Z_RunCD_Reco76X_Feb15.root";

    //const char* muonTrigSF_file_path = "SF/SingleMuonTrigger_Z_RunCD_Reco76X_Feb15.root";
    const char* muonTrigSF_hist_path1 = "runD_IsoMu20_OR_IsoTkMu20_HLTv4p2_PtEtaBins/pt_abseta_ratio";
    const char* muonTrigSF_hist_path2 = "runD_IsoMu20_OR_IsoTkMu20_HLTv4p3_PtEtaBins/pt_abseta_ratio";
    TFile* muonTrigSF_file = TFile::Open(muonTrigSF_file_path);
    if(!muonTrigSF_file->IsOpen()) throw "Error: Unable to open muon trigger SF file 1";
    TH1F* muonTrigSF_hist1 = (TH1F*)muonTrigSF_file->Get(muonTrigSF_hist_path1);
    TH1F* muonTrigSF_hist2 = (TH1F*)muonTrigSF_file->Get(muonTrigSF_hist_path2);




    //Find the pt bin into which a lepton of the given pt would fall.
    X1 = -99;
    Y1 = -99;
    for(int i = 1; i < muonTrigSF_hist1->GetXaxis()->GetNbins()+2; i++){
      if(lepton_pt<muonTrigSF_hist1->GetXaxis()->GetBinLowEdge(i)){
        X1 = i-1;
        break;
      }
    }
    if(lepton_pt>=muonTrigSF_hist1->GetXaxis()->GetBinLowEdge(muonTrigSF_hist1->GetXaxis()->GetNbins()+1)){
      X1=(muonTrigSF_hist1->GetXaxis()->GetNbins()+1)-1;
    }
    //Find the eta bin into which a lepton of given eta would fall.
    for(int i = 1; i < muonTrigSF_hist1->GetYaxis()->GetNbins()+2; i++){
      if(fabs(lepton_eta)<muonTrigSF_hist1->GetYaxis()->GetBinLowEdge(i)){
        Y1 = i-1;
        break;
      }
    }




    //Find the pt bin into which a lepton of the given pt would fall.
    X2 = -99;
    Y2 = -99;
    for(int i = 1; i < muonTrigSF_hist2->GetXaxis()->GetNbins()+2; i++){
      if(lepton_pt<muonTrigSF_hist2->GetXaxis()->GetBinLowEdge(i)){
        X2 = i-1;
        break;
      }
    }
    if(lepton_pt>=muonTrigSF_hist2->GetXaxis()->GetBinLowEdge(muonTrigSF_hist2->GetXaxis()->GetNbins()+1)){
      X2=(muonTrigSF_hist2->GetXaxis()->GetNbins()+1)-1;
    }
    //Find the eta bin into which a lepton of given eta would fall.
    for(int i = 1; i < muonTrigSF_hist2->GetYaxis()->GetNbins()+2; i++){
      if(fabs(lepton_eta)<muonTrigSF_hist2->GetYaxis()->GetBinLowEdge(i)){
        Y2 = i-1;
        break;
      }
    }



    trigger_SF_value = (717.3*muonTrigSF_hist1->GetBinContent(X1,Y1)+1808.5*muonTrigSF_hist2->GetBinContent(X2,Y2))/(717.3+1808.5);
    muonTrigSF_file->Close();
  }

  return trigger_SF_value;

}
