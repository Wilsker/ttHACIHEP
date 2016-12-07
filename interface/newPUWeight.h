#ifndef NEWPUWEIGHT_H
#define NEWPUWEIGHT_H

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TMath.h"
#include "TVector.h"
#include <iostream>
#include <vector>
#include "TH1.h"
//#include <math.h>

class PUWTool {
public:
  void newPUWeight(double PUWeight_, double &puweight,double &puweightUP,double &puweightDOWN);
private:
};

#endif
