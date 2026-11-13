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

using namespace std;
using namespace RooFit;







void setstyle(TH1D *dhisto)
{
    
    dhisto->SetTitleOffset(0.6,"X");
    dhisto->SetTitleSize(0.07,"X");
    dhisto->GetXaxis()->SetLabelOffset(0.003);
    dhisto->GetXaxis()->SetLabelSize(0.04);
    dhisto->SetTitleOffset(1.2,"Y");
    dhisto->SetTitleSize(0.04,"Y");
    dhisto->GetYaxis()->SetLabelSize(0);
    
    dhisto->SetMarkerSize(0.4);
    dhisto->SetMarkerStyle(20);
    dhisto->SetMarkerColor(kBlue+2);
    dhisto->SetLineColor(kBlack);
    dhisto->SetLineWidth(2);    
    dhisto->SetFillColor(kBlue-7);
    dhisto->SetFillStyle(0);//3016
}





void draw_allbins(TH1D **histo, int NN, double offset, const char *axisname, const char *pdfname)
{
  
  //  char const *trange[6] = {"0 #leq -t #leq 2 GeV^{2}","0 #leq -t #leq 0.25 GeV^{2}","0.25 GeV^{2} #leq -t #leq 0.5 GeV^{2}","0.5 GeV^{2} #leq -t #leq 0.75 GeV^{2}","0.75 GeV^{2} #leq -t #leq 1 GeV^{2}","1 GeV^{2} #leq -t #leq 2 GeV^{2}"};

  float textval = 1475;
  TPaveText *ptw[NN];    
  TCanvas *canva = new TCanvas("canva","canva",15,10,400,1100);

  gPad->SetBottomMargin(0.09);
  gPad->SetTopMargin(0.01);

  histo[NN-1]->SetMinimum(0.);
  histo[NN-1]->GetXaxis()->SetTitle(axisname);
  
  for (int jj=NN-1; jj>-1; jj--){

    textval -= 25;

    ptw[jj] = new TPaveText(0.69,0.07+jj*0.073,0.84,0.17+jj*0.073,"NDC");
    ptw[jj]->AddText(Form("%.f MeV\n",textval+12.5));
    ptw[jj]->SetTextSize(0.06);
    ptw[jj]->SetBorderSize(0);
    ptw[jj]->SetFillStyle(0);

    
    // Remove vertical gaps and sign the axis in the bottom
    //     gPad->SetFrameBorderMode(0);
    //    gPad->SetFrameLineWidth(0);

    
    setstyle(histo[jj]);    
    histo[jj]->Draw(jj==NN-1 ? "*H  ][" : "*H  ][ SAME"); //*H
    gPad->Update();


    TLine *hline = new TLine(gPad->GetUxmin(),offset*jj,gPad->GetUxmax(),offset*jj);
    hline->SetLineColor(kGray); hline->SetLineWidth(2); hline->SetLineStyle(kDashed);
    hline->Draw(jj==NN-1 ? "" : "SAME");

    if (strcmp(pdfname, "Binned_2pi_mass_tbin0.pdf") == 0)  ptw[jj]->Draw("SAME");
    
  }

  canva->Update();  

   
  
  canva->SaveAs(pdfname,"pdf");
  canva->Close();


}







void draw_binned_plots()
{

  const Int_t NB=11;
  const Double_t MIN = 1.200;
  
  //The file with 2D histograms
  const char *datafilename = "Histos2026/correlations_data_phaseI_allcuts_tbin0.root";

  
 TFile *datafile = TFile::Open(datafilename,"READ");
 if (!datafile) return;


 TH2D *h2d_data_m3pi_vs_m2pi =  (TH2D*) datafile->Get("PiplusUPiminusPiminus_mass_vs_PiplusUPiminus_mass");
 TH2D *h2d_data_m3pi_vs_costheta_GJ =  (TH2D*) datafile->Get("PiplusUPiminusPiminus_mass_vs_CosTheta_GJ");
 TH2D *h2d_data_m3pi_vs_costheta_HF =  (TH2D*) datafile->Get("PiplusUPiminusPiminus_mass_vs_CosTheta_HF");
 TH2D *h2d_data_m3pi_vs_phi_GJ =  (TH2D*) datafile->Get("PiplusUPiminusPiminus_mass_vs_Phi_GJ");
 TH2D *h2d_data_m3pi_vs_phi_HF =  (TH2D*) datafile->Get("PiplusUPiminusPiminus_mass_vs_Phi_HF"); 
 TH1D *h1d_m3pi = (TH1D*) h2d_data_m3pi_vs_m2pi->ProjectionY("data_3pi_overall",0,-1);

 

 Double_t m3pi_left, m3pi_right;
 Double_t offset = 0.6; //0.5 for 14 plots
 Int_t bin_left,bin_right,nbins;
 TH1D *hdata_2pi_mass[NB],*hdata_costheta_GJ[NB],*hdata_costheta_HF[NB],*hdata_phi_GJ[NB],*hdata_phi_HF[NB];


  gStyle->SetOptStat(0);
  gStyle->SetTitleFontSize(0.08);
  
     for (int i=0; i<NB; i++){

       m3pi_left = MIN+0.025*i;
       m3pi_right = MIN+0.025*(i+1);

       
       //Take 1dim histograms as projections
       hdata_2pi_mass[i] = h2d_data_m3pi_vs_m2pi->ProjectionX(Form("data_2pi_bin%d",i),h2d_data_m3pi_vs_m2pi->GetYaxis()->FindBin(m3pi_left),h2d_data_m3pi_vs_m2pi->GetYaxis()->FindBin(m3pi_right));
       hdata_2pi_mass[i]->GetXaxis()->SetRangeUser(0.,2.);

       hdata_costheta_GJ[i] = h2d_data_m3pi_vs_costheta_GJ->ProjectionX(Form("data_costhetaGJ_bin%d",i),h2d_data_m3pi_vs_costheta_GJ->GetYaxis()->FindBin(m3pi_left),h2d_data_m3pi_vs_costheta_GJ->GetYaxis()->FindBin(m3pi_right));
       hdata_costheta_HF[i] = h2d_data_m3pi_vs_costheta_HF->ProjectionX(Form("data_costhetaHF_bin%d",i),h2d_data_m3pi_vs_costheta_HF->GetYaxis()->FindBin(m3pi_left),h2d_data_m3pi_vs_costheta_HF->GetYaxis()->FindBin(m3pi_right));
       hdata_phi_GJ[i] = h2d_data_m3pi_vs_phi_GJ->ProjectionX(Form("data_phiGJ_bin%d",i),h2d_data_m3pi_vs_phi_GJ->GetYaxis()->FindBin(m3pi_left),h2d_data_m3pi_vs_phi_GJ->GetYaxis()->FindBin(m3pi_right));
       hdata_phi_HF[i] = h2d_data_m3pi_vs_phi_HF->ProjectionX(Form("data_phiHF_bin%d",i),h2d_data_m3pi_vs_phi_HF->GetYaxis()->FindBin(m3pi_left),h2d_data_m3pi_vs_phi_HF->GetYaxis()->FindBin(m3pi_right));
       
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

     
     
      //Normalize each histogram by its maximum value and add an offset
      for (int i=0; i<NB; i++){
	
        hdata_2pi_mass[i]->Scale(1./hdata_2pi_mass[i]->GetMaximum());
	hdata_costheta_GJ[i]->Scale(1./hdata_costheta_GJ[i]->GetMaximum());
	hdata_costheta_HF[i]->Scale(1./hdata_costheta_HF[i]->GetMaximum());
	hdata_phi_GJ[i]->Scale(1./hdata_phi_GJ[i]->GetMaximum());
	hdata_phi_HF[i]->Scale(1./hdata_phi_HF[i]->GetMaximum());

	
	for (int binnum = 1; binnum <= hdata_2pi_mass[i]->GetNbinsX(); binnum++) 
	  hdata_2pi_mass[i]->SetBinContent(binnum,hdata_2pi_mass[i]->GetBinContent(binnum) + i*offset);

	for (int binnum = 1; binnum <= hdata_costheta_GJ[i]->GetNbinsX(); binnum++) 
	  hdata_costheta_GJ[i]->SetBinContent(binnum,hdata_costheta_GJ[i]->GetBinContent(binnum) + i*offset);

	for (int binnum = 1; binnum <= hdata_costheta_HF[i]->GetNbinsX(); binnum++) 
	  hdata_costheta_HF[i]->SetBinContent(binnum,hdata_costheta_HF[i]->GetBinContent(binnum) + i*offset);

	for (int binnum = 1; binnum <= hdata_phi_GJ[i]->GetNbinsX(); binnum++) 
	  hdata_phi_GJ[i]->SetBinContent(binnum,hdata_phi_GJ[i]->GetBinContent(binnum) + i*offset);

	for (int binnum = 1; binnum <= hdata_phi_HF[i]->GetNbinsX(); binnum++) 
	  hdata_phi_HF[i]->SetBinContent(binnum,hdata_phi_HF[i]->GetBinContent(binnum) + i*offset);

	
     }

     
      draw_allbins(hdata_2pi_mass,NB,offset,"m(2#pi)  [GeV]","Binned_2pi_mass_tbin0.pdf");
      draw_allbins(hdata_costheta_GJ,NB,offset,"cos#theta^{[GJ]}","Binned_costheta_GJ_tbin0.pdf");
      draw_allbins(hdata_costheta_HF,NB,offset,"cos#theta^{[HF]}","Binned_costheta_HF_tbin0.pdf");
      draw_allbins(hdata_phi_GJ,NB,offset,"#phi^{[GJ]}  [deg]","Binned_phi_GJ_tbin0.pdf");
      draw_allbins(hdata_phi_HF,NB,offset,"#phi^{[HF]}  [deg]","Binned_phi_HF_tbin0.pdf");
     

    
   datafile->Close();

}
