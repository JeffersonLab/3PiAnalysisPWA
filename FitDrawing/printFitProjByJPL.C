#include <cmath>
#include <cstdlib>
#include <cstring>

#include "TDirectory.h"
#include "TDirectoryFile.h"
#include "parser.h"





void setstyle(TH1F *dhisto, TH1F *tothisto, TH1F *bghisto, TLegend *leg)
{

    leg->SetTextFont(62);   
    leg->SetTextSize(0.07);

    leg->AddEntry(dhisto,  "Data w/o acc BG", "p");
    leg->AddEntry(tothisto, "  Fit","lf");
    ///    leg->AddEntry(bghisto, "  Accidental BG ","lf");    

    
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





void setstyle_sums(TH1F *comp1histo, TH1F *comp2histo, TH1F *comp3histo, TH1F *comp4histo, TH1F *comp5histo, TLegend *leg)
{

    leg->SetTextFont(62);   
    leg->SetTextSize(0.07);
    
    
    leg->AddEntry(comp1histo,comp1histo->GetName(),"l");
    leg->AddEntry(comp2histo,comp2histo->GetName(),"l");
    leg->AddEntry(comp3histo,comp3histo->GetName(),"l");
    leg->AddEntry(comp4histo,comp4histo->GetName(),"l");
    leg->AddEntry(comp5histo," Incoherent BG ","l");


    comp1histo->SetTitleOffset(0.9,"X");
    comp1histo->SetTitleSize(0.06,"X");
    comp1histo->SetTitleOffset(1.0,"Y");
    comp1histo->SetTitleSize(0.05,"Y"); 


    comp1histo->SetMarkerColor(kMagenta);
    comp1histo->SetLineColor(kMagenta);
    comp1histo->SetLineWidth(3);    

    comp2histo->SetMarkerColor(kCyan);
    comp2histo->SetLineColor(kCyan);
    comp2histo->SetLineWidth(3);    
    
    comp3histo->SetMarkerColor(kAzure);
    comp3histo->SetLineColor(kAzure);
    comp3histo->SetLineWidth(3);    

    comp4histo->SetMarkerColor(kRed+1);
    comp4histo->SetLineColor(kRed+1);
    comp4histo->SetLineWidth(3);    

    comp5histo->SetMarkerColor(kBlack);
    comp5histo->SetLineColor(kBlack);
    comp5histo->SetLineWidth(3);    
    
    
}





void printSingleFitProjByJPL(std::string current_bin)
{

  //  std::string filename = "Histos25MeV/Fit_36waves_fixedBWs/isops_histos_tbin0_";
  //  std::string filename = "HistosEntire/isops_histos_floatResMasses_a1_pi2_tbin0_";
  std::string filename = "HistosEntire/isops_histos_tbin0_";
  
  filename += current_bin;
  filename += ".root";

  std::string legendname = "Sums/legend_sums_";
  legendname += current_bin;
  legendname += "_float.pdf";  

  std::string projectionname = "_sums_";
  projectionname += current_bin;
  projectionname += "_float.pdf";
  
  
  TFile *file = TFile::Open(filename.c_str(),"READ");
  if (!file) return;
  //   printListOfHistos(file);   

  const int NPROJ = 7;
  
   //Overall histograms with data, bkgnd and fit 
   std::string vars_data[NPROJ] = {"Prod_Ang_data","CosTheta_GJ_data","Phi_GJ_data","CosTheta_HF_data","Phi_HF_data","MIso_data","MIsoPs_data"};
   std::string vars_fit[NPROJ] = {"Prod_Ang_fit","CosTheta_GJ_fit","Phi_GJ_fit","CosTheta_HF_fit","Phi_HF_fit","MIso_fit","MIsoPs_fit"};
   std::string vars_bkgnd[NPROJ] = {"Prod_Ang_bkgnd","CosTheta_GJ_bkgnd","Phi_GJ_bkgnd","CosTheta_HF_bkgnd","Phi_HF_bkgnd","MIso_bkgnd","MIsoPs_bkgnd"};
   std::string vars_flat[NPROJ] = {"Prod_Ang_fit_Uniform_Flat","CosTheta_GJ_fit_Uniform_Flat","Phi_GJ_fit_Uniform_Flat","CosTheta_HF_fit_Uniform_Flat","Phi_HF_fit_Uniform_Flat","MIso_fit_Uniform_Flat","MIsoPs_fit_Uniform_Flat"};
   TH1F *hdata[NPROJ],  *hbg[NPROJ], *hflat[NPROJ], *htotal[NPROJ];
   
   //names of canvases and captions
   std::string pdf_names[NPROJ] = {"Prod_Ang_fit","CosTheta_GJ_fit","Phi_GJ_fit","CosTheta_HF_fit","Phi_HF_fit","MIso_fit","MIsoPs_fit"};   
   std::string captions[NPROJ] = {"#Phi  [deg]", "cos#theta^{[GJ]}", "#phi^{[GJ]}  [deg]", "cos#theta^{[HF]}", "#phi^{[HF]}  [deg]", "m(2#pi)  [GeV]", "m(3#pi)  [GeV]"};

   //Read overall histograms with data, accidental bg, total fit and flat wave (summed over all four diamond orient)
   for (int i=0; i<NPROJ; i++){

     
     hdata[i] = SumBySuffixPattern(file,vars_data[i]);
     hbg[i] = SumBySuffixPattern(file,vars_bkgnd[i]);
     hflat[i] = SumBySuffixPattern(file,vars_flat[i]);
     htotal[i] = SumBySuffixPattern(file,vars_fit[i]);

     
     //Sum up signal components with acidental bg
     //htotal[i]->Add(hbg[i]); 

     
     //Subtract accidental bg from data
     hdata[i]->Add(hbg[i],-1); 
     //Subtract uniform phase space contribution from both data and total fit
     //     hdata[i]->Add(hflat[i],-1);
     //     htotal[i]->Add(hflat[i],-1);
     

     printf("%-40s\t%-30s\t%d\t%.2f\n", hdata[i]->GetName(), hdata[i]->GetTitle(), (int) hdata[i]->GetEntries(), hdata[i]->GetXaxis()->GetBinWidth(1));
     //     printf("%-40s\t%-30s\t%d\t%.2f\n", hbg[i]->GetName(), hbg[i]->GetTitle(), (int) hbg[i]->GetEntries(), hbg[i]->GetXaxis()->GetBinWidth(1));
     //     printf("%-40s\t%-30s\t%d\t%.2f\n", htotal[i]->GetName(), htotal[i]->GetTitle(), (int) htotal[i]->GetEntries(), htotal[i]->GetXaxis()->GetBinWidth(1));   
   }	



         
   //building blocks for unique sum contributions
   std::string sums[6] = {"pipiIso_0-S","pipiIso_1+P","rhoIso_0-P","rhoIso_1+S","rhoIso_2+D","f2Iso_2-S"};
   std::string refls[2] = {"PosRefl","NegRefl"};


   //concatenate strings as refl_isobar_wave
   std::vector<std::string> components;
   for (const auto& r : refls) 
       for (const auto& w : sums) 
          components.push_back(r + "_" + w);

   const int NSUMS = components.size();
   std::vector<std::vector<TH1F*>> hcomp(NSUMS, std::vector<TH1F*>(NPROJ)); //use a vector, since it is easier to sort vectors
   //   TH1F *hcomp[NSUMS][NPROJ];

      
   //Read histograms for the distinct components (summed over all four diamond orient)
   for (int jj=0; jj<NSUMS; jj++){
     for (int ii=0; ii<NPROJ; ii++){
       hcomp[jj][ii] = SumInnerBySuffixPattern(file, vars_fit[ii] + "_" + components[jj]);
       //              printf("%-40s\t%-30s\t%d\t%.2f\n", hcomp[jj][ii]->GetName(), hcomp[jj][ii]->GetTitle(), (int) hcomp[jj][ii]->GetEntries(), hcomp[jj][ii]->GetXaxis()->GetBinWidth(1));   
     }
   }

   //Sort in the descending by intensity order using 0th projection
     std::sort(hcomp.begin(), hcomp.end(), [&](const auto& a, const auto& b) {
     return a[0]->Integral() > b[0]->Integral();
   } );   


   //Adjust name strings for the first four components and the last projection 
     for (int jj=0; jj<4; jj++){
      std::string current_name = hcomp[jj][NPROJ-1]->GetName();


      for (const auto& pointer : sumList_norefl) {
	if (endsWith(current_name, pointer.first)) {
	  hcomp[jj][NPROJ-1]->SetName(pointer.second.c_str());
	  break;
	}
      }

       
       printf("%-40s\t%-30s\t%d\t%.2f\t%.0f\n", hcomp[jj][NPROJ-1]->GetName(), hcomp[jj][NPROJ-1]->GetTitle(), (int) hcomp[jj][NPROJ-1]->GetEntries(), hcomp[jj][NPROJ-1]->GetXaxis()->GetBinWidth(1), hcomp[jj][NPROJ-1]->Integral());   
       }

   

   //DRAWING
   std::string outpdf;
   TLegend *leg;   
   double m2pi_edge = htotal[5]->GetXaxis()->GetBinCenter(htotal[5]->FindLastBinAbove(1));

   if (m2pi_edge < 1.0)  m2pi_edge = 1.2;
   else m2pi_edge = 1.8;
   
   htotal[5]->GetXaxis()->SetRangeUser(0.,m2pi_edge); //1.8 will cover all the range in m(3pi)
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

     outpdf = "Sums/pic_"+pdf_names[i]+projectionname;

     htotal[i]->SetTitle("");
     htotal[i]->GetXaxis()->SetTitle(captions[i].c_str());


     leg = new TLegend(0.1,0.1,0.8,0.9); 
     gStyle->SetOptStat(0);
     TGaxis::SetMaxDigits(3);
     setstyle(hdata[i],htotal[i],hbg[i],leg);
     setstyle_sums(hcomp[0][i],hcomp[1][i],hcomp[2][i],hcomp[3][i],hflat[i],leg);      



     TCanvas* canva = new TCanvas(vars_data[i].c_str(),vars_data[i].c_str(),15,10,400,300);
     gPad->SetBottomMargin(0.12);
     htotal[i]->SetMinimum(0.);
     htotal[i]->Draw("HIST F");

     hcomp[0][i]->Draw("HIST SAME");
     hcomp[1][i]->Draw("HIST SAME");
     hcomp[2][i]->Draw("HIST SAME");
     hcomp[3][i]->Draw("HIST SAME");
     hflat[i]->Draw("HIST SAME");     
     //     hbg[i]->Draw("HIST F SAME");
     hdata[i]->Draw("PE SAME "); 
 
     
     canva->Update();
     canva->Print(outpdf.c_str(),"pdf");    
     canva->Close();

   }

   
   TCanvas* cnv = new TCanvas("leg","leg",15,10,200,300);
   leg->Draw("");
   cnv->Print(legendname.c_str(),"pdf");    
   cnv->Close();

     
   file->Close();
   delete file;
   
}



void printFitProjByJPL()
{

  /*
  const char *subrange[44] = {"0.750_0.775","0.775_0.800","0.800_0.825","0.825_0.850","0.850_0.875","0.875_0.900","0.900_0.925","0.925_0.950","0.950_0.975","0.975_1.000","1.000_1.025","1.025_1.050","1.050_1.075","1.075_1.100","1.100_1.125","1.125_1.150","1.150_1.175","1.175_1.200","1.200_1.225","1.225_1.250","1.250_1.275","1.275_1.300","1.300_1.325","1.325_1.350","1.350_1.375","1.375_1.400","1.400_1.425","1.425_1.450","1.450_1.475","1.475_1.500","1.500_1.525","1.525_1.550","1.550_1.575","1.575_1.600","1.600_1.625","1.625_1.650","1.650_1.675","1.675_1.700","1.700_1.725","1.725_1.750","1.750_1.775","1.775_1.800","1.800_1.825","1.825_1.850"};
  
    
  for (int jj=0; jj<44; jj++) // 22...23
    printSingleFitProjByJPL(Form("m3pi_%s",subrange[jj]));  
  */

  const char *subrange = "0.750_1.850";
  printSingleFitProjByJPL(Form("m3pi_%s",subrange));  

  
}
