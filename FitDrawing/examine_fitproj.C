#include <cstdlib>
#include <cstring>
#include <string>
#include <complex>
#include "pipiSwaveAMPK.h"
#include "parser.h"






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
   defstyle2(histo2);
   histo1->Scale(1/histo1->Integral());
   histo2->Scale(1/histo2->Integral());

   Double_t height = 1.25*std::max(histo1->GetMaximum(),histo2->GetMaximum());
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






   

void examine_fitproj(std::string partial_wave)
{

  const char *filename = "Histos25MeV/isops_histos_tbin0_m3pi_1.725_1.750_nosymm.root";
  
   TFile *file = TFile::Open(filename,"READ");
   if (!file) return;


   std::string massIso_name = "Contributions/MIso_gen_";
   std::string costheta_GJ_name = "Contributions/CosTheta_GJ_gen_";
   std::string costheta_HF_name = "Contributions/CosTheta_HF_gen_";
   std::string phi_GJ_name = "Contributions/Phi_GJ_gen_";
   std::string phi_HF_name = "Contributions/Phi_HF_gen_";

   std::string pic_massIso_name = "Check_projections/m2pi/gen_massIso_"; 
   std::string pic_costheta_GJ_name = "Check_projections/GJ/gen_costhetaGJ_"; 
   std::string pic_costheta_HF_name = "Check_projections/HF/gen_costhetaHF_";
   std::string pic_phi_GJ_name = "Check_projections/GJ/gen_phiGJ_"; 
   std::string pic_phi_HF_name = "Check_projections/HF/gen_phiHF_";


   
   massIso_name += partial_wave;
   costheta_GJ_name += partial_wave;
   costheta_HF_name += partial_wave;
   phi_GJ_name += partial_wave;
   phi_HF_name += partial_wave;

   
   pic_massIso_name += (partial_wave + "_nosymm.pdf");
   pic_costheta_GJ_name += (partial_wave + "_nosymm.pdf");
   pic_costheta_HF_name += (partial_wave + "_nosymm.pdf");
   pic_phi_GJ_name += (partial_wave + "_nosymm.pdf");
   pic_phi_HF_name += (partial_wave + "_nosymm.pdf");


   
  
   TH1F *massIso1 = (TH1F*)file->TFile::Get(massIso_name.c_str());
   TH1F *massIso2 = (TH1F*)file->TFile::Get("MIso_gen_Uniform_Flat");
   TH1F *massIso3 = (TH1F*) massIso2->Clone();


   // reweight m(2pi) with the given m(2pi) lineshape
   for (int ii = 1; ii <= massIso3->GetNbinsX(); ++ii) {
     double x = massIso3->GetBinCenter(ii);
     double val = massIso3->GetBinContent(ii);

     complex <double> weight_amp = amp_pipiSwave(x);  
     double weight = std::norm(weight_amp);     

     massIso3->SetBinContent(ii, val*weight);
   }

   
   
   TH1F *costhetaGJ1 = (TH1F*)file->TFile::Get(costheta_GJ_name.c_str());
   TH1F *costhetaGJ2 = (TH1F*)file->TFile::Get("CosTheta_GJ_gen_Uniform_Flat");

   TH1F *phiGJ1 = (TH1F*)file->TFile::Get(phi_GJ_name.c_str());
   TH1F *phiGJ2 = (TH1F*)file->TFile::Get("Phi_GJ_gen_Uniform_Flat");
   
   TH1F *costhetaHF1 = (TH1F*)file->TFile::Get(costheta_HF_name.c_str());
   TH1F *costhetaHF2 = (TH1F*)file->TFile::Get("CosTheta_HF_gen_Uniform_Flat");

   TH1F *phiHF1 = (TH1F*)file->TFile::Get(phi_HF_name.c_str());
   TH1F *phiHF2 = (TH1F*)file->TFile::Get("Phi_HF_gen_Uniform_Flat");
      


   setstyle(massIso1,massIso2,massIso3);
   setstyle(costhetaGJ1,costhetaGJ2);
   setstyle(costhetaHF1,costhetaHF2);
   setstyle(phiGJ1,phiGJ2);
   setstyle(phiHF1,phiHF2);



   massIso1->SetTitle("");
   massIso1->GetXaxis()->SetRangeUser(0.,2.);
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


   
   TLegend *leg = new TLegend(0.1,0.72,0.35,0.92); //0.42, 0.92
   TLegend *legend = new TLegend(0.75,0.78,0.9,0.92); //0.42, 0.92
   legend->SetTextSize(0.06);
   //   leg->SetTextSize(0.06);   
   std::string label, type;

   
   if (endsWith(partial_wave, "0-S")) label = " #it{0^{-}S}";
   if (endsWith(partial_wave, "0-P")) label = " #it{0^{-}P}";  
   
   if (endsWith(partial_wave, "1+P-")) label = " #it{1^{+}P_{-1}}";
   if (endsWith(partial_wave, "1+P0")) label = " #it{1^{+}P_{0}}";
   if (endsWith(partial_wave, "1+P+")) label = " #it{1^{+}P_{1}}";

   if (endsWith(partial_wave, "1+S-")) label = " #it{1^{+}S_{-1}}";
   if (endsWith(partial_wave, "1+S0")) label = " #it{1^{+}S_{0}}";
   if (endsWith(partial_wave, "1+S+")) label = " #it{1^{+}S_{1}}";

   if (endsWith(partial_wave, "2+D--")) label = " #it{2^{+}D_{-2}}";
   if (endsWith(partial_wave, "2+D-")) label = " #it{2^{+}D_{-1}}";
   if (endsWith(partial_wave, "2+D0")) label = " #it{2^{+}D_{0}}";
   if (endsWith(partial_wave, "2+D+")) label = " #it{2^{+}D_{1}}";
   if (endsWith(partial_wave, "2+D++")) label = " #it{2^{+}D_{2}}";
   
   if (endsWith(partial_wave, "2-S--")) label = " #it{2^{-}S_{-2}}";
   if (endsWith(partial_wave, "2-S-")) label = " #it{2^{-}S_{-1}}";
   if (endsWith(partial_wave, "2-S0")) label = " #it{2^{-}S_{0}}";
   if (endsWith(partial_wave, "2-S+")) label = " #it{2^{-}S_{1}}";
   if (endsWith(partial_wave, "2-S++")) label = " #it{2^{-}S_{2}}";
   
   if (startsWith(partial_wave, "pipiIso")) type = " [#pi#pi]_{S} AMPK";
   if (startsWith(partial_wave, "rhoIso")) type = " #rho(770) BW";
   if (startsWith(partial_wave, "f2Iso")) type = " f_{2}(1270) BW";


   
   leg->AddEntry(massIso1, type.c_str(), "lpf");
   legend->AddEntry(costhetaGJ1, label.c_str(), "lpf");    

   
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
      if (startsWith(partial_wave,"pipiIso")){
	 massIso3->Draw("HF SAME");      
	 massIso3->Draw("PE SAME");      
	 leg->AddEntry(massIso3, " AMPK weighted", "lpf");    
      }
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
      legend->Draw("SAME");
      //      canva1->Print(pic_costheta_GJ_name.c_str(), "pdf");
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
      legend->Draw("SAME");
      //      canva2->Print(pic_costheta_HF_name.c_str(), "pdf");
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
      legend->Draw("SAME");
      //      canva3->Print(pic_phi_GJ_name.c_str(), "pdf");
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
      legend->Draw("SAME");
      //      canva4->Print(pic_phi_HF_name.c_str(), "pdf");
      canva4->Close();

      

      file->Close();
    
}
