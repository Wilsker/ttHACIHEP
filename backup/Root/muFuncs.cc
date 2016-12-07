#include "../interface/muFuncs.h"

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
