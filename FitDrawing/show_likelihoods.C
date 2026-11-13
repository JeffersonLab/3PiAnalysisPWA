#include <cstdlib>
#include <cstring>
#include <string>

#include "parser.h"
#include "FitInfo/minimized_likelihoods_36waves_200iters_fixedBWs.h"
//#include "FitInfo/minimized_likelihoods_36waves_100iters_floatIsoBWs.h"
//#include "FitInfo/minimized_likelihoods_36waves_100iters_floatBWWidths.h"
//#include "FitInfo/minimized_likelihoods_36waves_50iters_floatBWs.h"




void show_likelihoods()
{

  const char *pdfname = "likelihoods_36waves_200iters_fixedBWs.pdf";  
  const char *title = "Set of 36 waves (all fixed BW pars)";
  
  //recalculate edges in m(3pi)
  Double_t xedges[NBINS + 1];
  xedges[0] = Delta3Pi[0].bin_center[0] - BINWIDTH/2.0;
  for (int ibin=1; ibin<=NBINS; ibin++) xedges[ibin] = xedges[ibin-1] + BINWIDTH;

  //normalize log-likelihoods for every m3pi bin define range in Log L
  float L[NBINS]={};
  float Lnorm[NBINS][NFITS]={};
  
  for (int ibin=0; ibin < NBINS; ibin++) 
        for (int ifit=0; ifit < NFITS; ifit++) 
	   if (Delta3Pi[0].minimum[ibin][ifit] < L[ibin]) L[ibin] = Delta3Pi[0].minimum[ibin][ifit];
  
  for (int ibin=0; ibin < NBINS; ibin++) 
        for (int ifit = 0; ifit < NFITS; ifit++) 
	   Lnorm[ibin][ifit] = L[ibin]-Delta3Pi[0].minimum[ibin][ifit];
  
  //calculate the relevant range by y-axis
  float ymin = Lnorm[0][0];
  float ymax = Lnorm[0][0];

  for (int ibin = 0; ibin < NBINS; ++ibin) {
    for (int ifit = 0; ifit < NFITS; ++ifit) {
        ymin = std::min(ymin, Lnorm[ibin][ifit]);
        ymax = std::max(ymax, Lnorm[ibin][ifit]);
    }
  }
  ymax = 15.;
  ymin = -700.; //500.


  

  TH2F *likelihood_map = new TH2F("H2F", "",NBINS,xedges,100,ymin,ymax);


  for (int ibin = 0; ibin < NBINS; ibin++) 
    for (int ifit = 0; ifit < NFITS; ifit++)
        likelihood_map->Fill(Delta3Pi[0].bin_center[ibin],Lnorm[ibin][ifit]);


   likelihood_map->SetTitle(title);
   likelihood_map->GetXaxis()->SetTitle("#bf{m(3#pi)  [MeV]}");
   likelihood_map->GetYaxis()->SetTitle("#bf{2Log#it{L}_{min} - 2Log#it{L}}");

   
   //   likelihood_map->Scale(1.0 / likelihood_map->Integral());   
      likelihood_map->SetMinimum(1.);
      likelihood_map->SetMaximum(1000.);

      
   likelihood_map->GetXaxis()->SetTitleOffset(1.);
   likelihood_map->GetYaxis()->SetTitleOffset(1.);   
   likelihood_map->GetXaxis()->SetTitleSize(0.06);
   likelihood_map->GetYaxis()->SetTitleSize(0.06);
   likelihood_map->GetXaxis()->SetLabelSize(0.05);
   likelihood_map->GetYaxis()->SetLabelSize(0.04);
   
   
   
   //   gROOT->SetBatch(kTRUE);  
   gStyle->SetOptStat(0);
   //   gStyle->SetTextSize(0.08);
   gStyle->SetPalette(kViridis);
   gStyle->SetPalette(kTemperatureMap);

   
      
      TCanvas* canva = new TCanvas("c1","c1",15,10,400,300);
      gPad->SetBottomMargin(0.12);   
      gPad->SetLeftMargin(0.12);
	//        gPad->SetRightMargin(0.12);
        gPad->SetLogz();

      likelihood_map->Draw("COLZ");      
      canva->Update();


      
      canva->Print(pdfname, "pdf");
      canva->Close();
   
      likelihood_map->Delete();
      
  }




