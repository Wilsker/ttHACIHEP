#include "../interface/eleFuncs.h"

pair<double,double> eleFuncs::Electron_ID(double pt, double eta){
  double sfval = 1;
  double sferr = 1;
  if(pt<=20){//10<pt && pt<=20)
    if(-2.5<eta && eta<=-2){sfval = 0.83737; sferr = 0.0237874;}
    if(-2<eta && eta<=-1.566){sfval = 0.799714; sferr = 0.0141407;}
    if(-1.566<eta && eta<=-1.444){sfval = 0.951583; sferr = 0.0776466;}
    if(-1.444<eta && eta<=-0.8){sfval = 0.914992; sferr = 0.0352903;}
    if(-0.8<eta && eta<=0){sfval = 0.90313; sferr = 0.0272266;}
    if(0<eta && eta<=0.8){sfval = 0.927622; sferr = 0.0268911;}
    if(0.8<eta && eta<=1.444){sfval = 0.937206; sferr = 0.0351763;}
    if(1.444<eta && eta<=1.566){sfval = 0.968401; sferr = 0.0774164;}
    if(1.566<eta && eta<=2){sfval = 0.842647; sferr = 0.0138921;}
    if(2<eta && eta<=2.5){sfval = 0.850575; sferr = 0.0239472;}
  }
  if(20<pt && pt<=30){
    if(-2.5<eta && eta<=-2){sfval = 0.862958; sferr = 0.0132085;}
    if(-2<eta && eta<=-1.566){sfval = 0.871007; sferr = 0.0132405;}
    if(-1.566<eta && eta<=-1.444){sfval = 0.890744; sferr = 0.0517097;}
    if(-1.444<eta && eta<=-0.8){sfval = 0.923476; sferr = 0.0209732;}
    if(-0.8<eta && eta<=0){sfval = 0.9199; sferr = 0.0168032;}
    if(0<eta && eta<=0.8){sfval = 0.944931; sferr = 0.0168032;}
    if(0.8<eta && eta<=1.444){sfval = 0.919166; sferr = 0.0209278;}
    if(1.444<eta && eta<=1.566){sfval = 0.865356; sferr = 0.0515486;}
    if(1.566<eta && eta<=2){sfval = 0.866417; sferr = 0.0131643;}
    if(2<eta && eta<=2.5){sfval = 0.891856; sferr = 0.0132896;}
  }
  if(30<pt && pt<=40){
    if(-2.5<eta && eta<=-2){sfval = 0.91492; sferr = 0.00831223;}
    if(-2<eta && eta<=-1.566){sfval = 0.916188; sferr = 0.00481392;}
    if(-1.566<eta && eta<=-1.444){sfval = 0.920253; sferr = 0.0121675;}
    if(-1.444<eta && eta<=-0.8){sfval = 0.94881; sferr = 0.00803681;}
    if(-0.8<eta && eta<=0){sfval = 0.940904; sferr = 0.00442778;}
    if(0<eta && eta<=0.8){sfval = 0.961494; sferr = 0.00442778;}
    if(0.8<eta && eta<=1.444){sfval = 0.944844; sferr = 0.00803681;}
    if(1.444<eta && eta<=1.566){sfval = 0.918471; sferr = 0.0120133;}
    if(1.566<eta && eta<=2){sfval = 0.919262; sferr = 0.00481392;}
    if(2<eta && eta<=2.5){sfval = 0.927007; sferr = 0.00837403;}
  }
  if(40<pt && pt<=50){
    if(-2.5<eta && eta<=-2){sfval = 0.929825; sferr = 0.00558629;}
    if(-2<eta && eta<=-1.566){sfval = 0.938957; sferr = 0.0032966;}
    if(-1.566<eta && eta<=-1.444){sfval = 0.953162; sferr = 0.00758102;}
    if(-1.444<eta && eta<=-0.8){sfval = 0.95553; sferr = 0.00501427;}
    if(-0.8<eta && eta<=0){sfval = 0.950728; sferr = 0.0044236;}
    if(0<eta && eta<=0.8){sfval = 0.965051; sferr = 0.0044236;}
    if(0.8<eta && eta<=1.444){sfval = 0.954181; sferr = 0.00501427;}
    if(1.444<eta && eta<=1.566){sfval = 0.932322; sferr = 0.0074618;}
    if(1.566<eta && eta<=2){sfval = 0.943333; sferr = 0.00314095;}
    if(2<eta && eta<=2.5){sfval = 0.943155; sferr = 0.00558629;}
  }
  if(pt>50){//50<pt && pt<=200)
    if(-2.5<eta && eta<=-2){sfval = 0.936; sferr = 0.00652911;}
    if(-2<eta && eta<=-1.566){sfval = 0.949946; sferr = 0.00353559;}
    if(-1.566<eta && eta<=-1.444){sfval = 0.965438; sferr = 0.0081386;}
    if(-1.444<eta && eta<=-0.8){sfval = 0.957494; sferr = 0.00381244;}
    if(-0.8<eta && eta<=0){sfval = 0.956954; sferr = 0.00695617;}
    if(0<eta && eta<=0.8){sfval = 0.971143; sferr = 0.00695617;}
    if(0.8<eta && eta<=1.444){sfval = 0.963842; sferr = 0.00381244;}
    if(1.444<eta && eta<=1.566){sfval = 0.943936; sferr = 0.0081386;}
    if(1.566<eta && eta<=2){sfval = 0.958425; sferr = 0.00353559;}
    if(2<eta && eta<=2.5){sfval = 0.954442; sferr = 0.00619668;}
  }
  return make_pair(sfval,sferr);
}

pair<double,double> eleFuncs::Electron_GsfSF(double eta){
  double sfval = 1;
  double sferr = 1;
  if(-2.5<eta && eta<=-2.4){sfval = 1.17034; sferr = 0.00966552;}
  if(-2.4<eta && eta<=-2.3){sfval = 1.00852; sferr = 0.0120181;}
  if(-2.3<eta && eta<=-2.2){sfval = 1.01047; sferr = 0.0085874;}
  if(-2.2<eta && eta<=-2){sfval = 1.00519; sferr = 0.00814729;}
  if(-2<eta && eta<=-1.8){sfval = 0.997932; sferr = 0.00754439;}
  if(-1.8<eta && eta<=-1.63){sfval = 0.991701; sferr = 0.00761482;}
  if(-1.63<eta && eta<=-1.566){sfval = 0.986486; sferr = 0.00699237;}
  if(-1.566<eta && eta<=-1.444){sfval = 0.961582; sferr = 0.0185147;}
  if(-1.444<eta && eta<=-1.2){sfval = 0.986667; sferr = 0.00602468;}
  if(-1.2<eta && eta<=-1){sfval = 0.977505; sferr = 0.00696244;}
  if(-1<eta && eta<=-0.6){sfval = 0.969388; sferr = 0.00597084;}
  if(-0.6<eta && eta<=-0.4){sfval = 0.966361; sferr = 0.00662906;}
  if(-0.4<eta && eta<=-0.2){sfval = 0.963303; sferr = 0.00634912;}
  if(-0.2<eta && eta<=0){sfval = 0.96; sferr = 0.00656714;}
  if(0<eta && eta<=0.2){sfval = 0.966189; sferr = 0.00656714;}
  if(0.2<eta && eta<=0.4){sfval = 0.979633; sferr = 0.00634912;}
  if(0.4<eta && eta<=0.6){sfval = 0.976578; sferr = 0.00662906;}
  if(0.6<eta && eta<=1){sfval = 0.980652; sferr = 0.00597084;}
  if(1<eta && eta<=1.2){sfval = 0.986735; sferr = 0.00696244;}
  if(1.2<eta && eta<=1.444){sfval = 0.98668; sferr = 0.00602468;}
  if(1.444<eta && eta<=1.566){sfval = 0.970721; sferr = 0.0185147;}
  if(1.566<eta && eta<=1.63){sfval = 0.989669; sferr = 0.00699237;}
  if(1.63<eta && eta<=1.8){sfval = 0.995872; sferr = 0.00783568;}
  if(1.8<eta && eta<=2){sfval = 0.989733; sferr = 0.007487;}
  if(2<eta && eta<=2.2){sfval = 0.994861; sferr = 0.00819214;}
  if(2.2<eta && eta<=2.3){sfval = 0.992769; sferr = 0.00850434;}
  if(2.3<eta && eta<=2.4){sfval = 0.966632; sferr = 0.0119341;}
  if(2.4<eta && eta<=2.5){sfval = 0.884021; sferr = 0.00953672;}
  return make_pair(sfval,sferr);
}
