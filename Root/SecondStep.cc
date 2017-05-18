//=========================================
//              SecondStep.cc
//=========================================
// Institute of High Energy Physics 2016/2017
//=========================================
// Description:
//            - Analysis code to perform 'preselection' for ttHbb analysis.
//            - Code takes as input the .root files from BSMFramework.
//            - Tree structure is copied but slimmed keeping only wanted branches.
//            - Objects defined according:
//              https://gitlab.cern.ch/ttH/reference/blob/master/definitions/Moriond17.md
//            - Loose tHbb analysis pre-selection applied.
//            - Object corrections are applied temporarily to perform selection.
//            - Variables in original tree left unchanged.
//            - Input file is augmented with analysis specific variables.
//            - Pass output to "makeTable.py" to apply final event selections.
//=========================================
// To do:
//        - Update to cut based electron ID and relevant corrections/isolation. Means need to add dz and dxy (were previously included in )
//        - Update global tags
//        - Using old Electron trigger SFs. May need updating in future => "eleTrig_SF". For now (10/01/2016) it is fine.

#include "../interface/SecondStep.h"

// WARNING:: BDT been removed to run on IHEP farm
//#include "TTH/CommonClassifier/interface/BlrBDTClassifier.h"

struct stat sb;

SecondStep::SecondStep(){
  return;
}

SecondStep::~SecondStep(){
  return;
}

void SecondStep::Usage(){
  cout << "Incorrect usage of SecondStep code !!!!!" << endl;
  cout << "SecondStep Usage:" << endl;
  cout << "./SecondStep <input_file_name> <output_dir_path>" << endl;
  cout << "NOTE: For input_file_names, give absolute path and filename." << endl;
  cout << "      For output path no file is needed (uses input filename)." << endl;
  return;
}

void SecondStep::Process(char* inFile, string outDirPath){
  std::cout << "SecondStep.cc::SecondStep()" << std::endl;


  // WARNING:: BDT been removed to run on IHEP farm
  //MEMClassifier mem;

  // 'sample' used to distinguish between data and MC or
  // MC with HLT info from MC without HLT info.
  // > Usage:
  //      0 = data
  //      1 = MC (for MC samples with no HLT info)
  //      2 = reHLT MC (for MC samples with HLT info)

  string infilename=string(inFile);
  string suffix =".root";

  int sample=-999;

  if(infilename.find("/mc/")!=std::string::npos){sample=1;}
  else if(infilename.find("/data/")!=std::string::npos){
    sample=0;
  }
  else{
    cout << "Input file does not fit naming convention!" << endl;
    cout << "Check input file name"<< endl;
    cout << "Path should contain either \'/mc/\' or \'/data/\'" << endl;
    cout << "Input file name = " << infilename << endl;
    exit(0);
  }
  string inputFullPath = infilename;
  const char* Input = inputFullPath.c_str();

  string out_test = "SS_" + inputFullPath.substr(inputFullPath.find_last_of("/")+1);

  std::string outputFullPath = outDirPath+out_test;
  const char * Output = outputFullPath.c_str();

  cout << "Input file: " << Input << endl;
  cout << "Output file: " << Output << endl;
  cout << "Sample Type: " << sample << endl;

  //Get old file, old tree and set top branch address
  TFile *oldfile = TFile::Open(Input);
  if (!oldfile->IsOpen()) throw "Input file could not be opened.";

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
  oldtree->SetBranchStatus("Jet_energy",1);
  oldtree->SetBranchStatus("Jet_JesSF",1);
  oldtree->SetBranchStatus("Jet_JerSF",1);
  oldtree->SetBranchStatus("Jet_JesSFup",1);
  oldtree->SetBranchStatus("Jet_JerSFup",1);
  oldtree->SetBranchStatus("Jet_JesSFdown",1);
  oldtree->SetBranchStatus("Jet_JerSFdown",1);
  oldtree->SetBranchStatus("Jet_genpt",1);
  oldtree->SetBranchStatus("Jet_geneta",1);
  oldtree->SetBranchStatus("Jet_genphi",1);
  oldtree->SetBranchStatus("Jet_genenergy",1);
  oldtree->SetBranchStatus("Jet_newpfCombinedInclusiveSecondaryVertexV2BJetTags",1);
  oldtree->SetBranchStatus("Jet_pfCombinedInclusiveSecondaryVertexV2BJetTags",1);
  oldtree->SetBranchStatus("Jet_neutralHadEnergyFraction",1);
  oldtree->SetBranchStatus("Jet_chargedEmEnergyFraction",1);
  oldtree->SetBranchStatus("Jet_neutralEmEnergyFraction",1);
  oldtree->SetBranchStatus("Jet_numberOfConstituents",1);
  oldtree->SetBranchStatus("Jet_chargedHadronEnergyFraction",1);
  oldtree->SetBranchStatus("Jet_chargedMultiplicity",1);


  oldtree->SetBranchStatus("BJetness_isSingleLepton",1);
  oldtree->SetBranchStatus("BJetness_isDoubleLepton",1);
  //Gen info
  oldtree->SetBranchStatus("BJetness_ngenbh",1);
  oldtree->SetBranchStatus("BJetness_ngenbt",1);
  oldtree->SetBranchStatus("BJetness_ngenb",1);
  oldtree->SetBranchStatus("BJetness_ngenc",1);
  oldtree->SetBranchStatus("BJetness_partonFlavour",1);
  oldtree->SetBranchStatus("BJetness_hadronFlavour",1);
  oldtree->SetBranchStatus("BJetness_numjet",1);
  //Kinematics and csv
  oldtree->SetBranchStatus("BJetness_jetpt",1);
  oldtree->SetBranchStatus("BJetness_jeteta",1);
  oldtree->SetBranchStatus("BJetness_jetphi",1);
  oldtree->SetBranchStatus("BJetness_jetenergy",1);
  oldtree->SetBranchStatus("BJetness_jetcsv",1);
  oldtree->SetBranchStatus("BJetness_pfJetProbabilityBJetTags",1);
  oldtree->SetBranchStatus("BJetness_pfCombinedMVAV2BJetTags",1);
  //oldtree->SetBranchStatus("BJetness_pfCombinedCvsLJetTags ",1);
  oldtree->SetBranchStatus("BJetness_pfCombinedCvsBJetTags",1);
  oldtree->SetBranchStatus("BJetness_pt",1);
  oldtree->SetBranchStatus("BJetness_eta",1);
  oldtree->SetBranchStatus("BJetness_phi",1);
  oldtree->SetBranchStatus("BJetness_en",1);
  oldtree->SetBranchStatus("BJetness_ptOVen",1);
  //PFcand info
  oldtree->SetBranchStatus("BJetness_jetschpvass",1);
  oldtree->SetBranchStatus("BJetness_jetschfrompv",1);
  oldtree->SetBranchStatus("BJetness_jetschip3dval",1);
  oldtree->SetBranchStatus("BJetness_jetschip3dsig",1);
  oldtree->SetBranchStatus("BJetness_jetschip2dval",1);
  oldtree->SetBranchStatus("BJetness_jetschip2dsig",1);
  oldtree->SetBranchStatus("BJetness_jetschisgoodtrk",1);
  oldtree->SetBranchStatus("BJetness_jetschtrkpur",1);
  oldtree->SetBranchStatus("BJetness_jetschpt",1);
  oldtree->SetBranchStatus("BJetness_jetscheta",1);
  oldtree->SetBranchStatus("BJetness_jetschen",1);
  //Num_of_trks
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
  oldtree->SetBranchStatus("BJetness_npvTrkOVcollTrk",1);
  oldtree->SetBranchStatus("BJetness_pvTrkOVcollTrk",1);
  oldtree->SetBranchStatus("BJetness_npvTrkOVpvTrk",1);
  oldtree->SetBranchStatus("BJetness_npvPtOVcollPt",1);
  oldtree->SetBranchStatus("BJetness_pvPtOVcollPt",1);
  oldtree->SetBranchStatus("BJetness_npvPtOVpvPt",1);
  //Trk prop rel to jet dir
  oldtree->SetBranchStatus("BJetness_avprel",1);
  oldtree->SetBranchStatus("BJetness_avppar",1);
  oldtree->SetBranchStatus("BJetness_avetarel",1);
  oldtree->SetBranchStatus("BJetness_avetapar",1);
  oldtree->SetBranchStatus("BJetness_avdr",1);
  oldtree->SetBranchStatus("BJetness_avpreljetpt",1);
  oldtree->SetBranchStatus("BJetness_avpreljeten",1);
  oldtree->SetBranchStatus("BJetness_avpparjetpt",1);
  oldtree->SetBranchStatus("BJetness_avpparjeten",1);
  //Two_trk_info
  oldtree->SetBranchStatus("BJetness_avnum2v",1);
  oldtree->SetBranchStatus("BJetness_avnumno2v",1);
  oldtree->SetBranchStatus("BJetness_avdca3d2t",1);
  oldtree->SetBranchStatus("BJetness_avdca3dno2t",1);
  oldtree->SetBranchStatus("BJetness_avdca3d",1);
  oldtree->SetBranchStatus("BJetness_avdca2d2t",1);
  oldtree->SetBranchStatus("BJetness_avdca2dno2t",1);
  oldtree->SetBranchStatus("BJetness_avdca2d",1);
  //chi2
  oldtree->SetBranchStatus("BJetness_chi2",1);
  //ImpactParameter
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
  oldtree->SetBranchStatus("patElectron_pdgId",1);
  oldtree->SetBranchStatus("patElectron_energy",1);
  oldtree->SetBranchStatus("patElectron_SCeta",1);
  oldtree->SetBranchStatus("patElectron_gsfTrack_dz_pv",1);
  oldtree->SetBranchStatus("patElectron_d0",1);
  oldtree->SetBranchStatus("patElectron_passConversionVeto",1);
  oldtree->SetBranchStatus("patElectron_inCrack",1);
  oldtree->SetBranchStatus("patElectron_isPassMvatrig",1);
  oldtree->SetBranchStatus("patElectron_isPassMvanontrig",1);
  oldtree->SetBranchStatus("patElectron_isPassMedium",1);
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
  oldtree->SetBranchStatus("Muon_pdgId",1);
  oldtree->SetBranchStatus("Muon_energy",1);
  oldtree->SetBranchStatus("Muon_charge",1);
  oldtree->SetBranchStatus("Muon_loose",1);
  oldtree->SetBranchStatus("Muon_tight",1);
  oldtree->SetBranchStatus("Muon_TrackerIso",1);
  oldtree->SetBranchStatus("Muon_isHighPt",1);
  oldtree->SetBranchStatus("Muon_relIsoDeltaBetaR04",1);
  oldtree->SetBranchStatus("HLT_Ele27_eta2p1_WPTight_Gsf",1);//<< SL e
  oldtree->SetBranchStatus("HLT_IsoMu24",1);//<< SL mu
  oldtree->SetBranchStatus("HLT_IsoTkMu24",1);//<< SL mu
  oldtree->SetBranchStatus("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ",1);//<< DL ee
  oldtree->SetBranchStatus("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL",1);//<< DL emu
  oldtree->SetBranchStatus("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ",1);//<< DL emu
  oldtree->SetBranchStatus("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL",1);//<< DL emu
  oldtree->SetBranchStatus("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ",1);//<< DL emu
  oldtree->SetBranchStatus("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL",1);//<< DL mumu
  oldtree->SetBranchStatus("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ",1);//<< DL mumu
  oldtree->SetBranchStatus("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL",1);//<< DL mumu
  oldtree->SetBranchStatus("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ",1);//<< DL mumu
  oldtree->SetBranchStatus("EVENT_Q2tthbbWeightUp",1);
  oldtree->SetBranchStatus("EVENT_Q2tthbbWeightDown",1);
  oldtree->SetBranchStatus("EVENT_PDFtthbbWeightUp",1);
  oldtree->SetBranchStatus("EVENT_PDFtthbbWeightDown",1);
  oldtree->SetBranchStatus("EVENT_event",1);
  oldtree->SetBranchStatus("EVENT_run",1);
  oldtree->SetBranchStatus("EVENT_genWeight",1);
  oldtree->SetBranchStatus("EVENT_lumiBlock",1);
  oldtree->SetBranchStatus("PUWeight",1);
  oldtree->SetBranchStatus("trueInteractions",1);
  oldtree->SetBranchStatus("pvertex_ndof",1);
  oldtree->SetBranchStatus("pvertex_z",1);
  oldtree->SetBranchStatus("npuVertices",1);
  oldtree->SetBranchStatus("npuVerticesm1",1);
  oldtree->SetBranchStatus("npuVerticesp1",1);
  //==========================================
  // HACK pvertex_Rho not in first ntuples
  //==========================================
  //oldtree->SetBranchStatus("pvertex_Rho",1);
  //==========================================

  oldtree->SetBranchStatus("Met_type1PF_pt",1);
  oldtree->SetBranchStatus("Met_type1PF_px",1);
  oldtree->SetBranchStatus("Met_type1PF_py",1);
  oldtree->SetBranchStatus("Met_type1PF_phi",1);

  oldtree->SetBranchStatus("Flag_HBHENoiseFilter",1);
  oldtree->SetBranchStatus("Flag_HBHENoiseIsoFilter",1);
  oldtree->SetBranchStatus("Flag_CSCTightHalo2015Filter",1);
  oldtree->SetBranchStatus("Flag_EcalDeadCellTriggerPrimitiveFilter",1);
  oldtree->SetBranchStatus("Flag_goodVertices",1);

  //==========================================
  // HACK Flag_globalTightHalo2016Filter not in first ntuples
  //==========================================
  //oldtree->SetBranchStatus("Flag_globalTightHalo2016Filter",1);
  //==========================================

  oldtree->SetBranchStatus("Flag_eeBadScFilter",1);
  //==========================================
  // HACK EVENT_filterBadGlobalMuonTagger and EVENT_filtercloneGlobalMuonTagger not in first ntuples
  //==========================================
  //oldtree->SetBranchStatus("EVENT_filterBadGlobalMuonTagger",1);
  //oldtree->SetBranchStatus("EVENT_filtercloneGlobalMuonTagger",1);

  if(sample==1 || sample==2){
    oldtree->SetBranchStatus("ttHFCategory",1);
    oldtree->SetBranchStatus("Gen_pt",1);
    oldtree->SetBranchStatus("Gen_eta",1);
    oldtree->SetBranchStatus("Gen_phi",1);
    oldtree->SetBranchStatus("Gen_pdg_id",1);
    oldtree->SetBranchStatus("Gen_motherpdg_id",1);
  }

  //Create a new file + a clone of old tree in new file
  std::cout << "SecondStep()::Create new file + clone of old tree in file file." << std::endl;

  TFile *newfile = new TFile(Output,"recreate");
  TTree *newtree = oldtree->CloneTree(0); // Fill with (0) events initially but preserve tree structure.
  TTree *newevtree = evtree->CloneTree();

  cout << "File cloning complete." << endl;

  //NEW VARIABLES
  int data_stream_=-99;
  TBranch* data_stream = newtree->Branch("data_stream",&data_stream_,"data_stream/i");
  if(sample==0){
    if(infilename.find("DoubleEG")!=std::string::npos){data_stream_=11;}
    if(infilename.find("DoubleMuon")!=std::string::npos){data_stream_=22;}
    if(infilename.find("MuonEG")!=std::string::npos){data_stream_=12;}
    if(infilename.find("SingleElectron")!=std::string::npos){data_stream_=1;}
    if(infilename.find("SingleMuon")!=std::string::npos){data_stream_=2;}
  }
  //cout << "data_stream_ = " << data_stream_ << endl;

// WARNING:: BDT been removed to run on IHEP farm
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

  double sphericity_=-99;         TBranch *sphericity=newtree->Branch("sphericity",&sphericity_,"sphericity/D");
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


  int number_electrons_ = -99;
  TBranch* number_electrons = newtree->Branch("number_electrons",&number_electrons_,"number_electrons/I");
  double lead_el_pt_ = -99;
  TBranch* lead_el_pt = newtree->Branch("lead_el_pt",&lead_el_pt_,"lead_el_pt/D");
  double lead_el_eta_ = -99;
  TBranch* lead_el_eta = newtree->Branch("lead_el_eta",&lead_el_eta_,"lead_el_eta/D");
  double lead_el_phi_ = -99;
  TBranch* lead_el_phi = newtree->Branch("lead_el_phi",&lead_el_phi_,"lead_el_phi/D");

  int number_muons_ = -99;
  TBranch* number_muons = newtree->Branch("number_muons",&number_muons_,"number_muons/I");
  double lead_mu_pt_ = -99;
  TBranch* lead_mu_pt = newtree->Branch("lead_mu_pt",&lead_mu_pt_,"lead_mu_pt/D");
  double lead_mu_eta_ = -99;
  TBranch* lead_mu_eta = newtree->Branch("lead_mu_eta",&lead_mu_eta_,"lead_mu_eta/D");
  double lead_mu_phi_ = -99;
  TBranch* lead_mu_phi = newtree->Branch("lead_mu_phi",&lead_mu_phi_,"lead_mu_phi/D");

  int number_leptons_ = -99;
  TBranch* number_leptons = newtree->Branch("number_leptons",&number_leptons_,"number_leptons/I");
  double lead_lep_pt_ = -99;
  TBranch* lead_lep_pt = newtree->Branch("lead_lep_pt",&lead_lep_pt_,"lead_lep_pt/D");
  double lead_lep_eta_ = -99;
  TBranch* lead_lep_eta = newtree->Branch("lead_lep_eta",&lead_lep_eta_,"lead_lep_eta/D");
  double lead_lep_phi_ = -99;
  TBranch* lead_lep_phi = newtree->Branch("lead_lep_phi",&lead_lep_phi_,"lead_lep_phi/D");

  double first_jet_pt_=-99;
  TBranch *first_jet_pt=newtree->Branch("first_jet_pt",&first_jet_pt_,"first_jet_pt/D");
  double first_highest_btag_=-99;
  TBranch *first_highest_btag=newtree->Branch("first_highest_btag",&first_highest_btag_,"first_highest_btag/D");

  double second_jet_pt_=-99;
  TBranch *second_jet_pt=newtree->Branch("second_jet_pt",&second_jet_pt_,"second_jet_pt/D");
  double second_highest_btag_=-99;
  TBranch *second_highest_btag=newtree->Branch("second_highest_btag",&second_highest_btag_,"second_highest_btag/D");

  double third_jet_pt_=-99;
  TBranch *third_jet_pt=newtree->Branch("third_jet_pt",&third_jet_pt_,"third_jet_pt/D");
  double third_highest_btag_=-99;
  TBranch *third_highest_btag=newtree->Branch("third_highest_btag",&third_highest_btag_,"third_highest_btag/D");

  double fourth_jet_pt_=-99;
  TBranch *fourth_jet_pt=newtree->Branch("fourth_jet_pt",&fourth_jet_pt_,"fourth_jet_pt/D");
  double fourth_highest_btag_=-99;
  TBranch *fourth_highest_btag=newtree->Branch("fourth_highest_btag",&fourth_highest_btag_,"fourth_highest_btag/D");

  double fifth_highest_CSV_=-99;
  TBranch *fifth_highest_CSV=newtree->Branch("fifth_highest_CSV",&fifth_highest_CSV_,"fifth_highest_CSV/D");
  //Selection Variables:
  bool is_e_ = false;
  TBranch* is_e = newtree->Branch("is_e",&is_e_ ,"is_e/O");
  bool is_mu_ = false;
  TBranch* is_mu =newtree->Branch("is_mu",&is_mu_ ,"is_mu/O");

  double trigger_SF_ = 1;
  TBranch* trigger_SF = newtree->Branch("trigger_SF",&trigger_SF_,"trigger_SF/D");

  //MUONS LOAD
  vector<double>* Muon_pt=0;
  vector<double>* Muon_eta=0;
  vector<double>* Muon_phi=0;
  vector<double>* Muon_energy=0;
  vector<double>* Muon_relIsoDeltaBetaR04=0;
  vector<int>* Muon_tight=0;
  vector<int>* Muon_loose=0;
  TBranch *b_Muon_pt,*b_Muon_eta,*b_Muon_phi,*b_Muon_energy,*b_Muon_relIsoDeltaBetaR04,*b_Muon_tight,*b_Muon_loose;
  oldtree->SetBranchAddress("Muon_pt", &Muon_pt, &b_Muon_pt);
  oldtree->SetBranchAddress("Muon_eta",&Muon_eta,&b_Muon_eta);
  oldtree->SetBranchAddress("Muon_phi",&Muon_phi,&b_Muon_phi);
  oldtree->SetBranchAddress("Muon_energy",&Muon_energy,&b_Muon_energy);
  oldtree->SetBranchAddress("Muon_relIsoDeltaBetaR04",&Muon_relIsoDeltaBetaR04,&b_Muon_relIsoDeltaBetaR04);
  oldtree->SetBranchAddress("Muon_tight",&Muon_tight,&b_Muon_tight);
  oldtree->SetBranchAddress("Muon_loose",&Muon_loose,&b_Muon_loose);

  //ELECTRON LOAD
  vector<double>* patElectron_passConversionVeto=0;
  vector<double>* patElectron_inCrack=0;
  vector<double>* patElectron_pt=0;
  vector<double>* patElectron_eta=0;
  vector<double>* patElectron_SCeta=0;
  vector<double>* patElectron_phi=0;
  vector<double>* patElectron_energy=0;
  vector<double>* patElectron_d0=0;
  vector<double>* patElectron_gsfTrack_dz_pv=0;
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
  vector<int> *patElectron_isPassMedium=0;
  TBranch *b_patElectron_pt,*b_patElectron_eta,*b_patElectron_SCeta,*b_patElectron_phi,*b_patElectron_energy,*b_patElectron_d0,*b_patElectron_gsfTrack_dz_pv,*b_patElectron_isPassMvatrig,*b_patElectron_isPassMvanontrig,*b_patElectron_inCrack,*b_patElectron_passConversionVeto,*b_patElectron_full5x5_sigmaIetaIeta,*b_patElectron_hOverE,*b_patElectron_ecalPFClusterIso,*b_patElectron_hcalPFClusterIso,*b_patElectron_isolPtTracks,*b_patElectron_dEtaIn,*b_patElectron_dPhiIn,*b_patElectron_relIsoRhoEA, *b_patElectron_isPassMedium;
  oldtree->SetBranchAddress("patElectron_pt", &patElectron_pt, &b_patElectron_pt);
  oldtree->SetBranchAddress("patElectron_eta", &patElectron_eta, &b_patElectron_eta);
  oldtree->SetBranchAddress("patElectron_SCeta", &patElectron_SCeta, &b_patElectron_SCeta);
  oldtree->SetBranchAddress("patElectron_phi", &patElectron_phi, &b_patElectron_phi);
  oldtree->SetBranchAddress("patElectron_energy", &patElectron_energy, &b_patElectron_energy);
  oldtree->SetBranchAddress("patElectron_inCrack", &patElectron_inCrack, &b_patElectron_inCrack);
  oldtree->SetBranchAddress("patElectron_d0", &patElectron_d0, &b_patElectron_d0);
  oldtree->SetBranchAddress("patElectron_gsfTrack_dz_pv", &patElectron_gsfTrack_dz_pv, &b_patElectron_gsfTrack_dz_pv);
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
  oldtree->SetBranchAddress("patElectron_isPassMedium",&patElectron_isPassMedium,&b_patElectron_isPassMedium);

  //JET LOAD
  vector<double>* Jet_pt=0;
  vector<double>* Jet_Uncorr_pt=0;
  vector<double>* Jet_eta=0;
  vector<double>* Jet_phi=0;
  vector<double>* Jet_energy=0;
  vector<double>* Jet_genpt=0;
  vector<double>* Jet_geneta=0;
  vector<double>* Jet_genphi=0;
  vector<double>* Jet_genenergy=0;
  vector<double>* Jet_newpfCombinedInclusiveSecondaryVertexV2BJetTags=0;
  vector<double>* Jet_pfCombinedInclusiveSecondaryVertexV2BJetTags=0;
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
  TBranch *b_Jet_pt,*b_Jet_Uncorr_pt,*b_Jet_eta,*b_Jet_phi,*b_Jet_energy,*b_Jet_neutralHadEnergyFraction,*b_Jet_chargedEmEnergyFraction,*b_Jet_neutralEmEnergyFraction,*b_Jet_mass,*b_Jet_JesSF,*b_Jet_JesSFup,*b_Jet_JesSFdown;
  TBranch *b_Jet_newpfCombinedInclusiveSecondaryVertexV2BJetTags,*b_Jet_pfCombinedInclusiveSecondaryVertexV2BJetTags,*b_Jet_numberOfConstituents,*b_Jet_chargedHadronEnergyFraction,*b_Jet_chargedMultiplicity,*b_Jet_JerSF,*b_Jet_JerSFup,*b_Jet_JerSFdown;
  TBranch *b_Jet_genpt, *b_Jet_geneta, *b_Jet_genphi, *b_Jet_genenergy;
  oldtree->SetBranchAddress("Jet_pt", &Jet_pt, &b_Jet_pt);
  oldtree->SetBranchAddress("Jet_Uncorr_pt", &Jet_Uncorr_pt, &b_Jet_Uncorr_pt);
  oldtree->SetBranchAddress("Jet_eta", &Jet_eta, &b_Jet_eta);
  oldtree->SetBranchAddress("Jet_phi", &Jet_phi, &b_Jet_phi);
  oldtree->SetBranchAddress("Jet_energy", &Jet_energy, &b_Jet_energy);
  oldtree->SetBranchAddress("Jet_genpt", &Jet_genpt, &b_Jet_genpt);
  oldtree->SetBranchAddress("Jet_geneta", &Jet_geneta, &b_Jet_geneta);
  oldtree->SetBranchAddress("Jet_genphi", &Jet_genphi, &b_Jet_genphi);
  oldtree->SetBranchAddress("Jet_genenergy", &Jet_genenergy, &b_Jet_genenergy);
  oldtree->SetBranchAddress("Jet_newpfCombinedInclusiveSecondaryVertexV2BJetTags", &Jet_newpfCombinedInclusiveSecondaryVertexV2BJetTags, &b_Jet_newpfCombinedInclusiveSecondaryVertexV2BJetTags);
  oldtree->SetBranchAddress("Jet_pfCombinedInclusiveSecondaryVertexV2BJetTags", &Jet_pfCombinedInclusiveSecondaryVertexV2BJetTags, &b_Jet_pfCombinedInclusiveSecondaryVertexV2BJetTags);
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

  double bWeight, bWeightLFup, bWeightLFdown, bWeightHFup, bWeightHFdown, bWeightHFStats1up, bWeightHFStats1down, bWeightLFStats1up, bWeightLFStats1down, bWeightHFStats2up, bWeightHFStats2down, bWeightLFStats2up, bWeightLFStats2down, bWeightCErr1up, bWeightCErr1down, bWeightCErr2up, bWeightCErr2down, bWeightJESup, bWeightJESdown;

  int BJetness_isSingleLepton=0;
  int BJetness_isDoubleLepton=0;
  int BJetness_ngenbh=0;
  int BJetness_ngenbt=0;
  int BJetness_ngenb=0;
  int BJetness_ngenc=0;
  vector<double>* BJetness_partonFlavour = 0;
  vector<double>* BJetness_hadronFlavour = 0;
  vector<double>* BJetness_numjet = 0;
  vector<double>* BJetness_jetpt = 0;
  vector<double>* BJetness_jeteta = 0;
  vector<double>* BJetness_jetphi = 0;
  vector<double>* BJetness_jetenergy = 0;
  vector<double>* BJetness_jetcsv = 0;
  vector<double>* BJetness_pfJetProbabilityBJetTags = 0;
  vector<double>* BJetness_pfCombinedMVAV2BJetTags = 0;
  vector<double>* BJetness_pfCombinedCvsLJetTags = 0;
  vector<double>* BJetness_pfCombinedCvsBJetTags = 0;
  vector<double>* BJetness_pt = 0;
  vector<double>* BJetness_eta = 0;
  vector<double>* BJetness_phi = 0;
  vector<double>* BJetness_en = 0;
  vector<double>* BJetness_ptOVen = 0;
  vector<double>* BJetness_jetschpvass = 0;
  vector<double>* BJetness_jetschfrompv = 0;
  vector<double>* BJetness_jetschip3dval = 0;
  vector<double>* BJetness_jetschip3dsig = 0;
  vector<double>* BJetness_jetschip2dval = 0;
  vector<double>* BJetness_jetschip2dsig = 0;
  vector<double>* BJetness_jetschisgoodtrk = 0;
  vector<double>* BJetness_jetschtrkpur = 0;
  vector<double>* BJetness_jetschpt = 0;
  vector<double>* BJetness_jetscheta = 0;
  vector<double>* BJetness_jetschen = 0;
  vector<double>* BJetness_num_pdgid_eles = 0;
  vector<double>* BJetness_num_soft_eles = 0;
  vector<double>* BJetness_num_vetonoipnoiso_eles = 0;
  vector<double>* BJetness_num_loosenoipnoiso_eles = 0;
  vector<double>* BJetness_num_veto_eles = 0;
  vector<double>* BJetness_num_loose_eles = 0;
  vector<double>* BJetness_num_medium_eles = 0;
  vector<double>* BJetness_num_tight_eles = 0;
  vector<double>* BJetness_num_mvatrig_eles = 0;
  vector<double>* BJetness_num_mvanontrig_eles = 0;
  vector<double>* BJetness_num_mvatrigwp90_eles = 0;
  vector<double>* BJetness_num_mvanontrigwp90_eles = 0;
  vector<double>* BJetness_num_heep_eles = 0;
  vector<double>* BJetness_num_pdgid_mus = 0;
  vector<double>* BJetness_num_loose_mus = 0;
  vector<double>* BJetness_num_soft_mus = 0;
  vector<double>* BJetness_num_medium_mus = 0;
  vector<double>* BJetness_num_tight_mus = 0;
  vector<double>* BJetness_num_highpt_mus = 0;
  vector<double>* BJetness_num_POGisGood_mus = 0;
  vector<double>* BJetness_numjettrks = 0;
  vector<double>* BJetness_numjettrkspv = 0;
  vector<double>* BJetness_numjettrksnopv = 0;
  vector<double>* BJetness_npvTrkOVcollTrk = 0;
  vector<double>* BJetness_pvTrkOVcollTrk = 0;
  vector<double>* BJetness_npvTrkOVpvTrk = 0;
  vector<double>* BJetness_npvPtOVcollPt = 0;
  vector<double>* BJetness_pvPtOVcollPt = 0;
  vector<double>* BJetness_npvPtOVpvPt = 0;
  vector<double>* BJetness_avprel = 0;
  vector<double>* BJetness_avppar = 0;
  vector<double>* BJetness_avetarel = 0;
  vector<double>* BJetness_avetapar = 0;
  vector<double>* BJetness_avdr = 0;
  vector<double>* BJetness_avpreljetpt = 0;
  vector<double>* BJetness_avpreljeten = 0;
  vector<double>* BJetness_avpparjetpt = 0;
  vector<double>* BJetness_avpparjeten = 0;
  vector<double>* BJetness_avnum2v = 0;
  vector<double>* BJetness_avnumno2v = 0;
  vector<double>* BJetness_avdca3d2t = 0;
  vector<double>* BJetness_avdca3dno2t = 0;
  vector<double>* BJetness_avdca3d = 0;
  vector<double>* BJetness_avdca2d2t = 0;
  vector<double>* BJetness_avdca2dno2t = 0;
  vector<double>* BJetness_avdca2d = 0;
  vector<double>* BJetness_chi2 = 0;
  vector<double>* BJetness_avip3d_val = 0;
  vector<double>* BJetness_avip3d_sig = 0;
  vector<double>* BJetness_avsip3d_val = 0;
  vector<double>* BJetness_avsip3d_sig = 0;
  vector<double>* BJetness_numip3dpos = 0;
  vector<double>* BJetness_numip3dneg = 0;
  vector<double>* BJetness_avip2d_val = 0;
  vector<double>* BJetness_avip2d_sig = 0;
  vector<double>* BJetness_avsip2d_val = 0;
  vector<double>* BJetness_avsip2d_sig = 0;
  vector<double>* BJetness_numip2dpos = 0;
  vector<double>* BJetness_numip2dneg = 0;
  vector<double>* BJetness_avip1d_val = 0;
  vector<double>* BJetness_avip1d_sig = 0;
  vector<double>* BJetness_avsip1d_val = 0;
  vector<double>* BJetness_avsip1d_sig = 0;

  TBranch *b_BJetness_isSingleLepton;
  TBranch*b_BJetness_isDoubleLepton;
  TBranch*b_BJetness_ngenbh;
  TBranch *b_BJetness_ngenbt;
  TBranch *b_BJetness_ngenb;
  TBranch *b_BJetness_ngenc;
  TBranch *b_BJetness_partonFlavour;
  TBranch *b_BJetness_hadronFlavour;
  TBranch *b_BJetness_numjet;
  TBranch *b_BJetness_jetpt;
  TBranch *b_BJetness_jeteta;
  TBranch *b_BJetness_jetphi;
  TBranch *b_BJetness_jetenergy;
  TBranch *b_BJetness_jetcsv;
  TBranch *b_BJetness_pfJetProbabilityBJetTags;
  TBranch *b_BJetness_pfCombinedMVAV2BJetTags;
  TBranch *b_BJetness_pfCombinedCvsLJetTags;
  TBranch *b_BJetness_pfCombinedCvsBJetTags;
  TBranch *b_BJetness_pt;
  TBranch *b_BJetness_eta;
  TBranch *b_BJetness_phi;
  TBranch *b_BJetness_en;
  TBranch *b_BJetness_ptOVen;
  TBranch *b_BJetness_jetschpvass;
  TBranch *b_BJetness_jetschfrompv;
  TBranch *b_BJetness_jetschip3dval;
  TBranch *b_BJetness_jetschip3dsig;
  TBranch *b_BJetness_jetschip2dval;
  TBranch *b_BJetness_jetschip2dsig;
  TBranch *b_BJetness_jetschisgoodtrk;
  TBranch *b_BJetness_jetschtrkpur;
  TBranch *b_BJetness_jetschpt;
  TBranch *b_BJetness_jetscheta;
  TBranch *b_BJetness_jetschen;
  TBranch *b_BJetness_num_pdgid_eles;
  TBranch *b_BJetness_num_soft_eles;
  TBranch *b_BJetness_num_vetonoipnoiso_eles;
  TBranch *b_BJetness_num_loosenoipnoiso_eles;
  TBranch *b_BJetness_num_veto_eles;
  TBranch *b_BJetness_num_loose_eles;
  TBranch *b_BJetness_num_medium_eles;
  TBranch *b_BJetness_num_tight_eles;
  TBranch *b_BJetness_num_mvatrig_eles;
  TBranch *b_BJetness_num_mvanontrig_eles;
  TBranch *b_BJetness_num_mvatrigwp90_eles;
  TBranch *b_BJetness_num_mvanontrigwp90_eles;
  TBranch *b_BJetness_num_heep_eles;
  TBranch *b_BJetness_num_pdgid_mus;
  TBranch *b_BJetness_num_loose_mus;
  TBranch *b_BJetness_num_soft_mus;
  TBranch *b_BJetness_num_medium_mus;
  TBranch *b_BJetness_num_tight_mus;
  TBranch *b_BJetness_num_highpt_mus;
  TBranch *b_BJetness_num_POGisGood_mus;
  TBranch *b_BJetness_numjettrks;
  TBranch *b_BJetness_numjettrkspv;
  TBranch *b_BJetness_numjettrksnopv;
  TBranch *b_BJetness_npvTrkOVcollTrk;
  TBranch *b_BJetness_pvTrkOVcollTrk;
  TBranch *b_BJetness_npvTrkOVpvTrk;
  TBranch *b_BJetness_npvPtOVcollPt;
  TBranch *b_BJetness_pvPtOVcollPt;
  TBranch *b_BJetness_npvPtOVpvPt;
  TBranch *b_BJetness_avprel;
  TBranch *b_BJetness_avppar;
  TBranch *b_BJetness_avetarel;
  TBranch *b_BJetness_avetapar;
  TBranch *b_BJetness_avdr;
  TBranch *b_BJetness_avpreljetpt;
  TBranch *b_BJetness_avpreljeten;
  TBranch *b_BJetness_avpparjetpt;
  TBranch *b_BJetness_avpparjeten;
  TBranch *b_BJetness_avnum2v;
  TBranch *b_BJetness_avnumno2v;
  TBranch *b_BJetness_avdca3d2t;
  TBranch *b_BJetness_avdca3dno2t;
  TBranch *b_BJetness_avdca3d;
  TBranch *b_BJetness_avdca2d2t;
  TBranch *b_BJetness_avdca2dno2t;
  TBranch *b_BJetness_avdca2d;
  TBranch *b_BJetness_chi2;
  TBranch *b_BJetness_avip3d_val;
  TBranch *b_BJetness_avip3d_sig;
  TBranch *b_BJetness_avsip3d_val;
  TBranch *b_BJetness_avsip3d_sig;
  TBranch *b_BJetness_numip3dpos;
  TBranch *b_BJetness_numip3dneg;
  TBranch *b_BJetness_avip2d_val;
  TBranch *b_BJetness_avip2d_sig;
  TBranch *b_BJetness_avsip2d_val;
  TBranch *b_BJetness_avsip2d_sig;
  TBranch *b_BJetness_numip2dpos;
  TBranch *b_BJetness_numip2dneg;
  TBranch *b_BJetness_avip1d_val;
  TBranch *b_BJetness_avip1d_sig;
  TBranch *b_BJetness_avsip1d_val;
  TBranch *b_BJetness_avsip1d_sig;


  //oldtree->SetBranchAddress("BJetness_jetpt0",&BJetness_jetpt0,&b_BJetness_jetpt0);
  //oldtree->SetBranchAddress("BJetness_numleps",&BJetness_numleps,&b_BJetness_numleps);
  //oldtree->SetBranchAddress("BJetness_avip3dval",&BJetness_avip3dval,&b_BJetness_avip3dval);
  //oldtree->SetBranchAddress("BJetness_avip3dsig",&BJetness_avip3dsig,&b_BJetness_avip3dsig);
  //oldtree->SetBranchAddress("BJetness_avsip3dsig",&BJetness_avsip3dsig,&b_BJetness_avsip3dsig);
  //oldtree->SetBranchAddress("BJetness_avip1dsig",&BJetness_avip1dsig,&b_BJetness_avip1dsig);
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
  //oldtree->SetBranchAddress("BJetness_pfCombinedCvsLJetTags",&BJetness_pfCombinedCvsLJetTags,&b_BJetness_pfCombinedCvsLJetTags);
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
  oldtree->SetBranchAddress("BJetness_jetscheta",&BJetness_jetscheta,&b_BJetness_jetscheta);
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
  oldtree->SetBranchAddress("BJetness_npvTrkOVcollTrk",&BJetness_npvTrkOVcollTrk,&b_BJetness_npvTrkOVcollTrk);
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

  TBranch *b_bWeight, *b_bWeightLFup, *b_bWeightLFdown, *b_bWeightHFup, *b_bWeightHFdown, *b_bWeightHFStats1up, *b_bWeightHFStats1down, *b_bWeightLFStats1up, *b_bWeightLFStats1down, *b_bWeightHFStats2up, *b_bWeightHFStats2down, *b_bWeightLFStats2up, *b_bWeightLFStats2down, *b_bWeightCErr1up, *b_bWeightCErr1down, *b_bWeightCErr2up, *b_bWeightCErr2down, *b_bWeightJESup, *b_bWeightJESdown;

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
  int HLT_IsoMu24=0;
  int HLT_IsoTkMu24=0;
  int HLT_Ele27_eta2p1_WPTight_Gsf=0;
  int HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL=0;
  int HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ=0;
  int HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL=0;
  int HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ=0;
  int HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL=0;
  int HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ=0;
  int HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL=0;
  int HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ=0;
  int HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ=0;

  int ttHFCategory=0;
  ULong64_t EVENT_event;
  int EVENT_run,EVENT_lumiBlock;
  double EVENT_genWeight;
  double trueInteractions;
  double EVENT_Q2tthbbWeightUp,EVENT_Q2tthbbWeightDown,EVENT_PDFtthbbWeightUp,EVENT_PDFtthbbWeightDown,PUWeight;
  double Met_type1PF_pt = -99.;
  double Met_type1PF_px = -99.;
  double Met_type1PF_py = -99.;
  double Met_type1PF_phi = -99.;
  int Flag_CSCTightHalo2015Filter = -99;

  int Flag_goodVertices = -99;

  //==========================================
  // HACK Flag_globalTightHalo2016Filter not in first ntuples
  //==========================================
  int Flag_globalTightHalo2016Filter = -99;

  int Flag_HBHENoiseFilter = -99;
  int Flag_HBHENoiseIsoFilter = -99;
  int Flag_EcalDeadCellTriggerPrimitiveFilter = -99;
  int Flag_eeBadScFilter = -99;
  //==========================================
  // HACK EVENT_filterBadGlobalMuonTagger and EVENT_filtercloneGlobalMuonTagger not in first ntuples
  //==========================================
  bool EVENT_filterBadGlobalMuonTagger = 0;
  bool EVENT_filtercloneGlobalMuonTagger = 0;

  vector<double>* Gen_pt=0;
  vector<double>* Gen_eta=0;
  vector<double>* Gen_phi=0;
  vector<int>* Gen_motherpdg_id=0;
  vector<int>* Gen_pdg_id=0;
  vector<double>* pvertex_ndof=0;
  //==========================================
  // HACK pvertex_Rho not in first ntuples
  //==========================================
  vector<double>* pvertex_Rho=0;
  //==========================================

  vector<double>* pvertex_z=0;
  TBranch *b_HLT_IsoMu22,*b_HLT_IsoMu24,*b_HLT_IsoTkMu24,*b_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL,*b_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ,*b_HLT_IsoTkMu22,*b_HLT_Ele27_eta2p1_WPTight_Gsf,*b_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL,*b_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ,*b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL,*b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ,*b_HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL,*b_HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ,*b_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ,*b_ttHFCategory,*b_EVENT_event,*b_EVENT_run,*b_EVENT_genWeight,*b_EVENT_lumiBlock,*b_Met_type1PF_pt,*b_Met_type1PF_px,*b_Met_type1PF_py,*b_Met_type1PF_phi,*b_EVENT_Q2tthbbWeightUp,*b_EVENT_Q2tthbbWeightDown,*b_EVENT_PDFtthbbWeightUp,*b_EVENT_PDFtthbbWeightDown,*b_PUWeight,*b_Gen_pt,*b_Gen_eta,*b_Gen_phi,*b_Gen_pdg_id,*b_Gen_motherpdg_id,*b_trueInteractions,*b_pvertex_ndof,*b_pvertex_z;
  //==========================================
  // HACK pvertex_Rho not in first ntuples
  //==========================================
  TBranch *b_pvertex_Rho;
  //==========================================

  TBranch *b_Flag_HBHENoiseFilter,*b_Flag_HBHENoiseIsoFilter,*b_Flag_CSCTightHalo2015Filter,*b_Flag_EcalDeadCellTriggerPrimitiveFilter,*b_Flag_goodVertices;

  //==========================================
  // HACK Flag_globalTightHalo2016Filter not in first ntuples
  //==========================================
  TBranch *b_Flag_globalTightHalo2016Filter;
  TBranch *b_Flag_eeBadScFilter;
  //==========================================

  //==========================================
  // HACK EVENT_filterBadGlobalMuonTagger and EVENT_filtercloneGlobalMuonTagger not in first ntuples
  //==========================================
  //TBranch *b_EVENT_filterBadGlobalMuonTagger;
  //TBranch *b_EVENT_filtercloneGlobalMuonTagger;
  //==========================================

  oldtree->SetBranchAddress("HLT_IsoMu22",&HLT_IsoMu22,&b_HLT_IsoMu22);
  oldtree->SetBranchAddress("HLT_IsoTkMu22",&HLT_IsoTkMu22,&b_HLT_IsoTkMu22);
  oldtree->SetBranchAddress("HLT_IsoMu24",&HLT_IsoMu24,&b_HLT_IsoMu24);
  oldtree->SetBranchAddress("HLT_IsoTkMu24",&HLT_IsoTkMu24,&b_HLT_IsoTkMu24);
  oldtree->SetBranchAddress("HLT_Ele27_eta2p1_WPTight_Gsf",&HLT_Ele27_eta2p1_WPTight_Gsf,&b_HLT_Ele27_eta2p1_WPTight_Gsf);
  oldtree->SetBranchAddress("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL",&HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL,&b_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL);
  oldtree->SetBranchAddress("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ",&HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ,&b_HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ);
  oldtree->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL",&HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL,&b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL);
  oldtree->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ",&HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ,&b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ);
  oldtree->SetBranchAddress("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL",&HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL,&b_HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL);
  oldtree->SetBranchAddress("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ",&HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ,&b_HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ);
  oldtree->SetBranchAddress("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ",&HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ,&b_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ);
  oldtree->SetBranchAddress("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL",&HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL,&b_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL);
  oldtree->SetBranchAddress("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ",&HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ,&b_HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ);

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
  oldtree->SetBranchAddress("EVENT_genWeight",&EVENT_genWeight,&b_EVENT_genWeight);
  oldtree->SetBranchAddress("EVENT_lumiBlock",&EVENT_lumiBlock,&b_EVENT_lumiBlock);
  oldtree->SetBranchAddress("PUWeight",&PUWeight,&b_PUWeight);
  oldtree->SetBranchAddress("trueInteractions",&trueInteractions,&b_trueInteractions);
  oldtree->SetBranchAddress("Met_type1PF_pt",&Met_type1PF_pt,&b_Met_type1PF_pt);
  oldtree->SetBranchAddress("Met_type1PF_px",&Met_type1PF_px,&b_Met_type1PF_px);
  oldtree->SetBranchAddress("Met_type1PF_py",&Met_type1PF_py,&b_Met_type1PF_py);
  oldtree->SetBranchAddress("Met_type1PF_phi",&Met_type1PF_phi,&b_Met_type1PF_phi);
  oldtree->SetBranchAddress("Flag_HBHENoiseFilter",&Flag_HBHENoiseFilter,&b_Flag_HBHENoiseFilter);
  oldtree->SetBranchAddress("Flag_HBHENoiseIsoFilter",&Flag_HBHENoiseIsoFilter,&b_Flag_HBHENoiseIsoFilter);
  oldtree->SetBranchAddress("Flag_CSCTightHalo2015Filter",&Flag_CSCTightHalo2015Filter,&b_Flag_CSCTightHalo2015Filter);
  oldtree->SetBranchAddress("Flag_EcalDeadCellTriggerPrimitiveFilter",&Flag_EcalDeadCellTriggerPrimitiveFilter,&b_Flag_EcalDeadCellTriggerPrimitiveFilter);
  oldtree->SetBranchAddress("Flag_goodVertices",&Flag_goodVertices,&b_Flag_goodVertices);
  //==========================================
  // HACK Flag_globalTightHalo2016Filter not in first ntuples
  //==========================================
  //oldtree->SetBranchAddress("Flag_globalTightHalo2016Filter",&Flag_globalTightHalo2016Filter,&b_Flag_globalTightHalo2016Filter);
  //==========================================

  oldtree->SetBranchAddress("Flag_eeBadScFilter",&Flag_eeBadScFilter,&b_Flag_eeBadScFilter);

  //==========================================
  // HACK EVENT_filterBadGlobalMuonTagger and EVENT_filtercloneGlobalMuonTagger not in first ntuples
  //==========================================
  //oldtree->SetBranchAddress("EVENT_filterBadGlobalMuonTagger",&EVENT_filterBadGlobalMuonTagger,&b_EVENT_filterBadGlobalMuonTagger);
  //oldtree->SetBranchAddress("EVENT_filtercloneGlobalMuonTagger",&EVENT_filtercloneGlobalMuonTagger,&b_EVENT_filtercloneGlobalMuonTagger);
  //==========================================

  oldtree->SetBranchAddress("pvertex_ndof",&pvertex_ndof,&b_pvertex_ndof);
  oldtree->SetBranchAddress("pvertex_z",&pvertex_z,&b_pvertex_z);
  //==========================================
  // HACK pvertex_Rho not in first ntuples
  //==========================================
  //oldtree->SetBranchAddress("pvertex_Rho",&pvertex_Rho,&b_pvertex_Rho);
  //==========================================

  Int_t nentries = (Int_t)oldtree->GetEntries();
  std::cout << "SecondStep.cc:: Entering event loop with: " << nentries << " entries." << std::endl;

  try{
    if(nentries==0){
      throw runtime_error("No entries in file.");
    }
  }
  catch(std::exception const& exc){
    std::cerr << exc.what() << "\n";
  }

  std::cout << "SecondStep.cc:: sample #: " << sample << std::endl;

  //Instance of BlrBDT must be outside loop.
  //Class contains pointers (TMVA Reader objects) that casue memory leak.
  //BlrBDTClassifier bdt;

  vector<int> eventNums_passed;
  double bTagWP_DiscrCut = 0.8484;

  int firstEvent = 0;
  int neventsfilled = 0;
  for (Int_t i=firstEvent;i<nentries; i++) {//if(i>100) break;
    Long64_t tentry = oldtree->LoadTree(i);
    oldtree->GetEntry(i);

    if(i % 1000 == 0){
      cout << "Entry: " << i << endl;
    }

    //DONT FORGET TO REMOVE THIS!!!!!!!
    //cout << "EVENT_event = " << EVENT_event << endl;
    //if(EVENT_event!=1761047103){continue;}
    //cout << "============ Next event ===============" << endl;
    //cout << "EVENT_event = " << EVENT_event << endl;


    //DONT FORGET TO REMOVE THIS!!!!!!!

    if(sample==1 || sample==2){
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
    }

    // Primary Vertex must fulfill vertex selection.
    //cout << "pvertex_ndof size = " << pvertex_ndof->size() << endl;
    if(pvertex_ndof->size()<1){continue;}

    //==========================================
    // HACK pvertex_Rho not in first ntuples
    //==========================================
    //if(pvertex_ndof->at(0)<=4 && abs(pvertex_Rho->at(0))>=24 && abs(pvertex_z->at(0))>=2) {
    //  continue;
    //}
    //==========================================
    if(pvertex_ndof->at(0)<=4 && abs(pvertex_z->at(0))>=2) {
      continue;
    }
    //==========================================

    // MUON SELECTION
    // - Selects veto/subleading muons to perform overlap removal.
    vector<double> SelMuon_pt,SelMuon_eta,SelMuon_phi,SelMuon_iso,SelMuon_energy;
    for (UInt_t j = 0; j < Muon_pt->size(); ++j){
      if(!(Muon_pt->at(j)>15))                     continue;
      if(!(fabs(Muon_eta->at(j))<2.4))             continue;
      if(!(Muon_tight->at(j)==1))                  continue;
      if(!(Muon_relIsoDeltaBetaR04->at(j)<0.25))   continue;// = Loose iso
      SelMuon_pt.push_back(Muon_pt->at(j));
      SelMuon_eta.push_back(Muon_eta->at(j));
      SelMuon_phi.push_back(Muon_phi->at(j));
      SelMuon_energy.push_back(Muon_energy->at(j));
      SelMuon_iso.push_back(Muon_relIsoDeltaBetaR04->at(j));
    }
    // ELECTRON SELECTION
    // - Selects veto/subleading electrons to perform overlap removal.
    vector<double> SelElectronMVA_pt,SelElectronMVA_eta,SelElectronMVA_phi,SelElectronMVA_energy,SelElectronMVA_iso;
    for (UInt_t j = 0; j < patElectron_pt->size(); ++j){
      if(!(patElectron_pt->at(j)>15)) continue;
      if(!(fabs(patElectron_eta->at(j))<2.4)) continue;
      if(!(patElectron_inCrack->at(j)==0)) continue;
      if(!(patElectron_isPassMedium->at(j)==1)) continue;
      if(fabs(patElectron_SCeta->at(j))<=1.479){//Barrel impact parameter cuts
        if(fabs(patElectron_d0->at(j))>0.05) continue;
        if(fabs(patElectron_gsfTrack_dz_pv->at(j))>0.10) continue;
      }
      if(fabs(patElectron_SCeta->at(j))>1.479){//End-cap impact parameter cuts
        if(fabs(patElectron_d0->at(j))>0.10) continue;
        if(fabs(patElectron_gsfTrack_dz_pv->at(j))>0.20) continue;
      }
      //if(!(patElectron_relIsoRhoEA->at(j)<0.15))    continue;
      SelElectronMVA_pt.push_back(patElectron_pt->at(j));
      SelElectronMVA_eta.push_back(patElectron_eta->at(j));
      SelElectronMVA_phi.push_back(patElectron_phi->at(j));
      SelElectronMVA_energy.push_back(patElectron_energy->at(j));
      SelElectronMVA_iso.push_back(patElectron_relIsoRhoEA->at(j));
    }

    // JET SELECTION:
    // Subleading Jets in event.
    // Leading Jets in event.
    vector<double> SelJet_pt,SelJet_eta,SelJet_phi,SelJet_mass,SelJet_pfCombinedInclusiveSecondaryVertexV2BJetTags;
    vector<double> SelTightJet_pt,SelTightJet_eta,SelTightJet_phi,SelTightJet_mass,SelTightJet_pfCombinedInclusiveSecondaryVertexV2BJetTags;

    for (UInt_t j = 0; j < Jet_pt->size(); ++j){
      //Jet_Uncorr_pt here should be uncorrected Jet_Uncorr_pt from BSMFramework.
      double jet_pt = Jet_Uncorr_pt->at(j)*Jet_JesSF->at(j)*Jet_JerSF->at(j);

      if(!(jet_pt>20)) continue;
      if(!(fabs(Jet_eta->at(j))<2.4)) continue;
      // Jet kinematic & loose ID requirements
      if(!(Jet_neutralHadEnergyFraction->at(j)<0.99)) continue;
      if(!(Jet_chargedHadronEnergyFraction->at(j)>0.0)) continue;
      if(!(Jet_neutralEmEnergyFraction->at(j)<0.99)) continue;
      if(!(Jet_chargedEmEnergyFraction->at(j)<0.99)) continue;
      if(!(Jet_numberOfConstituents->at(j)>1)) continue;
      if(!(Jet_chargedMultiplicity->at(j)>0.0)) continue;
      bool deltaRJetLepBoolean = false;
      for (UInt_t k = 0; k < SelMuon_pt.size(); ++k){
        float deltaEta = SelMuon_eta[k]-Jet_eta->at(j);
        float deltaPhi = fabs(SelMuon_phi[k]-Jet_phi->at(j));
        if(deltaPhi>=TMath::Pi()) deltaPhi = 2*TMath::Pi() - deltaPhi;
        if(sqrt(deltaEta*deltaEta + deltaPhi*deltaPhi)<=0.4) deltaRJetLepBoolean=true;
      }
      for (UInt_t k = 0; k < SelElectronMVA_pt.size(); ++k){
        float deltaEta = SelElectronMVA_eta[k]-Jet_eta->at(j);
        float deltaPhi = fabs(SelElectronMVA_phi[k]-Jet_phi->at(j));
        if(deltaPhi>=TMath::Pi()) deltaPhi = 2*TMath::Pi() - deltaPhi;
        if(sqrt(deltaEta*deltaEta + deltaPhi*deltaPhi)<=0.4) deltaRJetLepBoolean=true;
      }
      if(!deltaRJetLepBoolean){
        SelJet_pt.push_back(jet_pt);
        SelJet_eta.push_back(Jet_eta->at(j));
        SelJet_phi.push_back(Jet_phi->at(j));
        SelJet_mass.push_back(Jet_mass->at(j));
        SelJet_pfCombinedInclusiveSecondaryVertexV2BJetTags.push_back(Jet_pfCombinedInclusiveSecondaryVertexV2BJetTags->at(j));
        if(!(jet_pt>30)) continue;
        SelTightJet_pt.push_back(jet_pt);
        SelTightJet_eta.push_back(Jet_eta->at(j));
        SelTightJet_phi.push_back(Jet_phi->at(j));
        SelTightJet_mass.push_back(Jet_mass->at(j));
        SelTightJet_pfCombinedInclusiveSecondaryVertexV2BJetTags.push_back(Jet_pfCombinedInclusiveSecondaryVertexV2BJetTags->at(j));
      }
    }

    // ============ B-tagging =============
    // DILEPTON: Uses both leading and subleading jets so b-tags are counted from jets that are subleading or tighter.
    // SINGLE LEPTON: Only uses leading (tight) jets so b-tags are counted from tight jet collection.
    // B-tag part of selection being performed in makeTable.py so not necessary here unless we are going to perform rest of selection.

    int nBCSVM_DL = 0;
    int nBCSVM_SL = 0;
    for(UInt_t k = 0; k < SelJet_pt.size(); ++k){
      if(SelJet_pfCombinedInclusiveSecondaryVertexV2BJetTags[k]>bTagWP_DiscrCut) nBCSVM_DL=nBCSVM_DL+1;
    }
    for(UInt_t k = 0; k < SelTightJet_pt.size(); ++k){
      if(SelTightJet_pfCombinedInclusiveSecondaryVertexV2BJetTags[k]>bTagWP_DiscrCut) nBCSVM_SL=nBCSVM_SL+1;
    }

    //=================================
    //    ttH EVENT PRE-SELECTION
    //=================================
    // - Ensure events fall into a single lepton or dilepton channel.
    // - Ensure passing minimum jet and b-tag requirements.
    //=================================

    bool MUON = false;
    bool ELECTRON = false;
    bool ELEL = false;
    bool MUONMUON = false;
    bool ELMUON = false;

    double leadEl_ptcut_SL = 30;
    double leadEl_absetacut_SL = 2.1;

    double leadMu_ptcut_SL = 26;
    double leadMu_absetacut_SL = 2.1;
    double leadMu_isocut_SL = 0.15;

    double leadEl_ptcut_DL = 25;
    double leadEl_absetacut_DL = 2.4;

    double leadMu_ptcut_DL = 25;
    double leadMu_absetacut_DL = 2.4;
    double leadMu_isocut_DL = 0.25;

    double subleadEl_ptcut_DL = 15;
    double subleadEl_absetacut_DL = 2.4;

    double subleadMu_ptcut_DL = 15;
    double subleadMu_absetacut_DL = 2.4;
    double subleadMu_isocut_DL = 0.25;

    double invMcut_DL = 20;
    int METFilters = 0;

    if(sample!=0){
      //==========================================
      // HACK Flag_globalTightHalo2016Filter not in first ntuples
      //==========================================
      //if(Flag_goodVertices==1 && Flag_globalTightHalo2016Filter==1 && Flag_HBHENoiseFilter==1 && Flag_HBHENoiseIsoFilter==1 && Flag_EcalDeadCellTriggerPrimitiveFilter==1 && Flag_eeBadScFilter==1){
      if(Flag_goodVertices==1 && Flag_HBHENoiseFilter==1 && Flag_HBHENoiseIsoFilter==1 && Flag_EcalDeadCellTriggerPrimitiveFilter==1){
        //==========================================
        //tt.EVENT_filterBadGlobalMuonTagger==1 and tt.EVENT_filtercloneGlobalMuonTagger==1
        METFilters = 1;
      }
    }
    else{
      //==========================================
      // HACK Flag_globalTightHalo2016Filter not in first ntuples
      //==========================================
      //if(Flag_goodVertices==1 && Flag_globalTightHalo2016Filter==1 && Flag_HBHENoiseFilter==1 && Flag_HBHENoiseIsoFilter==1 && Flag_EcalDeadCellTriggerPrimitiveFilter==1 && Flag_eeBadScFilter==1){
      if(Flag_goodVertices==1 && Flag_HBHENoiseFilter==1 && Flag_HBHENoiseIsoFilter==1 && Flag_EcalDeadCellTriggerPrimitiveFilter==1 && Flag_eeBadScFilter==1 ){
      //==========================================
        //tt.EVENT_filterBadGlobalMuonTagger==1 and tt.EVENT_filtercloneGlobalMuonTagger==1
        METFilters = 1;
      }
    }

    if(SelMuon_pt.size()==1 && SelElectronMVA_pt.size()==0){// # leptons
      if(SelMuon_pt[0]>leadMu_ptcut_SL && fabs(SelMuon_eta[0])<leadMu_absetacut_SL && SelMuon_iso[0]<leadMu_isocut_SL){// lepton kinematics
        if((HLT_IsoMu24==1 || HLT_IsoTkMu24==1)){// Triggers
          if(METFilters==1){
            MUON=true;
          }
        }
      }
    }
    if(SelElectronMVA_pt.size()==1 && SelMuon_pt.size()==0) {// # leptons
      if(SelElectronMVA_pt[0]>leadEl_ptcut_SL && fabs(SelElectronMVA_eta[0])<leadEl_absetacut_SL){// lepton kinematics
        if(HLT_Ele27_eta2p1_WPTight_Gsf==1){// Triggers
          if(METFilters==1){
            ELECTRON=true;
          }
        }
      }
    }
    if(SelElectronMVA_pt.size()==2&&SelMuon_pt.size()==0){// # leptons
      if(SelElectronMVA_pt[0]>leadEl_ptcut_DL && SelElectronMVA_pt[1]>subleadEl_ptcut_DL && fabs(SelElectronMVA_eta[0])<leadEl_absetacut_DL && fabs(SelElectronMVA_eta[1])<subleadEl_absetacut_DL){// lepton kinematics
        if(HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ==1){// Triggers
          TLorentzVector temp_lep1;
          TLorentzVector temp_lep2;
          temp_lep1.SetPtEtaPhiE(SelElectronMVA_pt[0],SelElectronMVA_eta[0],SelElectronMVA_phi[0],SelElectronMVA_energy[0]);
          temp_lep2.SetPtEtaPhiE(SelElectronMVA_pt[1],SelElectronMVA_eta[1],SelElectronMVA_phi[1],SelElectronMVA_energy[1]);
          double dilep = (temp_lep1+temp_lep2).M();
          if(METFilters==1){
          //if(dilep>invMcut_DL && (dilep<76 || dilep>106)){
            //if(Met_type1PF_pt>40){
              ELEL=true;
            //}
          //}
          }
        }
      }
    }
    if(SelElectronMVA_pt.size()==1 && SelMuon_pt.size()==1) {
      if(SelElectronMVA_pt[0]>leadEl_ptcut_DL && fabs(SelElectronMVA_eta[0])<leadEl_absetacut_DL && SelMuon_pt[0]>subleadMu_ptcut_DL && fabs(SelMuon_eta[0])<subleadMu_absetacut_DL && SelMuon_iso[0]<subleadMu_isocut_DL){
        if(HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL==1 || HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL==1 || HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ==1 || HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ==1){
          TLorentzVector temp_lep1;
          TLorentzVector temp_lep2;
          temp_lep1.SetPtEtaPhiE(SelElectronMVA_pt[0],SelElectronMVA_eta[0],SelElectronMVA_phi[0],SelElectronMVA_energy[0]);
          temp_lep2.SetPtEtaPhiE(SelMuon_pt[0],SelMuon_eta[0],SelMuon_phi[0],SelMuon_energy[0]);
          double M_ll = (temp_lep1+temp_lep2).M();
          if(METFilters==1){
          //cout << "M_ll = " << M_ll << endl;
          //if(M_ll>invMcut_DL&& (M_ll<76 || M_ll>106)){
            //cout << "Met_type1PF_pt = " << Met_type1PF_pt << endl;
            //if(Met_type1PF_pt>40){
              ELMUON=true;
            //}
          //}
          }
        }
      }
    }
    if(SelElectronMVA_pt.size()==1&&SelMuon_pt.size()==1){
      if(SelElectronMVA_pt[0]>subleadEl_ptcut_DL && fabs(SelElectronMVA_eta[0])<subleadEl_absetacut_DL && SelMuon_pt[0]>leadMu_ptcut_DL && fabs(SelMuon_eta[0])<leadMu_absetacut_DL && SelMuon_iso[0]<leadMu_isocut_DL){
        if(HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL==1 || HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL==1 || HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ==1 || HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ==1){
          TLorentzVector temp_lep1;
          TLorentzVector temp_lep2;
          temp_lep1.SetPtEtaPhiE(SelElectronMVA_pt[0],SelElectronMVA_eta[0],SelElectronMVA_phi[0],SelElectronMVA_energy[0]);
          temp_lep2.SetPtEtaPhiE(SelMuon_pt[0],SelMuon_eta[0],SelMuon_phi[0],SelMuon_energy[0]);
          double M_ll = (temp_lep1+temp_lep2).M();
          if(METFilters==1){
          //cout << "M_ll = " << M_ll << endl;
          //if(M_ll>invMcut_DL && (M_ll<76 || M_ll>106)){
            //cout << "Met_type1PF_pt = " << Met_type1PF_pt << endl;
            //if(Met_type1PF_pt>40){
              ELMUON=true;
            //}
          //}
          }
        }
      }
    }
    if(SelMuon_pt.size()==2&&SelElectronMVA_pt.size()==0){
      if(SelMuon_pt[0]>leadMu_ptcut_DL && SelMuon_pt[1]>subleadMu_ptcut_DL && fabs(SelMuon_eta[0])<leadMu_absetacut_DL && fabs(SelMuon_eta[1])<subleadMu_absetacut_DL && SelMuon_iso[0]<leadMu_isocut_DL && SelMuon_iso[1]<subleadMu_isocut_DL){
        if(HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL==1 || HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL==1 || HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ==1 || HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ==1){
          TLorentzVector temp_lep1;
          TLorentzVector temp_lep2;
          temp_lep1.SetPtEtaPhiE(SelMuon_pt[0],SelMuon_eta[0],SelMuon_phi[0],SelMuon_energy[0]);
          temp_lep2.SetPtEtaPhiE(SelMuon_pt[1],SelMuon_eta[1],SelMuon_phi[1],SelMuon_energy[1]);
          double M_ll = (temp_lep1+temp_lep2).M();
          if(METFilters==1){
          //cout << "M_ll = " << M_ll << endl;
          //if(M_ll>invMcut_DL && (M_ll<76 || M_ll>106)){
            //cout << "Met_type1PF_pt = " << Met_type1PF_pt << endl;
            //if(Met_type1PF_pt>40){
              MUONMUON=true;
            //}
          //}
          }
        }
      }
    }



    // Categorise events into analysis channels
    is_e_ = false;
    is_mu_ = false;
    if(!MUON && !ELECTRON && !ELEL && !MUONMUON && !ELMUON){
      continue;
    }
    if(ELECTRON && !MUON){
      if(!(SelTightJet_pt.size()>=4&&nBCSVM_SL>=2)) {
        continue;
      }
      else{is_e_ = true;}
    }
    else if(MUON && !ELECTRON){
      if(!(SelTightJet_pt.size()>=4&&nBCSVM_SL>=2)) {
        continue;
      }
      else{is_mu_ = true;}
    }
    if(ELEL || MUONMUON || ELMUON){
        if(!(SelTightJet_pt.size()>=2)&&nBCSVM_DL>=1){continue;}
    }

    if((MUON || ELECTRON) && (ELEL||MUONMUON||ELMUON)){
      cout << "WARNING: Event passes both single lepton and dilepton selections!" << endl;
      cout << "Event number: " << EVENT_event << endl;
      cout << "# Electrons: " << SelElectronMVA_pt.size() << endl;
      cout << "# Muons: " << SelMuon_pt.size() << endl;
    }

    // HACK Just running SL for now
    if (is_e_ != true && is_mu_ != true){
      continue;
    }

    //!!! BDT VARIABLES !!!
    std::vector<TLorentzVector> selectedLeptonP4;
    std::vector<TLorentzVector> selectedJetP4;
    std::vector<double> selectedJetCSV;
    std::vector<TLorentzVector> looseSelectedJetP4;
    std::vector<double> looseSelectedJetCSV;
    for(unsigned int j=0; j<SelMuon_pt.size(); j++){
      TLorentzVector prov;
      prov.SetPtEtaPhiE(SelMuon_pt[j],SelMuon_eta[j],SelMuon_phi[j],SelMuon_energy[j]);
      selectedLeptonP4.push_back(prov);
    }
    for(unsigned int j=0; j<SelElectronMVA_pt.size(); j++){
      TLorentzVector prov;
      prov.SetPtEtaPhiE(SelElectronMVA_pt[j],SelElectronMVA_eta[j],SelElectronMVA_phi[j],SelElectronMVA_energy[j]);
      selectedLeptonP4.push_back(prov);
    }
    for(unsigned int j=0; j<SelTightJet_pt.size(); j++){
      TLorentzVector prov;
      prov.SetPtEtaPhiM(SelTightJet_pt[j],SelTightJet_eta[j],SelTightJet_phi[j],SelTightJet_mass[j]);
      selectedJetP4.push_back(prov);
      selectedJetCSV.push_back(SelTightJet_pfCombinedInclusiveSecondaryVertexV2BJetTags[j]);
    }
    for(unsigned int j=0; j<SelJet_pt.size(); j++){
      TLorentzVector prov;
      prov.SetPtEtaPhiM(SelJet_pt[j],SelJet_eta[j],SelJet_phi[j],SelJet_mass[j]);
      looseSelectedJetP4.push_back(prov);
      looseSelectedJetCSV.push_back(SelJet_pfCombinedInclusiveSecondaryVertexV2BJetTags[j]);
    }

    TLorentzVector metP4;
    std::vector<double> selectedJetCSV_fixed;
    metP4.SetPtEtaPhiM(Met_type1PF_pt,0.,Met_type1PF_phi,0.);

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
      if(selectedJetCSV_fixed[j]>bTagWP_DiscrCut){
        selectedTaggedJetP4.push_back(selectedJetP4[j]);
      }
    }
    double Aplanarity=-99.;
    double Sphericity=-99.;
    // HACK:: BDT been removed to run on IHEP farm
    //bdtVarCalculator bdtVarCalc;
    //bdtVarCalc.getSp(selectedLeptonP4[0],metP4,selectedJetP4,Aplanarity,Sphericity);
    double H0=-99;
    double H1 = -99;
    double H2 = -99;
    double H3 = -99;
    double H4= -99;
    // HACK:: BDT been removed to run on IHEP farm
    //bdtVarCalc.getFox(selectedJetP4,H0,H1,H2,H3,H4);
    double minChi=-99;
    double dRbb =-99;
    TLorentzVector bjet1,bjet2;
    // HACK:: BDT been removed to run on IHEP farm
    double bestHiggsMass = -99;
    //bestHiggsMass = bdtVarCalc.getBestHiggsMass(selectedLeptonP4[0],metP4,selectedJetP4,selectedJetCSV_fixed,minChi,dRbb,bjet1,bjet2,looseSelectedJetP4,looseSelectedJetCSV);
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
    // HACK:: BDT been removed to run on IHEP farm
    //bdtVarCalc.study_tops_bb_syst(metP4.Pt(), metP4.Phi(), dummy_metv, selectedLeptonP4[0], jets_vvdouble, selectedJetCSV_fixed, minChiStudy, chi2lepW, chi2leptop, chi2hadW, chi2hadtop, mass_lepW, mass_leptop, mass_hadW, mass_hadtop, dRbbStudy, testquant1, testquant2, testquant3, testquant4, testquant5, testquant6, testquant7, b1, b2);
    double DEta_fn=testquant6;
    // HACK:: BDT been removed to run on IHEP farm
    double pt_E_ratio = -99;
    //pt_E_ratio = bdtVarCalc.pt_E_ratio_jets(jets_vvdouble);
    double jet_jet_etamax = -99;
    //jet_jet_etamax = bdtVarCalc.get_jet_jet_etamax (jets_vvdouble);
    double jet_tag_etamax = -99;
    //jet_tag_etamax = bdtVarCalc.get_jet_tag_etamax (jets_vvdouble,selectedJetCSV_fixed);
    double tag_tag_etamax = -99;
    //tag_tag_etamax = bdtVarCalc.get_tag_tag_etamax (jets_vvdouble,selectedJetCSV_fixed);
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
      if(*itCSV<bTagWP_DiscrCut) continue;
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
      if(*itCSV<bTagWP_DiscrCut) continue;
      csvDev += pow(*itCSV - averageCSV_tagged,2);
    }
    if(ntags>0) csvDev /= ntags;
    else csvDev=-1.;
    double met=metP4.Pt();
    double Fifth_highest_CSV=njets>4?sortedCSV[njets-5]:-1.;
    double Fourth_highest_btag=njets>3?sortedCSV[njets-4]:-1.;
    double Fourth_jet_pt=selectedJetP4.size()>3?selectedJetP4[3].Pt():-99;
    double Third_highest_btag=njets>2?sortedCSV[njets-3]:-1.;
    double Third_jet_pt=selectedJetP4.size()>2?selectedJetP4[2].Pt():-99;
    double Second_highest_btag=njets>1?sortedCSV[njets-2]:-1.;
    double Second_jet_pt=selectedJetP4.size()>1?selectedJetP4[1].Pt():-99;
    double First_highest_btag=njets>0?sortedCSV[njets-1]:-1.;
    double First_jet_pt=selectedJetP4.size()>0?selectedJetP4[0].Pt():-99;

    //btagging likelihood ratio
    double eth_blr=-1;
    std::vector<unsigned int> out_best_perm;
    double out_P_4b=-1;
    double out_P_2b=-1;
    // HACK:: BDT been removed to run on IHEP farm
    //eth_blr=mem.GetBTagLikelihoodRatio(selectedJetP4,selectedJetCSV,out_best_perm,out_P_4b,out_P_2b);

    int bJetness_num_soft_leps = 1;//(int)BJetness_num_soft_leps[0];
    double bJetness_avip3d_val = 0.2;//BJetness_avip3d_val[0];

    // HACK:: BDT been removed to run on IHEP farm
    double result = -99;
    //result = bdt.GetBDTOutput(selectedLeptonP4,selectedJetP4,selectedJetCSV,looseSelectedJetP4,looseSelectedJetCSV,metP4,eth_blr);
    //if(!(eth_blr>=0.0 || eth_blr<0.0)) eth_blr=0.001;
    //eth_blr = TMath::Log(eth_blr/(1-eth_blr));

    muFuncs muF;
    PUWTool PileupTool;
    double trigger_SF=1;

    if(sample!=0){
      triggerSFs trigSFTool;
      if(is_e_){
        trigger_SF = trigSFTool.getEventTriggerSF(is_e_, is_mu_, SelElectronMVA_pt[0], SelElectronMVA_eta[0]);
      }
      if(is_mu_){
        trigger_SF = trigSFTool.getEventTriggerSF(is_e_, is_mu_, SelMuon_pt[0], SelMuon_eta[0]);
      }
    }

    if(sample!=0){
      PileupTool.newPUWeight(PUWeight, puweight_,puweightUP_,puweightDOWN_);
      if(MUON==true && ELECTRON==false){
        type_ = 0;
        Muon_IDSFval_ = muF.Muon_IDSF( SelMuon_pt[0],SelMuon_eta[0]).first;
        Muon_IsoSFval_= muF.Muon_IsoSF(SelMuon_pt[0],SelMuon_eta[0]).first;
        Muon_TrkSFval_= muF.Muon_TrkSF(SelMuon_eta[0]).first;
        Muon_IDSFerr_ = muF.Muon_IDSF( SelMuon_pt[0],SelMuon_eta[0]).second;
        Muon_IsoSFerr_= muF.Muon_IsoSF(SelMuon_pt[0],SelMuon_eta[0]).second;
        Muon_TrkSFerr_= muF.Muon_TrkSF(SelMuon_eta[0]).second;
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

        eleFuncs eleF;
        Electron_IDSFval_ = eleF.Electron_ID(SelElectronMVA_pt[0],SelElectronMVA_eta[0]).first;
        Electron_IDSFerr_ = eleF.Electron_ID(SelElectronMVA_pt[0],SelElectronMVA_eta[0]).second;
        Electron_GsfSFval_= eleF.Electron_GsfSF(SelElectronMVA_eta[0]).first;
        Electron_GsfSFerr_= eleF.Electron_GsfSF(SelElectronMVA_eta[0]).second;
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
    // HACK:: BDT been removed to run on IHEP farm
    BDT_ = result;
    NumberOfJets_  = SelTightJet_pt.size();
    if((ELECTRON || MUON) && !(ELEL || ELMUON || MUONMUON)){
      NumberOfBJets_ = nBCSVM_SL;
    }
    if(!(ELECTRON || MUON) && (ELEL || ELMUON || MUONMUON)){
      NumberOfBJets_ = nBCSVM_DL;
    }
    all_sum_pt_with_met_ = sum_pt_with_met;
    aplanarity_ = Aplanarity;
    avg_btag_disc_btags_ = averageCSV_tagged;
    avg_dr_tagged_jets_ = avgDrTagged;
    best_higgs_mass_ = bestHiggsMass;
    closest_tagged_dijet_mass_ = Closest_tagged_dijet_mass;
    dEta_fn_ = DEta_fn;
    dev_from_avg_disc_btags_ = csvDev;
    dr_between_lep_and_closest_jet_ = Dr_between_lep_and_closest_jet;
    if(SelElectronMVA_pt.size()==1 && is_e_ == true){
        lead_el_pt_ = SelElectronMVA_pt[0];
        lead_el_eta_ = SelElectronMVA_eta[0];
        lead_el_phi_ = SelElectronMVA_phi[0];
        lead_mu_pt_ = -99;
        lead_mu_eta_ = -99;
        lead_mu_phi_ = -99;
        lead_lep_pt_ = SelElectronMVA_pt[0];
        lead_lep_eta_ = SelElectronMVA_eta[0];
        lead_lep_phi_ = SelElectronMVA_phi[0];
    }
    else if(SelMuon_pt.size()==1 && is_mu_ == true){
      lead_mu_pt_ = SelMuon_pt[0];
      lead_mu_eta_ = SelMuon_eta[0];
      lead_mu_phi_ = SelMuon_phi[0];
      lead_el_pt_ = -99;
      lead_el_eta_ = -99;
      lead_el_phi_ = -99;
      lead_lep_pt_ = SelMuon_pt[0];
      lead_lep_eta_ = SelMuon_eta[0];
      lead_lep_phi_ = SelMuon_phi[0];
    }


    number_electrons_ = SelElectronMVA_pt.size();
    number_muons_ = SelMuon_pt.size();
    number_leptons_ = SelElectronMVA_pt.size() + SelMuon_pt.size();

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
    first_jet_pt_ = First_jet_pt;
    first_highest_btag_ = First_highest_btag;
    second_jet_pt_ = Second_jet_pt;
    second_highest_btag_ = Second_highest_btag;
    third_jet_pt_ = Third_jet_pt;
    third_highest_btag_ = Third_highest_btag;
    fourth_jet_pt_ = Fourth_jet_pt;
    fourth_highest_btag_ = Fourth_highest_btag;
    fifth_highest_CSV_ = Fifth_highest_CSV;
    sphericity_ = Sphericity;
    tagged_dijet_mass_closest_to_125_ = Tagged_dijet_mass_closest_to_125;
    Evt_CSV_Average_ = averageCSV_all;
    Evt_Deta_JetsAverage_ = detaJetsAverage;
    blr_ = eth_blr;
    trigger_SF_ = trigger_SF;
    //is_e_ = ELECTRON;
    //is_mu_ = MUON;

    newtree->Fill();
    neventsfilled = neventsfilled +1;
  }//End of event loop
  cout << "events filled : " << neventsfilled << endl;
  std::cout << "SecondStep.cc:: Print and save newtree." << std::endl;
  cout << "SecondStep was run on Input file: " << Input << endl;
  cout << "Output in file: " << Output << endl;
  newfile->Write();
  oldfile->Close();
  newfile->Close();
  delete oldfile;
  delete newfile;
}


//NOTE: maybe dont want this here:
//      means have to combine all files before SecondStep
//      takes much longer to run thank indivdiual OutputTrees.

//double SecondStep::lumi_weight(string file_name){

  //May want to move these and make them constants + available throughout code.
//  std::map<string,double> xs_map;
//  input_path = "/publicfs/cms/data/TopQuark/cms13TeV/FullMoriond2017/mc/";
//  xs_map['']=;

//  std::map<string,double> BR_map;
//  std::map<string,double> sumWeights_map;

  //if(file_name.find("TT_TuneCUETP8M2T4_13TeV-powheg-pythia8") != std::string::npos)
//}



//Main function inside ifndef mean code can still be run by interpreter.
# ifndef __CINT__

int main(int argc, char* argv[]){
  std::cout << "SecondStep.cc::main()" << std::endl;
  SecondStep SS;
  const char* Input;

  const char* outpath_test_string;

  string outputPath = "";

  if(argc!=3){
    SS.Usage();
    exit(0);
  }
  else{
    string infilename=string(argv[1]);
    string suffix =".root";
    string inputFullPath = infilename;
    Input = inputFullPath.c_str();
    ifstream inputFile(Input);

    outputPath = string(argv[2]);
    if(outputPath.compare(outputPath.length()-1,1,"/")){
      cout << "Adding / to end of output path" << endl;
      outputPath += "/";
    }
    outpath_test_string = outputPath.c_str();
    if (stat(outpath_test_string, &sb) == 1 || (!S_ISDIR(sb.st_mode))){
      cout << "ERROR : \'outputPath\' does not exist!" << endl;
      cout << "Please check the output directory you want to write to exists." << endl;
      exit(0);
    }

    if(!inputFile.is_open()){
      std::cout << "File " << Input << " could not be openend!"<< std::endl;
      exit(0);
    }
    else{
      cout << "File " << Input << " opened successfully!"<<endl;;
    }
  }

  SS.Process(argv[1],outputPath);

  return 0;
}

# endif
