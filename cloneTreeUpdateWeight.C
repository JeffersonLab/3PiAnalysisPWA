void printListOfBranches(TTree *decaytree)
{
  TList:TObjArray *brslist = decaytree->TTree::GetListOfBranches();
  brslist->SetOwner(kFALSE);
  //brslist->Sort();
  //  cout << "New branches are added" << endl;
//  for(int ii = brslist->GetEntries()-1; ii > brslist->GetEntries()-2; ii--)
//    cout << "BR #"<< ii << "\t\t" << brslist->At(ii)->GetTitle() << endl;
}



void cloneTreeUpdateWeight(const char *infile, const char *outfile, const char *BranchName)
{
  const char *treename = "kin"; //remains intact   
  std::string BranchTitle = std::string(BranchName)+"/F";
  Float_t value = 1.; // for overwriting with unity


  
  TFile *fileIn  =TFile::Open(infile,"READ");
  if (!fileIn) return;
  TTree *intree = (TTree*)fileIn->Get(treename);
  
  //One can activate only the needed branches here
  intree->SetBranchStatus(BranchName, 0);

  
  TFile *fileOut = TFile::Open(outfile,"RECREATE");
  TTree *outtree = (TTree*) intree->TTree::CloneTree(0);
  //  outtree->SetName("kin");
  //  outtree->CopyEntries(dctrChain);
  outtree->Branch(BranchName,&value,BranchTitle.c_str()); 


  
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
