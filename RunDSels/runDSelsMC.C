#include "TProof.h"
#include "TProofDebug.h"
#include "/work/halld/home/ilbelov/DPROOFLiteManager.h"


void runDSelsMC(unsigned int Proof_Nthreads = 32)
{
 
  const char *folder = "";

  const char *input0 = "/work/halld/home/ilbelov/OLD/MCphspOldSpring2017/ThreeSlopesSamples/tree_pippippimpim__B4_gen_amp_V2_3slopes__sum_30274_31057.root";
  //  const char *input0 = "/work/halld/home/ilbelov/MCcombSpring2017/tree_pippippimpim__B4_phsp_bgcomb5_gen_amp_V2_3slopes__sum_030274_031057.root";
  //  const char *input0 = "/cache/halld/gluex_simulations/REQUESTED_MC/MC_ppippippimpim_phsp_mask010_2018-08_70M_4640/root/thrown/tree_thrown_gen_amp_V2_051764.root";

  const char *myDSelector = "/work/halld/home/ilbelov/RunDSels/DSelMC_2pip2pim.C";

  
  //  TFile *file0 = TFile::Open(input0,"READ");
  //  TTree *intree = (TTree*)file0->Get("pippippimpim__B4_Tree"); 
  
  /*
  TFile *file1 = TFile::Open(input1,"READ");
  TFile *file2 = TFile::Open(input2,"READ");
  TFile *file3 = TFile::Open(input3,"READ");
  
  if ((!file1) || (!file2) || (!file3)) return;
  */
  

  TChain *intree = new TChain("pippippimpim__B4_Tree");
  intree->Add(input0);
  
  /*
  intree->Add(input1);
  intree->Add(input2);
  intree->Add(input3);
  intree->Add(input4);
  intree->Add(input5);  
  */

  
  std::cout << "The \'intree\' TChain is loaded" << endl;

  // Load DSelector library
  gROOT->ProcessLine(".x $ROOT_ANALYSIS_HOME/scripts/Load_DSelector.C");
  std::cout << "The DSelectors library is loaded" << endl;

   //  std::cout << "Now, running the \'DSelMC_2pip2pim\' script " << endl;  
   //  intree->Process("DSelMC_2pip2pim.C+");
  
  
   gEnv->SetValue("ProofLite.Sandbox", "/volatile/halld/home/ilbelov/.proof/");	 
   DPROOFLiteManager *dproof = new DPROOFLiteManager();
   dproof->DPROOFLiteManager::Process_Chain(intree, Form("%s+", myDSelector), Proof_Nthreads,"","");
   //or call Process_Tree




  /*
  file1->TFile::Close();
  file2->TFile::Close();
  file3->TFile::Close();
  file4->TFile::Close();
  file5->TFile::Close();
  */
}
