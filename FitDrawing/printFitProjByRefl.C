#include <cmath>
#include <cstdlib>
#include <cstring>

#include "TDirectory.h"
#include "TDirectoryFile.h"
#include "parser.h"




void setstyle(TH1F *dhisto, TH1F *tothisto, TH1F *bghisto)
{
       
    Double_t height = 1.1*std::max(dhisto->GetMaximum(),tothisto->GetMaximum());//1.2
    dhisto->SetMaximum(height);
    tothisto->SetMaximum(height);    
    
    tothisto->SetMarkerSize(0.5);
    tothisto->SetMarkerStyle(20);
    tothisto->SetMarkerColor(kGreen+3);
    tothisto->SetTitleOffset(0.9,"X");
    tothisto->SetTitleSize(0.06,"X");
    tothisto->SetTitleOffset(0.8,"Y");
    tothisto->SetTitleSize(0.06,"Y"); 
    tothisto->GetXaxis()->SetLabelSize(0.05);
    tothisto->GetYaxis()->SetLabelSize(0.05);

    tothisto->SetLineColor(kGreen+3);
    tothisto->SetLineWidth(2);    
    tothisto->SetFillStyle(3002);
    tothisto->SetFillColor(kGreen+3);

    bghisto->SetMarkerSize(0.5);
    bghisto->SetMarkerStyle(20);
    bghisto->SetMarkerColor(kRed+2);
    bghisto->SetTitleOffset(0.9,"X");
    bghisto->SetTitleSize(0.06,"X");
    bghisto->SetTitleOffset(1.0,"Y");
    bghisto->SetTitleSize(0.05,"Y"); 
    bghisto->SetLineColor(kRed+2);
    bghisto->SetLineWidth(2);    
    bghisto->SetFillStyle(3008);
    bghisto->SetFillColor(kRed+2);
    
    dhisto->SetMarkerSize(0.5);
    dhisto->SetMarkerStyle(20);
    dhisto->SetTitleOffset(0.9,"X");
    dhisto->SetTitleSize(0.06,"X");
    dhisto->SetTitleOffset(1.0,"Y");
    dhisto->SetTitleSize(0.05,"Y"); 
    dhisto->SetLineColor(kBlack);
    dhisto->SetMarkerColor(kBlack);
    dhisto->SetLineWidth(2);    
    
}



void setstyle_refls(TH1F *mc1histo, TH1F *mc2histo, TH1F *mc3histo)
{
         
    mc1histo->SetTitleOffset(0.9,"X");
    mc1histo->SetTitleSize(0.06,"X");
    mc1histo->SetTitleOffset(1.0,"Y");
    mc1histo->SetTitleSize(0.05,"Y"); 

    
    mc1histo->SetMarkerSize(0.5);
    mc1histo->SetMarkerStyle(25);
    mc1histo->SetMarkerColor(kRed);
    mc1histo->SetLineColor(kRed);
    mc1histo->SetLineWidth(2);    
      
    mc2histo->SetMarkerSize(0.5);
    mc2histo->SetMarkerStyle(24);
    mc2histo->SetMarkerColor(kBlue);
    mc2histo->SetLineColor(kBlue);
    mc2histo->SetLineWidth(2);    

    mc3histo->SetLineColor(kBlack);
    mc3histo->SetLineWidth(3);    
        
}


void printLegend(const char *pdfname)
{

  TH1F *data = new TH1F();
  TH1F *fit = new TH1F();
  TH1F *bg = new TH1F();
  TH1F *posrefl = new TH1F();
  TH1F *negrefl = new TH1F();
  TH1F *flat = new TH1F();

  data->SetMarkerSize(0.5);
  data->SetMarkerStyle(20);
  data->SetLineColor(kBlue);
  data->SetMarkerColor(kBlack);
  data->SetLineWidth(2);    
  
  fit->SetMarkerSize(0.5);
  fit->SetMarkerStyle(20);
  fit->SetMarkerColor(kGreen+3);
  fit->SetLineColor(kGreen+3);
  fit->SetLineWidth(2);    
  fit->SetFillStyle(3002);
  fit->SetFillColor(kGreen+3);

  bg->SetMarkerSize(0.5);
  bg->SetMarkerStyle(20);
  bg->SetMarkerColor(kRed+2);
  bg->SetLineColor(kRed+2);
  bg->SetLineWidth(2);    
  bg->SetFillStyle(3008);
  bg->SetFillColor(kRed+2);
    
  posrefl->SetMarkerSize(0.5);
  posrefl->SetMarkerStyle(25);
  posrefl->SetMarkerColor(kRed);
  posrefl->SetLineColor(kRed);
  posrefl->SetLineWidth(2);    

  negrefl->SetMarkerSize(0.5);
  negrefl->SetMarkerStyle(24);
  negrefl->SetMarkerColor(kBlue);
  negrefl->SetLineColor(kBlue);
  negrefl->SetLineWidth(2);    

  flat->SetLineColor(kBlack);
  flat->SetLineWidth(3);    
    
  
  TLegend *leg = new TLegend(0.1,0.1,0.8,0.9);  
  leg->SetTextFont(62);   
  leg->SetTextSize(0.07);

  leg->AddEntry(data,  "  Data", "p");
  leg->AddEntry(fit, "  Total Fit","lf");
  leg->AddEntry(bg, "  Accidental BG ","lf");    
  leg->AddEntry(posrefl,"  PosRefl comp ","lp");
  leg->AddEntry(negrefl,"  NegRefl comp ","lp");
  leg->AddEntry(flat,"  Incoherent BG ","l");

  TCanvas* cnv = new TCanvas("leg","leg",15,10,200,300);
  leg->Draw("");
  cnv->Print(pdfname,"pdf");    
  cnv->Close();
    
  delete cnv;
  delete leg;
}




void printSingleFitProjByRefl(std::string current_bin)
{

  //  std::string filename = "Histos25MeV/Fit_36waves_fixedBWs/isops_histos_tbin0_";
   std::string filename = "HistosEntire/isops_histos_tbin0_";

  filename += current_bin;
  filename += ".root";

  std::string projectionname = "_refls_";
  projectionname += current_bin;
  projectionname += ".pdf";

  
  TFile *file = TFile::Open(filename.c_str(),"READ");
  if (!file) return;
  //   printListOfHistos(file);   
    
  const int NPROJ = 7;


  //Overall histograms with data, bkgnd and fit   
  std::string vars_data[NPROJ] = {"Prod_Ang_data","Theta_GJ_data","Phi_GJ_data","Theta_HF_data","Phi_HF_data","MIso_data","MIsoPs_data"};//,"ProtonPiminus_M_data","ProtonPipluLPiminus_M_data"};
  std::string vars_mc[NPROJ] = {"Prod_Ang_fit","Theta_GJ_fit","Phi_GJ_fit","Theta_HF_fit","Phi_HF_fit","MIso_fit","MIsoPs_fit"};//,"ProtonPiminus_M_fit","ProtonPipluLPiminus_M_fit"};
  std::string vars_bkgnd[NPROJ] = {"Prod_Ang_bkgnd","Theta_GJ_bkgnd","Phi_GJ_bkgnd","Theta_HF_bkgnd","Phi_HF_bkgnd","MIso_bkgnd","MIsoPs_bkgnd"};//,"ProtonPiminus_M_bkgnd","ProtonPipluLPiminus_M_bkgnd"};


  std::string vars_mc_PosRefl[NPROJ] = {"Prod_Ang_fit_PosRefl","Theta_GJ_fit_PosRefl","Phi_GJ_fit_PosRefl","Theta_HF_fit_PosRefl","Phi_HF_fit_PosRefl","MIso_fit_PosRefl","MIsoPs_fit_PosRefl"};//,"ProtonPiminus_M_fit_PosRefl","ProtonPipluLPiminus_M_fit_PosRefl"};
  std::string vars_mc_NegRefl[NPROJ] = {"Prod_Ang_fit_NegRefl","Theta_GJ_fit_NegRefl","Phi_GJ_fit_NegRefl","Theta_HF_fit_NegRefl","Phi_HF_fit_NegRefl","MIso_fit_NegRefl","MIsoPs_fit_NegRefl"};//,"ProtonPiminus_M_fit_NegRefl","ProtonPipluLPiminus_M_fit_NegRefl"};
  std::string vars_mc_waveflat[NPROJ] = {"Prod_Ang_fit_Uniform_Flat","Theta_GJ_fit_Uniform_Flat","Phi_GJ_fit_Uniform_Flat","Theta_HF_fit_Uniform_Flat","Phi_HF_fit_Uniform_Flat","MIso_fit_Uniform_Flat","MIsoPs_fit_Uniform_Flat"};//,"ProtonPiminus_M_fit_Uniform_Flat","ProtonPipluLPiminus_M_fit_Uniform_Flat"};


   //names of canvases and captions
  std::string pdf_names[NPROJ] = {"Prod_Ang_fit","CosTheta_GJ_fit","Phi_GJ_fit","CosTheta_HF_fit","Phi_HF_fit","MIso_fit","MIsoPs_fit"};//,"ProtonPipluLPiminus_M_fit","ProtonPiminus_M_fit"};   
  std::string captions[NPROJ] = {"#Phi  [deg]", "cos#theta^{[GJ]}", "#phi^{[GJ]}  [deg]", "cos#theta^{[HF]}", "#phi^{[HF]}  [deg]", "m(2#pi)  [GeV]", "m(3#pi)  [GeV]"};//, "m(p#pi^{+}_{L}#pi^{-})  [GeV]", "m(p#pi^{-})  [GeV]"};
   TH1F *hdata[NPROJ],  *hbg[NPROJ], *hmc_waveflat[NPROJ], *hmc_PosRefl[NPROJ], *hmc_NegRefl[NPROJ], *htotal[NPROJ];
   




   

   //Read overall histograms with data, accidental bg, total fit and flat wave (summed over all four diamond orient)
   for (int i=0; i<NPROJ; i++){

     hdata[i] = SumBySuffixPattern(file,vars_data[i]);
     hbg[i] = SumBySuffixPattern(file,vars_bkgnd[i]);
     htotal[i] = SumBySuffixPattern(file,vars_mc[i]);
     htotal[i]->Add(hbg[i]); //Sum up scaled MC and acidental BG

     hmc_PosRefl[i] = SumBySuffixPattern(file,vars_mc_PosRefl[i]);
     hmc_NegRefl[i] = SumBySuffixPattern(file,vars_mc_NegRefl[i]);
     hmc_waveflat[i] = SumBySuffixPattern(file,vars_mc_waveflat[i]);


     printf("%-30s\t%-30s\t%d\t%.2f\n", hdata[i]->GetName(), hdata[i]->GetTitle(), (int) hdata[i]->GetEntries(), hdata[i]->GetXaxis()->GetBinWidth(1));
     printf("%-30s\t%-30s\t%d\t%.2f\n", hbg[i]->GetName(), hbg[i]->GetTitle(), (int) hbg[i]->GetEntries(), hbg[i]->GetXaxis()->GetBinWidth(1));
     printf("%-30s\t%-30s\t%d\t%.2f\n", hmc_PosRefl[i]->GetName(), hmc_PosRefl[i]->GetTitle(), (int) hmc_PosRefl[i]->GetEntries(), hmc_PosRefl[i]->GetXaxis()->GetBinWidth(1));
     printf("%-30s\t%-30s\t%d\t%.2f\n", hmc_NegRefl[i]->GetName(), hmc_NegRefl[i]->GetTitle(), (int) hmc_NegRefl[i]->GetEntries(), hmc_NegRefl[i]->GetXaxis()->GetBinWidth(1));
     printf("%-30s\t%-30s\t%d\t%.2f\n", hmc_waveflat[i]->GetName(), hmc_waveflat[i]->GetTitle(), (int) hmc_waveflat[i]->GetEntries(), hmc_waveflat[i]->GetXaxis()->GetBinWidth(1));

	
   }	

   
   //DRAWING   
   std::string outpdf;
   double m2pi_edge = htotal[5]->GetXaxis()->GetBinCenter(htotal[5]->FindLastBinAbove(1));

   if (m2pi_edge < 1.0)  m2pi_edge = 1.2;
   else m2pi_edge = 1.8;
			   
   htotal[5]->GetXaxis()->SetRangeUser(0.,m2pi_edge);  //1.8 will cover all the range in m(3pi)
   htotal[6]->GetXaxis()->SetRangeUser(0.5,2.0);  

   
   htotal[0]->GetYaxis()->SetTitle("Events / 7.2 deg");
   htotal[1]->GetYaxis()->SetTitle("Events / 0.04");
   htotal[2]->GetYaxis()->SetTitle("Events / 7.2 deg");
   htotal[3]->GetYaxis()->SetTitle("Events / 0.04");
   htotal[4]->GetYaxis()->SetTitle("Events / 7.2 deg");
   htotal[5]->GetYaxis()->SetTitle("Events / 15 MeV");   
   htotal[6]->GetYaxis()->SetTitle("Events / 15 MeV");   


   //   htotal[0]->GetXaxis()->SetTitle("#Phi [deg]");   
   //   htotal[1]->GetXaxis()->SetTitle("#it{cos#theta}");   
   //   htotal[2]->GetXaxis()->SetTitle("#it{#phi} [deg]");
   //   htotal[3]->GetXaxis()->SetTitle("#it{cos#theta_{H}}");
   //   htotal[4]->GetXaxis()->SetTitle("#it{#phi_{H}} [deg]");


   
   for (int i=0; i<NPROJ; i++){

     outpdf = "Refls/pic_"+pdf_names[i]+projectionname;

     htotal[i]->SetTitle("");
     //     htotal[i]->GetXaxis()->SetTitle(captions[i].c_str());
     

     gStyle->SetOptStat(0);
     TGaxis::SetMaxDigits(3);
     setstyle(hdata[i],htotal[i],hbg[i]);
     setstyle_refls(hmc_PosRefl[i],hmc_NegRefl[i],hmc_waveflat[i]);      

 

     TCanvas* canva = new TCanvas(vars_data[i].c_str(),vars_data[i].c_str(),15,10,400,300);
     gPad->SetBottomMargin(0.12); gPad->Update();

     htotal[i]->SetMinimum(0.);
     htotal[i]->Draw("HIST F");

     hmc_PosRefl[i]->Draw("P SAME");
     hmc_NegRefl[i]->Draw("P SAME");
     hmc_waveflat[i]->Draw("HIST SAME");     
     hbg[i]->Draw("HIST F SAME");
     hdata[i]->Draw("PE SAME "); 

 
     
     canva->Update();
     canva->Print(outpdf.c_str(),"pdf");    
     canva->Close();

   }


   file->Close();  
   delete file;
   
   
}


void printFitProjByRefl()
{

  /*
  const char *subrange[44] = {"0.750_0.775","0.775_0.800","0.800_0.825","0.825_0.850","0.850_0.875","0.875_0.900","0.900_0.925","0.925_0.950","0.950_0.975","0.975_1.000","1.000_1.025","1.025_1.050","1.050_1.075","1.075_1.100","1.100_1.125","1.125_1.150","1.150_1.175","1.175_1.200","1.200_1.225","1.225_1.250","1.250_1.275","1.275_1.300","1.300_1.325","1.325_1.350","1.350_1.375","1.375_1.400","1.400_1.425","1.425_1.450","1.450_1.475","1.475_1.500","1.500_1.525","1.525_1.550","1.550_1.575","1.575_1.600","1.600_1.625","1.625_1.650","1.650_1.675","1.675_1.700","1.700_1.725","1.725_1.750","1.750_1.775","1.775_1.800","1.800_1.825","1.825_1.850"};

    
   printLegend("Refls/legend_refls.pdf");  

   for (int jj=22; jj<23; jj++) 
        printSingleFitProjByRefl(Form("tbin0_m3pi_%s",subrange[jj]));  
  */

  const char *subrange = "0.750_1.850";
  printSingleFitProjByRefl(Form("m3pi_%s",subrange));  
  
}

