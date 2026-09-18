#include "cutter.h"




void skimTreeArgs(const char *inputdir, const char *outputdir, const char *filename, const char *cuts){

  
  //  gROOT->ProcessLine(".L cutter.C+");     
  
  cutter(filename,"kin",".root",inputdir,outputdir,cuts);
 

   
}
