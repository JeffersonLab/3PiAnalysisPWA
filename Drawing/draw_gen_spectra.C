#include <cstdlib>
#include <cstring>
#include <string>
#include <complex>




void defstyle1(TH1 *dhisto)
{  
  dhisto->SetMarkerSize(0.7);//0.5
    dhisto->SetMarkerStyle(20);
    dhisto->SetMarkerColor(kRed+1);
    dhisto->SetTitleOffset(0.8,"X");
    dhisto->SetTitleSize(0.08,"X");
    dhisto->SetTitleOffset(1.0,"Y");
    dhisto->SetTitleSize(0.06,"Y");
    dhisto->GetXaxis()->SetLabelSize(0.04);
    dhisto->GetYaxis()->SetLabelSize(0.04);
    dhisto->SetLineColor(kRed+1);
    dhisto->SetLineWidth(2);
    dhisto->SetFillStyle(3005);//1001 
    dhisto->SetFillColor(kRed+1);    
}


void defstyle2(TH1 *dhisto)
{
    dhisto->SetMarkerSize(0.7);//0.5
    dhisto->SetMarkerStyle(20);
    dhisto->SetMarkerColor(kGray+2);
    dhisto->SetTitleOffset(0.8,"X");
    dhisto->SetTitleSize(0.08,"X");
    dhisto->SetTitleOffset(1.0,"Y");
    dhisto->SetTitleSize(0.06,"Y");
    dhisto->GetXaxis()->SetLabelSize(0.04);
    dhisto->GetYaxis()->SetLabelSize(0.04);
    dhisto->SetLineColor(kGray+2);
    dhisto->SetLineWidth(2);
    dhisto->SetFillStyle(3004);//1001 
    dhisto->SetFillColor(kGray+2);
}


void defstyle3(TH1 *dhisto)
{
  
  dhisto->SetMarkerSize(0.7); //0.5
    dhisto->SetMarkerStyle(20);
    dhisto->SetMarkerColor(kBlue+2);
    dhisto->SetTitleOffset(0.8,"X");
    dhisto->SetTitleSize(0.08,"X");
    dhisto->SetTitleOffset(1.0,"Y");
    dhisto->SetTitleSize(0.06,"Y");
    dhisto->GetXaxis()->SetLabelSize(0.04);
    dhisto->GetYaxis()->SetLabelSize(0.04);
    dhisto->SetLineColor(kBlue+2);
    dhisto->SetLineWidth(2);
    dhisto->SetFillStyle(3005);  //1001 //3005
    dhisto->SetFillColor(kBlue+2);    
}


void setstyle(TH1 *histo1, TH1 *histo2)
{

   defstyle3(histo1);
   defstyle2(histo2);
   histo1->Scale(1/histo1->Integral());
   histo2->Scale(1/histo2->Integral());

   Double_t height = 1.25*std::max(histo1->GetMaximum(),histo2->GetMaximum());
   histo1->SetMaximum(height);
   histo2->SetMaximum(height);

   histo1->SetMinimum(0.);      
   histo2->SetMinimum(0.);      

}


void setstyle(TH1 *histo1, TH1 *histo2, TH1 *histo3)
{

   defstyle1(histo1);
   defstyle2(histo2); 
   defstyle3(histo3);
   histo1->Scale(1/histo1->Integral());
   histo2->Scale(1/histo2->Integral());
   histo3->Scale(1/histo3->Integral());

   Double_t height = 1.1*std::max({histo1->GetMaximum(),histo2->GetMaximum(),histo3->GetMaximum()});
   histo1->SetMaximum(height);
   histo2->SetMaximum(height);
   histo3->SetMaximum(height);  

   histo1->SetMinimum(0.);      
   histo2->SetMinimum(0.);      
   histo3->SetMinimum(0.);      

}



   

void load_spectra(TFile *file, TH1D **histos)
{
  double scale_fact;  
  //  static TH1D* histos[9]; //either use static, or define the histograms as global, or pass the array into the function

   histos[0] = (TH1D*)file->TFile::Get("Mass_2pi");
   histos[1] = (TH1D*)file->TFile::Get("Mass_3pi");
   histos[2] = (TH1D*)file->TFile::Get("Mass_ppip");
   histos[3] = (TH1D*)file->TFile::Get("minustvar");
   histos[4] = (TH1D*)file->TFile::Get("CosThetaGJ");
   histos[5] = (TH1D*)file->TFile::Get("CosThetaHF");   
   histos[6] = (TH1D*)file->TFile::Get("PhiGJ");
   histos[7] = (TH1D*)file->TFile::Get("PhiHF");
   histos[8] = (TH1D*)file->TFile::Get("BigPhi");

   
   histos[0]->SetTitle("");
   histos[0]->GetYaxis()->SetTitle("");
   histos[0]->GetXaxis()->SetTitle("m(2#pi)  [GeV]");

   histos[1]->SetTitle("");
   histos[1]->GetYaxis()->SetTitle("");
   histos[1]->GetXaxis()->SetTitle("m(3#pi)  [GeV]");
   
   histos[2]->SetTitle("");
   histos[2]->GetYaxis()->SetTitle("");
   histos[2]->GetXaxis()->SetTitle("m(p#pi^{+})  [GeV]");

   histos[3]->SetTitle("");
   histos[3]->GetYaxis()->SetTitle("");
   histos[3]->Rebin(4);
   histos[3]->GetXaxis()->SetTitle("-t  [GeV^{2}]");
  
   histos[4]->SetTitle("");
   histos[4]->GetYaxis()->SetTitle("");
   histos[4]->GetXaxis()->SetTitle("cos#theta^{[GJ]}");

   histos[5]->SetTitle("");
   histos[5]->GetYaxis()->SetTitle("");
   histos[5]->GetXaxis()->SetTitle("cos#theta^{[HF]}");

   histos[6]->SetTitle("");
   histos[6]->GetYaxis()->SetTitle("");
   histos[6]->GetXaxis()->SetTitle("#phi^{[GJ]}  [deg]");

   histos[7]->SetTitle("");
   histos[7]->GetYaxis()->SetTitle("");
   histos[7]->GetXaxis()->SetTitle("#phi^{[HF]}  [deg]");

   histos[8]->SetTitle("Polarized beam (P#gamma = 0.35)");
   histos[8]->GetYaxis()->SetTitle("");
   histos[8]->GetXaxis()->SetTitle("#Phi  [deg]");

   //   for (int jj=0; jj<9; jj++){
   //	histos[jj]->Scale(1/histos[jj]->Integral());
   //	histos[jj]->SetMinimum(0.);      
   //	scale_fact = (jj<4) ? 1.1 : 1.25; //higher offset on top for the angles
   ///	histos[jj]->SetMaximum(scale_fact*histos[jj]->GetMaximum());      
   //   }
}



void draw_single_wave( const char *filename_phsp, const char *filename, const char *leg_entry, std::string pdf_entry)
{

  TFile *file = TFile::Open(filename,"READ");
  TFile *file_phsp = TFile::Open(filename_phsp,"READ");
  if (!file || !file_phsp) return;
  
  TH1D *hist_phsp[9];
  TH1D *hist_single_wave[9];

  load_spectra(file,hist_single_wave);
  load_spectra(file_phsp,hist_phsp);


  std::string pdfnames[9] = {"_massIso.pdf","_massX.pdf","_massB.pdf","_minust.pdf","_costhetaGJ.pdf","_costhetaHF.pdf","_phiGJ.pdf","_phiHF.pdf","_bigPhi.pdf"};
  TCanvas *canva[9];
  
  gStyle->SetOptStat(0);
  // gStyle->SetTextSize(0.05);      


  for (int jj=4; jj<8; jj++){
    pdfnames[jj] = "GenAmpHistos/gen_"+pdf_entry+pdfnames[jj];
	
    setstyle(hist_single_wave[jj],hist_phsp[jj]);
    TLegend *legend = new TLegend(0.75,0.78,0.9,0.92); 
    //	TLegend *legend = new TLegend(0.65,0.14,0.9,0.34); //0.42, 0.92
    legend->SetTextSize(0.05); 
    legend->AddEntry(hist_single_wave[jj], leg_entry, "lpf");    	
	
    canva[jj] = new TCanvas("c1","c1",15,10,400,300);     
    gPad->SetTopMargin(0.08);   
    gPad->SetBottomMargin(0.14);   
    hist_phsp[jj]->Draw("HIST");      
    hist_phsp[jj]->Draw("P SAME");      
    hist_single_wave[jj]->Draw("HIST SAME");      
    hist_single_wave[jj]->Draw("P SAME");      
    legend->Draw("SAME");
    canva[jj]->Print(pdfnames[jj].c_str(), "pdf");
    canva[jj]->Close();
  }    

  file->Close();
  file_phsp->Close();

  
}




void draw_gen_spectra()
{

  /*  
  TFile *file0 = TFile::Open("GenAmpHistos/test_rhoBW_deltaBW_histogrammed.root","READ");
  TFile *file1 = TFile::Open("GenAmpHistos/test_rhoBW_deltaBW_a2BW_histogrammed.root","READ");
  TFile *file2 = TFile::Open("GenAmpHistos/test_pipiIso_rhoIso_f2Iso_histogrammed.root","READ");  
  */

  const char *phase_space = "GenAmpHistos/test_phsp_histogrammed.root";
  /*
  draw_single_wave(phase_space,"GenAmpHistos/test_pipiIso_0-S_histogrammed.root"," #it{0^{-}S}","pipiIso_0-S");
  draw_single_wave(phase_space,"GenAmpHistos/test_rhoIso_0-P_histogrammed.root"," #it{0^{-}P}","rhoIso_0-P");
  
  draw_single_wave(phase_space,"GenAmpHistos/test_pipiIso_1+P-_histogrammed.root"," #it{1^{+}P_{-1}}","pipiIso_1+P-");
  draw_single_wave(phase_space,"GenAmpHistos/test_pipiIso_1+P0_histogrammed.root"," #it{1^{+}P_{0}}","pipiIso_1+P0");
  draw_single_wave(phase_space,"GenAmpHistos/test_pipiIso_1+P+_histogrammed.root"," #it{1^{+}P_{1}}","pipiIso_1+P+");
  
  draw_single_wave(phase_space,"GenAmpHistos/test_rhoIso_1+S-_histogrammed.root"," #it{1^{+}S_{-1}}","rhoIso_1+S-");
  draw_single_wave(phase_space,"GenAmpHistos/test_rhoIso_1+S0_histogrammed.root"," #it{1^{+}S_{0}}","rhoIso_1+S0");
  draw_single_wave(phase_space,"GenAmpHistos/test_rhoIso_1+S+_histogrammed.root"," #it{1^{+}S_{1}}","rhoIso_1+S+");
  
  draw_single_wave(phase_space,"GenAmpHistos/test_rhoIso_2+D--_histogrammed.root"," #it{2^{+}D_{-2}}","rhoIso_2+D--");
  draw_single_wave(phase_space,"GenAmpHistos/test_rhoIso_2+D-_histogrammed.root"," #it{2^{+}D_{-1}}","rhoIso_2+D-");
  draw_single_wave(phase_space,"GenAmpHistos/test_rhoIso_2+D0_histogrammed.root"," #it{2^{+}D_{0}}","rhoIso_2+D0");
  draw_single_wave(phase_space,"GenAmpHistos/test_rhoIso_2+D+_histogrammed.root"," #it{2^{+}D_{1}}","rhoIso_2+D+");
  draw_single_wave(phase_space,"GenAmpHistos/test_rhoIso_2+D++_histogrammed.root"," #it{2^{+}D_{2}}","rhoIso_2+D++");
  */
  draw_single_wave(phase_space,"GenAmpHistos/test_f2Iso_2-S--_histogrammed.root"," #it{2^{-}S_{-2}}","f2Iso_2-S--");
  draw_single_wave(phase_space,"GenAmpHistos/test_f2Iso_2-S-_histogrammed.root"," #it{2^{-}S_{-1}}","f2Iso_2-S-");
  draw_single_wave(phase_space,"GenAmpHistos/test_f2Iso_2-S0_histogrammed.root"," #it{2^{-}S_{0}}","f2Iso_2-S0");
  draw_single_wave(phase_space,"GenAmpHistos/test_f2Iso_2-S+_histogrammed.root"," #it{2^{-}S_{1}}","f2Iso_2-S+");
  draw_single_wave(phase_space,"GenAmpHistos/test_f2Iso_2-S++_histogrammed.root"," #it{2^{-}S_{2}}","f2Iso_2-S++");



  
}
