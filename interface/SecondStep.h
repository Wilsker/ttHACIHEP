#ifndef SECONDSTEP_H
#define SECONDSTEP_H
#include "TGraph.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include "TVector.h"
#include <iostream>
#include <stdexcept>
#include <exception>
#include "TMVA/Reader.h"
#include <vector>
#include <map>
#include <math.h>
//#include "../../TTH/CommonClassifier/interface/MEMClassifier.h"
//#include "../../TTH/CommonClassifier/interface/BlrBDTClassifier.h"
#include "newPUWeight.h"
#include "eleFuncs.h"
#include "muFuncs.h"
#include "jetFuncs.h"
#include "bdtVarCalculator.h"
#include "triggerSFs.h"
#include <sys/types.h>
#include <sys/stat.h>
using namespace std;

class SecondStep{

public:
  SecondStep();
  void Process(char* inFile, string outDirPath);
  void Usage();
  double lumi_weight(string);
  ~SecondStep();

private:

};

#endif
