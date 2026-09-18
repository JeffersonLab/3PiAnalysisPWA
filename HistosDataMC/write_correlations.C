#include "calc_angles.h"



void printListOfBranches(TChain *dectree)
{
  TList:TObjArray *brslist = dectree->TChain::GetListOfBranches();
  brslist->SetOwner(kFALSE); 
  brslist->Sort();  
   for(int ii = 0; ii < brslist->GetEntries(); ii++) 
     cout << "BR #"<< ii << "\t\t" << brslist->At(ii)->GetTitle() << endl; 
}



void RemoveNegativeBins(TH2* histo)
{
  for (int ix=0; ix<=histo->GetNbinsX()+1; ++ix)
    for (int iy=0; iy<=histo->GetNbinsY()+1; ++iy)
     if (histo->GetBinContent(ix,iy) < 0) histo->SetBinContent(ix,iy,0);
}


void RemoveNegativeBins(TH3* histo)
{
  for (int ix=0; ix<=histo->GetNbinsX()+1; ++ix)
    for (int iy=0; iy<=histo->GetNbinsY()+1; ++iy)
      for (int iz=0; iz<=histo->GetNbinsZ()+1; ++iz)      
	if (histo->GetBinContent(ix,iy,iz) < 0) histo->SetBinContent(ix,iy,iz,0);
}




void LoopOverTree(TChain *tchain)
{
   
  //  tchain->SetBranchStatus("*",0);
  //  tchain->SetBranchStatus("*M",1);  
  
  const Int_t DEFBINS = 100;
  const Int_t DEFTBINS = 80;
  const Float_t ProtonMass = 0.938272;

  
  //*******************local variables****************************
  Double_t  wt, bz, deltatheta, deltaphi;
  Long64_t ientry;
  Bool_t  isFile1, isFile2; 
  
  TLorentzVector Delta_P4, X_P4;
  
  
  //********************************Reading the tree using TTreeReader**************************************
  TTreeReader *myReader = new TTreeReader(tchain);
  Long64_t nevents = myReader->TTreeReader::GetEntries(true);


  //MOMENTA
  TTreeReaderValue <TLorentzVector> PiplusL_P4(*myReader, "PiplusL_P4");
  TTreeReaderValue <TLorentzVector> PiplusU_P4(*myReader, "PiplusU_P4");
  TTreeReaderValue <TLorentzVector> Piminus1_P4(*myReader, "Piminus1_P4");
  TTreeReaderValue <TLorentzVector> Piminus2_P4(*myReader, "Piminus2_P4");
  TTreeReaderValue <TLorentzVector> Proton_P4(*myReader, "Proton_P4");

  TTreeReaderValue <TLorentzVector> Beam_Meas_P4(*myReader, "Beam_Meas_P4");
  TTreeReaderValue <TLorentzVector> PiplusL_MP4(*myReader, "PiplusL_Meas_P4");
  TTreeReaderValue <TLorentzVector> PiplusU_MP4(*myReader, "PiplusU_Meas_P4");
  TTreeReaderValue <TLorentzVector> Piminus1_MP4(*myReader, "Piminus1_Meas_P4");
  TTreeReaderValue <TLorentzVector> Piminus2_MP4(*myReader, "Piminus2_Meas_P4");
  TTreeReaderValue <TLorentzVector> Proton_MP4(*myReader, "Proton_Meas_P4");
  
  
  // MASSES
  TTreeReaderValue <Double_t> PiplusLPiminus1_M(*myReader,"PiplusLPiminus1_M");
  TTreeReaderValue <Double_t> PiplusLPiminus2_M(*myReader,"PiplusLPiminus2_M");    
  TTreeReaderValue <Double_t> PiplusUPiminus1_M(*myReader,"PiplusUPiminus1_M");
  TTreeReaderValue <Double_t> PiplusUPiminus2_M(*myReader,"PiplusUPiminus2_M");

  TTreeReaderValue <Double_t> PiplusLPiplusUPiminus1_M(*myReader,"PiplusLPiplusUPiminus1_M");
  TTreeReaderValue <Double_t> PiplusLPiplusUPiminus2_M(*myReader,"PiplusLPiplusUPiminus2_M");
  TTreeReaderValue <Double_t> PiplusLPiminus1Piminus2_M(*myReader,"PiplusLPiminus1Piminus2_M");
  TTreeReaderValue <Double_t> PiplusUPiminus1Piminus2_M(*myReader,"PiplusUPiminus1Piminus2_M");

  TTreeReaderValue <Double_t> PiplusLProton_M(*myReader,"PiplusLProton_M");
  TTreeReaderValue <Double_t> PiplusUProton_M(*myReader,"PiplusUProton_M");
  TTreeReaderValue <Double_t> Piminus1Proton_M(*myReader,"Piminus1Proton_M");
  TTreeReaderValue <Double_t> Piminus2Proton_M(*myReader,"Piminus2Proton_M");

  TTreeReaderValue <Double_t> ProtonPiplusLPiminus1_M(*myReader,"ProtonPiplusLPiminus1_M");
  TTreeReaderValue <Double_t> ProtonPiplusLPiminus2_M(*myReader,"ProtonPiplusLPiminus2_M");    
  TTreeReaderValue <Double_t> ProtonPiplusUPiminus1_M(*myReader,"ProtonPiplusUPiminus1_M");
  TTreeReaderValue <Double_t> ProtonPiplusUPiminus2_M(*myReader,"ProtonPiplusUPiminus2_M");
  TTreeReaderValue <Double_t> ProtonPiplusLPiplusUPiminus1_M(*myReader,"ProtonPiplusLPiplusUPiminus1_M");
  TTreeReaderValue <Double_t> ProtonPiplusLPiplusUPiminus2_M(*myReader,"ProtonPiplusLPiplusUPiminus2_M");
  TTreeReaderValue <Double_t> ProtonPiplusLPiminus1Piminus2_M(*myReader,"ProtonPiplusLPiminus1Piminus2_M");
  TTreeReaderValue <Double_t> ProtonPiplusUPiminus1Piminus2_M(*myReader,"ProtonPiplusUPiminus1Piminus2_M");
  TTreeReaderValue <Double_t> FourPions_M(*myReader,"FourPions_M");


  //ANGLES
  TTreeReaderValue <Double_t> CosThetaPiminus1Bachelor_GJ(*myReader,"CosTheta_Piminus1Bachelor_GJ");
  TTreeReaderValue <Double_t> CosThetaPiminus2Bachelor_GJ(*myReader,"CosTheta_Piminus2Bachelor_GJ");
  TTreeReaderValue <Double_t> CosThetaPiminus1Isobar_HF(*myReader,"CosTheta_Piminus1Isobar_HF");
  TTreeReaderValue <Double_t> CosThetaPiminus2Isobar_HF(*myReader,"CosTheta_Piminus2Isobar_HF");
  TTreeReaderValue <Double_t> PhiPiminus1Bachelor_GJ(*myReader,"Phi_Piminus1Bachelor_GJ");
  TTreeReaderValue <Double_t> PhiPiminus2Bachelor_GJ(*myReader,"Phi_Piminus2Bachelor_GJ");
  TTreeReaderValue <Double_t> PhiPiminus1Isobar_HF(*myReader,"Phi_Piminus1Isobar_HF");
  TTreeReaderValue <Double_t> PhiPiminus2Isobar_HF(*myReader,"Phi_Piminus2Isobar_HF");
  
  
  //OTHER
  TTreeReaderValue <Double_t> RFTimeWeight(*myReader,"RFTimeWeight");
  TTreeReaderValue <Double_t> tvar(*myReader,"tvar");
  TTreeReaderValue <Double_t> Miss_M2(*myReader,"Miss_M2");
  TTreeReaderValue <Float_t> E_Beam(*myReader,"E_Beam");
  TTreeReaderValue <Double_t> Chi2Ndof(*myReader,"Chi2Ndof");
  TTreeReaderValue <UInt_t> NumUnusedTracks(*myReader,"NumUnusedTracks");

  
  
  //*****************Declaring histograms to be filled********************

  //Angles and van-Hove plots
  TH2D *anglesGJ = new TH2D("AnglesGJ","",4*DEFBINS,-1.,1.,4*DEFBINS,-180.,180.);
  TH2D *anglesHF = new TH2D("AnglesHF","",4*DEFBINS,-1.,1.,4*DEFBINS,-180.,180.);
  TH2D *vanHoveXY_ppipLX = new TH2D("vanHoveXY","",4*DEFBINS,-2.5,2.5,4*DEFBINS,-2.5,2.5);
  TH2D *vanHovePhiTheta_deltapippimpim = new TH2D("vanHovePhiTheta","",4*DEFBINS,0,2*TMath::Pi(),4*DEFBINS,0,TMath::Pi());

  
  //Isobar mass and angles correlation to 3pi mass
  TH2D *pipUpimpim_mass_vs_isobar_mass = new TH2D("PiplusUPiminusPiminus_mass_vs_PiplusUPiminus_mass","",2*DEFBINS,0.,3.,2*DEFBINS,0.2,3.2);
  TH2D *pipUpimpim_mass_vs_CosTheta_GJ = new TH2D("PiplusUPiminusPiminus_mass_vs_CosTheta_GJ","",DEFBINS,-1.,1.,2*DEFBINS,0.2,3.2); //2*DEFBINS
  TH2D *pipUpimpim_mass_vs_Phi_GJ = new TH2D("PiplusUPiminusPiminus_mass_vs_Phi_GJ","",DEFBINS,-180.,180.,2*DEFBINS,0.2,3.2);
  TH2D *pipUpimpim_mass_vs_CosTheta_HF = new TH2D("PiplusUPiminusPiminus_mass_vs_CosTheta_HF","",DEFBINS,-1.,1.,2*DEFBINS,0.2,3.2);
  TH2D *pipUpimpim_mass_vs_Phi_HF = new TH2D("PiplusUPiminusPiminus_mass_vs_Phi_HF","",DEFBINS,-180.,180.,2*DEFBINS,0.2,3.2);

  
  //Masses and angles correlation to momentum transfer
  TH2D *pipUpimpim_mass_vs_minust = new TH2D("PiplusUPiminusPiminus_mass_vs_minust","",2*DEFBINS,0.2,3.2,DEFTBINS,0.,2.);
  TH2D *pipUpim_mass_vs_minust = new TH2D("PiplusUPiminus_mass_vs_minust","",2*DEFBINS,0.,3.,DEFTBINS,0.,2.);
  TH2D *ppipL_mass_vs_minust = new TH2D("ProtonPiplusL_mass_vs_minust","",2*DEFBINS,0.8,3.8,DEFTBINS,0.,2.);
  TH2D *fourpion_mass_vs_minust = new TH2D("FourPion_mass_vs_minust","",2*DEFBINS,0.5,3.5,DEFTBINS,0.,2.);
  TH3D *anglesGJ_vs_minust = new TH3D("AnglesGJ_vs_minust","",4*DEFBINS,-1.,1.,4*DEFBINS,-180.,180.,DEFTBINS,0.,2.);
  TH3D *anglesHF_vs_minust = new TH3D("AnglesHF_vs_minust","",4*DEFBINS,-1.,1.,4*DEFBINS,-180.,180.,DEFTBINS,0.,2.);


  //Correlation between the two pi+pi- masses
  TH2D *isobar1_mass_vs_isobar2_mass = new TH2D("PiplusUPiminus1_mass_vs_PiplusUPiminus2_mass","",2*DEFBINS,0.,3.,2*DEFBINS,0.,3.);

  
    
  cout << "\n\nMAIN EVENT LOOP OVER ~" << nevents/1000000 << "M EVENTS" << endl;
  wt = 1.;
  
  //************Main event loop********************************************
  while (myReader->Next()){
    ientry = myReader->GetCurrentEntry(); //if (ientry==10000) break;
    if (ientry%1000000==0){
      cout << "Arrived at event #" << ientry/1000000 << "M in the current file:" << tchain->GetCurrentFile()->GetName() << endl;
    ////dectree->Show(ientry);
    }

    //FILLING DESIRED HISTOGRAMS      
    wt = *RFTimeWeight;
    
    //    if (wt==1.){

    //Angles and van-Hove plots
    X_P4 = *PiplusU_P4 + *Piminus1_P4 + *Piminus2_P4;
    Delta_P4 = *Proton_P4 + *PiplusL_P4;
    
    vanHoveXY_ppipLX->Fill(vanHoveX(*Proton_P4,*PiplusL_P4,X_P4),vanHoveY(*Proton_P4,*PiplusL_P4,X_P4),wt);    
    vanHovePhiTheta_deltapippimpim->Fill(vanHovePhi(Delta_P4,*PiplusU_P4,*Piminus1_P4,*Piminus2_P4),vanHoveTheta(Delta_P4,*PiplusU_P4,*Piminus1_P4,*Piminus2_P4),wt);

    
    anglesGJ->Fill(*CosThetaPiminus1Bachelor_GJ,*PhiPiminus1Bachelor_GJ,wt);
    anglesGJ->Fill(*CosThetaPiminus2Bachelor_GJ,*PhiPiminus2Bachelor_GJ,wt);

    anglesHF->Fill(*CosThetaPiminus2Isobar_HF,*PhiPiminus2Isobar_HF,wt);  
    anglesHF->Fill(*CosThetaPiminus1Isobar_HF,*PhiPiminus1Isobar_HF,wt);  

    
    
    //Isobar mass and angles correlation to 3pi mass
    pipUpimpim_mass_vs_isobar_mass->Fill(*PiplusUPiminus1_M,*PiplusUPiminus1Piminus2_M,wt);
    pipUpimpim_mass_vs_isobar_mass->Fill(*PiplusUPiminus2_M,*PiplusUPiminus1Piminus2_M,wt);

    pipUpimpim_mass_vs_CosTheta_GJ->Fill(*CosThetaPiminus1Bachelor_GJ,*PiplusUPiminus1Piminus2_M,wt);
    pipUpimpim_mass_vs_CosTheta_GJ->Fill(*CosThetaPiminus2Bachelor_GJ,*PiplusUPiminus1Piminus2_M,wt);

    pipUpimpim_mass_vs_Phi_GJ->Fill(*PhiPiminus1Bachelor_GJ,*PiplusUPiminus1Piminus2_M,wt);
    pipUpimpim_mass_vs_Phi_GJ->Fill(*PhiPiminus2Bachelor_GJ,*PiplusUPiminus1Piminus2_M,wt);

    pipUpimpim_mass_vs_CosTheta_HF->Fill(*CosThetaPiminus2Isobar_HF,*PiplusUPiminus1Piminus2_M,wt);
    pipUpimpim_mass_vs_CosTheta_HF->Fill(*CosThetaPiminus1Isobar_HF,*PiplusUPiminus1Piminus2_M,wt);

    pipUpimpim_mass_vs_Phi_HF->Fill(*PhiPiminus2Isobar_HF,*PiplusUPiminus1Piminus2_M,wt);
    pipUpimpim_mass_vs_Phi_HF->Fill(*PhiPiminus1Isobar_HF,*PiplusUPiminus1Piminus2_M,wt);


    
    //Masses and angles correlation to momentum transfer    
    pipUpimpim_mass_vs_minust->Fill(*PiplusUPiminus1Piminus2_M,-*tvar,wt); 
    ppipL_mass_vs_minust->Fill(*PiplusLProton_M,-*tvar,wt);
    fourpion_mass_vs_minust->Fill(*FourPions_M,-*tvar,wt);

    pipUpim_mass_vs_minust->Fill(*PiplusUPiminus1_M,-*tvar,wt); 
    pipUpim_mass_vs_minust->Fill(*PiplusUPiminus2_M,-*tvar,wt); 

    anglesGJ_vs_minust->Fill(*CosThetaPiminus1Bachelor_GJ,*PhiPiminus1Bachelor_GJ,-*tvar,wt);
    anglesGJ_vs_minust->Fill(*CosThetaPiminus2Bachelor_GJ,*PhiPiminus2Bachelor_GJ,-*tvar,wt);
    
    anglesHF_vs_minust->Fill(*CosThetaPiminus2Isobar_HF,*PhiPiminus2Isobar_HF,-*tvar,wt);
    anglesHF_vs_minust->Fill(*CosThetaPiminus1Isobar_HF,*PhiPiminus1Isobar_HF,-*tvar,wt);
    

    //Correlation between the two isobar masses
    isobar1_mass_vs_isobar2_mass->Fill(*PiplusUPiminus1_M,*PiplusUPiminus2_M,wt);

      
      //    }
      
    }


  //Turn to zero bin entries in case they are negative  
  RemoveNegativeBins(isobar1_mass_vs_isobar2_mass);
  RemoveNegativeBins(vanHoveXY_ppipLX);
  RemoveNegativeBins(vanHovePhiTheta_deltapippimpim);

	  
  //Write histograms
  pipUpimpim_mass_vs_isobar_mass->Write();
  pipUpimpim_mass_vs_CosTheta_GJ->Write();
  pipUpimpim_mass_vs_Phi_GJ->Write();
  pipUpimpim_mass_vs_CosTheta_HF->Write();
  pipUpimpim_mass_vs_Phi_HF->Write();
  
  pipUpimpim_mass_vs_minust->Write();
  pipUpim_mass_vs_minust->Write();
  ppipL_mass_vs_minust->Write();
  fourpion_mass_vs_minust->Write();
  anglesGJ_vs_minust->Write();
  anglesHF_vs_minust->Write();
  
  anglesGJ->Write();
  anglesHF->Write();

  isobar1_mass_vs_isobar2_mass->Write();
  vanHoveXY_ppipLX->Write();
  vanHovePhiTheta_deltapippimpim->Write();  
      
}




void write_correlations(const char *input0, const char *output)
{

  const char *treename = "kin"; // remains intact


 //*******************Root files: the tree chain and its branches********************
  TFile *file0 = TFile::Open(input0,"READ");
  /*
  TFile *file1 = TFile::Open(input1,"READ");
  TFile *file2 = TFile::Open(input2,"READ");
  TFile *file3 = TFile::Open(input3,"READ");
  TFile *file4 = TFile::Open(input4,"READ");
  TFile *file5 = TFile::Open(input5,"READ");

  if (!file1 || !file2 || !file3 || !file4 || !file5) return;
  */
  
  TFile *fileOut = TFile::Open(output,"RECREATE");

  
  TChain *dectree = new TChain(treename);
  dectree->Add(input0);


  //    printListOfBranches(dectree);
    LoopOverTree(dectree);


  
    fileOut->Close();
    file0->Close();
    //    file1->Close();
    //    file2->Close();
    //    file3->Close();
    //    file4->Close();
    //    file5->Close();

}



