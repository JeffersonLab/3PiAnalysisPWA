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
    dhisto->SetMarkerSize(0.4);
    dhisto->SetMarkerStyle(20);
    dhisto->SetMarkerColor(kBlue+3);
    dhisto->GetXaxis()->SetTitleOffset(1.0);
    dhisto->GetYaxis()->SetTitleOffset(1.0);
    dhisto->GetXaxis()->SetTitleSize(0.07);
    dhisto->GetYaxis()->SetTitleSize(0.07); 
    dhisto->GetXaxis()->SetLabelSize(0.06);
    dhisto->GetYaxis()->SetLabelSize(0.06);
    dhisto->SetLineColor(kBlack);
    dhisto->SetLineWidth(2);    
    dhisto->SetFillColor(kBlue-7);
    dhisto->SetFillStyle(3016);
}





void draw_bin(TH1D *h_ppip, TH1D *h_3pi, TH1D *h_2pi, char const *title, int i)
{

  std::string bin_num = Form("%d",i);
  std::string pdfname = "BinnedFigures/bin"+bin_num+".pdf";

  h_ppip->SetMinimum(0.);   
  h_3pi->SetMinimum(0.);
  h_2pi->SetMinimum(0.);

  
  setstyle(h_ppip);
  setstyle(h_3pi);
  setstyle(h_2pi);


  //  h_ppip->SetTitle(Form("    %s",title));
  h_ppip->GetXaxis()->SetTitle("m(p#pi^{+})  [GeV]");
  h_ppip->GetXaxis()->SetRangeUser(0.8,3.);

  h_2pi->GetXaxis()->SetTitle("m(2#pi)  [GeV]");
  h_3pi->GetXaxis()->SetTitle("m(3#pi)  [GeV]");


  if (i<2){
    h_ppip->GetYaxis()->SetTitle("Events / 15 MeV");
    h_2pi->GetYaxis()->SetTitle("Events / 15 MeV");
    h_3pi->GetYaxis()->SetTitle("Events / 15 MeV");
  }
  

    TPaveText *ptw = new TPaveText(0.6,0.68,0.75,0.78,"NDC");
    ptw->AddText("#bf{#pi}_{2}(1670), #bf{#pi}_{1}(1600) ?\n");
    ptw->SetTextSize(0.06);
    ptw->SetBorderSize(0);
    ptw->SetFillStyle(0);

    TPaveText *pte = new TPaveText(0.5,0.38,0.6,0.48,"NDC");
    pte->AddText("f_{2}(1270)\n");
    pte->SetTextSize(0.06); // was 0.08
    pte->SetBorderSize(0);
    pte->SetFillStyle(0);

    TPaveText *ptr = new TPaveText(0.53,0.75,0.63,0.85,"NDC");
    ptr->AddText("a_{2}(1320)\n");
    ptr->SetTextSize(0.06); // was 0.08
    ptr->SetBorderSize(0);
    ptr->SetFillStyle(0);

    TPaveText *pts = new TPaveText(0.41,0.75,0.51,0.85,"NDC");
    pts->AddText("#rho(770)\n");
    pts->SetTextSize(0.06); // was 0.08
    pts->SetBorderSize(0);
    pts->SetFillStyle(0);


    
  
  //New canvas to draw everything on // PLOTTING STARTS HERE ////////////////////////////////////////////
  TCanvas *canva = new TCanvas(Form("c_%s",title),Form("c_%s",title),15,10,400,1100); 
  canva->Divide(1,3);

  canva->cd(1);
  gPad->SetBottomMargin(0.15);
  gPad->SetLeftMargin(0.14);
  TGaxis::SetMaxDigits(4);

  h_ppip->Draw("*H"); 
  gPad->Update();
  TLine *lrange = new TLine(1.15,0.,1.15,gPad->GetUymax());
  TLine *rrange = new TLine(1.35,0.,1.35,gPad->GetUymax());
  lrange->SetLineColor(kRed); lrange->SetLineWidth(3); lrange->SetLineStyle(kDashed);
  rrange->SetLineColor(kRed); rrange->SetLineWidth(3); rrange->SetLineStyle(kDashed);
  lrange->Draw("SAME");
  rrange->Draw("SAME");
  canva->Update();  



  
  
  canva->cd(2);
  gPad->SetBottomMargin(0.15);
  gPad->SetLeftMargin(0.14);
  TGaxis::SetMaxDigits(4);

  h_3pi->Draw("*H");
  if (i==1) ptr->Draw("SAME");
  if (i==2) ptw->Draw("SAME");
  canva->Update();

  canva->cd(3);
  gPad->SetBottomMargin(0.15);
  gPad->SetLeftMargin(0.14);
  TGaxis::SetMaxDigits(4);
  
  h_2pi->Draw("*H"); 
  if (i==1) pts->Draw("SAME");
  if (i==2) pte->Draw("SAME");
  canva->Update();


  
  canva->SaveAs(pdfname.c_str(),"pdf");
  canva->Close();


}



void draw_bin_abc(TH1D *h_ppip, TH1D *h_3pi, TH1D *h_2pi, char const *title, int i)
{

  std::string bin_num = Form("%d",i);
  std::string pdfname_a = "BinnedFigures/bin"+bin_num+"_a_mod.pdf";
  std::string pdfname_b = "BinnedFigures/bin"+bin_num+"_b_mod.pdf";
  std::string pdfname_c = "BinnedFigures/bin"+bin_num+"_c_mod.pdf";

  
  h_ppip->SetMinimum(0.); 
  h_3pi->SetMinimum(0.);
  h_2pi->SetMinimum(0.);

  
  setstyle(h_ppip);
  setstyle(h_3pi);
  setstyle(h_2pi);


  //  h_ppip->SetTitle(Form("    %s",title));
  h_ppip->GetXaxis()->SetTitle("m(p#pi^{+})  [GeV]");
  h_ppip->GetXaxis()->SetRangeUser(0.8,2.5);//3.

  //  h_2pi->SetTitle(Form("    %s",title));
  h_2pi->GetXaxis()->SetTitle("m(2#pi)  [GeV]");
  h_2pi->GetXaxis()->SetRangeUser(0.0,2.5); //3.


  //  h_3pi->SetTitle(Form("    %s",title));
  h_3pi->GetXaxis()->SetTitle("m(3#pi)  [GeV]");
  h_3pi->GetXaxis()->SetRangeUser(0.2,2.5); //3.

  
  h_ppip->GetYaxis()->SetTitle("Events / 15 MeV");
  h_2pi->GetYaxis()->SetTitle("Events / 15 MeV");
  h_3pi->GetYaxis()->SetTitle("Events / 15 MeV");

  

    TPaveText *ptw = new TPaveText(0.61,0.55,0.76,0.65,"NDC"); //0.6,0.68,0.75,0.78
    ptw->AddText("#bf{#pi}_{2}(1670), #bf{#pi}_{1}(1600) ?\n");
    ptw->SetTextSize(0.06);
    ptw->SetBorderSize(0);
    ptw->SetFillStyle(0);

    TPaveText *pte = new TPaveText(0.5,0.38,0.6,0.48,"NDC");
    pte->AddText("f_{2}(1270)\n");
    pte->SetTextSize(0.06);
    pte->SetBorderSize(0);
    pte->SetFillStyle(0);

    TPaveText *ptr = new TPaveText(0.57,0.75,0.67,0.85,"NDC"); //(0.5,0.75,0.6,0.85,"NDC")
    ptr->AddText("a_{2}(1320)\n");
    ptr->SetTextSize(0.06);
    ptr->SetBorderSize(0);
    ptr->SetFillStyle(0);

    TPaveText *pts = new TPaveText(0.42,0.75,0.52,0.85,"NDC"); //(0.41,0.75,0.51,0.85,"NDC")
    pts->AddText("#rho(770)\n");
    pts->SetTextSize(0.06);
    pts->SetBorderSize(0);
    pts->SetFillStyle(0);


    
  
  //Three canvases to draw everything on // PLOTTING STARTS HERE ////////////////////////////////////////////
  TCanvas *canva1 = new TCanvas(Form("c_%s",title),Form("c_%s",title),15,10,400,300);

  gPad->SetBottomMargin(0.15);
  gPad->SetLeftMargin(0.14);
  TGaxis::SetMaxDigits(4);

  h_ppip->Draw("*H"); //"P"
  gPad->Update();
  TLine *lrange = new TLine(1.15,0.,1.15,gPad->GetUymax());
  TLine *rrange = new TLine(1.35,0.,1.35,gPad->GetUymax());
  lrange->SetLineColor(kRed); lrange->SetLineWidth(3); lrange->SetLineStyle(kDashed);
  rrange->SetLineColor(kRed); rrange->SetLineWidth(3); rrange->SetLineStyle(kDashed);
  lrange->Draw("SAME");
  rrange->Draw("SAME");
  canva1->Update();
  canva1->SaveAs(pdfname_a.c_str(),"pdf");
  canva1->Close();


  
  TCanvas *canva2 = new TCanvas(Form("c_%s",title),Form("c_%s",title),15,10,400,300);  

  gPad->SetBottomMargin(0.15);
  gPad->SetLeftMargin(0.14);
  TGaxis::SetMaxDigits(4);

  h_3pi->Draw("*H"); // "P"
  if (i==1 || i==0) ptr->Draw("SAME");
  if (i==3 || i==0) ptw->Draw("SAME");
  canva2->Update();
  canva2->SaveAs(pdfname_b.c_str(),"pdf");
  canva2->Close();


  TCanvas *canva3 = new TCanvas(Form("c_%s",title),Form("c_%s",title),15,10,400,300);  
  
  gPad->SetBottomMargin(0.15);
  gPad->SetLeftMargin(0.14);
  TGaxis::SetMaxDigits(4);
  
  h_2pi->Draw("*H"); // "P"
  if (i==1 || i==0) pts->Draw("SAME");
  if (i==2 || i==0) pte->Draw("SAME");
  canva3->Update();
  canva3->SaveAs(+pdfname_c.c_str(),"pdf");
  canva3->Close(); 

}







void draw_tbins()
{

  //Import the files with histograms made out of our data
  const char *datafilename1 = "Histos2026/correlations_data_phaseI_allcuts_nodeltacut.root";
  const char *datafilename2 = "Histos2026/correlations_data_phaseI_allcuts.root";
                              
  
 TFile *datafile1 = TFile::Open(datafilename1,"READ");
 TFile *datafile2 = TFile::Open(datafilename2,"READ");

   
   if (!datafile1 || !datafile2 ) return;



  TH2D *h2d_data_ppip_mass =  (TH2D*) datafile1->Get("ProtonPiplusL_mass_vs_minust");
  TH2D *h2d_data_2pi_mass = (TH2D*) datafile2->Get("PiplusUPiminus_mass_vs_minust");
  TH2D *h2d_data_3pi_mass = (TH2D*) datafile2->Get("PiplusUPiminusPiminus_mass_vs_minust");


  

  Double_t left[6] = {0.,0.,0.25,0.5,0.75,1.};
  Double_t right[6] = {2.,0.25,0.5,0.75,1.,2.};
  char const *trange[6] = {"-t #leq 2 GeV^{2}","-t #leq 0.25 GeV^{2}","0.25 GeV^{2} #leq -t #leq 0.5 GeV^{2}","0.5 GeV^{2} #leq -t #leq 0.75 GeV^{2}","0.75 GeV^{2} #leq -t #leq 1 GeV^{2}","1 GeV^{2} #leq -t #leq 2 GeV^{2}"};

  
  TH1D *hdata_ppip_mass[6],*hdata_2pi_mass[6],*hdata_3pi_mass[6];


  gStyle->SetOptStat(0);
  gStyle->SetTitleFontSize(0.08);
  
     for (int i=0; i<6; i++){

       hdata_ppip_mass[i]  = h2d_data_ppip_mass->ProjectionX(Form("data_ppip_bin%d_px",i),h2d_data_ppip_mass->GetYaxis()->FindBin(left[i]),h2d_data_ppip_mass->GetYaxis()->FindBin(right[i]));
       hdata_2pi_mass[i]  = h2d_data_2pi_mass->ProjectionX(Form("data_2pi_bin%d_px",i),h2d_data_2pi_mass->GetYaxis()->FindBin(left[i]),h2d_data_2pi_mass->GetYaxis()->FindBin(right[i]));
       hdata_3pi_mass[i]  = h2d_data_3pi_mass->ProjectionX(Form("data_3pi_bin%d_px",i),h2d_data_3pi_mass->GetYaxis()->FindBin(left[i]),h2d_data_3pi_mass->GetYaxis()->FindBin(right[i]));

       //       draw_bin(hdata_ppip_mass[i],hdata_3pi_mass[i],hdata_2pi_mass[i],trange[i],i);
       
     }

     
     //  draw_bin_abc(hdata_ppip_mass[0],hdata_3pi_mass[0],hdata_2pi_mass[0],trange[0],0);
     //  draw_bin_abc(hdata_ppip_mass[1],hdata_3pi_mass[1],hdata_2pi_mass[1],trange[1],1);
    draw_bin_abc(hdata_ppip_mass[2],hdata_3pi_mass[2],hdata_2pi_mass[2],trange[2],2);


     
   

   
  
   datafile1->Close();
   datafile2->Close();

}
