
#include "/work/halld/home/ilbelov/DPROOFLiteManager.h"




void runDSelsLocal(unsigned int Proof_Nthreads = 8)
{

  
  const char *folder = "";
  //  const char *input = "/volatile/halld/home/ilbelov/MC_ppippippimpim_PHSP_70M/root/thrown/tree_thrown_gen_amp_V2_030618_000.root";
  const char *input = "/volatile/halld/home/ilbelov/MC_ppippippimpim_phsp_mask011_2018-01_70M/root/thrown/gen_amp_V2_042246.root";
  const char *treename = "kin";
  //const char *treename = "Thrown_Tree";
  


  //Spring-2018
    //  const char *input1 = "/volatile/halld/home/afzal/RunPeriod-2018-01/analysis/ver24/tree_pippippimpim__B4/040856/tree_pippippimpim__B4_040856_100.root";
    //  const char *input2 = "/volatile/halld/home/afzal/RunPeriod-2018-01/analysis/ver24/tree_pippippimpim__B4/040902/tree_pippippimpim__B4_040902_030.root";
    //  const char *input3 = "/volatile/halld/home/afzal/RunPeriod-2018-01/analysis/ver24/tree_pippippimpim__B4/040907/tree_pippippimpim__B4_040907_010.root";
    //  const char *input4 = "/volatile/halld/home/afzal/RunPeriod-2018-01/analysis/ver24/tree_pippippimpim__B4/040911/tree_pippippimpim__B4_040911_020.root";
    //  const char *input5 = "/volatile/halld/home/afzal/RunPeriod-2018-01/analysis/ver24/tree_pippippimpim__B4/040949/tree_pippippimpim__B4_040949_150.root";

  
  // Spring-2017
  // const char *input1 = "/cache/halld/RunPeriod-2017-01/analysis/ver20/tree_pippippimpim__B4/merged/tree_pippippimpim__B4_031050.root";
  // const char *input2 = "/cache/halld/RunPeriod-2017-01/analysis/ver20/tree_pippippimpim__B4/merged/tree_pippippimpim__B4_030981.root";
  // const char *input3 = "/cache/halld/RunPeriod-2017-01/analysis/ver20/tree_pippippimpim__B4/merged/tree_pippippimpim__B4_030982.root";
  // const char *input4 = "/cache/halld/RunPeriod-2017-01/analysis/ver20/tree_pippippimpim__B4/merged/tree_pippippimpim__B4_030980.root";
  // const char *input5 = "/cache/halld/RunPeriod-2017-01/analysis/ver20/tree_pippippimpim__B4/merged/tree_pippippimpim__B4_031049.root";

    

     TFile *file = TFile::Open(input,"READ");
     TTree *intree = (TTree*)file->Get(treename); 



  
  /* 
  TFile *file1 = TFile::Open(input1,"READ");
  TFile *file2 = TFile::Open(input2,"READ");
  TFile *file3 = TFile::Open(input3,"READ");
  
  if ((!file1) || (!file2) || (!file3)) return;

  
  TChain *intree = new TChain("pippippimpim__B4_Tree");

  intree->Add(input1);
  intree->Add(input2);
  intree->Add(input3);
  intree->Add(input4);
  intree->Add(input5);  
  */


  
  std::cout << "The \'intree\' TChain is loaded" << endl;

  gROOT->ProcessLine(".x $ROOT_ANALYSIS_HOME/scripts/Load_DSelector.C");
  std::cout << "The DSelectors library is loaded" << endl;

  std::cout << "Now, running the \'DSelThrown_2pip2pim\' script " << endl;  
  intree->Process("DSelData_2pip2pim.C+");

  DPROOFLiteManager *dproof = new DPROOFLiteManager();
  dproof->DPROOFLiteManager::Process_Tree(input,treename,"DSelData_2pip2pim.C+", Proof_Nthreads,"","");


  
    //file->TFile::Close();
  /*
  file1->TFile::Close();
  file2->TFile::Close();
  file3->TFile::Close();
  file4->TFile::Close();
  file5->TFile::Close();
  */
}
