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





void draw_allbins(TH1D **histo, int NN, const char *axisname, const char *pdfname)
{

  
    
  /*
    TPaveText *ptw = new TPaveText(0.59,0.68,0.74,0.78,"NDC");
    ptw->AddText("#bf{#pi}_{2}(1670), #bf{#pi}_{1}(1600) ?\n");
    ptw->SetTextSize(0.06);
    ptw->SetBorderSize(0);
    ptw->SetFillStyle(0);
*/


  //  char const *trange[6] = {"0 #leq -t #leq 2 GeV^{2}","0 #leq -t #leq 0.25 GeV^{2}","0.25 GeV^{2} #leq -t #leq 0.5 GeV^{2}","0.5 GeV^{2} #leq -t #leq 0.75 GeV^{2}","0.75 GeV^{2} #leq -t #leq 1 GeV^{2}","1 GeV^{2} #leq -t #leq 2 GeV^{2}"};


    
  TCanvas *canva = new TCanvas("canva","canva",15,10,400,1100);

  gPad->SetBottomMargin(0.09);
  gPad->SetTopMargin(0.01);

  histo[NN-1]->SetMinimum(0.);
  histo[NN-1]->GetXaxis()->SetTitle(axisname);
  
  for (int jj=NN-1; jj>=0; jj--){

       
    // Remove vertical gaps and sign the axis in the bottom
    //     gPad->SetFrameBorderMode(0);
    //    gPad->SetFrameLineWidth(0);

    
    setstyle(histo[jj]);    
    histo[jj]->Draw(jj==NN-1 ? "*H ][" : "*H ][ SAME");
    gPad->Update();


    TLine *hline = new TLine(gPad->GetUxmin(),0.5*jj,gPad->GetUxmax(),0.5*jj);
    hline->SetLineColor(kGray); hline->SetLineWidth(2); hline->SetLineStyle(kDashed);
    hline->Draw(jj==NN-1 ? "" : "SAME");

  }
  
  canva->Update();  

  
  canva->SaveAs(pdfname,"pdf");
  canva->Close();


}







void draw_binned_plots_v1()
{

  const Int_t NB=20;
  
  //The file with 2D histograms
  const char *datafilename = "Histos2026/correlations_data_phaseI_allcuts.root";
                             
  
 TFile *datafile = TFile::Open(datafilename,"READ");
 if (!datafile) return;


 TH2D *h2d_data_m3pi_vs_m2pi =  (TH2D*) datafile->Get("PiplusUPiminusPiminus_mass_vs_PiplusUPiminus_mass");
 TH1D *h1d_m3pi = (TH1D*) h2d_data_m3pi_vs_m2pi->ProjectionY("data_3pi_overall",0,-1);


 
 Double_t m3pi_left, m3pi_right;
 Double_t offset = 0.5;
 Int_t bin_left,bin_right,nbins;
 TH1D *hdata_2pi_mass[NB],*hdata_3pi_mass[NB];


  gStyle->SetOptStat(0);
  gStyle->SetTitleFontSize(0.08);
  
     for (int i=0; i<NB; i++){

       m3pi_left = 0.2; //0.2+0.15*i
       m3pi_right = 0.2+0.15*(i+1);

       
       //Take 2pi mass histograms as a projection
       hdata_2pi_mass[i] = h2d_data_m3pi_vs_m2pi->ProjectionX(Form("data_2pi_bin%d",i),h2d_data_m3pi_vs_m2pi->GetYaxis()->FindBin(m3pi_left),h2d_data_m3pi_vs_m2pi->GetYaxis()->FindBin(m3pi_right));
       hdata_2pi_mass[i]->GetXaxis()->SetRangeUser(0.,2.);

       
       //Define 3pi mass histograms by manual slicing to sub-histograms
       bin_left = h1d_m3pi->FindBin(m3pi_left);
       bin_right = h1d_m3pi->FindBin(m3pi_right);
       nbins = bin_right - bin_left + 1;       
       hdata_3pi_mass[i] = new TH1D(Form("data_3pi_bin%d",i),"", nbins, m3pi_left, m3pi_right);

       for (int ii = bin_left; ii <= bin_right; ii++) {
	 hdata_3pi_mass[i]->SetBinContent(ii-bin_left+1, h1d_m3pi->GetBinContent(ii));
	 hdata_3pi_mass[i]->SetBinError(ii-bin_left+1, h1d_m3pi->GetBinError(ii));
       }       
       hdata_3pi_mass[i]->GetXaxis()->SetRangeUser(0.2,3.);
       //std::cout << "Number of entries:\t" << hdata_3pi_mass[i]->GetEntries() << "\t" << hdata_2pi_mass[i]->GetEntries() << endl;
     }


      //Normalize each histogram by its maximum value and add an offset
      for (int i=0; i<NB; i++){

        hdata_3pi_mass[i]->Scale(1./hdata_3pi_mass[i]->GetMaximum());
        hdata_2pi_mass[i]->Scale(1./hdata_2pi_mass[i]->GetMaximum());

	for (int binnum = 1; binnum <= hdata_3pi_mass[i]->GetNbinsX(); binnum++) 
	  hdata_3pi_mass[i]->SetBinContent(binnum,hdata_3pi_mass[i]->GetBinContent(binnum) + i*offset);

	for (int binnum = 1; binnum <= hdata_2pi_mass[i]->GetNbinsX(); binnum++) 
	  hdata_2pi_mass[i]->SetBinContent(binnum,hdata_2pi_mass[i]->GetBinContent(binnum) + i*offset);
	
	
     }

     
       draw_allbins(hdata_3pi_mass,NB,"m(3#pi)  [GeV]","Binned_3pi_mass_v1.pdf");
       draw_allbins(hdata_2pi_mass,NB,"m(2#pi)  [GeV]","Binned_2pi_mass_v1.pdf");

     
    
   datafile->Close();

}
