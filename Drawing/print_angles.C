#include <cstdlib>
#include <cstring>



void defstyle(TH1 *dhisto)
{

  
    dhisto->SetMarkerSize(0.5);
    dhisto->SetMarkerStyle(20);
    dhisto->SetTitleOffset(0.8,"X");
    dhisto->SetTitleSize(0.08,"X");
    dhisto->SetTitleOffset(1.2,"Y");
    dhisto->SetTitleSize(0.04,"Y");
    dhisto->SetLineColor(kBlack);
    dhisto->SetLineWidth(2);

    //    dhisto->SetFillStyle(3005);//1001 //3005
    //    dhisto->SetFillColor(kGray);

    
}


void defstyle1(TH1 *dhisto)
{
  
    dhisto->SetMarkerSize(0.5);
    dhisto->SetMarkerStyle(20);
    dhisto->SetMarkerColor(kGray+1);
    dhisto->SetTitleOffset(0.8,"X");
    dhisto->SetTitleSize(0.08,"X");
    dhisto->SetTitleOffset(1.2,"Y");
    dhisto->SetTitleSize(0.04,"Y");
    dhisto->SetLineColor(kGray+1);
    dhisto->SetLineWidth(2);

    dhisto->SetFillStyle(3005);//1001 
    dhisto->SetFillColor(kGray+1);

}



void defstyle2(TH1 *dhisto)
{
  
    dhisto->SetMarkerSize(0.5);
    dhisto->SetMarkerStyle(20);
    dhisto->SetMarkerColor(kGray+2);
    dhisto->SetTitleOffset(0.8,"X");
    dhisto->SetTitleSize(0.08,"X");
    dhisto->SetTitleOffset(1.2,"Y");
    dhisto->SetTitleSize(0.04,"Y");
    dhisto->SetLineColor(kGray+2);
    dhisto->SetLineWidth(2);

    dhisto->SetFillStyle(3004);//1001 
    dhisto->SetFillColor(kGray+2);

    
}



void print_angles2(const char *filename)
{
  
   TFile *file = TFile::Open(filename,"READ");
   if (!file) return;

   TH1F *h = (TH1F*)file->TFile::Get("Prod_Ang_data");

   double factor = 180./TMath::Pi();
   int nbins = h->GetNbinsX();
   double xmin = h->GetXaxis()->GetXmin() * factor;
   double xmax = h->GetXaxis()->GetXmax() * factor;

   TH1F *prodPhi = new TH1F("prodPhi", "prodPhi", nbins, xmin, xmax);

   for (int i = 1; i <= nbins; i++) {
     prodPhi->SetBinContent(i, h->GetBinContent(i));
     prodPhi->SetBinError(i, h->GetBinError(i));
   }
   
   prodPhi->SetTitle("");
   prodPhi->GetXaxis()->SetTitle("#Phi  [deg]");
   prodPhi->GetYaxis()->SetTitle("");
   defstyle(prodPhi);

   
   gStyle->SetOptStat(0);

   TCanvas* canva = new TCanvas("c","c",15,10,400,300);
   gPad->SetTopMargin(0.08);   
   gPad->SetBottomMargin(0.14);   
   prodPhi->SetMinimum(0.);      
   prodPhi->Draw("HF");      
   prodPhi->Draw("PE SAME");      
   canva->Print("prodPhi.pdf", "pdf");
   canva->Close();

   file->Close();

}
   

void print_angles(const char *filename)
{

  
   TFile *file = TFile::Open(filename,"READ");
   if (!file) return;
   
   TH1D *costhetaGJ1 = (TH1D*)file->TFile::Get("CosThetaGJ_Piminus1Bachelor");
   TH1D *costhetaGJ2 = (TH1D*)file->TFile::Get("CosThetaGJ_Piminus2Bachelor");
   TH1D *costhetaHF1 = (TH1D*)file->TFile::Get("CosThetaHF_Piminus1Bachelor");
   TH1D *costhetaHF2 = (TH1D*)file->TFile::Get("CosThetaHF_Piminus2Bachelor");

   TH1D *phiGJ1 = (TH1D*)file->TFile::Get("PhiGJ_Piminus1Bachelor");
   TH1D *phiGJ2 = (TH1D*)file->TFile::Get("PhiGJ_Piminus2Bachelor");
   TH1D *phiHF1 = (TH1D*)file->TFile::Get("PhiHF_Piminus1Bachelor");
   TH1D *phiHF2 = (TH1D*)file->TFile::Get("PhiHF_Piminus2Bachelor");


   TH1D* phiGJ = (TH1D*) phiGJ1->Clone("phiGJ");
   TH1D* phiHF = (TH1D*) phiHF1->Clone("phiHF");
   TH1D* costhetaGJ = (TH1D*) costhetaGJ1->Clone("costhetaGJ");
   TH1D* costhetaHF = (TH1D*) costhetaHF1->Clone("costhetaHF");


   
    costhetaGJ->Add(costhetaGJ2);
    costhetaHF->Add(costhetaHF2);
    phiGJ->Add(phiGJ2);
    phiHF->Add(phiHF2);


   //continue with four angles

   defstyle1(costhetaGJ1);
   defstyle1(costhetaHF1);
   defstyle1(phiGJ1);
   defstyle1(phiHF1);
   defstyle2(costhetaGJ2);
   defstyle2(costhetaHF2);
   defstyle2(phiGJ2);
   defstyle2(phiHF2);
   defstyle(costhetaGJ);
   defstyle(costhetaHF);
   defstyle(phiGJ);
   defstyle(phiHF);
 

   
   costhetaGJ->SetTitle("");
   costhetaGJ->GetXaxis()->SetTitle("cos#theta^{[GJ]}");
   costhetaGJ->GetYaxis()->SetTitle("");

   
   costhetaHF->SetTitle("");
   costhetaHF->GetXaxis()->SetTitle("cos#theta^{[HF]}");
   costhetaHF->GetYaxis()->SetTitle("");


   phiGJ->SetTitle("");
   phiGJ->GetXaxis()->SetTitle("#phi^{[GJ]}  [deg]");
   phiGJ->GetYaxis()->SetTitle("");

   phiHF->SetTitle("");
   phiHF->GetXaxis()->SetTitle("#phi^{[HF]}  [deg]");
   phiHF->GetYaxis()->SetTitle("");

   
    
    /*    
    defstyle(piplusL_theta); piplusL_theta->SetLineColor(kMagenta); piplusL_theta->SetFillStyle(3005); piplusL_theta->SetFillColor(kMagenta);
    piplus1_theta->GetXaxis()->SetRangeUser(0.,150.);

    TLegend *leg1 = new TLegend(0.7,0.6,0.9,0.9); //0.74, 0.75
    leg1->AddEntry(piplusU_mag,"  #pi^{+}_{U},    MC True ","f");    
    leg1->AddEntry(piplus2_mag,"  #pi^{+}_{slow}, MC Reco ","f");
    */

      
      gStyle->SetOptStat(0);
      //      gStyle->SetTextSize(0.05);

      
      TCanvas* canva1 = new TCanvas("c1","c1",15,10,400,300);
      gPad->SetTopMargin(0.08);   
      gPad->SetBottomMargin(0.14);   
      costhetaHF->SetMinimum(0.);      
      costhetaHF->Draw("HIST");      
      costhetaHF->Draw("PE SAME");      
            costhetaHF1->Draw("HF SAME");      
            costhetaHF1->Draw("PE SAME");      
            costhetaHF2->Draw("HF SAME");      
            costhetaHF2->Draw("PE SAME");      
            canva1->Print("costhetaHF.pdf", "pdf");
      canva1->Close();

      TCanvas* canva2 = new TCanvas("c2","c2",15,10,400,300);
      gPad->SetTopMargin(0.08);   
      gPad->SetBottomMargin(0.14);   
      costhetaGJ->SetMinimum(0.);      
      costhetaGJ->Draw("HIST");      
      costhetaGJ->Draw("PE SAME");      
            costhetaGJ1->Draw("HF SAME");      
            costhetaGJ1->Draw("PE SAME");      
            costhetaGJ2->Draw("HF SAME");      
            costhetaGJ2->Draw("PE SAME");      
            canva2->Print("costhetaGJ.pdf", "pdf");
      canva2->Close();

      TCanvas* canva3 = new TCanvas("c3","c3",15,10,400,300);
      gPad->SetTopMargin(0.08);   
      gPad->SetBottomMargin(0.14);   
      phiHF->SetMinimum(0.);      
      phiHF->Draw("HIST");
      phiHF->Draw("PE SAME");      
            phiHF1->Draw("HF SAME");      
            phiHF1->Draw("PE SAME");      
            phiHF2->Draw("HF SAME");      
            phiHF2->Draw("PE SAME");      
            canva3->Print("phiHF.pdf", "pdf");
      canva3->Close();

      TCanvas* canva4 = new TCanvas("c4","c4",15,10,400,300);
      gPad->SetTopMargin(0.08);   
      gPad->SetBottomMargin(0.14);   
      // phiGJ->Scale(1./phiGJ->Integral()); 
      phiGJ->SetMinimum(0.);
      //phiGJ->SetMaximum(0.025);
      phiGJ->Draw("HIST");
      phiGJ->Draw("PE SAME");
      gPad->Update();
            phiGJ1->Draw("HF SAME");      
            phiGJ1->Draw("PE SAME");      
            phiGJ2->Draw("HF SAME");      
            phiGJ2->Draw("PE SAME");      
      canva4->Print("phiGJ.pdf", "pdf");
      canva4->Close();
      

      file->Close();
    
}
