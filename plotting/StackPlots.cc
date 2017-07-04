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
string path = "";

// ==== Samples ====

const char *samples[]   = {"ttjets_incl/ttjets_incl_Merged_rootplas", "ttHbb/ttHbb_Merged_rootplas", "combined_SLep"};

// ==== Selection ====
const string selection  = "_SL"; //_SingleEle, _SingleMu
const bool nodata       = false;  //You must always comment data in samples if you don't want it
const bool show_ratio   = true;

// ==== Weights ====
const double Luminosity = 35900; //pb^-1
const bool   LumiNorm   = true;
const bool   PUcorr     = true;
const bool   SF         = true; //For the TTHbb analysis it represents the bWeight factor
const bool   LeptonSFs  = true;
const bool   triggerSFs = true;
const double scale      = 0;    //0 means no scaling; any other values means scale histo by the value of scale

// ===== Normalisation of plots =====
// One must run the script once with "normalised = false" to get the value for the background normalisation.
const bool normalised   = false;
//normbkg and normdata values have to be taken after 1 iteration of the macro with normalised = false
double normbkg;
double normdata;
double normsig;

// ===== Plots =====
const bool save_plots   = true;
const bool show_title   = true;
const bool doasym       = false;
const double asymbin[6] = {0,3,4,9,15,20};
const int    numVar     = 93;
const int logYscale[numVar] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
//const int logYscale[numVar] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
const int    col_size   = 500; //>= highest bin

// Number of variables you want to loop over:
const unsigned int ini_var = 64;
const unsigned int fin_var = 93;
const int posvtcr          = 0;

// ======= Variables map ======= ???
// Key: const char *variables
// Value: vector< const char *titleXaxis, const int    bin, const double inRange, const double endRange >
// Description: Each variable has an entry in the map. The entry contains a 4D vector:
//              - 1st vector element contains the x-axis title.
//              - 2nd vector element contains # bins.
//              - 3rd vector element contains x-axis minimum
//              - 4th vector element contains x-axis maximum


// Variables list:

const char *variables[]         = {
  "BJetness_num_pdgid_eles",
  "BJetness_num_soft_eles",
  "BJetness_num_vetonoipnoiso_eles",
  "BJetness_num_loosenoipnoiso_eles",
  "BJetness_num_veto_eles",
  "BJetness_num_loose_eles",
  "BJetness_num_medium_eles",
  "BJetness_num_tight_eles",
  "BJetness_num_mvatrig_eles",
  "BJetness_num_mvanontrig_eles",
  "BJetness_num_mvatrigwp90_eles",
  "BJetness_num_mvanontrigwp90_eles",
  "BJetness_num_heep_eles",
  "BJetness_num_pdgid_mus",
  "BJetness_num_loose_mus",
  "BJetness_num_soft_mus",
  "BJetness_num_medium_mus",
  "BJetness_num_tight_mus",
  "BJetness_num_highpt_mus",
  "BJetness_num_POGisGood_mus",
  "NumberOfJets",
  "NumberOfBJets",
  "first_jet_pt",
  "second_jet_pt",
  "second_highest_btag",
  "third_jet_pt",
  "third_highest_btag",
  "fourth_jet_pt",
  "fourth_highest_btag",
  "lead_el_pt",
  "lead_el_eta",
  "lead_el_phi",
  "lead_mu_pt",
  "lead_mu_eta",
  "lead_mu_phi",
  "lead_lep_pt",
  "lead_lep_eta",
  "lead_lep_phi",
  "BJetness_numjettrks",
  "BJetness_numjettrkspv",
  "BJetness_numjettrksnopv",
  "BJetness_pvTrkOVcollTrk",
  "BJetness_npvTrkOVpvTrk",
  "BJetness_npvPtOVcollPt",
  "BJetness_pvPtOVcollPt",
  "BJetness_npvPtOVpvPt",
  "BJetness_avprel",
  "BJetness_avppar",
  "BJetness_avetarel",
  "BJetness_avetapar",
  "BJetness_avdr",
  "BJetness_avpreljetpt",
  "BJetness_avpreljeten",
  "BJetness_avpparjetpt",
  "BJetness_avpparjeten",
  "BJetness_chi2",
  "BJetness_avip3d_val",
  "BJetness_avip3d_sig",
  "BJetness_avsip3d_val",
  "BJetness_avsip3d_sig",
  "BJetness_numip3dpos",
  "BJetness_numip3dneg",
  "BJetness_jetschpvass",
  "BJetness_jetschfrompv",
  "BJetness_jetschip3dval",
  "BJetness_jetschip3dsig",
  "BJetness_jetschip2dval",
  "BJetness_jetschip2dsig",
  "BJetness_jetschisgoodtrk",
  "BJetness_jetschtrkpur",
  "BJetness_jetschpt",
  "BJetness_jetscheta",
  "BJetness_jetschen",
  "BJetness_avip1d_val",
  "BJetness_avip1d_sig",
  "BJetness_avsip1d_val",
  "BJetness_avsip1d_sig",
  "BJetness_avip2d_val",
  "BJetness_avip2d_sig",
  "BJetness_avsip2d_val",
  "BJetness_avsip2d_sig",
  "trueInteractions",
  "pvertex_ndof",
  "npuVertices",
  "BJetness_avjetschip2dval",
  "BJetness_avjetschip2dsig",
  "BJetness_avjetschip3dval",
  "BJetness_avjetschip3dsig",
  "BJetness_sumjetschip2dval",
  "BJetness_sumjetschip2dsig",
  "BJetness_sumjetschip3dval",
  "BJetness_sumjetschip3dsig",
  "allJetsCSVvalue"
};
const char *titleXaxis[]        = {
  "BJetness # pdg ID el",
  "BJetness # Soft el",
  "BJetness # Veto ele no ip no iso",
  "BJetness # Loose el no ip no iso",
  "BJetness # Veto el",
  "BJetness # Loose el",
  "BJetness # Medium el",
  "BJetness # Tight el",
  "BJetness # MVA trig el",
  "BJetness # MVA nontrig el",
  "BJetness # MVA trig 90 el",
  "BJetness # MVA nontrig 90 el",
  "BJetness # heep el",
  "BJetness # pdg ID mu",
  "BJetness # Loose mu",
  "BJetness # Soft mu",
  "BJetness # Medium mu",
  "BJetness # Tight mu",
  "BJetness num High Pt mu",
  "BJetness num POGisGood mus",
  "# Jets",
  "# BJets",
  "Lead Jet Pt",
  "Sublead Jet Pt",
  "Second Highest CSV",
  "Third Jet Pt",
  "Third Highest CSV",
  "Fourth Jet Pt",
  "Fourth Highest CSV",
  "Lead el Pt",
  "Lead el Eta",
  "Lead el Phi",
  "Lead mu Pt",
  "Lead mu Eta",
  "Lead mu Phi",
  "Lead Lepton Pt",
  "Lead Lepton Eta",
  "Lead Lepton Phi",
  "BJetness # Jet Trks",
  "BJetness # Jet Trks PV",
  "BJetness # Jet Trks No PV",
  "BJetness pvTrkOVcollTrk",
  "BJetness npvTrkOVpvTrk",
  "BJetness npvPtOVcollPt",
  "BJetness pvPtOVcollPt",
  "BJetness npvPtOVpvPt",
  "BJetness Av. Projection Perp.",
  "BJetness Av. Projection Par.",
  "BJetness Av. Eta Perp.",
  "BJetness Av. Eta Par.",
  "BJetness Av. dR",
  "BJetness Av. prel Jet Pt",
  "BJetness Av. prel Jet Energy ",
  "BJetness Av. ppar Jet Pt",
  "BJetness Av. ppar Jet Energy",
  "BJetness Chi2",
  "BJetness Av. IP 3D Val.",
  "BJetness Av. IP 3D Sig.",
  "BJetness Av. Signed IP 3D Val.",
  "BJetness Av. Signed IP 3D Sig.",
  "BJetness # IP 3D Pos.",
  "BJetness # IP 3D Neg.",
  "BJetness Jets ch PV Ass.",
  "BJetness Jets ch from PV",
  "BJetness Jets ch IP 3D Val.",
  "BJetness Jets ch IP 3D Sig.",
  "BJetness Jets ch IP 2D Val.",
  "BJetness Jets ch IP 2D Sig.",
  "BJetness Jets ch isgoodtrk",
  "BJetness Jets ch Trk. High Purity",
  "BJetness Jets ch Pt",
  "BJetness Jets ch Eta",
  "BJetness Jets ch Energy",
  "BJetness Av. IP 1D Val.",
  "BJetness Av. IP 1D Sig.",
  "BJetness Av. Signed IP 1D Val.",
  "BJetness Av. Signed IP 1D Sig.",
  "BJetness Av. IP 2D Val.",
  "BJetness Av. IP 2D Sig.",
  "BJetness Av. Signed IP 2D Val.",
  "BJetness Av. Signed IP 2D Sig.",
  "# Interactions",
  "pvertex ndof",
  "npuVertices",
  "BJetness_avjetschip2dval",
  "BJetness_avjetschip2dsig",
  "BJetness_avjetschip3dval",
  "BJetness_avjetschip3dsig",
  "BJetness_sumjetschip2dval",
  "BJetness_sumjetschip2dsig",
  "BJetness_sumjetschip3dval",
  "BJetness_sumjetschip3dsig",
  "CSV value"
};
const int    bin[numVar]        = {
  4,//BJetness_num_pdgid_eles
  4,//BJetness_num_soft_eles
  3,//BJetness_num_vetonoipnoiso_eles
  3,//BJetness_num_loosenoipnoiso_eles
  6,//BJetness_num_veto_eles
  6,//BJetness_num_loose_eles
  6,//BJetness_num_medium_eles
  6,//BJetness_num_tight_eles
  3,//BJetness_num_mvatrig_eles
  3,//BJetness_num_mvanontrig_eles
  3,//BJetness_num_mvatrigwp90_eles
  3,//BJetness_num_mvanontrigwp90_eles
  6,//BJetness_num_heep_eles
  4,//BJetness_num_pdgid_mus
  4,//BJetness_num_loose_mus
  4,//BJetness_num_soft_mus
  4,//BJetness_num_medium_mus
  6,//BJetness_num_tight_mus
  4,//BJetness_num_highpt_mus
  4,//BJetness_num_POGisGood_mus
  6,//NumberOfJets
  4,//NumberOfBJets
  50,//first_jet_pt
  50,//second_jet_pt
  10,//second_highest_btag
  50,//third_jet_pt
  10,//third_highest_btag
  50,//fourth_jet_pt
  10,//fourth_highest_btag
  40,//lead_el_pt
  20,//lead_el_eta
  20,//lead_el_phi
  40,//lead_mu_pt
  20,//lead_mu_eta
  20,//lead_mu_phi
  40,//lead_lep_pt
  20,//lead_lep_eta
  20,//lead_lep_phi
  100,//BJetness_numjettrks
  60,//BJetness_numjettrkspv
  30,//BJetness_numjettrksnopv
  8,//BJetness_pvTrkOVcollTrk
  6,//BJetness_npvTrkOVpvTrk
  10,//BJetness_npvPtOVcollPt
  10,//BJetness_pvPtOVcollPt
  10,//BJetness_npvPtOVpvPt
  20,//BJetness_avprel
  20,//BJetness_avppar
  20,//BJetness_avetarel
  30,//BJetness_avetapar
  20,//BJetness_avdr
  20,//BJetness_avpreljetpt
  20,//BJetness_avpreljeten
  20,//BJetness_avpparjetpt
  10,//BJetness_avpparjeten
  20,//BJetness_chi2
  20,//BJetness_avip3d_val
  40,//BJetness_avip3d_sig
  20,//BJetness_avsip3d_val
  20,//BJetness_avsip3d_sig
  20,//BJetness_numip3dpos
  26,//BJetness_numip3dneg
  8,//BJetness_jetschpvass
  7,//BJetness_jetschfrompv
  40,//BJetness_jetschip3dval
  40,//BJetness_jetschip3dsig
  40,//BJetness_jetschip2dval
  40,//BJetness_jetschip2dsig
  2,//BJetness_jetschisgoodtrk
  2,//BJetness_jetschtrkpur
  50,//BJetness_jetschpt
  6,//BJetness_jetscheta
  30,//BJetness_jetschen
  10,//BJetness_avip1d_val
  20,//BJetness_avip1d_sig
  30,//BJetness_avsip1d_val
  30,//BJetness_avsip1d_sig
  12,//BJetness_avip2d_val
  15,//BJetness_avip2d_sig
  20,//BJetness_avsip2d_val
  20,//BJetness_avsip2d_sig
  50,//trueInteractions
  50,//pvertex_ndof
  50,//npuVertices
  40,//BJetness_avjetschip2dval
  38,//BJetness_avjetschip2dsig
  40,//BJetness_avjetschip3dval
  40,//BJetness_avjetschip3dsig
  40,//BJetness_sumjetschip2dval
  40,//BJetness_sumjetschip2dsig
  40,//BJetness_sumjetschip3dval
  40,//BJetness_sumjetschip3dsig
  20//allJetsCSVvalue
};
const double inRange[numVar]    = {
  0,//BJetness_num_pdgid_eles
  0,//BJetness_num_soft_eles
  0,//BJetness_num_vetonoipnoiso_eles
  0,//BJetness_num_loosenoipnoiso_eles
  0,//BJetness_num_veto_eles
  0,//BJetness_num_loose_eles
  0,//BJetness_num_medium_eles
  0,//BJetness_num_tight_eles
  0,//BJetness_num_mvatrig_eles
  0,//BJetness_num_mvanontrig_eles
  0,//BJetness_num_mvatrigwp90_eles
  0,//BJetness_num_mvanontrigwp90_eles
  0,//BJetness_num_heep_eles
  0,//BJetness_num_pdgid_mus
  0,//BJetness_num_loose_mus
  0,//BJetness_num_soft_mus
  0,//BJetness_num_medium_mus
  0,//BJetness_num_tight_mus
  0,//BJetness_num_highpt_mus
  0,//BJetness_num_POGisGood_mus
  4,//NumberOfJets
  2,//NumberOfBJets
  0,//first_jet_pt
  0,//second_jet_pt
  0,//second_highest_btag
  0,//third_jet_pt
  0,//third_highest_btag
  0,//fourth_jet_pt
  0,//fourth_highest_btag
  10,//lead_el_pt
  -3,//lead_el_eta
  -4,//lead_el_phi
  10,//lead_mu_pt
  -3,//lead_mu_eta
  -4,//lead_mu_phi
  10,//lead_lep_pt
  -3,//lead_lep_eta
  -4,//lead_lep_phi
  0,//BJetness_numjettrks
  0,//BJetness_numjettrkspv
  0,//BJetness_numjettrksnopv
  0.2,//BJetness_pvTrkOVcollTrk
  0,//BJetness_npvTrkOVpvTrk
  0,//BJetness_npvPtOVcollPt
  0,//BJetness_pvPtOVcollPt
  0,//BJetness_npvPtOVpvPt
  0,//BJetness_avprel
  0,//BJetness_avppar
  0,//BJetness_avetarel
  2,//BJetness_avetapar
  0,//BJetness_avdr
  0,//BJetness_avpreljetpt
  0,//BJetness_avpreljeten
  0,//BJetness_avpparjetpt
  0,//BJetness_avpparjeten
  995,//BJetness_chi2
  0,//BJetness_avip3d_val
  0,//BJetness_avip3d_sig
  -0.1,//BJetness_avsip3d_val
  -10,//BJetness_avsip3d_sig
  0,//BJetness_numip3dpos
  4,//BJetness_numip3dneg
  0,//BJetness_jetschpvass
  0,//BJetness_jetschfrompv
  0,//BJetness_jetschip3dval
  0,//BJetness_jetschip3dsig
  0,//BJetness_jetschip2dval
  0,//BJetness_jetschip2dsig
  0,//BJetness_jetschisgoodtrk
  0,//BJetness_jetschtrkpur
  0,//BJetness_jetschpt
  -3,//BJetness_jetscheta
  0,//BJetness_jetschen
  0,//BJetness_avip1d_val
  0,//BJetness_avip1d_sig
  -0.04,//BJetness_avsip1d_val
  -10,//BJetness_avsip1d_sig
  0,//BJetness_avip2d_val
  0,//BJetness_avip2d_sig
  -0.02,//BJetness_avsip2d_val
  -10,//BJetness_avsip2d_sig
  0,//trueInteractions
  0,//pvertex_ndof
  0,//npuVertices
  0,//BJetness_avjetschip2dval
  0,//BJetness_avjetschip2dsig
  0,//BJetness_avjetschip3dval
  0,//BJetness_avjetschip3dsig
  0,//BJetness_sumjetschip2dval
  0,//BJetness_sumjetschip2dsig
  0,//BJetness_sumjetschip3dval
  0,//BJetness_sumjetschip3dsig
  0.8//allJetsCSVvalue

};
const double endRange[numVar]   = {
  4,//BJetness_num_pdgid_eles
  4,//BJetness_num_soft_eles
  3,//BJetness_num_vetonoipnoiso_eles
  3,//BJetness_num_loosenoipnoiso_eles
  6,//BJetness_num_veto_eles
  6,//BJetness_num_loose_eles
  6,//BJetness_num_medium_eles
  6,//BJetness_num_tight_eles
  3,//BJetness_num_mvatrig_eles
  3,//BJetness_num_mvanontrig_eles
  3,//BJetness_num_mvatrigwp90_eles
  3,//BJetness_num_mvanontrigwp90_eles
  6,//BJetness_num_heep_eles
  4,//BJetness_num_pdgid_mus
  4,//BJetness_num_loose_mus
  4,//BJetness_num_soft_mus
  4,//BJetness_num_medium_mus
  6,//BJetness_num_tight_mus
  4,//BJetness_num_highpt_mus
  4,//BJetness_num_POGisGood_mus
  10,//NumberOfJets
  6,//NumberOfBJets
  300,//first_jet_pt
  250,//second_jet_pt
  1,//second_highest_btag
  200,//third_jet_pt
  1,//third_highest_btag
  200,//fourth_jet_pt
  1,//fourth_highest_btag
  200,//lead_el_pt
  3,//lead_el_eta
  4,//lead_el_phi
  200,//lead_mu_pt
  3,//lead_mu_eta
  4,//lead_mu_phi
  200,//lead_lep_pt
  3,//lead_lep_eta
  4,//lead_lep_phi
  100,//BJetness_numjettrks
  60,//BJetness_numjettrkspv
  30,//BJetness_numjettrksnopv
  1,//BJetness_pvTrkOVcollTrk
  1.2,//BJetness_npvTrkOVpvTrk
  1,//BJetness_npvPtOVcollPt
  1,//BJetness_pvPtOVcollPt
  3,//BJetness_npvPtOVpvPt
  1,//BJetness_avprel
  30,//BJetness_avppar
  0.2,//BJetness_avetarel
  5,//BJetness_avetapar
  0.3,//BJetness_avdr
  0.1,//BJetness_avpreljetpt
  0.1,//BJetness_avpreljeten
  2,//BJetness_avpparjetpt
  1,//BJetness_avpparjeten
  1000,//BJetness_chi2
  0.4,//BJetness_avip3d_val
  80,//BJetness_avip3d_sig
  0.2,//BJetness_avsip3d_val
  40,//BJetness_avsip3d_sig
  40,//BJetness_numip3dpos
  30,//BJetness_numip3dneg
  8,//BJetness_jetschpvass
  7,//BJetness_jetschfrompv
  0.4,//BJetness_jetschip3dval
  40,//BJetness_jetschip3dsig
  0.2,//BJetness_jetschip2dval
  40,//BJetness_jetschip2dsig
  1,//BJetness_jetschisgoodtrk
  1,//BJetness_jetschtrkpur
  100,//BJetness_jetschpt
  3,//BJetness_jetscheta
  30,//BJetness_jetschen
  0.3,//BJetness_avip1d_val
  120,//BJetness_avip1d_sig
  0.06,//BJetness_avsip1d_val
  30,//BJetness_avsip1d_sig
  0.06,//BJetness_avip2d_val
  12,//BJetness_avip2d_sig
  0.05,//BJetness_avsip2d_val
  15,//BJetness_avsip2d_sig
  50,//trueInteractions
  50,//pvertex_ndof
  50,//npuVertices
  0.05,//BJetness_avjetschip2dval
  16,//BJetness_avjetschip2dsig
  0.5,//BJetness_avjetschip3dval
  50,//BJetness_avjetschip3dsig
  1,//BJetness_sumjetschip2dval
  400,//BJetness_sumjetschip2dsig
  4,//BJetness_sumjetschip3dval
  400,//BJetness_sumjetschip3dsig
  1//allJetsCSVvalue
};

/////
//   Declare functions
/////
TFile* Call_TFile(string rootpla);
TH1F* double_h_var(unsigned int v, string var, string vaT, uint i, string rootplas, double err_AllBkg[][col_size], double ent_AllBkg[][col_size], int datatype);
TH1F* int_h_var(unsigned int v, string var, string varT, uint i, string rootplas, double err_AllBkg[][col_size], double ent_AllBkg[][col_size], int datatype);
TH1F* vector_double_h_var(unsigned int v, string var, string varT, uint i, string rootplas, double err_AllBkg[][col_size], double ent_AllBkg[][col_size], int datatype);
void draw_plots(TCanvas* c1, TH1F* h_sum_var, THStack* hstack, TH1F* h_data_var, TH1F* h_sig, TLegend* leg, double err_AllBkg[][col_size], double ent_AllBkg[][col_size], uint rootplas_size, int v, string var, string vartitle, double highestbinval);
void draw_lines(double x1, double y1, double x2, double y2);
int      get_col(string name);
double   get_highestbinval(TH1F* h_data_var, TH1F* h_sig, THStack* hstack, int v);
void save_canvas(TCanvas* c1, string var);
TLegend* get_legend(string varname);
TH1F*    get_th1f(string var, int v);
TH1F*    get_datath1f(string var, string title, int v);
void setTDRStyle();


/////
//   Main function
/////
void StackPlots(){
  cout << "Running StackPlots . . . . ." << endl;
  setTDRStyle();

  //Loop over all variables
  vector<string> var(variables, variables + sizeof(variables)/sizeof(variables[0]));
  vector<string> varTitleXaxis(titleXaxis, titleXaxis + sizeof(titleXaxis)/sizeof(titleXaxis[0]));
  for(uint v=ini_var; v<fin_var; v++){
    cout << "============ Variable index = " << v << "=============" << endl;
    cout << "variable = " << var[v] << endl;

    //Declare legend
    TLegend *leg = get_legend(var[v]);

    //Declare histograms:
    //MC
    double bkgstackintegral = 0.;
    THStack* hstack = new THStack("hstack","hstack");
    TH1F* h_sum_var = get_th1f(var[v], v); h_sum_var->Sumw2();//Bckg histogram
    TH1F* h_sig     = get_th1f(var[v], v);//Signal histogram
    //Data
    TH1F* h_data_var = get_datath1f(var[v], varTitleXaxis[v], v); h_data_var->Sumw2();

    //Loop over samples
    vector<string> rootplas(samples, samples + sizeof(samples)/sizeof(samples[0]));
    const uint rootplas_size = rootplas.size();
    double err_AllBkg[rootplas_size][col_size];
    double ent_AllBkg[rootplas_size][col_size];
    for(uint i=0; i<rootplas_size; i++) for(int j=0; j<bin[v]; j++) err_AllBkg[i][j] = 0.;
    for(uint i=0; i<rootplas_size; i++) for(int j=0; j<bin[v]; j++) ent_AllBkg[i][j] = 0.;

    for(uint i=0; i<rootplas_size; i++){
      cout << ">>>> Sample : " << rootplas[i] << "<<<<" << endl;
      int datatype = -999;
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
        path = "/publicfs/cms/data/TopQuark/ttHbb/JTW/2017_07_v2/ttHACIHEP/output/MC/";
      }
      else{
        path = "/publicfs/cms/data/TopQuark/ttHbb/JTW/2017_07_v2/ttHACIHEP/output/DATA/";
      }

      //For individual background MC, declare temp variable histogram.
      TH1F *h_var = get_th1f(var[v], v);

      //Histograms construction, fill, scaling etc.


      if(datatype==2){
        if(var[v].find("BJetness_avjetschip")==std::string::npos && var[v].find("BJetness_sumjetschip")==std::string::npos && (var[v].find("BJetness")!=std::string::npos || var[v].find("pvertex_ndof")!=std::string::npos || var[v].find("allJetsCSVvalue")!=std::string::npos)){
          h_var = vector_double_h_var(v,var[v],varTitleXaxis[v],i,rootplas[i],err_AllBkg,ent_AllBkg,datatype);
        }
        else if (var[v]=="NumberOfJets" || var[v]=="NumberOfBJets" || var[v]=="npuVertices") {
          h_var  = int_h_var(v,var[v],varTitleXaxis[v],i,rootplas[i],err_AllBkg,ent_AllBkg,datatype);
        }
        else{
          h_var  = double_h_var(v,var[v],varTitleXaxis[v],i,rootplas[i],err_AllBkg,ent_AllBkg,datatype);
        }
      }
      else if(datatype==1){
        if(var[v].find("BJetness_avjetschip")==std::string::npos && var[v].find("BJetness_sumjetschip")==std::string::npos && (var[v].find("BJetness")!=std::string::npos || var[v].find("pvertex_ndof")!=std::string::npos || var[v].find("allJetsCSVvalue")!=std::string::npos)){
          h_sig = vector_double_h_var(v,var[v],varTitleXaxis[v],i,rootplas[i],err_AllBkg,ent_AllBkg,datatype);
        }
        else if (var[v]=="NumberOfJets" || var[v]=="NumberOfBJets"|| var[v]=="npuVertices") {
          h_sig  = int_h_var(v,var[v],varTitleXaxis[v],i,rootplas[i],err_AllBkg,ent_AllBkg,datatype);
        }
        else{
          h_sig  = double_h_var(v,var[v],varTitleXaxis[v],i,rootplas[i],err_AllBkg,ent_AllBkg,datatype);
        }
      }
      else{
        if(var[v].find("BJetness_avjetschip")==std::string::npos && var[v].find("BJetness_sumjetschip")==std::string::npos && (var[v].find("BJetness")!=std::string::npos || var[v].find("pvertex_ndof")!=std::string::npos || var[v].find("allJetsCSVvalue")!=std::string::npos)){
          h_data_var = vector_double_h_var(v,var[v],varTitleXaxis[v],i,rootplas[i],err_AllBkg,ent_AllBkg,datatype);
        }
        else if (var[v]=="NumberOfJets" || var[v]=="NumberOfBJets"|| var[v]=="npuVertices") {
          h_data_var  = int_h_var(v,var[v],varTitleXaxis[v],i,rootplas[i],err_AllBkg,ent_AllBkg,datatype);
        }
        else{
          h_data_var  = double_h_var(v,var[v],varTitleXaxis[v],i,rootplas[i],err_AllBkg,ent_AllBkg,datatype);
        }
      }

      if(datatype==2){

        // Create nickname for sample on plots (used in legend)
        string bckg_mc_nickname = "";
        string full_samplename = string(rootplas[i]);
        if(full_samplename.find("/")!=std::string::npos){bckg_mc_nickname = full_samplename.substr(0,full_samplename.find("/"));}
        else{
          cout << "Could not find  in sample name. Using deafult name for legend!" << endl;
          bckg_mc_nickname = "<Default bckg name>";
        }

        //Put histos in the hstack
        int col = get_col(bckg_mc_nickname);

        if(rootplas[i].find("ttjets_incl") != std::string::npos){
          h_var->SetMarkerColor(kRed+col);
          h_var->SetFillColor(kRed+col);
          h_var->SetLineColor(kRed+col);
        }else{
          h_var->SetMarkerColor(kCyan+col);
          h_var->SetFillColor(kCyan+col);
          h_var->SetLineColor(kCyan+col);
        }

        //Add background to stack

        hstack->Add(h_var);
        //Get integral for bckg histogram of given variable.
        int nbins = h_var->GetNbinsX();

        if(var[v].find("lead_el")!=std::string::npos){
          cout<<setw(5)<<"Bckg Histogram integral (lead_el) :"<<setw(15)<<bckg_mc_nickname<<setw(15)<<h_var->Integral()<<endl;
          cout<<setw(5)<<"Bckg Histogram integral + overflow (lead_el) :"<<setw(15)<<bckg_mc_nickname<<setw(15)<<h_var->Integral(0,nbins+1)<<endl;
        }
        else if(var[v].find("lead_mu")!=std::string::npos){
          cout<<setw(5)<<"Bckg Histogram integral (lead_mu) :"<<setw(15)<<bckg_mc_nickname<<setw(15)<<h_var->Integral()<<endl;
          cout<<setw(5)<<"Bckg Histogram integral + overflow (lead_mu) :"<<setw(15)<<bckg_mc_nickname<<setw(15)<<h_var->Integral(0,nbins+1)<<endl;
        }
        else{
          cout<<setw(5)<<"Bckg Histogram integral :"<<setw(15)<<bckg_mc_nickname<<setw(15)<<h_var->Integral()<<endl;
          cout<<setw(5)<<"Bckg Histogram integral + overflow :"<<setw(15)<<bckg_mc_nickname<<setw(15)<<h_var->Integral(0,nbins+1)<<endl;
        }

        //Add integral to total bckg integral of given variable.
        bkgstackintegral += h_var->Integral(0,nbins+1);

        leg->AddEntry(h_var,bckg_mc_nickname.c_str(),"F");

        //Sum them for the error
        h_sum_var->Add(h_sum_var,h_var);

      }
      else if(datatype==0){
        //Get integral of data histogram for given variable.
        int nbins = h_data_var->GetNbinsX();

        if(var[v].find("lead_el")!=std::string::npos){
          cout<<setw(5)<<"Data Histogram integral:"<<setw(15)<<rootplas[i]<<setw(15)<<h_data_var->Integral()<<endl;
        }
        if(var[v].find("lead_mu")!=std::string::npos){
          cout<<setw(5)<<"Data Histogram integral:"<<setw(15)<<rootplas[i]<<setw(15)<<h_data_var->Integral()<<endl;
        }
        else{
          cout<<setw(5)<<"Data Histogram integral + overflow:"<<setw(15)<<rootplas[i]<<setw(15)<<h_data_var->Integral(0,nbins+1)<<endl;
        }

      }
      else if (datatype==1){
        int nbins_sig = h_sig->GetNbinsX();

        if(var[v].find("lead_el")!=std::string::npos){
          cout<<setw(5)<<"Total Signal Historgram Integral:"<<setw(15)<<"Sig"<<setw(15)<<h_sig->Integral()<<endl;
        }
        if(var[v].find("lead_mu")!=std::string::npos){
          cout<<setw(5)<<"Total Signal Historgram Integral:"<<setw(15)<<"Sig"<<setw(15)<<h_sig->Integral()<<endl;
        }
        else{
          cout<<setw(5)<<"Total Signal Historgram Integral + overflow:"<<setw(15)<<"Sig"<<setw(15)<<h_sig->Integral(0,nbins_sig+1)<<endl;
        }
        //cout<<setw(5)<<"Total Signal Historgram Integral + overflow:"<<setw(15)<<"Sig"<<setw(15)<<h_sig->Integral(0,nbins_sig+1)<<endl;
      }
    }

    cout << "========= TOTAL BCKG =========" << endl;
    if(var[v].find("lead_el")!=std::string::npos){
      cout<<setw(5)<<"Total Bckg Histogram Integral:"<<setw(15)<<"Bkg"<<setw(15)<<h_sum_var->Integral()<<endl;
    }
    else if(var[v].find("lead_mu")!=std::string::npos){
      cout<<setw(5)<<"Total Bckg Histogram Integral:"<<setw(15)<<"Bkg"<<setw(15)<<h_sum_var->Integral()<<endl;
    }
    else{
      cout<<setw(5)<<"Total Bckg Histogram Integral + overflow :"<<setw(15)<<"Bkg"<<setw(15)<<h_sum_var->Integral(0,h_sum_var->GetNbinsX()+1)<<endl;
    }
    //cout<<setw(5)<<"Total Bckg Histogram Integral:"<<setw(15)<<"Bkg"<<setw(15)<<h_sum_var->Integral(0,h_sum_var->GetNbinsX()+1)<<endl;

    //Total background and signal integrals.
    //cout<<setw(5)<<"bkgstackintegral :"<<setw(15)<<"Bkg"<<setw(15)<<bkgstackintegral<<endl;




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
  TFile* f = new TFile(file_name.c_str(),"update");
  return f;
}

/////
//   Fill histo with double type
/////
TH1F* double_h_var(unsigned int v, string var, string varT, uint i, string rootplas, double err_AllBkg[][col_size], double ent_AllBkg[][col_size], int datatype){
  cout << "=========== double_h_var ========== " << endl;
  //Call tree and variables
  TFile* f = Call_TFile(rootplas); TTree *tree; f->GetObject("BOOM",tree);

  double curr_var;
  TBranch *b_curr_var = 0;
  tree->SetBranchAddress(var.c_str(),&curr_var,&b_curr_var);
  double PUWeight;
  TBranch *b_PUWeight = 0;
  tree->SetBranchAddress("PUWeight",&PUWeight,&b_PUWeight);
  double bWeight;
  TBranch *b_bWeight = 0;
  tree->SetBranchAddress("bWeight",&bWeight,&b_bWeight);

  //====== Lepton SFs ======

  //Electron Reco SFs
  double Electron_GsfSFval;
  TBranch* b_Electron_GsfSFval = 0;
  tree->SetBranchAddress("Electron_GsfSFval",&Electron_GsfSFval,&b_Electron_GsfSFval);
  //Electron ID SFs
  double Electron_IDSFval;
  TBranch* b_Electron_IDSFval = 0;
  tree->SetBranchAddress("Electron_IDSFval",&Electron_IDSFval,&b_Electron_IDSFval);
  //Muon ID SFs
  double Muon_IDSFval;
  TBranch* b_Muon_IDSFval;
  tree->SetBranchAddress("Muon_IDSFval",&Muon_IDSFval,&b_Muon_IDSFval);
  //Muon Iso SFs
  double Muon_IsoSFval;
  TBranch* b_Muon_IsoSFval = 0;
  tree->SetBranchAddress("Muon_IsoSFval",&Muon_IsoSFval,&b_Muon_IsoSFval);
  //Muon Track SF
  double Muon_TrkSFval;
  TBranch* b_Muon_TrkSFval = 0;
  tree->SetBranchAddress("Muon_TrkSFval",&Muon_TrkSFval,&b_Muon_TrkSFval);

  //=========================

  double trigger_SF;
  TBranch* b_trigger_SF = 0;
  Float_t lumiweight;
  TBranch *b_lumiweight = 0;
  if(datatype!=0){
    tree->SetBranchAddress("lumiweight",&lumiweight,&b_lumiweight);
    tree->SetBranchAddress("trigger_SF",&trigger_SF,&b_trigger_SF);
  }
  else{lumiweight=1;}

  //Construct histogram
  TH1F *hist = get_th1f(var, v);

  //Dress histogram
  hist->SetTitle(0);
  hist->SetLineColor(1);

  TH1F *hist_err;
  if(var=="BJetness_num_vetonoipnoiso_leps" && doasym) hist_err = new TH1F("hist_err","hist_err",bin[v],asymbin);
  else                         hist_err = new TH1F("hist_err","hist_err",bin[v],inRange[v],endRange[v]);
  hist_err->Sumw2();
  int tripwire = 0;
  for(int j=0; j<tree->GetEntries(); j++)
  {
    double w = 1.;
    Long64_t tentry = tree->LoadTree(j);
    b_curr_var->GetEntry(tentry);
    b_PUWeight->GetEntry(tentry);
    b_bWeight->GetEntry(tentry);
    b_Electron_GsfSFval->GetEntry(tentry);
    b_Electron_IDSFval->GetEntry(tentry);
    b_Muon_IDSFval->GetEntry(tentry);
    b_Muon_IsoSFval->GetEntry(tentry);
    b_Muon_TrkSFval->GetEntry(tentry);

    if (var.find("lead_el_pt")!=std::string::npos || var.find("lead_mu_pt")!=std::string::npos ){
      if (curr_var<0){continue;}
    }

    if(datatype!=0){
      b_lumiweight->GetEntry(tentry);
      b_trigger_SF->GetEntry(tentry);

      if(LumiNorm) {
        w = w*lumiweight*Luminosity;
      }
      if(PUcorr){
        w = w*PUWeight;
      }
      if(SF){
        w = w*bWeight;
      }
      if(scale!=0){
        w = w*scale;
      }
      if(LeptonSFs) {
        w = w*Electron_GsfSFval*Electron_IDSFval*Muon_IDSFval*Muon_IsoSFval*Muon_TrkSFval;
      }
      if(triggerSFs){
        //cout << "triggerSFs = " << trigger_SF << endl;
        w = w*trigger_SF;
      }
      if(inRange[v]<curr_var && curr_var<endRange[v]){hist->Fill(curr_var,w);         hist_err->Fill(curr_var,w*w);}
      if(curr_var>=endRange[v])                      {hist->Fill(0.99*endRange[v],w); hist_err->Fill(0.99*endRange[v],w*w);}
      if(curr_var<=inRange[v])                       {hist->Fill(1.01*inRange[v],w);  hist_err->Fill(1.01*inRange[v],w*w);}
    }else{
      if(inRange[v]<curr_var && curr_var<endRange[v]) hist->Fill(curr_var);
      if(curr_var>=endRange[v])                       hist->Fill(0.99*endRange[v]);
      if(curr_var<=inRange[v])                        hist->Fill(1.01*inRange[v]);
    }
  }
  //Get errors, normalise
  int nbins = hist->GetNbinsX();
  if(normalised){
    if(var.find("lead_el")!=std::string::npos){
      normbkg = 1.06648e+07;
      normdata = 300731;
      normsig = 7007.4;
    }
    else if(var.find("lead_mu")!=std::string::npos){
      normbkg = 1.56299e+07;
      normdata = 316919;
      normsig = 10003.1;
    }
    else{
      normbkg = 2.62946e+07;
      normdata = 617650;
      normsig = 17010.5;
    }

    cout << "============= normalised ===============" << endl;
    if(datatype==0) hist->Scale(1/normdata);
    if(datatype==1) hist->Scale(1/normsig);
    if(datatype==2) hist->Scale(1/normbkg);
    cout << "Check hist integral (should ==1): " << hist->Integral(0,nbins+1) << endl;
    cout << "========================================" << endl;
  }
  else{
    cout << "hist integral (pre-scaling): " << hist->Integral(0,nbins+1) << endl;
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


TH1F* int_h_var(unsigned int v, string var, string varT, uint i, string rootplas, double err_AllBkg[][col_size], double ent_AllBkg[][col_size], int datatype){
  cout << " =========== int_h_var ============" << endl;
  //Call tree and variables
  TFile* f = Call_TFile(rootplas); TTree *tree; f->GetObject("BOOM",tree);

  int curr_var;
  TBranch *b_curr_var = 0;
  tree->SetBranchAddress(var.c_str(),&curr_var,&b_curr_var);
  double PUWeight;
  TBranch *b_PUWeight = 0;
  tree->SetBranchAddress("PUWeight",&PUWeight,&b_PUWeight);

  //====== Lepton SFs ======

  //Electron Reco SFs
  double Electron_GsfSFval;
  TBranch* b_Electron_GsfSFval = 0;
  tree->SetBranchAddress("Electron_GsfSFval",&Electron_GsfSFval,&b_Electron_GsfSFval);
  //Electron ID SFs
  double Electron_IDSFval;
  TBranch* b_Electron_IDSFval = 0;
  tree->SetBranchAddress("Electron_IDSFval",&Electron_IDSFval,&b_Electron_IDSFval);
  //Muon ID SFs
  double Muon_IDSFval;
  TBranch* b_Muon_IDSFval;
  tree->SetBranchAddress("Muon_IDSFval",&Muon_IDSFval,&b_Muon_IDSFval);
  //Muon Iso SFs
  double Muon_IsoSFval;
  TBranch* b_Muon_IsoSFval = 0;
  tree->SetBranchAddress("Muon_IsoSFval",&Muon_IsoSFval,&b_Muon_IsoSFval);
  //Muon Track SF
  double Muon_TrkSFval;
  TBranch* b_Muon_TrkSFval = 0;
  tree->SetBranchAddress("Muon_TrkSFval",&Muon_TrkSFval,&b_Muon_TrkSFval);

  //=========================
  double trigger_SF;
  TBranch* b_trigger_SF = 0;
  Float_t lumiweight;
  TBranch *b_lumiweight = 0;
  if(datatype!=0){
    tree->SetBranchAddress("lumiweight",&lumiweight,&b_lumiweight);
    tree->SetBranchAddress("trigger_SF",&trigger_SF,&b_trigger_SF);
  }
  else{lumiweight=1;}

  double bWeight;
  TBranch *b_bWeight = 0;
  tree->SetBranchAddress("bWeight",&bWeight,&b_bWeight);

  //Construct histogram
  TH1F *hist = get_th1f(var, v);

  //Dress histogram
  hist->SetTitle(0);
  hist->SetLineColor(1);

  TH1F *hist_err;
  if(var=="BJetness_num_vetonoipnoiso_leps" && doasym) hist_err = new TH1F("hist_err","hist_err",bin[v],asymbin);
  else                         hist_err = new TH1F("hist_err","hist_err",bin[v],inRange[v],endRange[v]);
  hist_err->Sumw2();
  int tripwire=0;
  for(int j=0; j<tree->GetEntries(); j++)
  {
    double w = 1.;
    Long64_t tentry = tree->LoadTree(j);
    b_curr_var->GetEntry(tentry);
    b_PUWeight->GetEntry(tentry);
    b_bWeight->GetEntry(tentry);
    b_Electron_GsfSFval->GetEntry(tentry);
    b_Electron_IDSFval->GetEntry(tentry);
    b_Muon_IDSFval->GetEntry(tentry);
    b_Muon_IsoSFval->GetEntry(tentry);
    b_Muon_TrkSFval->GetEntry(tentry);

    if(datatype!=0){
      b_trigger_SF->GetEntry(tentry);
      b_lumiweight->GetEntry(tentry);

      if(LumiNorm) w = w*lumiweight*Luminosity;
      if(PUcorr)   w = w*PUWeight;
      if(SF)       w = w*bWeight;
      if(scale!=0) w = w*scale;
      if(LeptonSFs) {w = w*Electron_GsfSFval*Electron_IDSFval*Muon_IDSFval*Muon_IsoSFval*Muon_TrkSFval;}
      if(triggerSFs) {
        //cout << "triggerSFs = " << trigger_SF << endl;
        w = w*trigger_SF;
      }
      if(inRange[v]<curr_var && curr_var<endRange[v]){hist->Fill(curr_var,w);         hist_err->Fill(curr_var,w*w);}
      if(curr_var>=endRange[v])                      {hist->Fill(0.99*endRange[v],w); hist_err->Fill(0.99*endRange[v],w*w);}
      if(curr_var<=inRange[v])                       {hist->Fill(1.01*inRange[v],w);  hist_err->Fill(1.01*inRange[v],w*w);}
    }else{
      if(inRange[v]<curr_var && curr_var<endRange[v]) hist->Fill(curr_var);
      if(curr_var>=endRange[v])                       hist->Fill(0.99*endRange[v]);
      if(curr_var<=inRange[v])                        hist->Fill(1.01*inRange[v]);
    }
  }
  //Get errors, normalise
  int nbins = hist->GetNbinsX();

  if(normalised){
    if(var.find("BJetness_jetsch")!=std::string::npos){
      normbkg = 5.98384e+08; //normbkg and normdata values have to be taken after 1 iteration of the macro with normalised = false
      normdata= 1.38596e+07;
      normsig = 608113;
    }
    else if(var.find("BJetness")!=std::string::npos){
      normbkg = 2.62943e+07;
      normdata= 528946;
      normsig = 17017.8;
    }
    else{
      normbkg = 2.62928e+07;
      normdata = 617650;
      normsig = 17010.5;
    }

    cout << "============= normalised ===============" << endl;
    if(datatype==0) hist->Scale(1/normdata);
    if(datatype==1) hist->Scale(1/normsig);
    if(datatype==2) hist->Scale(1/normbkg);
    cout << "Check hist integral (should ==1): " << hist->Integral(0,nbins+1) << endl;
    cout << "========================================" << endl;

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


//Vector<doubles>
/////
//   Fill histo with vector<double> type
/////
TH1F* vector_double_h_var(unsigned int v, string var, string varT, uint i, string rootplas, double err_AllBkg[][col_size], double ent_AllBkg[][col_size], int datatype){
  //Call tree and variables
  cout << "============ vector_double_h_var ============" << endl;

  TFile* f = Call_TFile(rootplas); TTree *tree; f->GetObject("BOOM",tree);
  vector <double> * var_vals =0;
  TBranch *b_var_vals = 0;
  tree->SetBranchAddress(var.c_str(),&var_vals,&b_var_vals);

  double curr_var;

  double PUWeight;
  TBranch *b_PUWeight = 0;
  tree->SetBranchAddress("PUWeight",&PUWeight,&b_PUWeight);

  //====== Lepton SFs ======

  //Electron Reco SFs
  double Electron_GsfSFval;
  TBranch* b_Electron_GsfSFval = 0;
  tree->SetBranchAddress("Electron_GsfSFval",&Electron_GsfSFval,&b_Electron_GsfSFval);
  //Electron ID SFs
  double Electron_IDSFval;
  TBranch* b_Electron_IDSFval = 0;
  tree->SetBranchAddress("Electron_IDSFval",&Electron_IDSFval,&b_Electron_IDSFval);
  //Muon ID SFs
  double Muon_IDSFval;
  TBranch* b_Muon_IDSFval;
  tree->SetBranchAddress("Muon_IDSFval",&Muon_IDSFval,&b_Muon_IDSFval);
  //Muon Iso SFs
  double Muon_IsoSFval;
  TBranch* b_Muon_IsoSFval = 0;
  tree->SetBranchAddress("Muon_IsoSFval",&Muon_IsoSFval,&b_Muon_IsoSFval);
  //Muon Track SF
  double Muon_TrkSFval;
  TBranch* b_Muon_TrkSFval = 0;
  tree->SetBranchAddress("Muon_TrkSFval",&Muon_TrkSFval,&b_Muon_TrkSFval);

  //=========================

  double trigger_SF;
  TBranch* b_trigger_SF = 0;
  Float_t lumiweight;
  TBranch *b_lumiweight = 0;
  if(datatype!=0){
    tree->SetBranchAddress("lumiweight",&lumiweight,&b_lumiweight);
    tree->SetBranchAddress("trigger_SF",&trigger_SF,&b_trigger_SF);
  }
  else{lumiweight=1;}

  double bWeight;
  TBranch *b_bWeight = 0;
  tree->SetBranchAddress("bWeight",&bWeight,&b_bWeight);

  //Construct histogram
  TH1F *hist = get_th1f(var, v);
  hist->Sumw2();

  //Dress histogram
  hist->SetTitle(0);
  hist->SetLineColor(1);

  TH1F *hist_err;
  if(var=="BJetness_num_vetonoipnoiso_leps" && doasym) hist_err = new TH1F("hist_err","hist_err",bin[v],asymbin);
  else                         hist_err = new TH1F("hist_err","hist_err",bin[v],inRange[v],endRange[v]);
  hist_err->Sumw2();
  int tripwire = 0;

  for(int j=0; j<tree->GetEntries(); j++)
  {
    double w = 1.;
    Long64_t tentry = tree->LoadTree(j);
    b_var_vals->GetEntry(tentry);
    b_PUWeight->GetEntry(tentry);
    b_bWeight->GetEntry(tentry);
    b_Electron_GsfSFval->GetEntry(tentry);
    b_Electron_IDSFval->GetEntry(tentry);
    b_Muon_IDSFval->GetEntry(tentry);
    b_Muon_IsoSFval->GetEntry(tentry);
    b_Muon_TrkSFval->GetEntry(tentry);

    if(var_vals->size()==0) continue;


    for(uint32_t k =0; k<var_vals->size(); k++){
      tripwire = tripwire+1;
      w=1;
      curr_var = var_vals->at(k);

      //cout << "curr_var = " << curr_var << endl;
      if(datatype!=0){
        b_lumiweight->GetEntry(tentry);
        b_trigger_SF->GetEntry(tentry);

        if(LumiNorm) {
          w = w*lumiweight*Luminosity;
        }
        if(PUcorr){
          w = w*PUWeight;
        }
        if(SF){
          w = w*bWeight;
        }
        if(scale!=0){
          w = w*scale;
        }
        if(LeptonSFs) {
          w = w*Electron_GsfSFval*Electron_IDSFval*Muon_IDSFval*Muon_IsoSFval*Muon_TrkSFval;
        }
        if(triggerSFs){
          //cout << "triggerSFs = " << trigger_SF << endl;
          w = w*trigger_SF;
        }

        if(inRange[v]<curr_var && curr_var<endRange[v]){hist->Fill(curr_var,w);         hist_err->Fill(curr_var,w*w);}
        if(curr_var>=endRange[v])                      {hist->Fill(0.99*endRange[v],w); hist_err->Fill(0.99*endRange[v],w*w);}
        if(curr_var<=inRange[v])                       {hist->Fill(1.01*inRange[v],w);  hist_err->Fill(1.01*inRange[v],w*w);}
      }else{
        if(inRange[v]<curr_var && curr_var<endRange[v]) hist->Fill(curr_var);
        if(curr_var>=endRange[v])                       hist->Fill(0.99*endRange[v]);
        if(curr_var<=inRange[v])                        hist->Fill(1.01*inRange[v]);
      }
    }
    //tripwire=0;
  }
  //Get errors, normalise
  int nbins = hist->GetNbinsX();

  if(normalised){
    if(var.find("BJetness_jetschip3dval")!=std::string::npos){
      normbkg = 3.745322931e+08;//4.25817e+08; //normbkg and normdata values have to be taken after 1 iteration of the macro with normalised = false
      normdata = 1.07626e+07;
      normsig = 399163;
    }
    else if(var.find("BJetness_jetschip2dval")!=std::string::npos){
      normbkg = 3.633738759e+08; //normbkg and normdata values have to be taken after 1 iteration of the macro with normalised = false
      normdata = 1.07626e+07;
      normsig = 398879;
    }
    else if(var.find("BJetness_jetschip3dsig")!=std::string::npos){
      normbkg=4.131175021e+08;//4.55933e+08;
      normdata=1.07626e+07;
      normsig=401376;
    }
    else if(var.find("BJetness_jetschip2dsig")!=std::string::npos){
      normbkg=3.99988333e+08;
      normdata=1.07626e+07;
      normsig=400700;
    }
    else if(var.find("BJetness_jetschisgoodtrk")!=std::string::npos || var.find("BJetness_jetschtrkpur")!=std::string::npos){
      normbkg=4.882940762e+08;
      normdata=1.07626e+07;
      normsig=400700;
    }
    else if(var.find("BJetness_jetsch")!=std::string::npos){
      normbkg = 4.62176e+08;
      normdata = 1.07626e+07;
      normsig = 401736;
    }
    else if(var.find("BJetness_avip")!=std::string::npos||var.find("BJetness_avsip")!=std::string::npos){
      normbkg = 2.62925e+07;
      normdata = 617648;
      normsig = 17010.4;
    }
    else if(var.find("BJetness_avjets")!=std::string::npos){
      normbkg = 2.6288e+07;
      normdata= 617476;
      normsig = 17009.3;
    }
    else if(var.find("BJetness_sumjets")!=std::string::npos){
      normbkg = 2.62912e+07;
      normdata= 617650;
      normsig = 17010.3;
    }
    else{
      normbkg = 2.62946e+07;
      normdata = 617648;
      normsig = 17010.5;
    }


    cout << "============= normalised ===============" << endl;
    if(datatype==0) hist->Scale(1/normdata);
    if(datatype==1) hist->Scale(1/normsig);
    if(datatype==2) hist->Scale(1/normbkg);
    cout << "Check hist integral (should ==1): " << hist->Integral(0,nbins+1) << endl;
    cout << "========================================" << endl;
  }
  else{
    cout << "hist integral (pre-scaling): " << hist->Integral(0,nbins+1) << endl;
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
  cout << "====== StackPlots::draw_plots() ======" << endl;
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

    h_data_var->Sumw2();
    h_sum_var->Sumw2();

    //Get values for ratio plot
    double dataSUmc_x[bin[v]]; double dataSUmc_y[bin[v]]; double dataSUmc_xerr[bin[v]]; double dataSUmc_yerr[bin[v]];
    for(int j=0; j<bin[v]; j++){
      dataSUmc_x[j] = 0; dataSUmc_y[j] = 0; dataSUmc_xerr[j] = 0; dataSUmc_yerr[j] = 0;
      dataSUmc_x[j] = h_sum_var->GetBinCenter(j+1);
      dataSUmc_xerr[j] = 0;
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


    //Plot values in ratio plot
    TGraphErrors *dataSUmc = new TGraphErrors(bin[v], dataSUmc_x, dataSUmc_y, dataSUmc_xerr, dataSUmc_yerr);
    dataSUmc->SetTitle(0);
    dataSUmc->SetMarkerStyle(1);
    dataSUmc->GetXaxis()->SetRangeUser(inRange[v],endRange[v]);
    dataSUmc->GetXaxis()->SetTitle(vartitle.c_str());
    dataSUmc->GetXaxis()->SetTitleSize(0.2);
    dataSUmc->GetYaxis()->SetTitle("Data/MC");
    dataSUmc->GetYaxis()->SetLabelSize(0.075);
    dataSUmc->GetYaxis()->SetTitleSize(0.15);
    dataSUmc->GetYaxis()->SetTitleOffset(0.35);
    dataSUmc->SetMinimum(0.5);
    dataSUmc->SetMaximum(1.5);
    dataSUmc->GetXaxis()->SetRangeUser(inRange[v],endRange[v]);
    dataSUmc->GetXaxis()->SetLimits(inRange[v],endRange[v]);
    //dataSUmc->Draw("APZ");
    dataSUmc->GetXaxis()->SetRangeUser(inRange[v],endRange[v]);
    dataSUmc->GetXaxis()->SetLimits(inRange[v],endRange[v]);


    TH1F* temp_data = (TH1F*)h_data_var->Clone("temp_data");
    TH1F* temp_bkg = (TH1F*)h_sum_var->Clone("temp_bkg");
    temp_data->Print();
    temp_data->Sumw2();
    temp_bkg->Sumw2();
    //cout << "temp_data bin 1 size = " << temp_data->GetBinContent(10) << endl;
    //cout << "temp_bkg bin 1 size = " << temp_bkg->GetBinContent(10) << endl;
    /*if(normalised){
      cout << "Scaling histos" << endl;
        temp_data->Scale(1/normdata);
        temp_bkg->Scale(1/normbkg);
        cout << "temp_data bin 1 size after scaling = " <<temp_data->GetBinContent(10) << endl;
        cout << "temp_bkg bin 1 size after scaling = " << temp_bkg->GetBinContent(10)<< endl;
    }*/


    TH1F* ratio_plot = (TH1F*)temp_data->Clone("ratio_plot");
    ratio_plot->SetTitle(0);
    ratio_plot->SetMarkerStyle(7);
    ratio_plot->SetMaximum(1.5);
    ratio_plot->SetMinimum(0.5);
    ratio_plot->Sumw2();
    ratio_plot->SetStats(0);
    ratio_plot->Divide(temp_bkg);
    ratio_plot->Draw("E0");
    ratio_plot->SetTitle("");
    ratio_plot->GetYaxis()->SetTitle("Data/MC");
    ratio_plot->GetYaxis()->SetTitleSize(20);
    ratio_plot->GetYaxis()->SetTitleFont(43);
    ratio_plot->GetYaxis()->SetTitleOffset(1.55);
    ratio_plot->GetYaxis()->SetLabelSize(10);
    ratio_plot->GetYaxis()->SetLabelFont(43);
    ratio_plot->GetXaxis()->SetTitle(vartitle.c_str());
    ratio_plot->GetXaxis()->SetTitleSize(20);
    ratio_plot->GetXaxis()->SetTitleFont(43);
    ratio_plot->GetXaxis()->SetTitleOffset(2);

    TLine* line = new TLine(inRange[v],1,endRange[v],1);
    line->SetLineColor(kRed);
    line->SetLineWidth(2);
    line->Draw();

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


  // Histogram titles and dressing
  TGaxis::SetMaxDigits(4);
  stringstream Title_ss;
  Title_ss << "#scale[0.90]{CMS preliminary,   #sqrt{s} = 13 TeV, L = " << (Luminosity/1000) <<" fb^{-1}}";
  string Title_str = Title_ss.str();
  const char* Plot_Title = Title_str.c_str();
  if(!show_ratio) h_data_var->GetXaxis()->SetTitle(vartitle.c_str());
  if(show_title)  h_data_var->SetTitle(Plot_Title);
  if(h_data_var->GetEntries()==0) gStyle->SetOptStat(0);
  h_sig->SetMarkerColor(kGreen+4);
  h_sig->SetLineWidth(2);
  h_sig->SetLineColor(kGreen+4);

  //Draw data and bckg MC

  h_data_var->Draw("PE0");
  hstack->Draw("HIST same");

  if(!normalised) h_data_var->Draw("PE0 same");
  else            h_data_var->Draw("same");
  gPad->RedrawAxis();
  h_sig->Draw("HIST same");


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
  if(name=="ttjets_incl")    col = -10;
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
TLegend* get_legend(string varname){
  double x1;
  double x2;
  double y1;
  double y2;

  if(varname!="BJetness_jetschisgoodtrk" && varname!="BJetness_jetschtrkpur" && varname!="BJetness_jetschpvass"){
    x1 = 0.75;
    y1 = 0.75;
    x2 = 0.95;
    y2 = 0.95;
  }
  if(varname=="BJetness_jetschisgoodtrk" || varname=="BJetness_jetschtrkpur" || varname=="BJetness_jetschpvass"){
    cout << "Change TLegened position" << endl;
    x1 = 0.2;
    y1 = 0.65;
    x2 = 0.45;
    y2 = 0.90;
  }
  //cout << "x1 = " << x1 << endl;
  TLegend *leg = new TLegend(x1,y1,x2,y2);
  leg->SetHeader("Samples");
  leg->SetBorderSize(0);
  leg->SetTextSize(0.03);
  return leg;
}


TH1F* get_th1f(string var, int v){
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
