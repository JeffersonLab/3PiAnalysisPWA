#include <cstdlib>
#include <cstring>
#include <string>
#include <complex>
#include "pipiSwaveAMPK.h"





void defstyle1(TH1 *dhisto)
{  
    dhisto->SetMarkerSize(0.5);
    dhisto->SetMarkerStyle(20);
    dhisto->SetMarkerColor(kRed+1);
    dhisto->SetTitleOffset(0.8,"X");
    dhisto->SetTitleSize(0.08,"X");
    dhisto->SetTitleOffset(1.2,"Y");
    dhisto->SetTitleSize(0.04,"Y");
    dhisto->SetLineColor(kRed+1);
    dhisto->SetLineWidth(2);
    dhisto->SetFillStyle(3005);//1001 
    dhisto->SetFillColor(kRed+1);

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


void defstyle3(TH1 *dhisto)
{
  
    dhisto->SetMarkerSize(0.5);
    dhisto->SetMarkerStyle(20);
    dhisto->SetMarkerColor(kBlue+2);
    dhisto->SetTitleOffset(0.8,"X");
    dhisto->SetTitleSize(0.08,"X");
    dhisto->SetTitleOffset(1.2,"Y");
    dhisto->SetTitleSize(0.04,"Y");
    dhisto->SetLineColor(kBlue+2);
    dhisto->SetLineWidth(2);
    //    dhisto->SetFillStyle(3005);//1001 //3005
    //    dhisto->SetFillColor(kBlue+2);    
}


void setstyle(TH1 *histo1, TH1 *histo2)
{

   defstyle1(histo1);
   defstyle3(histo2);
   histo1->Scale(1/histo1->Integral());
   histo2->Scale(1/histo2->Integral());

   Double_t height = 1.1*std::max(histo1->GetMaximum(),histo2->GetMaximum());
   histo1->SetMaximum(height);
   histo2->SetMaximum(height);
  
}


void setstyle(TH1 *histo1, TH1 *histo2, TH1 *histo3)
{

   defstyle1(histo1);
   defstyle2(histo2);
   defstyle3(histo3);
   histo1->Scale(1/histo1->Integral());
   histo2->Scale(1/histo2->Integral());
   histo3->Scale(1/histo3->Integral());

   Double_t height = 1.1*std::max(histo1->GetMaximum(),histo3->GetMaximum());
   histo1->SetMaximum(height);
   histo2->SetMaximum(height);
   histo3->SetMaximum(height);

  
}






   

void overlay_fitproj(string partial_wave1, string partial_wave2)
{

  //  const char *filename = "Histos25MeV/isops_histos_tbin0_m3pi_0.825_0.850.root";
  const char *filename = "Histos25MeV/Fit_36waves_fixedBWs/isops_histos_tbin0_m3pi_1.200_1.225.root";
  
   TFile *file = TFile::Open(filename,"READ");
   if (!file) return;


   std::string massIso_name = "Contributions/MIso_gen_";
   std::string costheta_GJ_name = "Contributions/CosTheta_GJ_gen_";
   std::string costheta_HF_name = "Contributions/CosTheta_HF_gen_";
   std::string phi_GJ_name = "Contributions/Phi_GJ_gen_";
   std::string phi_HF_name = "Contributions/Phi_HF_gen_";

   std::string pic_massIso_name = "Check_projections/Overlay/gen_massIso_"; 
   std::string pic_costheta_GJ_name = "Check_projections/Overlay/gen_costhetaGJ_"; 
   std::string pic_costheta_HF_name = "Check_projections/Overlay/gen_costhetaHF_";
   std::string pic_phi_GJ_name = "Check_projections/Overlay/gen_phiGJ_"; 
   std::string pic_phi_HF_name = "Check_projections/Overlay/gen_phiHF_";


   
   std::string massIso_name1 = massIso_name + partial_wave1;
   std::string costheta_GJ_name1 = costheta_GJ_name + partial_wave1;
   std::string costheta_HF_name1 = costheta_HF_name + partial_wave1;
   std::string phi_GJ_name1 = phi_GJ_name + partial_wave1;
   std::string phi_HF_name1 = phi_HF_name + partial_wave1;

   std::string massIso_name2 = massIso_name + partial_wave2;
   std::string costheta_GJ_name2 = costheta_GJ_name + partial_wave2;
   std::string costheta_HF_name2 = costheta_HF_name + partial_wave2;
   std::string phi_GJ_name2 = phi_GJ_name + partial_wave2;
   std::string phi_HF_name2 = phi_HF_name + partial_wave2;

   
   
   pic_massIso_name += (partial_wave1 + "_vs_" + partial_wave2 + ".pdf");
   pic_costheta_GJ_name += (partial_wave1 + "_vs_" + partial_wave2 + ".pdf");
   pic_costheta_HF_name += (partial_wave1 + "_vs_" + partial_wave2 + ".pdf");
   pic_phi_GJ_name += (partial_wave1 + "_vs_" + partial_wave2 + ".pdf");
   pic_phi_HF_name += (partial_wave1 + "_vs_" + partial_wave2 + ".pdf");


   
  
   TH1F *massIso1 = (TH1F*)file->TFile::Get(massIso_name1.c_str());
   TH1F *massIso2 = (TH1F*)file->TFile::Get(massIso_name2.c_str());
   
   
   TH1F *costhetaGJ1 = (TH1F*)file->TFile::Get(costheta_GJ_name1.c_str());
   TH1F *costhetaGJ2 = (TH1F*)file->TFile::Get(costheta_GJ_name2.c_str());

   TH1F *phiGJ1 = (TH1F*)file->TFile::Get(phi_GJ_name1.c_str());
   TH1F *phiGJ2 = (TH1F*)file->TFile::Get(phi_GJ_name2.c_str());
   
   TH1F *costhetaHF1 = (TH1F*)file->TFile::Get(costheta_HF_name1.c_str());
   TH1F *costhetaHF2 = (TH1F*)file->TFile::Get(costheta_HF_name2.c_str());

   TH1F *phiHF1 = (TH1F*)file->TFile::Get(phi_HF_name1.c_str());
   TH1F *phiHF2 = (TH1F*)file->TFile::Get(phi_HF_name2.c_str());
      


   setstyle(massIso1,massIso2);
   setstyle(costhetaGJ1,costhetaGJ2);
   setstyle(costhetaHF1,costhetaHF2);
   setstyle(phiGJ1,phiGJ2);
   setstyle(phiHF1,phiHF2);



   massIso1->SetTitle("");
   massIso1->GetXaxis()->SetRangeUser(0.,1.2);
   massIso1->GetXaxis()->SetTitle("m(2#pi)  [GeV]");
   massIso1->GetYaxis()->SetTitle("");
   
   costhetaGJ1->SetTitle("");
   costhetaGJ1->GetXaxis()->SetTitle("cos#theta^{[GJ]}");
   costhetaGJ1->GetYaxis()->SetTitle("");

   costhetaHF1->SetTitle("");
   costhetaHF1->GetXaxis()->SetTitle("cos#theta^{[HF]}");
   costhetaHF1->GetYaxis()->SetTitle("");

   phiGJ1->SetTitle("");
   phiGJ1->GetXaxis()->SetTitle("#phi^{[GJ]}  [deg]");
   phiGJ1->GetYaxis()->SetTitle("");

   phiHF1->SetTitle("");
   phiHF1->GetXaxis()->SetTitle("#phi^{[HF]}  [deg]");
   phiHF1->GetYaxis()->SetTitle("");

   
    
   TLegend *leg = new TLegend(0.1,0.7,0.35,0.92); //0.42, 0.92
   leg->AddEntry(massIso1, partial_wave1.c_str(), "lpf");
   leg->AddEntry(massIso2, partial_wave2.c_str(), "lpf");    

      
      gStyle->SetOptStat(0);
      // gStyle->SetTextSize(0.05);


      TCanvas* canva0 = new TCanvas("c0","c0",15,10,400,300);
      gPad->SetTopMargin(0.08);   
      gPad->SetBottomMargin(0.14);   
      massIso1->SetMinimum(0.);      
      massIso1->Draw("HIST");      
      massIso1->Draw("PE SAME");      
      massIso1->Draw("HF SAME");      
      massIso1->Draw("PE SAME");      
      massIso2->Draw("HF SAME");      
      massIso2->Draw("PE SAME");      
      leg->Draw("SAME");
      canva0->Print(pic_massIso_name.c_str(), "pdf");
      canva0->Close();

      
      
      TCanvas* canva1 = new TCanvas("c1","c1",15,10,400,300);
      gPad->SetTopMargin(0.08);   
      gPad->SetBottomMargin(0.14);   
      costhetaGJ1->SetMinimum(0.);      
      costhetaGJ1->Draw("HIST");      
      costhetaGJ1->Draw("PE SAME");      
      costhetaGJ1->Draw("HF SAME");      
      costhetaGJ1->Draw("PE SAME");      
      costhetaGJ2->Draw("HF SAME");      
      costhetaGJ2->Draw("PE SAME");      
      canva1->Print(pic_costheta_GJ_name.c_str(), "pdf");
      canva1->Close();


      TCanvas* canva2 = new TCanvas("c2","c2",15,10,400,300);
      gPad->SetTopMargin(0.08);   
      gPad->SetBottomMargin(0.14);   
      costhetaHF1->SetMinimum(0.);      
      costhetaHF1->Draw("HIST");      
      costhetaHF1->Draw("PE SAME");      
      costhetaHF1->Draw("HF SAME");      
      costhetaHF1->Draw("PE SAME");      
      costhetaHF2->Draw("HF SAME");      
      costhetaHF2->Draw("PE SAME");      
      canva2->Print(pic_costheta_HF_name.c_str(), "pdf");
      canva2->Close();

            
      TCanvas* canva3 = new TCanvas("c3","c3",15,10,400,300);
      gPad->SetTopMargin(0.08);   
      gPad->SetBottomMargin(0.14);   
      phiGJ1->SetMinimum(0.);      
      phiGJ1->Draw("HIST");      
      phiGJ1->Draw("PE SAME");      
      phiGJ1->Draw("HF SAME");      
      phiGJ1->Draw("PE SAME");      
      phiGJ2->Draw("HF SAME");      
      phiGJ2->Draw("PE SAME");      
      canva3->Print(pic_phi_GJ_name.c_str(), "pdf");
      canva3->Close();


      TCanvas* canva4 = new TCanvas("c4","c4",15,10,400,300);
      gPad->SetTopMargin(0.08);   
      gPad->SetBottomMargin(0.14);   
      phiHF1->SetMinimum(0.);      
      phiHF1->Draw("HIST");      
      phiHF1->Draw("PE SAME");      
      phiHF1->Draw("HF SAME");      
      phiHF1->Draw("PE SAME");      
      phiHF2->Draw("HF SAME");      
      phiHF2->Draw("PE SAME");      
      canva4->Print(pic_phi_HF_name.c_str(), "pdf");
      canva4->Close();

      

      file->Close();
    
}
