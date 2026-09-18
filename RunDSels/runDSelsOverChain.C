// macro to process analysis TTree with TSelector
#include <iostream> 


#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TProof.h"
#include "TProofDebug.h"

#include "/work/halld/home/ilbelov/DPROOFLiteManager.h"



void runDSelsOverChain(unsigned int Proof_Nthreads = 32) 
{



  /*
  TString runNumber = "5"; 
  const char *sampleDir = "/cache/halld/gluex_simulations/REQUESTED_MC/MC_ppippippimpim_phsp_mask010_2018-08_70M_4640/root/thrown/";
  const char *treeName = "Thrown_Tree"; 
  const char *myDSelector = "/work/halld/home/ilbelov/RunDSels/DSelThrown_2pip2pim.C";
  */

    

  TString runNumber = "5"; 
  const char *sampleDir = "/cache/halld/gluex_simulations/REQUESTED_MC/MC_ppippippimpim_phsp_mask010_2018-08_70M_4640/root/trees/tree_pippippimpim__B4_gen_amp_V2/";
  const char *treeName = "pippippimpim__B4_Tree";  
  const char *myDSelector = "/work/halld/home/ilbelov/RunDSels/DSelMC_2pip2pim.C";

  
  /*
  TString runNumber = "5";
  const char *sampleDir = "/cache/halld/RunPeriod-2018-08/analysis/ver25/tree_pippippimpim__B4/merged/";
  const char *treeName = "pippippimpim__B4_Tree";
  const char *myDSelector = "/work/halld/home/ilbelov/RunDSels/DSelData_2pip2pim.C";
  */
  
  /*
  TString runNumber = "4";
  const char *sampleDir = "/cache/halld/RunPeriod-2018-01/analysis/ver26/tree_pippippimpim__B4/merged/";
  const char *treeName = "pippippimpim__B4_Tree";
  const char *myDSelector = "/work/halld/home/ilbelov/RunDSels/DSelData_2pip2pim.C";
  */

  /*
  TString runNumber = "3";
  const char *sampleDir = "/cache/halld/RunPeriod-2017-01/analysis/ver71/tree_pippippimpim__B4/merged/";
  const char *treeName = "pippippimpim__B4_Tree";
  const char *myDSelector = "/work/halld/home/ilbelov/RunDSels/DSelData_2pip2pim.C";
  */
  
  // Load DSelector library
  gROOT->ProcessLine(".x $ROOT_ANALYSIS_HOME/scripts/Load_DSelector.C");
  std::cout << "The DSelectors library is loaded" << endl;


  //sampleDir += Form("0%s/", runNumber.Data());
  //if(myPath.Contains("thrown")) treeName = "Thrown_Tree"; 

  
  cout << "Now running selector on files in: " << sampleDir <<endl;


  TChain *chain = new TChain(treeName);
  TSystemDirectory dir(sampleDir, sampleDir);
  TList *files = dir.GetListOfFiles();
  int ifile = 0;
  if(files) {
	  TSystemFile *file;
	  TString fileName;
	  TIter next(files);
	  cout << "Found files are:" << endl;  

	  // Loop over files
	  while ((file=(TSystemFile*)next())) {
		  fileName = file->GetName();
		  if(fileName.Contains(runNumber)) {
			  cout << fileName << endl;
			  
			  // Check if file corrupted
			  TFile *f = TFile::Open(sampleDir+fileName);
			  if(f->TestBit(TFile::kRecovered)) {
				  cout<<"file corrupted -> skipping"<<endl;
				  continue;
			  }
			  if(f->IsZombie()) {
				  cout<<"file is a Zombie -> skipping"<<endl;
				  continue;
			  }
			  
			  chain->Add(sampleDir+fileName);
			  ifile++; //if (ifile == 5) break;
		  }
	  }
	  
	 cout << "Total entries in TChain = " << chain->GetEntries() << " from " << ifile << " files" << endl;
	 //chain->Process(Form("%s+", myDSelector));


	 
	 gEnv->SetValue("ProofLite.Sandbox", "/volatile/halld/home/ilbelov/.proof/");
	 
	 DPROOFLiteManager *dproof = new DPROOFLiteManager();
	 dproof->DPROOFLiteManager::Process_Chain(chain, Form("%s+", myDSelector), Proof_Nthreads,"","");

  }

  return;
}
