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
  

    TPaveText *ptw1 = new TPaveText(0.68,0.22,0.83,0.32,"NDC");
    ptw1->AddText("1075 MeV\n");
    ptw1->SetTextSize(0.06);
    ptw1->SetBorderSize(0);
    ptw1->SetFillStyle(0);

    TPaveText *ptw2 = new TPaveText(0.68,0.3,0.83,0.4,"NDC");
    ptw2->AddText("1125 MeV\n");
    ptw2->SetTextSize(0.06);
    ptw2->SetBorderSize(0);
    ptw2->SetFillStyle(0);
    
    TPaveText *ptw3 = new TPaveText(0.68,0.37,0.83,0.47,"NDC");
    ptw3->AddText("1175 MeV\n");
    ptw3->SetTextSize(0.06);
    ptw3->SetBorderSize(0);
    ptw3->SetFillStyle(0);
  
    TPaveText *ptw4 = new TPaveText(0.68,0.44,0.83,0.54,"NDC");
    ptw4->AddText("1225 MeV\n");
    ptw4->SetTextSize(0.06);
    ptw4->SetBorderSize(0);
    ptw4->SetFillStyle(0);

    TPaveText *ptw5 = new TPaveText(0.68,0.51,0.83,0.61,"NDC");
    ptw5->AddText("1275 MeV\n");
    ptw5->SetTextSize(0.06);
    ptw5->SetBorderSize(0);
    ptw5->SetFillStyle(0);

    
    TPaveText *ptw6 = new TPaveText(0.68,0.58,0.83,0.68,"NDC");
    ptw6->AddText("1325 MeV\n");
    ptw6->SetTextSize(0.06);
    ptw6->SetBorderSize(0);
    ptw6->SetFillStyle(0);

    

  //  char const *trange[6] = {"0 #leq -t #leq 2 GeV^{2}","0 #leq -t #leq 0.25 GeV^{2}","0.25 GeV^{2} #leq -t #leq 0.5 GeV^{2}","0.5 GeV^{2} #leq -t #leq 0.75 GeV^{2}","0.75 GeV^{2} #leq -t #leq 1 GeV^{2}","1 GeV^{2} #leq -t #leq 2 GeV^{2}"};


    
  TCanvas *canva = new TCanvas("canva","canva",15,10,400,1100);

  gPad->SetBottomMargin(0.09);
  gPad->SetTopMargin(0.01);

  histo[NN-1]->SetMinimum(0.);
  histo[NN-1]->GetXaxis()->SetTitle(axisname);
  
  for (int jj=NN-1; jj>-1; jj--){

       
    // Remove vertical gaps and sign the axis in the bottom
    //     gPad->SetFrameBorderMode(0);
    //    gPad->SetFrameLineWidth(0);

    
    setstyle(histo[jj]);    
    histo[jj]->Draw(jj==NN-1 ? "*H  ][" : "*H  ][ SAME"); //*H
    gPad->Update();


    TLine *hline = new TLine(gPad->GetUxmin(),offset*jj,gPad->GetUxmax(),offset*jj);
    hline->SetLineColor(kGray); hline->SetLineWidth(2); hline->SetLineStyle(kDashed);
    hline->Draw(jj==NN-1 ? "" : "SAME");

  }

  canva->Update();  

  if (strcmp(pdfname, "Binned_2pi_mass_tbin0.pdf") == 0){
  ptw1->Draw("SAME");
  ptw2->Draw("SAME");
  ptw3->Draw("SAME");
  ptw4->Draw("SAME");
  ptw5->Draw("SAME");
  ptw6->Draw("SAME");
  }
  
  canva->SaveAs(pdfname,"pdf");
  canva->Close();


}







void draw_binned_plots()
{

  const Int_t NB=11;
  
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

       m3pi_left = 1.0+0.05*i;
       m3pi_right = 1.0+0.05*(i+1);

       
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
