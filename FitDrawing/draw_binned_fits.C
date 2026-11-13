#include "RooStats/SPlot.h"
#include "TSPlot.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"
#include "TLegend.h"
#include "TROOT.h"
#include "TChain.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooConstVar.h"
#include "RooPolynomial.h"
#include "RooAddPdf.h"
#include "RooProdPdf.h"
#include "RooFitResult.h"
#include "RooPlot.h"
#include "TH1F.h"
#include <TCanvas.h>
#include <map>
#include <string>

#include "parser.h"

using namespace std;
using namespace RooFit;







void setstyle(TH1F *dhisto, TH1F *fithisto)
{

    dhisto->GetYaxis()->SetLabelSize(0);    // no ticks at all
    dhisto->GetYaxis()->SetTitle("");
    dhisto->SetTitle("");
    fithisto->GetYaxis()->SetTitle("");
    fithisto->SetTitle("");


  
    dhisto->SetTitleOffset(0.6,"X");
    dhisto->SetTitleSize(0.07,"X");
    dhisto->GetXaxis()->SetLabelOffset(0.003);
    dhisto->GetXaxis()->SetLabelSize(0.04);
    dhisto->SetTitleOffset(1.2,"Y");
    dhisto->SetTitleSize(0.04,"Y");

    
    dhisto->SetMarkerSize(0.4);
    dhisto->SetMarkerStyle(20);
    dhisto->SetMarkerColor(kBlue+2);
    dhisto->SetLineColor(kBlack);
    dhisto->SetLineWidth(2);    
    dhisto->SetFillColor(kBlue-7);
    dhisto->SetFillStyle(0);//3016

    fithisto->SetMarkerColor(kRed);
    fithisto->SetLineColor(kRed);
    fithisto->SetLineWidth(2);    
    fithisto->SetFillColor(kRed);
    fithisto->SetFillStyle(0);

    
}





void draw_allbins(TH1F **dhisto, TH1F **fithisto, int NN, double offset, const char *axisname, const char *pdfname)
{
  
  //  char const *trange[6] = {"0 #leq -t #leq 2 GeV^{2}","0 #leq -t #leq 0.25 GeV^{2}","0.25 GeV^{2} #leq -t #leq 0.5 GeV^{2}","0.5 GeV^{2} #leq -t #leq 0.75 GeV^{2}","0.75 GeV^{2} #leq -t #leq 1 GeV^{2}","1 GeV^{2} #leq -t #leq 2 GeV^{2}"};

  int textval = 1525;
  TPaveText *ptw[NN];    
  TCanvas *canva = new TCanvas("canva","canva",15,10,400,1100);

  gPad->SetBottomMargin(0.09);
  gPad->SetTopMargin(0.01);

  dhisto[NN-1]->SetMinimum(0.);
  dhisto[NN-1]->GetXaxis()->SetTitle(axisname);
  
  for (int jj=NN-1; jj>-1; jj--){

    textval -= 50;

    ptw[jj] = new TPaveText(0.69,0.07+jj*0.073,0.84,0.17+jj*0.073,"NDC");
    ptw[jj]->AddText(Form("%d MeV\n",textval));
    ptw[jj]->SetTextSize(0.06);
    ptw[jj]->SetBorderSize(0);
    ptw[jj]->SetFillStyle(0);

    
    // Remove vertical gaps and sign the axis in the bottom
    //     gPad->SetFrameBorderMode(0);
    //    gPad->SetFrameLineWidth(0);

    
    setstyle(dhisto[jj],fithisto[jj]);

    
    dhisto[jj]->Draw(jj==NN-1 ? "*H  ][" : "*H  ][ SAME"); //*H
    if (jj > 0)    fithisto[jj]->Draw("HL ][ SAME"); 
    gPad->Update();


    TLine *hline = new TLine(gPad->GetUxmin(),offset*jj,gPad->GetUxmax(),offset*jj);
    hline->SetLineColor(kGray); hline->SetLineWidth(2); hline->SetLineStyle(kDashed);
    hline->Draw(jj==NN-1 ? "" : "SAME");

    if (strcmp(pdfname, "BinnedFit_2pi_mass_spring2017_tbin0.pdf") == 0)  ptw[jj]->Draw("SAME");
    
  }

  canva->Update();  

   
  
  canva->SaveAs(pdfname,"pdf");
  canva->Close();


}







void draw_binned_fits()
{

  const Int_t NB=11;
  TH1::AddDirectory(kFALSE); //this prevents ROOT from auto-registering histograms in gDirectory, which is a very common memory leak
  
  //The files with 1D histograms
  char const *resfilename[NB] = {
    "Histos/isops_histos_tbin0_m3pi_0.95_1.00.root",
    "Histos/isops_histos_tbin0_m3pi_1.00_1.05.root",
    "Histos/isops_histos_tbin0_m3pi_1.05_1.10.root",
    "Histos/isops_histos_tbin0_m3pi_1.10_1.15.root",
    "Histos/isops_histos_tbin0_m3pi_1.15_1.20.root",
    "Histos/isops_histos_tbin0_m3pi_1.20_1.25.root",
    "Histos/isops_histos_tbin0_m3pi_1.25_1.30.root",
    "Histos/isops_histos_tbin0_m3pi_1.30_1.35.root",
    "Histos/isops_histos_tbin0_m3pi_1.35_1.40.root",
    "Histos/isops_histos_tbin0_m3pi_1.40_1.45.root",
    "Histos/isops_histos_tbin0_m3pi_1.45_1.50.root"
  };

 TFile *resfile[NB];
 TH1F *hdata_2pi_mass[NB],*hdata_costheta_GJ[NB],*hdata_costheta_HF[NB],*hdata_phi_GJ[NB],*hdata_phi_HF[NB];
 TH1F *hfit_2pi_mass[NB],*hfit_costheta_GJ[NB],*hfit_costheta_HF[NB],*hfit_phi_GJ[NB],*hfit_phi_HF[NB];
 TH1F *hbg_2pi_mass[NB],*hbg_costheta_GJ[NB],*hbg_costheta_HF[NB],*hbg_phi_GJ[NB],*hbg_phi_HF[NB];
  
 
 
 Double_t offset = 0.6; //0.5 for 14 plots
 Int_t bin_left,bin_right,nbins;


  gStyle->SetOptStat(0);
  gStyle->SetTitleFontSize(0.08);
  
     for (int i=0; i<NB; i++){

       resfile[i] = TFile::Open(resfilename[i],"READ");
       if (!resfile[i]) return;

       
       //Read 1dim histogram projections with data 
       hdata_2pi_mass[i] = SumBySuffixPattern(resfile[i],"MIso_data");
       hdata_2pi_mass[i]->GetXaxis()->SetRangeUser(0.,2.0);

       hdata_costheta_GJ[i] = SumBySuffixPattern(resfile[i],"CosTheta_GJ_data");
       hdata_costheta_HF[i] = SumBySuffixPattern(resfile[i],"CosTheta_HF_data");
       hdata_phi_GJ[i] = SumBySuffixPattern(resfile[i],"Phi_GJ_data");
       hdata_phi_HF[i] = SumBySuffixPattern(resfile[i],"Phi_HF_data");


       //Read 1dim histogram projections with scaled MC 
       hfit_2pi_mass[i] = SumBySuffixPattern(resfile[i],"MIso_fit");
       hfit_2pi_mass[i]->GetXaxis()->SetRangeUser(0.,2.0);

       hfit_costheta_GJ[i] = SumBySuffixPattern(resfile[i],"CosTheta_GJ_fit");
       hfit_costheta_HF[i] = SumBySuffixPattern(resfile[i],"CosTheta_HF_fit");
       hfit_phi_GJ[i] = SumBySuffixPattern(resfile[i],"Phi_GJ_fit");
       hfit_phi_HF[i] = SumBySuffixPattern(resfile[i],"Phi_HF_fit");

       //Read 1dim histogram projections with the BG 
       hbg_2pi_mass[i] = SumBySuffixPattern(resfile[i],"MIso_bkgnd");
       hbg_2pi_mass[i]->GetXaxis()->SetRangeUser(0.,2.0);

       hbg_costheta_GJ[i] = SumBySuffixPattern(resfile[i],"CosTheta_GJ_bkgnd");
       hbg_costheta_HF[i] = SumBySuffixPattern(resfile[i],"CosTheta_HF_bkgnd");
       hbg_phi_GJ[i] = SumBySuffixPattern(resfile[i],"Phi_GJ_bkgnd");
       hbg_phi_HF[i] = SumBySuffixPattern(resfile[i],"Phi_HF_bkgnd");

       //Subtract bkgnd from data
       hdata_2pi_mass[i]->Add(hbg_2pi_mass[i],-1.);
       hdata_costheta_GJ[i]->Add(hbg_costheta_GJ[i],-1.);
       hdata_costheta_HF[i]->Add(hbg_costheta_HF[i],-1.);
       hdata_phi_GJ[i]->Add(hbg_phi_GJ[i],-1.);
       hdata_phi_HF[i]->Add(hbg_phi_HF[i],-1.);
       
       
       }


     //Temporary rebinning, remove when use higher amount of data
     /*
     for (int i=0; i<NB; i++){
       hdata_2pi_mass[i]->Rebin(2.);
       hdata_costheta_GJ[i]->Rebin(2.);
       hdata_costheta_HF[i]->Rebin(2.);
       hdata_phi_GJ[i]->Rebin(2.);
       hdata_phi_HF[i]->Rebin(2.);
     }
     */

     
     
      //Normalize each projection histogram by its maximum value 
      for (int i=0; i<NB; i++){

        hdata_2pi_mass[i]->Scale(1./hdata_2pi_mass[i]->GetMaximum());
	hdata_costheta_GJ[i]->Scale(1./hdata_costheta_GJ[i]->GetMaximum());
	hdata_costheta_HF[i]->Scale(1./hdata_costheta_HF[i]->GetMaximum());
	hdata_phi_GJ[i]->Scale(1./hdata_phi_GJ[i]->GetMaximum());
	hdata_phi_HF[i]->Scale(1./hdata_phi_HF[i]->GetMaximum());

        hfit_2pi_mass[i]->Scale(1./hfit_2pi_mass[i]->GetMaximum());
	hfit_costheta_GJ[i]->Scale(1./hfit_costheta_GJ[i]->GetMaximum());
	hfit_costheta_HF[i]->Scale(1./hfit_costheta_HF[i]->GetMaximum());
	hfit_phi_GJ[i]->Scale(1./hfit_phi_GJ[i]->GetMaximum());
	hfit_phi_HF[i]->Scale(1./hfit_phi_HF[i]->GetMaximum());

	/*
        hdata_2pi_mass[i]->Scale(16./hdata_2pi_mass[i]->Integral());
	hdata_costheta_GJ[i]->Scale(16./hdata_costheta_GJ[i]->Integral());
	hdata_costheta_HF[i]->Scale(16./hdata_costheta_HF[i]->Integral());
	hdata_phi_GJ[i]->Scale(16./hdata_phi_GJ[i]->Integral());
	hdata_phi_HF[i]->Scale(16./hdata_phi_HF[i]->Integral());

        hfit_2pi_mass[i]->Scale(16./hfit_2pi_mass[i]->Integral());
	hfit_costheta_GJ[i]->Scale(16./hfit_costheta_GJ[i]->Integral());
	hfit_costheta_HF[i]->Scale(16./hfit_costheta_HF[i]->Integral());
	hfit_phi_GJ[i]->Scale(16./hfit_phi_GJ[i]->Integral());
	hfit_phi_HF[i]->Scale(16./hfit_phi_HF[i]->Integral());
	*/
	
	
	//Add a vertical offset to each of the projected histograms
	for (int binnum = 1; binnum <= hdata_2pi_mass[i]->GetNbinsX(); binnum++){ 
	  hdata_2pi_mass[i]->SetBinContent(binnum,hdata_2pi_mass[i]->GetBinContent(binnum) + i*offset);
	  hfit_2pi_mass[i]->SetBinContent(binnum,hfit_2pi_mass[i]->GetBinContent(binnum) + i*offset);
	}  
	  
	for (int binnum = 1; binnum <= hdata_costheta_GJ[i]->GetNbinsX(); binnum++){ 
	  hdata_costheta_GJ[i]->SetBinContent(binnum,hdata_costheta_GJ[i]->GetBinContent(binnum) + i*offset);
	  hfit_costheta_GJ[i]->SetBinContent(binnum,hfit_costheta_GJ[i]->GetBinContent(binnum) + i*offset);
	}
	  
	for (int binnum = 1; binnum <= hdata_costheta_HF[i]->GetNbinsX(); binnum++){ 
	  hdata_costheta_HF[i]->SetBinContent(binnum,hdata_costheta_HF[i]->GetBinContent(binnum) + i*offset);
	  hfit_costheta_HF[i]->SetBinContent(binnum,hfit_costheta_HF[i]->GetBinContent(binnum) + i*offset);
	}
	  
	for (int binnum = 1; binnum <= hdata_phi_GJ[i]->GetNbinsX(); binnum++){ 
	  hdata_phi_GJ[i]->SetBinContent(binnum,hdata_phi_GJ[i]->GetBinContent(binnum) + i*offset);
	  hfit_phi_GJ[i]->SetBinContent(binnum,hfit_phi_GJ[i]->GetBinContent(binnum) + i*offset);
	}
	  
	for (int binnum = 1; binnum <= hdata_phi_HF[i]->GetNbinsX(); binnum++){ 
	  hdata_phi_HF[i]->SetBinContent(binnum,hdata_phi_HF[i]->GetBinContent(binnum) + i*offset);
	  hfit_phi_HF[i]->SetBinContent(binnum,hfit_phi_HF[i]->GetBinContent(binnum) + i*offset);
	}
	
     }

     
      draw_allbins(hdata_2pi_mass,hfit_2pi_mass,NB,offset,"m(2#pi)  [GeV]","BinnedFit_2pi_mass_spring2017_tbin0.pdf");
      draw_allbins(hdata_costheta_GJ,hfit_costheta_GJ,NB,offset,"cos#theta^{[GJ]}","BinnedFit_costheta_GJ_spring2017_tbin0.pdf");
      draw_allbins(hdata_costheta_HF,hfit_costheta_HF,NB,offset,"cos#theta^{[HF]}","BinnedFit_costheta_HF_spring2017_tbin0.pdf");
      draw_allbins(hdata_phi_GJ,hfit_phi_GJ,NB,offset,"#phi^{[GJ]}  [deg]","BinnedFit_phi_GJ_spring2017_tbin0.pdf");
      draw_allbins(hdata_phi_HF,hfit_phi_HF,NB,offset,"#phi^{[HF]}  [deg]","BinnedFit_phi_HF_spring2017_tbin0.pdf");
     

      for (int i=0; i<NB; i++)  resfile[i]->Close();

}
