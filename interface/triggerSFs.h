#ifndef TRIGGERSFS_H
#define TRIGGERSFS_H

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1F.h"
//#include <sys/types.h>
//#include <sys/stat.h>
#include "TMath.h"
#include <stdexcept>
#include <exception>
#include <iostream>
#include <math.h>


using namespace std;

class triggerSFs {
public:
  triggerSFs();
  ~triggerSFs();
  double getEventTriggerSF(bool is_el, bool is_mu, double ele_pt, double ele_eta);

private:
};

#endif
