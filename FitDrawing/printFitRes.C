#include <cmath>
#include <cstdlib>
#include <cstring>


void printListOfKeys(TFile *file)
{
 TList:TObjArray *keylist = (TObjArray*) file->TFile::GetListOfKeys();
   keylist->SetOwner(kFALSE);
   keylist->Sort();
   for(int ii = 0; ii < keylist->GetEntries(); ii++)
     cout << "KEY #"<< ii << "\t\t" << keylist->At(ii)->GetName() << endl;
 }


void printListOfHistos(TFile *file)
{
 TList *keylist = file->TFile::GetListOfKeys();
 TIter keyList(keylist);
 TKey *key;
 keylist->SetOwner(kFALSE);
 keylist->Sort();

 for(int ii = 0; ii < keylist->GetEntries(); ii++){
     key = (TKey*)keyList();
     if (key->GetClassName() != TString("TH1F")) continue;

     TH1F *histo = (TH1F*)key->ReadObj();
     cout << histo->GetName() << endl;
  }
 }


bool endsWith(const std::string& str, const std::string& suffix) {
    return str.size() >= suffix.size() && str.substr(str.size() - suffix.size()) == suffix;
}




TH1F* SumBySuffixPattern(TFile *file, std::string suffix) {

 TList *keylist = file->TFile::GetListOfKeys();
 TIter keyList(keylist);
 TKey *key;
 keylist->SetOwner(kFALSE);
 keylist->Sort();

 std::string histname;
 std::string sumhistname = "SummedBy_" + suffix;
   

 TH1F *histoSum = nullptr;
 
 for(int ii = 0; ii < keylist->GetEntries(); ii++){
     key = (TKey*)keyList();
     if (key->GetClassName() != TString("TH1F")) continue;

     TH1F *histo = (TH1F*)key->ReadObj();
     histname =  histo->GetName();    

     if (!endsWith(histname,suffix)) continue; 
     //          cout << histo->GetName() << endl;
     
     if (!histoSum) histoSum = (TH1F*)histo->Clone(sumhistname.c_str());
     else  histoSum->Add(histo);      

 }
 
 return histoSum;
}



void setstyle(TH1F *dhisto, TH1F *mchisto, TH1F *bghisto, TLegend *leg)
{

    leg->SetTextFont(62);   
    leg->SetTextSize(0.07);

    leg->AddEntry(dhisto,  "  Data", "p");
    leg->AddEntry(mchisto, "  Total Fit","lf");
    leg->AddEntry(bghisto, "  Accidental BG ","lf");

    
    
    Double_t height = 1.1*std::max(dhisto->GetMaximum(),mchisto->GetMaximum());//1.5
    dhisto->SetMaximum(height);
    mchisto->SetMaximum(height);
    
    
    mchisto->SetMarkerSize(0.5);
    mchisto->SetMarkerStyle(20);
    mchisto->SetMarkerColor(kGreen+3);
    mchisto->SetTitleOffset(0.9,"X");
    mchisto->SetTitleSize(0.06,"X");
    mchisto->SetTitleOffset(1.0,"Y");
    mchisto->SetTitleSize(0.05,"Y"); 
    mchisto->SetLineColor(kGreen+3);
    mchisto->SetLineWidth(2);    
    mchisto->SetFillStyle(3002);
    mchisto->SetFillColor(kGreen+3);


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
    dhisto->SetLineColor(kBlue);
    dhisto->SetMarkerColor(kBlack);
    dhisto->SetLineWidth(2);    


    
}







void printFitRes()
{

  const int NUM = 7;

  TFile *file = TFile::Open("isops_histos_m3pi_1.20_1.30.root","READ");
  if (!file) return;

   
   std::string vars_data[7] = {"Prod_Ang_data","CosTheta_GJ_data","Phi_GJ_data","CosTheta_HF_data","Phi_HF_data","MIso_data","MIsoPs_data"};
   std::string vars_mc[7] = {"Prod_Ang_acc","CosTheta_GJ_acc","Phi_GJ_acc","CosTheta_HF_acc","Phi_HF_acc","MIso_acc","MIsoPs_acc"};
   std::string vars_bkgnd[7] = {"Prod_Ang_bkgnd","CosTheta_GJ_bkgnd","Phi_GJ_bkgnd","CosTheta_HF_bkgnd","Phi_HF_bkgnd","MIso_bkgnd", "MIsoPs_bkgnd"};

   std::string pdfnames[7] = {"Prod_Ang_fit","CosTheta_GJ_fit","Phi_GJ_fit","CosTheta_HF_fit","Phi_HF_fit","MIso_fit","MIsoPs_fit"};      
   std::string captions[7] = {"#Phi  [deg]", "cos#theta^{[GJ]}", "#phi^{[GJ]}  [deg]", "cos#theta^{[HF]}", "#phi^{[HF]}  [deg]", "m(2#pi)  [GeV]", "m(3#pi)  [GeV]"};
   TH1F *hdata[NUM], *hmc[NUM], *hbg[NUM];


   //   loopdir(file);
   //   printListOfHistos(file);   
   std::string outpdf;
   double posl,posr;

   
   //Sum over histograms with subsamples for different diamond settings
   for (int i=0; i<NUM; i++){
        hdata[i] = SumBySuffixPattern(file,vars_data[i]);
        hmc[i] = SumBySuffixPattern(file,vars_mc[i]);
        hbg[i] = SumBySuffixPattern(file,vars_bkgnd[i]);
		printf("%-30s\t%-30s\t%d\t%.2f\n", hdata[i]->GetName(), hdata[i]->GetTitle(), (int) hdata[i]->GetEntries(), hdata[i]->GetXaxis()->GetBinWidth(1));
		printf("%-30s\t%-30s\t%d\t%.2f\n", hmc[i]->GetName(), hmc[i]->GetTitle(), (int) hmc[i]->GetEntries(), hmc[i]->GetXaxis()->GetBinWidth(1));
		printf("%-30s\t%-30s\t%d\t%.2f\n", hbg[i]->GetName(), hbg[i]->GetTitle(), (int) hbg[i]->GetEntries(), hbg[i]->GetXaxis()->GetBinWidth(1));			
   }


   hmc[5]->GetXaxis()->SetRangeUser(0.,1.5);
   hbg[5]->GetXaxis()->SetRangeUser(0.,1.5);


   //Sum up scaled MC and acidental BG
   for (int i=0; i<NUM; i++) hmc[i]->Add(hbg[i]);


   TLegend *leg;
   
   for (int i=0; i<NUM; i++){

     outpdf = "Amps/pic_"+pdfnames[i]+".pdf";

     hmc[i]->SetTitle("");
     hmc[i]->GetXaxis()->SetTitle(captions[i].c_str());

     

     leg = new TLegend(0.1,0.1,0.9,0.5); 
     setstyle(hdata[i],hmc[i],hbg[i],leg);      
     gStyle->SetOptStat(0);


     TCanvas* canva = new TCanvas(vars_data[i].c_str(),vars_data[i].c_str(),15,10,400,300);
     gPad->SetBottomMargin(0.12);
     hmc[i]->SetMinimum(0.);
     hmc[i]->Draw("HIST F");
     hbg[i]->Draw("HIST F SAME");
     hdata[i]->Draw("PE SAME "); 

     
     canva->Update();
     canva->Print(outpdf.c_str(),"pdf");    
     canva->Close();

   }

   TCanvas* cnv = new TCanvas("leg","leg",15,10,200,300);
   leg->Draw("");
   //   cnv->Print("Amps/legend.pdf","pdf");    
   cnv->Close();

   
    file->Close();
    
}
