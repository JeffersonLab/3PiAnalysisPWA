void printListOfBranches(TTree *decaytree)
{
  TList:TObjArray *brslist = decaytree->TTree::GetListOfBranches();
  brslist->SetOwner(kFALSE);
  //brslist->Sort();
  //  cout << "New branches are added" << endl;
//  for(int ii = brslist->GetEntries()-1; ii > brslist->GetEntries()-2; ii--)
//    cout << "BR #"<< ii << "\t\t" << brslist->At(ii)->GetTitle() << endl;
}



void cloneTreeUpdateWeightInner(const char *infile, const char *outfile, Float_t value)
{
  //later change function with rewriting two instead of one paramaters
  const char *treename = "kin"; //remains intact   
  const char *BranchName = "Py_Beam";
  const char *BranchTitle = "Py_Beam/F"; //available for passing an extra parameter

  
  TFile *fileIn  =TFile::Open(infile,"READ");
  if (!fileIn) return;
  TTree *intree = (TTree*)fileIn->Get(treename);
  
  //One can activate only the needed branches here
  intree->SetBranchStatus(BranchName, 0);

  
  TFile *fileOut = TFile::Open(outfile,"RECREATE");
  TTree *outtree = (TTree*) intree->TTree::CloneTree(0);
  //  outtree->SetName("kin");
  //  outtree->CopyEntries(dctrChain);
  outtree->Branch(BranchName,&value,BranchTitle); 


  
  TTreeReader *myReader = new TTreeReader(intree);
  Long64_t nevents = myReader->TTreeReader::GetEntries(true);
  Long64_t ientry;  


  //  TTreeReaderValue <Double_t> wt(*myReader,"MCWeight");

  
  //read event by event and replace the 'BranchName' entry to a new Float type value
  while (myReader->Next()){
    ientry = myReader->GetCurrentEntry();
    intree->GetEntry(ientry);    
    outtree->Fill();
  }


  
   fileIn->Close();
  
   printListOfBranches(outtree);
   outtree->Write();
   fileOut->Close();
     
}



void cloneTreeUpdateWeight()
{

/*
cloneTreeUpdateWeightInner("/lustre/beloviq2/3PiTreesToFit/Fall2018/flattree_2pip2pim__sum_PARA_0_50685_51768_prompt_allcuts.root","/lustre/beloviq2/3PiTreesToFit/Fall2018/tmp/flattree_2pip2pim__sum_PARA_0_50685_51768_prompt_allcuts.root",3.3);
cloneTreeUpdateWeightInner("/lustre/beloviq2/3PiTreesToFit/Fall2018/flattree_2pip2pim__sum_PARA_0_50685_51768_sideband_allcuts.root","/lustre/beloviq2/3PiTreesToFit/Fall2018/tmp/flattree_2pip2pim__sum_PARA_0_50685_51768_sideband_allcuts.root",3.3);  
cloneTreeUpdateWeightInner("/lustre/beloviq2/3PiTreesToFit/Fall2018/mctree_pippippimpim_phsp_mask010__sum_PARA_0_50685_51768_allcuts.root","/lustre/beloviq2/3PiTreesToFit/Fall2018/tmp/mctree_pippippimpim_phsp_mask010__sum_PARA_0_50685_51768_allcuts.root",3.3);

  
cloneTreeUpdateWeightInner("/lustre/beloviq2/3PiTreesToFit/Fall2018/flattree_2pip2pim__sum_PERP_45_50685_51768_prompt_allcuts.root","/lustre/beloviq2/3PiTreesToFit/Fall2018/tmp/flattree_2pip2pim__sum_PERP_45_50685_51768_prompt_allcuts.root",48.3);
cloneTreeUpdateWeightInner("/lustre/beloviq2/3PiTreesToFit/Fall2018/flattree_2pip2pim__sum_PERP_45_50685_51768_sideband_allcuts.root","/lustre/beloviq2/3PiTreesToFit/Fall2018/tmp/flattree_2pip2pim__sum_PERP_45_50685_51768_sideband_allcuts.root",48.3);  
cloneTreeUpdateWeightInner("/lustre/beloviq2/3PiTreesToFit/Fall2018/mctree_pippippimpim_phsp_mask010__sum_PERP_45_50685_51768_allcuts.root","/lustre/beloviq2/3PiTreesToFit/Fall2018/tmp/mctree_pippippimpim_phsp_mask010__sum_PERP_45_50685_51768_allcuts.root",48.3);
  
cloneTreeUpdateWeightInner("/lustre/beloviq2/3PiTreesToFit/Fall2018/flattree_2pip2pim__sum_PERP_90_50685_51768_prompt_allcuts.root","/lustre/beloviq2/3PiTreesToFit/Fall2018/tmp/flattree_2pip2pim__sum_PERP_90_50685_51768_prompt_allcuts.root",92.9);
cloneTreeUpdateWeightInner("/lustre/beloviq2/3PiTreesToFit/Fall2018/flattree_2pip2pim__sum_PERP_90_50685_51768_sideband_allcuts.root","/lustre/beloviq2/3PiTreesToFit/Fall2018/tmp/flattree_2pip2pim__sum_PERP_90_50685_51768_sideband_allcuts.root",92.9);  
cloneTreeUpdateWeightInner("/lustre/beloviq2/3PiTreesToFit/Fall2018/mctree_pippippimpim_phsp_mask010__sum_PERP_90_50685_51768_allcuts.root","/lustre/beloviq2/3PiTreesToFit/Fall2018/tmp/mctree_pippippimpim_phsp_mask010__sum_PERP_90_50685_51768_allcuts.root",92.9);


cloneTreeUpdateWeightInner("/lustre/beloviq2/3PiTreesToFit/Fall2018/flattree_2pip2pim__sum_PARA_135_50685_51768_prompt_allcuts.root","/lustre/beloviq2/3PiTreesToFit/Fall2018/tmp/flattree_2pip2pim__sum_PARA_135_50685_51768_prompt_allcuts.root",137.9);
cloneTreeUpdateWeightInner("/lustre/beloviq2/3PiTreesToFit/Fall2018/flattree_2pip2pim__sum_PARA_135_50685_51768_sideband_allcuts.root","/lustre/beloviq2/3PiTreesToFit/Fall2018/tmp/flattree_2pip2pim__sum_PARA_135_50685_51768_sideband_allcuts.root",137.9);  
cloneTreeUpdateWeightInner("/lustre/beloviq2/3PiTreesToFit/Fall2018/mctree_pippippimpim_phsp_mask010__sum_PARA_135_50685_51768_allcuts.root","/lustre/beloviq2/3PiTreesToFit/Fall2018/tmp/mctree_pippippimpim_phsp_mask010__sum_PARA_135_50685_51768_allcuts.root",137.9);
*/ 

cloneTreeUpdateWeightInner("/lustre/beloviq2/3PiTreesToFit/Spring2017/flattree_2pip2pim__sum_POL_30274_31057_prompt_allcuts.root","/lustre/beloviq2/3PiTreesToFit/Spring2017/flattree_2pip2pim__sum_POL_30274_31057_prompt_allcuts_copied.root",0.350);
cloneTreeUpdateWeightInner("/lustre/beloviq2/3PiTreesToFit/Spring2017/flattree_2pip2pim__sum_POL_30274_31057_sideband_allcuts.root","/lustre/beloviq2/3PiTreesToFit/Spring2017/flattree_2pip2pim__sum_POL_30274_31057_sideband_allcuts_copied.root",0.350);
cloneTreeUpdateWeightInner("/lustre/beloviq2/3PiTreesToFit/Spring2017/mctree_pippippimpim_phsp_mask010__sum_POL_30274_31057_allcuts.root","/lustre/beloviq2/3PiTreesToFit/Spring2017/mctree_pippippimpim_phsp_mask010__sum_POL_30274_31057_allcuts_copied.root",0.350);



  
}

