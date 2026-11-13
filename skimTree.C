#include "cutter.h"




void skimTree(){
  

  //  gROOT->ProcessLine(".L cutter.C+");

  
  std::string fitdir = "/work/halld/home/ilbelov/TreesFitted/tbin0_m3pitestbin";
  std::string datadir = "/data/iltschi/ibelov/DataSpring2018";
  std::string mcsigdir = "/lustre/beloviq2/MCsignal_partial_waves";
  std::string mccombdir = "/work/halld/home/ilbelov/MCcombSpring2017";

  
 
  const char *ntracks = "NumUnusedTracks == 1";
  const char *precuts = "Chi2Ndof <= 5. && NumUnusedTracks <= 1";
   
  const char *diffpipluscut = "DiffMomPiplus >= 0.4";
  const char *deltacut = "PiplusLProton_M >= 1.15 && PiplusLProton_M <= 1.35 && tvar >= -2.";
  const char *fourpionscut = "FourPions_M >= 1.8";
  const char *piminuscut = "Piminus2_P4.Pz() >= 0.7";
  const char *cuts = "DiffMomPiplus >= 0.4 && tvar >= -2. && FourPions_M >= 1.8 && Piminus2_P4.Pz() >= 0.7";
  const char *allcuts = "PiplusLProton_M >= 1.15 && PiplusLProton_M <= 1.35 && tvar >= -2. && DiffMomPiplus >= 0.4 && FourPions_M >= 1.8 && Piminus2_P4.Pz() >= 0.7";

  const char *tcut = "tvar >= -0.25";  
  const char *temporary = "Chi2Ndof <= 5. && NumUnusedTracks <= 1 && tvar >= -2. && DiffMomPiplus >= 0.4 && FourPions_M >= 1.8 && Piminus2_P4.Pz() >= 0.7";
  const char *ALLCUTS = "Chi2Ndof <= 5. && NumUnusedTracks <= 1 && PiplusLProton_M >= 1.15 && PiplusLProton_M <= 1.35 && tvar >= -2. && DiffMomPiplus >= 0.4 && FourPions_M >= 1.8 && Piminus2_P4.Pz() >= 0.7";


  
  const char *testbincut = "tvar >= -0.25 && PiplusUPiminus1Piminus2_M >= 1.2 && PiplusUPiminus1Piminus2_M <= 1.3";  
  const char *extracut = "PiplusUPiminus1_M >= 0.5  && PiplusUPiminus1_M <= 1.1 && PiplusUPiminus2_M >= 0.5 && PiplusUPiminus2_M <= 1.1";

  
  const char *sidebandcut = "RFTimeWeight < 0.";
  const char *promptcut = "RFTimeWeight == 1.";

  const char *amocut = "DiamondSet == 0";
  const char *para0cut = "DiamondSet == 1";
  const char *perp90cut = "DiamondSet == 2";
  const char *para135cut = "DiamondSet == 3";
  const char *perp45cut = "DiamondSet == 4";
  
 
 
 
/*
    cutter("flattree_2pip2pim__sum_ALL_40856_42559_prompt_allcuts.root","kin","_AMO.root",datadir,datadir,amocut);
    cutter("flattree_2pip2pim__sum_ALL_40856_42559_sideband_allcuts.root","kin","_AMO.root",datadir,datadir,amocut);

    cutter("flattree_2pip2pim__sum_ALL_40856_42559_prompt_allcuts.root","kin","_PARA_0.root",datadir,datadir,para0cut);
    cutter("flattree_2pip2pim__sum_ALL_40856_42559_sideband_allcuts.root","kin","_PARA_0.root",datadir,datadir,para0cut);

    cutter("flattree_2pip2pim__sum_ALL_40856_42559_prompt_allcuts.root","kin","_PARA_135.root",datadir,datadir,para135cut);
    cutter("flattree_2pip2pim__sum_ALL_40856_42559_sideband_allcuts.root","kin","_PARA_135.root",datadir,datadir,para135cut);
    
    cutter("flattree_2pip2pim__sum_ALL_40856_42559_prompt_allcuts.root","kin","_PERP_45.root",datadir,datadir,perp45cut);
    cutter("flattree_2pip2pim__sum_ALL_40856_42559_sideband_allcuts.root","kin","_PERP_45.root",datadir,datadir,perp45cut);

    cutter("flattree_2pip2pim__sum_ALL_40856_42559_prompt_allcuts.root","kin","_PERP_90.root",datadir,datadir,perp90cut);
    cutter("flattree_2pip2pim__sum_ALL_40856_42559_sideband_allcuts.root","kin","_PERP_90.root",datadir,datadir,perp90cut);
*/

  
   cutter("mcsigtree_36waves_pi0_a1_a2_pi2__sum_30274_31057_precuts.root","kin","_cuts.root",mcsigdir,mcsigdir,cuts);  
   

  /*
    cutter("mctree_pippippimpim_phsp_mask010__sum_ALL_40856_42559_allcuts.root","kin","_AMO.root",mcsigdir,mcsigdir,amocut);
    cutter("mctree_pippippimpim_phsp_mask010__sum_ALL_40856_42559_allcuts.root","kin","_PARA_0.root",mcsigdir,mcsigdir,para0cut);
    cutter("mctree_pippippimpim_phsp_mask010__sum_ALL_40856_42559_allcuts.root","kin","_PARA_135.root",mcsigdir,mcsigdir,para135cut);
    cutter("mctree_pippippimpim_phsp_mask010__sum_ALL_40856_42559_allcuts.root","kin","_PERP_45.root",mcsigdir,mcsigdir,perp45cut);
    cutter("mctree_pippippimpim_phsp_mask010__sum_ALL_40856_42559_allcuts.root","kin","_PERP_90.root",mcsigdir,mcsigdir,perp90cut);
  */
  
    //  cutter("bggentree_pippippimpim__B2__sum_30274_31057.root","kin","_allcuts_nounusedtrackscut.root","/work/halld/home/ilbelov/MCbggenSpring2017","/work/halld/home/ilbelov/MCbggenSpring2017",allcuts);
 

   
}


