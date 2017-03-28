/**
This Macro
1. Plots variables in stack mode with the option to compare them with data
It also allows the option to normalise them to 1

Need to specify
1. See Declare Constants
2. In double_h_var you have to specify if you want to access 1d or vector variables
3. datatype in the main
Make also sure that the data sample is the last one in the list const char *samples[]
Usually the signal comes just before the data sample
4. You will have to custimazie color, aesthetics, and style according to the specific plot needs
5. Remember that if doasym is true
. v = n-1: where v is in bin[v] and n is inasymbin[n]
. the range must be the same between [inRange-endRange] e asymbin
*/
/////
//   To run: root -l StackPlots.cc+
/////
/////
//   Prepare Root and Roofit
/////
#include "TFile.h"
#include "TH1F.h"
#include "THStack.h"
#include "TLegend.h"
#include "TTree.h"
#include "TTreePlayer.h"
#include "TStyle.h"
#include "TGaxis.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TEfficiency.h"
#include "TBranch.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <map>
using namespace std;
/////
//   Declare constants
/////

// ==== Path ====
//const string path       = "/afs/cern.ch/work/f/fromeo/public/TTHbb/8012/";
string path = "";

// ==== Samples ====
//const char *samples[]   = {"TT_2b","TT_bb","TT_b","TT_cc","TT_lf", "ttHTobb", "SLep"};
/*const char *samples[]   = {"/MC/ttjets_incl/ttjets_incl_Merged_rootplas",
"/MC/ttHbb/ttHbb_Merged_rootplas",
"/DATA/combined_SLep"};*/
const char *samples[]   = {"ttjets_incl/ttjets_incl_Merged_rootplas", "ttHbb/ttHbb_Merged_rootplas", "combined_SLep"};

// ==== Selection ====
const string selection  = "_SL"; //_SingleEle, _SingleMu
const bool nodata       = false;  //You must always comment data in samples if you don't want it
const bool show_ratio   = true;

// ==== Weights ====
const double Luminosity = 3590; //pb^-1
const bool   LumiNorm   = false;
const bool   PUcorr     = false;
const bool   SF         = true; //For the TTHbb analysis it represents the bWeight factor
const double scale      = 0;    //0 means no scaling; any other values means scale histo by the value of scale

// ===== Normalisation of plots =====
// One must run the script once with "normalised = false" to get the value for the background normalisation.
const bool normalised   = false;
const double normbkg    = 2.1549e+06; //normbkg and normdata values have to be taken after 1 iteration of the macro with normalised = false
const double normdata   = 126940;
const double normsig    = 376858;

// ===== Plots =====
const bool save_plots   = true;
const bool show_title   = true;
const bool doasym       = false;
const double asymbin[6] = {0,3,4,9,15,20};
const int    numVar     = 40;
//const int logYscale[numVar] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const int logYscale[numVar] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
const int    col_size   = 500; //>= highest bin

//Variables
const unsigned int ini_var = 0;
const unsigned int fin_var = 5;
const int posvtcr          = 0;
const char *variables[]         = {
  //"BJetness_num_vetonoipnoiso_leps", "BJetness_num_soft_leps", "BJetness_num_pdgid_leps", "BJetness_num_loosenoipnoiso_leps", "BJetness_npvTrkOVcollTrk", "BJetness_pvTrkOVcollTrk", "BJetness_numjettrksnopv", "BJetness_avsip3d_sig", "BJetness_avip3d_val", "BJetness_avip3d_sig", "BJetness_avip1d_sig", "nBestVtx"
  "BJetness_avsip3dsig", "BJetness_avip3dval", "BJetness_avip3dsig", "BJetness_avip1dsig", "BJetness_numleps", "BJetness_jetpt0"
};
const char *titleXaxis[]        = {
  //"Num Veto Lep", "Num Soft Lep", "Num pdgid Lep", "Num loose Lep", "npvTrkOVcollTrk", "pvTrkOVcollTrk", "Num of not PV tracks", "Average Signed IP 3D Sig", "Average IP 3D Val", "Average IP 3D Sig", "Average IP 1D Sig", "Vertices"
  "Average Signed IP 3D Sig", "Average IP 3D Val", "Average IP 3D Sig", "Average IP 1D Sig", "Number of Leptons", "0th Jet Pt"
};
const int    bin[numVar]        = {
  //8, 8, 8, 8, 7, 7, 15, 45, 20, 40, 40, 30
  45, 20, 40, 40, 5, 40
};
const double inRange[numVar]    = {
  //0, 0, 0, 0, 0, 0, 0, -10, 0, 0, 0, 0
  -10, 0, 0, 0, 0, 0
};
const double endRange[numVar]   = {
  //8, 8, 8, 8, 1.5, 1.5, 15, 80, 1, 80, 100, 30
  80, 1, 80, 100, 5, 300
};
/*
const char *variables[]         = {
"BJetness_numjet", "BJetness_numjettrks", "BJetness_numjettrkspv", "BJetness_numjettrksnopv", "BJetness_npvTrkOVcollTrk", "BJetness_npvTrkOVcollTrk", "BJetness_pvTrkOVcollTrk", "BJetness_npvTrkOVpvTrk", "BJetness_npvPtOVcollPt", "BJetness_pvPtOVcollPt", "BJetness_npvPtOVpvPt", "BJetness_avnum2v", "BJetness_avnumno2v", "BJetness_avdca3d", "BJetness_avdca2d", "BJetness_chi2", "BJetness_avip3d_val", "BJetness_avip3d_sig", "BJetness_avip2d_val", "BJetness_avip2d_sig", "BJetness_avip1d_val", "BJetness_avip1d_sig", "BJetness_avsip3d_val", "BJetness_avsip3d_sig", "BJetness_avsip2d_val", "BJetness_avsip2d_sig", "BJetness_avsip1d_val", "BJetness_avsip1d_sig", "BJetness_num_pdgid_leps", "BJetness_num_soft_leps", "BJetness_num_vetonoipnoiso_leps", "BJetness_num_loosenoipnoiso_leps"
};
const char *titleXaxis[]        = {
"BJetness_numjet", "BJetness_numjettrks", "BJetness_numjettrkspv", "BJetness_numjettrksnopv", "BJetness_npvTrkOVcollTrk", "BJetness_npvTrkOVcollTrk", "BJetness_pvTrkOVcollTrk", "BJetness_npvTrkOVpvTrk", "BJetness_npvPtOVcollPt", "BJetness_pvPtOVcollPt", "BJetness_npvPtOVpvPt", "BJetness_avnum2v", "BJetness_avnumno2v", "BJetness_avdca3d", "BJetness_avdca2d", "BJetness_chi2", "BJetness_avip3d_val", "BJetness_avip3d_sig", "BJetness_avip2d_val", "BJetness_avip2d_sig", "BJetness_avip1d_val", "BJetness_avip1d_sig", "BJetness_avsip3d_val", "BJetness_avsip3d_sig", "BJetness_avsip2d_val", "BJetness_avsip2d_sig", "BJetness_avsip1d_val", "BJetness_avsip1d_sig", "BJetness_num_pdgid_leps", "BJetness_num_soft_leps", "BJetness_num_vetonoipnoiso_leps", "BJetness_num_loosenoipnoiso_leps"
};
const int    bin[numVar]        = {
5, 60, 50, 15, 10, 10, 50, 20, 20, 50, 20, 20, 20, 50, 50, 50, 50, 50, 50, 50, 50, 50, 100, 100, 100, 100, 100, 100, 5, 5, 5, 5
};
const double inRange[numVar]    = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.1, -10, -0.1, -10, -0.1, -10, 0, 0, 0, 0
};
const double endRange[numVar]   = {
20, 60, 50, 15, 1, 1, 2, 1, 1, 1, 1, 1, 1, 100, 100, 100, 0.5, 100, 0.5, 100, 0.5, 100, 0.1, 30, 0.1, 30, 0.1, 30, 5, 5, 5, 5
};
*/


/////
//   Declare functions
/////
TFile* Call_TFile(string rootpla);
TH1F* double_h_var(unsigned int v, string var, string vaT, uint i, string rootplas, double err_AllBkg[][col_size], double ent_AllBkg[][col_size], int datatype);
void draw_plots(TCanvas* c1, TH1F* h_sum_var, THStack* hstack, TH1F* h_data_var, TH1F* h_sig, TLegend* leg, double err_AllBkg[][col_size], double ent_AllBkg[][col_size], uint rootplas_size, int v, string var, string vartitle, double highestbinval);
void draw_lines(double x1, double y1, double x2, double y2);
int      get_col(string name);
double   get_highestbinval(TH1F* h_data_var, TH1F* h_sig, THStack* hstack, int v);
void save_canvas(TCanvas* c1, string var);
TLegend* get_legend();
TH1F*    get_th1f(string var, int v);
TH1F*    get_datath1f(string var, string title, int v);
void setTDRStyle();




/////
//   Main function
/////
void StackPlots(){
  cout << "Running StackPlots()" << endl;
  cout << "setTDRStyle()" << endl;
  setTDRStyle();
  cout << "var()" << endl;

  //Loop over all variables
  vector<string> var(variables, variables + sizeof(variables)/sizeof(variables[0]));
  cout << "varTitleXaxis()" << endl;
  vector<string> varTitleXaxis(titleXaxis, titleXaxis + sizeof(titleXaxis)/sizeof(titleXaxis[0]));
  cout << "loop over variables" << endl;
  for(uint v=ini_var; v<fin_var; v++){
    cout<<var[v]<<endl;
    //Declare legend and histograms
    cout << "get_legend()" << endl;
    TLegend *leg = get_legend();

    //MC
    double bkgstackintegral = 0.;
    THStack* hstack = new THStack("hstack","hstack");
    cout << "get_th1f()" << endl;
    TH1F* h_sum_var = get_th1f(var[v], v); h_sum_var->Sumw2();
    TH1F* h_sig     = get_th1f(var[v], v);
    //Data
    cout << "get_datath1f()" << endl;
    TH1F* h_data_var = get_datath1f(var[v], varTitleXaxis[v], v); h_data_var->Sumw2();

    //Loop over samples
    cout << "rootplas()" << endl;
    vector<string> rootplas(samples, samples + sizeof(samples)/sizeof(samples[0]));
    const uint rootplas_size = rootplas.size();
    cout << "rootplas_size = " << rootplas_size << endl;
    double err_AllBkg[rootplas_size][col_size];
    double ent_AllBkg[rootplas_size][col_size];
    for(uint i=0; i<rootplas_size; i++) for(int j=0; j<bin[v]; j++) err_AllBkg[i][j] = 0.;
    for(uint i=0; i<rootplas_size; i++) for(int j=0; j<bin[v]; j++) ent_AllBkg[i][j] = 0.;
    for(uint i=0; i<rootplas_size; i++){
      int datatype = -999;
      cout << "rootplas[i] = " << rootplas[i] << endl;
      if(rootplas[i].find("ttHbb_Merged") != std::string::npos){
        datatype=1;
      }
      else if(rootplas[i].find("combined_SLep")!= std::string::npos){
        datatype=0;
      }
      else{
        datatype=2;
      }

      if(datatype!=0){
        path = "/publicfs/cms/data/TopQuark/ttHbb/JTW/2017_03/ttHACIHEP/output/MC/";
      }
      else{
        path = "/publicfs/cms/data/TopQuark/ttHbb/JTW/2017_03/ttHACIHEP/output/DATA/";
      }

  cout << "Data type = " << datatype << endl;

  //Declare histograms for variables
  TH1F *h_var = get_th1f(var[v], v);

  cout << "Selecting type of variables . . . . "<< endl;
  //Choose type of variables
  if(datatype==2){       h_var  = double_h_var(v,var[v],varTitleXaxis[v],i,rootplas[i],err_AllBkg,ent_AllBkg,datatype);
  }else if(datatype==1){ h_sig  = double_h_var(v,var[v],varTitleXaxis[v],i,rootplas[i],err_AllBkg,ent_AllBkg,datatype);
  }else{                 h_data_var = double_h_var(v,var[v],varTitleXaxis[v],i,rootplas[i],err_AllBkg,ent_AllBkg,datatype);}

  if(datatype==2){
    cout << "Background MC" << endl;

    //Put histos in the hstack
    int col = get_col(rootplas[i]);

    if(rootplas[i].find("ttjets_incl") != std::string::npos){
      cout << "ttjets_incl sample found"<< endl;
      h_var->SetMarkerColor(kRed+col);
      h_var->SetFillColor(kRed+col);
      h_var->SetLineColor(kRed+col);
    }else{
      cout << "Other bckg sample found"<< endl;
      h_var->SetMarkerColor(kCyan+col);
      h_var->SetFillColor(kCyan+col);
      h_var->SetLineColor(kCyan+col);
    }

    cout << "Adding background to stack" << endl;
    hstack->Add(h_var);

    // Create nickname for sample on plots (used in legend)
    string bckg_mc_nickname = "";
    string full_samplename = string(rootplas[i]);
    if(full_samplename.find("/")!=std::string::npos){bckg_mc_nickname = full_samplename.substr(0,full_samplename.find("/"));}
    else{
      cout << "Could not find  in sample name. Using deafult name for legend!" << endl;
      bckg_mc_nickname = "<Default bckg name>";
    }
    cout << "bckg_mc_nickname = " << bckg_mc_nickname << endl;

    leg->AddEntry(h_var,bckg_mc_nickname.c_str(),"F");

    //Sum them for the error
    h_sum_var->Add(h_sum_var,h_var);
    cout<<setw(5)<<"Evt"<<setw(15)<<rootplas[i]<<setw(15)<<h_var->Integral()<<endl;
    bkgstackintegral += h_var->Integral();
  }else if(datatype==0){
    cout<<setw(5)<<"Evt"<<setw(15)<<rootplas[i]<<setw(15)<<h_data_var->Integral()<<endl;
  }
}
cout<<setw(5)<<"Evt"<<setw(15)<<"Bkg"<<setw(15)<<bkgstackintegral<<endl;
cout<<setw(5)<<"Evt"<<setw(15)<<"Sig"<<setw(15)<<h_sig->Integral()<<endl;
cout << "Drawing histpogram to canvas"<< endl;



//Draw
double highestbinval = get_highestbinval(h_data_var,h_sig,hstack,v);
TCanvas* c1 = new TCanvas(var[v].c_str(),var[v].c_str(),200,200,700,600);
draw_plots(c1,h_sum_var,hstack,h_data_var,h_sig,leg,err_AllBkg,ent_AllBkg,rootplas_size,v,var[v],varTitleXaxis[v],highestbinval);
save_canvas(c1,var[v]);
}
}


/////
//   Call TFile to be read
/////
TFile* Call_TFile(string rootpla){
  string file_name = path+rootpla+".root";
  cout << "Call_TFile() : " << file_name << endl;
  TFile* f = new TFile(file_name.c_str(),"update");
  return f;
}



/////
//   Fill histo with double type
/////
TH1F* double_h_var(unsigned int v, string var, string varT, uint i, string rootplas, double err_AllBkg[][col_size], double ent_AllBkg[][col_size], int datatype){
  //Call tree and variables
  cout << "double_h_var()"<< endl;
  //cout << "Open file: " << rootplas << endl;
  TFile* f = Call_TFile(rootplas); TTree *tree; f->GetObject("BOOM",tree);
  //vector <double> * curr_var;
  //curr_var = 0;
  //TBranch *b_curr_var = 0;
  //tree->SetBranchAddress(var.c_str(),&curr_var,&b_curr_var);
  cout << "Getting variable: " << var.c_str() << endl;
  double curr_var;
  TBranch *b_curr_var = 0;
  tree->SetBranchAddress(var.c_str(),&curr_var,&b_curr_var);
  cout << "Getting variable: " << "PUWeight" << endl;
  double PUWeight;
  TBranch *b_PUWeight = 0;
  tree->SetBranchAddress("PUWeight",&PUWeight,&b_PUWeight);

  Float_t lumiweight;
  TBranch *b_lumiweight = 0;
  if(datatype!=0){
    cout << "Getting variable: " << "lumiweight" << endl;
    tree->SetBranchAddress("lumiweight",&lumiweight,&b_lumiweight);
  }
  else{lumiweight=1;}


  cout << "Getting variable: " << "bWeight" << endl;
  double bWeight;
  TBranch *b_bWeight = 0;
  tree->SetBranchAddress("bWeight",&bWeight,&b_bWeight);
  /*
  double HLT_Ele27_eta2p1_WPTight_Gsf;
  TBranch *b_HLT_Ele27_eta2p1_WPTight_Gsf = 0;
  tree->SetBranchAddress("HLT_Ele27_eta2p1_WPTight_Gsf",&HLT_Ele27_eta2p1_WPTight_Gsf,&b_HLT_Ele27_eta2p1_WPTight_Gsf);
  double HLT_IsoMu22;
  TBranch *b_HLT_IsoMu22 = 0;
  tree->SetBranchAddress("HLT_IsoMu22",&HLT_IsoMu22,&b_HLT_IsoMu22);
  double HLT_IsoTkMu22;
  TBranch *b_HLT_IsoTkMu22 = 0;
  tree->SetBranchAddress("HLT_IsoTkMu22",&HLT_IsoTkMu22,&b_HLT_IsoTkMu22);
  */
  //Fill histo
  TH1F *hist = get_th1f(var, v);
  hist->SetTitle(0); hist->SetMarkerStyle(8); hist->SetMarkerColor(1); hist->SetLineColor(1);
  TH1F *hist_err;
  if(var=="BJetness_num_vetonoipnoiso_leps" && doasym) hist_err = new TH1F("hist_err","hist_err",bin[v],asymbin);
  else                         hist_err = new TH1F("hist_err","hist_err",bin[v],inRange[v],endRange[v]);
  hist_err->Sumw2();

  for(int j=0; j<tree->GetEntries(); j++)
  //for(int j=0; j<10; j++)
  {
    double w = 1.;
    Long64_t tentry = tree->LoadTree(j);
    b_curr_var->GetEntry(tentry);
    b_PUWeight->GetEntry(tentry);
    b_bWeight->GetEntry(tentry);
    //b_HLT_Ele27_eta2p1_WPTight_Gsf->GetEntry(tentry);
    //b_HLT_IsoMu22->GetEntry(tentry);
    //b_HLT_IsoTkMu22->GetEntry(tentry);
    //if(!(HLT_Ele27_eta2p1_WPTight_Gsf==1 || HLT_IsoMu22==1 || HLT_IsoTkMu22==1)) continue;
    if(datatype!=0){
      //Float_t lumiweight;
      //TBranch *b_lumiweight = 0;
      //tree->SetBranchAddress("lumiweight",&lumiweight,&b_lumiweight);
      //b_lumiweight->GetEntry(tentry);
      if(LumiNorm) w = w*lumiweight*Luminosity;
      if(PUcorr)   w = w*PUWeight;
      if(SF)       w = w*bWeight;
      if(scale!=0) w = w*scale;
      if(inRange[v]<curr_var && curr_var<endRange[v]){hist->Fill(curr_var,w);         hist_err->Fill(curr_var,w*w);}
      if(curr_var>=endRange[v])                      {hist->Fill(0.99*endRange[v],w); hist_err->Fill(0.99*endRange[v],w*w);}
      if(curr_var<=inRange[v])                       {hist->Fill(1.01*inRange[v],w);  hist_err->Fill(1.01*inRange[v],w*w);}
      //if(inRange[v]<curr_var->at(posvtcr) && curr_var->at(posvtcr)<endRange[v]){hist->Fill(curr_var->at(posvtcr),w);hist_err->Fill(curr_var->at(posvtcr),w*w);}
      //if(curr_var->at(posvtcr)>=endRange[v])                                   {hist->Fill(0.99*endRange[v],w); hist_err->Fill(0.99*endRange[v],w*w);}
      //if(curr_var->at(posvtcr)<=inRange[v])                                    {hist->Fill(1.01*inRange[v],w);  hist_err->Fill(1.01*inRange[v],w*w);}
    }else{
      if(inRange[v]<curr_var && curr_var<endRange[v]) hist->Fill(curr_var);
      if(curr_var>=endRange[v])                       hist->Fill(0.99*endRange[v]);
      if(curr_var<=inRange[v])                        hist->Fill(1.01*inRange[v]);
      //if(inRange[v]<curr_var->at(posvtcr) && curr_var->at(posvtcr)<endRange[v]) hist->Fill(curr_var->at(posvtcr));
      //if(curr_var->at(posvtcr)>=endRange[v])                                    hist->Fill(0.99*endRange[v]);
      //if(curr_var->at(posvtcr)<=inRange[v])                                     hist->Fill(1.01*inRange[v]);
    }
  }
  //Get errors, normalise
  if(normalised){
    if(datatype==0) hist->Scale(1/normdata);
    if(datatype==1) hist->Scale(1/normsig);
    if(datatype==2) hist->Scale(1/normbkg);
  }
  if(datatype==2){
    for(int j=0; j<bin[v]; j++){
      ent_AllBkg[i][j] = hist->GetBinContent(j+1);
      err_AllBkg[i][j] = sqrt(hist_err->GetBinContent(j+1));
      if(normalised)   err_AllBkg[i][j] = err_AllBkg[i][j]/normbkg;
    }
  }
  delete tree;
  return hist;
}


void draw_plots(TCanvas* c1, TH1F* h_sum_var, THStack* hstack, TH1F* h_data_var, TH1F* h_sig, TLegend* leg, double err_AllBkg[][col_size], double ent_AllBkg[][col_size], uint rootplas_size, int v, string var, string vartitle, double highestbinval){
  //Canvas
  if(logYscale[v]==1) c1->SetLogy();
  if(show_ratio){
    //Bottom plot
    TPad *c1_1 = new TPad("c1_1", "newpad",0.01,0.01,0.99,0.32);
    c1_1->Draw();
    c1_1->cd();
    c1_1->SetTopMargin(0.01);
    c1_1->SetBottomMargin(0.4);
    c1_1->SetRightMargin(0.01);
    c1_1->SetLeftMargin(0.125);
    //c1_1->SetFillStyle(0);

    //Get values
    double dataSUmc_x[bin[v]]; double dataSUmc_y[bin[v]]; double dataSUmc_xerr[bin[v]]; double dataSUmc_yerr[bin[v]];
    for(int j=0; j<bin[v]; j++){
      dataSUmc_x[j] = 0; dataSUmc_y[j] = 0; dataSUmc_xerr[j] = 0; dataSUmc_yerr[j] = 0;
      dataSUmc_x[j] = h_sum_var->GetBinCenter(j+1);  dataSUmc_xerr[j] = 0;
      double mc_err = 0;
      for(uint i=0; i<rootplas_size; i++) mc_err += err_AllBkg[i][j]*err_AllBkg[i][j];
      if(h_sum_var->GetBinContent(j+1)!=0){
        double rd = h_data_var->GetBinContent(j+1);
        double mc = h_sum_var->GetBinContent(j+1);
        dataSUmc_y[j]    = rd/mc;
        if(normalised){
          rd = rd*normdata;
          mc = mc*normbkg;
        }
        dataSUmc_yerr[j] = sqrt(pow(sqrt(rd)/mc,2) + pow((rd*sqrt(mc_err))/(mc*mc),2));
      }else{
        dataSUmc_y[j]    = -1000000;
        dataSUmc_yerr[j] = 0;
      }
    }
    if(var=="massVis"){
      cout<<"Variables is "<<var<<endl;
      cout<<"const double sf[bin] = {";
      for(int ar=0; ar<bin[v]; ar++) cout<<dataSUmc_y[ar]<<",";
      cout<<"};"<<endl;
      cout<<"const double sferr[bin] = {";
      for(int ar=0; ar<bin[v]; ar++) cout<<dataSUmc_yerr[ar]<<",";
      cout<<"};"<<endl;
    }
    //Plot values
    TGraphErrors *dataSUmc = new TGraphErrors(bin[v], dataSUmc_x, dataSUmc_y, dataSUmc_xerr, dataSUmc_yerr);
    dataSUmc->SetTitle(0);
    //dataSUmc->SetTitleSize(10);
    dataSUmc->SetMarkerStyle(7);
    //dataSUmc->SetMarkerColor(1);
    //dataSUmc->SetLineColor(1);
    dataSUmc->GetXaxis()->SetRangeUser(inRange[v],endRange[v]);
    dataSUmc->GetXaxis()->SetTitle(vartitle.c_str());
    dataSUmc->GetXaxis()->SetTitleSize(0.2);
    dataSUmc->GetYaxis()->SetTitle("Data/MC");
    dataSUmc->GetYaxis()->SetLabelSize(0.075);
    dataSUmc->GetYaxis()->SetTitleSize(0.15);
    dataSUmc->GetYaxis()->SetTitleOffset(0.35);
    dataSUmc->SetMinimum(0.75);
    dataSUmc->SetMaximum(1.25);
    dataSUmc->GetXaxis()->SetRangeUser(inRange[v],endRange[v]);
    dataSUmc->GetXaxis()->SetLimits(inRange[v],endRange[v]);
    dataSUmc->Draw("APZ");
    dataSUmc->GetXaxis()->SetRangeUser(inRange[v],endRange[v]);
    dataSUmc->GetXaxis()->SetLimits(inRange[v],endRange[v]);
    TLine* line = new TLine(inRange[v],1,endRange[v],1);
    line->SetLineColor(kRed);
    line->SetLineWidth(2);
    line->Draw("same");
    //Top plots
    c1->cd();
    TPad *c1_2 = new TPad("c1_2", "newpad",0.01,0.33,0.99,0.99);
    if(logYscale[v]==1) c1_2->SetLogy();
    c1_2->Draw();
    c1_2->cd();
    c1_2->SetTopMargin(0.065);
    c1_2->SetBottomMargin(0.1);
    c1_2->SetRightMargin(0.01);
    c1_2->SetLeftMargin(0.125);
    //c1_2->SetFillStyle(0);
  }
  //Set maximum Y value
  if(logYscale[v]==0){
    h_data_var->SetMaximum(highestbinval+0.25*highestbinval);
    if(normalised) h_data_var->SetMinimum(0.0001);
  }else{
    h_data_var->SetMaximum((highestbinval+0.25*highestbinval)*10);
    if(normalised) h_data_var->SetMinimum(0.0001);
  }
  //Data and bkg
  TGaxis::SetMaxDigits(4);
  stringstream Title_ss;
  Title_ss << "#scale[0.90]{CMS preliminary,   #sqrt{s} = 13 TeV, L = " << (Luminosity/1000) <<" fb^{-1}}";
  //string Title_s = Title_ss.string();
  cout << "Title_ss.str() = " << Title_ss.str() << endl;
  string Title_str = Title_ss.str();
  const char* Plot_Title = Title_str.c_str();
  cout << "Plot_Title = " << Plot_Title << endl;
  if(!show_ratio) h_data_var->GetXaxis()->SetTitle(vartitle.c_str());
  if(show_title)  h_data_var->SetTitle(Plot_Title);
  if(h_data_var->GetEntries()==0) gStyle->SetOptStat(0);

  h_data_var->Draw("P");
  //hstack->SetMarkerStyle(6);
  //hstack->SetMarkerColor(2);
  hstack->Draw("same");

  if(!normalised) h_data_var->Draw("PEsame");
  else            h_data_var->Draw("Psame");

  gPad->RedrawAxis();
  h_sig->SetLineWidth(2);
  h_sig->SetLineColor(kGreen+4);
  h_sig->Draw("same");
  if(!(h_data_var->GetEntries()==0)) leg->AddEntry(h_data_var,"data","P");
  if(!(h_sig->GetEntries()==0))      leg->AddEntry(h_sig,"TTHbb","L");
  leg->Draw();


  //Bkg err
  double all_bkg_statErr_x[bin[v]]; double all_bkg_statErr_y[bin[v]]; double all_bkg_statErr_xerr[bin[v]]; double all_bkg_statErr_yerr[bin[v]];
  for(int j=0; j<bin[v]; j++){
    all_bkg_statErr_x[j] = 0; all_bkg_statErr_y[j] = 0; all_bkg_statErr_xerr[j] = 0; all_bkg_statErr_yerr[j] = 0;
    all_bkg_statErr_x[j] = h_sum_var->GetBinCenter(j+1);
    if(var=="BJetness_num_vetonoipnoiso_leps" && doasym){
      all_bkg_statErr_xerr[j] = (asymbin[j+1]-asymbin[j])*0.5;
    }else{
      all_bkg_statErr_xerr[j] = ((endRange[v]-inRange[v])/bin[v])*0.5;
    }
    for(uint i=0; i<rootplas_size; i++) all_bkg_statErr_yerr[j] += err_AllBkg[i][j]*err_AllBkg[i][j];
    all_bkg_statErr_y[j] = h_sum_var->GetBinContent(j+1); all_bkg_statErr_yerr[j] = sqrt(all_bkg_statErr_yerr[j]);
  }
  TGraphErrors *all_bkg_statErr = new TGraphErrors(bin[v], all_bkg_statErr_x, all_bkg_statErr_y, all_bkg_statErr_xerr, all_bkg_statErr_yerr);
  all_bkg_statErr->SetLineColor(kGray+3);
  all_bkg_statErr->SetFillStyle(3004);
  all_bkg_statErr->SetFillColor(kGray+3);
  all_bkg_statErr->Draw("E2same");
}
void draw_lines(double x1, double y1, double x2, double y2){
  TLine* line1 = new TLine(x1,y1,x2,y2);
  line1->SetLineColor(kRed);
  line1->SetLineWidth(3);
  line1->Draw("same");
}





int get_col(string name){
  int col;
  if(name=="VV")    col = -10;  //Non main bkg need a different color type
  if(name=="VJets") col = -9;   //Possibly with soft intensity
  if(name=="tW")    col = -8;
  if(name=="TT")    col = -10;
  if(name=="TT_lf") col = -10; //For main bkg uses same color type with different numbers
  if(name=="TT_cc") col = -8;  //darker for lower bkg (that should come first)
  if(name=="TT_b")  col = -6;  //softer for higher bkg (that should come after)
  if(name=="TT_bb") col = -4;
  if(name=="TT_2b") col = -2;
  return col;
}





double get_highestbinval(TH1F* h_data_var, TH1F* h_sig, THStack* hstack, int v){
  double highestbinval = 0;
  for(int h=1; h<=h_data_var->GetNbinsX(); h++) if(h_data_var->GetBinContent(h)>highestbinval) highestbinval=h_data_var->GetBinContent(h);
  for(int h=1; h<=h_sig->GetNbinsX(); h++) if(h_sig->GetBinContent(h)>highestbinval) highestbinval=h_sig->GetBinContent(h);
  if(hstack->GetMaximum()>highestbinval) highestbinval=hstack->GetMaximum();
  return highestbinval;
}
void save_canvas(TCanvas* c1, string var){
  string namefile = var+selection+".pdf";
  if(save_plots)  c1->SaveAs(namefile.c_str());
  namefile = var+selection+".png";
  if(save_plots)  c1->SaveAs(namefile.c_str());
}





/////
//   Get legends and histos
/////
TLegend* get_legend(){
  TLegend *leg = new TLegend(0.75, 0.45, 0.95, 0.9);
  leg->SetHeader("Samples");
  leg->SetBorderSize(0);
  leg->SetTextSize(0.05);
  return leg;
}


TH1F* get_th1f(string var, int v){
  cout << "get_th1f"<< endl;
  TH1F *th1f;
  if(var=="BJetness_num_vetonoipnoiso_leps" && doasym) th1f = new TH1F("","",bin[v],asymbin);
  else                         th1f = new TH1F("","",bin[v],inRange[v],endRange[v]);
  return th1f;
}


TH1F* get_datath1f(string var, string title, int v){
  TH1F *datath1f = get_th1f(var, v);
  datath1f->SetTitle(0); datath1f->SetMarkerStyle(8); datath1f->SetMarkerColor(1); datath1f->SetLineColor(1);
  datath1f->GetXaxis()->SetRangeUser(inRange[v],endRange[v]);
  datath1f->GetXaxis()->SetLimits(inRange[v],endRange[v]);
  //if(!show_ratio) datath1f->GetXaxis()->SetTitle(title.c_str());
  //char bin_size_c[col_size]; float bin_size_f = ((endRange[v]-inRange[v])/bin[v]); sprintf(bin_size_c,"%.2f",bin_size_f);
  //string titleYaxis;
  //if(name=="massVis" && doasym) titleYaxis = "Events";
  //else                          titleYaxis = "Events/"+(string) bin_size_c;
  //datath1f->GetYaxis()->SetTitle(titleYaxis.c_str());
  //TGaxis::SetMaxDigits(4);
  //if(show_title) datath1f->SetTitle("#scale[0.90]{CMS preliminary,   #sqrt{s} = 13 TeV, L = 2.2 fb^{-1}}");
  return datath1f;
}






/////
//   Set setTDRStyle_modified (from link https://twiki.cern.ch/twiki/pub/CMS/TRK10001/setTDRStyle_modified.C)
/////
void setTDRStyle(){
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");
  //For the canvas:
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(600); //Height of canvas
  tdrStyle->SetCanvasDefW(600); //Width of canvas
  tdrStyle->SetCanvasDefX(0);   //POsition on screen
  tdrStyle->SetCanvasDefY(0);
  //For the Pad:
  tdrStyle->SetPadBorderMode(0);
  //tdrStyle->SetPadBorderSize(Width_t size = 1);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(false);
  tdrStyle->SetPadGridY(false);
  tdrStyle->SetGridColor(0);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);
  //For the frame:
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(0);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(1);
  tdrStyle->SetFrameLineStyle(1);
  tdrStyle->SetFrameLineWidth(1);
  //For the histo:
  tdrStyle->SetHistFillColor(0);
  //tdrStyle->SetHistFillStyle(0);
  tdrStyle->SetHistLineColor(1);
  tdrStyle->SetHistLineStyle(0);
  tdrStyle->SetHistLineWidth(1);
  //tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
  //tdrStyle->SetNumberContours(Int_t number = 20);
  //tdrStyle->SetEndErrorSize(0);
  tdrStyle->SetErrorX(0.);
  //tdrStyle->SetErrorMarker(20);
  tdrStyle->SetMarkerStyle(20);
  //For the fit/function:
  tdrStyle->SetOptFit(1);
  tdrStyle->SetFitFormat("5.4g");
  //tdrStyle->SetFuncColor(1);
  tdrStyle->SetFuncStyle(1);
  tdrStyle->SetFuncWidth(1);
  //For the date:
  tdrStyle->SetOptDate(0);
  //tdrStyle->SetDateX(Float_t x = 0.01);
  //tdrStyle->SetDateY(Float_t y = 0.01);
  //For the statistics box:
  tdrStyle->SetOptFile(0);
  tdrStyle->SetOptStat(""); // To display the mean and RMS:   SetOptStat("mr");
  //tdrStyle->SetStatColor(kWhite);
  tdrStyle->SetStatColor(kGray);
  tdrStyle->SetStatFont(42);
  tdrStyle->SetTextSize(11);
  tdrStyle->SetTextAlign(11);
  tdrStyle->SetStatTextColor(1);
  tdrStyle->SetStatFormat("6.4g");
  tdrStyle->SetStatBorderSize(0);
  tdrStyle->SetStatX(1.); //Starting position on X axis
  tdrStyle->SetStatY(1.); //Starting position on Y axis
  tdrStyle->SetStatFontSize(0.025); //Vertical Size
  tdrStyle->SetStatW(0.25); //Horizontal size
  //tdrStyle->SetStatStyle(Style_t style = 1001);
  //Margins:
  tdrStyle->SetPadTopMargin(0.095);
  tdrStyle->SetPadBottomMargin(0.125);
  tdrStyle->SetPadLeftMargin(0.14);
  tdrStyle->SetPadRightMargin(0.1);
  //For the Global title:
  //tdrStyle->SetOptTitle(0);
  tdrStyle->SetTitleFont(42);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(10);
  tdrStyle->SetTitleFontSize(0.05);
  tdrStyle->SetTitleH(0.045); // Set the height of the title box
  //tdrStyle->SetTitleW(0); // Set the width of the title box
  tdrStyle->SetTitleX(0.20); // Set the position of the title box
  tdrStyle->SetTitleY(1.0); // Set the position of the title box
  //tdrStyle->SetTitleStyle(Style_t style = 1001);
  tdrStyle->SetTitleBorderSize(0);
  //For the axis titles:
  tdrStyle->SetTitleColor(1, "XYZ");
  tdrStyle->SetTitleFont(42, "XYZ");
  tdrStyle->SetTitleSize(0.06, "XYZ");
  //tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  //tdrStyle->SetTitleYSize(Float_t size = 0.02);
  tdrStyle->SetTitleXOffset(0.9);
  tdrStyle->SetTitleYOffset(1.0);
  //tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset
  //For the axis labels:
  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize(0.05, "XYZ");
  //For the axis:
  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.03, "XYZ");
  tdrStyle->SetNdivisions(510, "XYZ");
  tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY(1);
  //Change for log plots:
  tdrStyle->SetOptLogx(0);
  tdrStyle->SetOptLogy(0);
  tdrStyle->SetOptLogz(0);
  //Postscript options:
  //tdrStyle->SetPaperSize(15.,15.);
  //tdrStyle->SetLineScalePS(Float_t scale = 3);
  //tdrStyle->SetLineStyleString(Int_t i, const char* text);
  //tdrStyle->SetHeaderPS(const char* header);
  //tdrStyle->SetTitlePS(const char* pstitle);
  //tdrStyle->SetBarOffset(Float_t baroff = 0.5);
  //tdrStyle->SetBarWidth(Float_t barwidth = 0.5);
  //tdrStyle->SetPaintTextFormat(const char* format = "g");
  //tdrStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
  //tdrStyle->SetTimeOffset(Double_t toffset);
  //tdrStyle->SetHistMinimumZero(kTRUE);
  tdrStyle->cd();
}
//if(logYscale[v]==1 && !normalised){h_data_var->SetMaximum((highestbinval+0.25*highestbinval)*10);}else if(logYscale[v]==1 && normalised)
//{h_data_var->SetMaximum(2);}
//if(logYscale[v]==1 && normalised){h_data_var->SetMinimum(0.0001);}else if(logYscale[v]==1 && !normalised){h_data_var->SetMinimum(0.01);}else{h_data_var->SetMinimum(0);}
//if(logYscale[v]==1){hstack->SetMaximum((highestbinval+0.25*highestbinval)*10);}else{hstack->SetMaximum(highestbinval+0.25*highestbinval);}
//if(logYscale[v]==1){hstack->SetMinimum(0.1);}else{hstack->SetMinimum(0);}
