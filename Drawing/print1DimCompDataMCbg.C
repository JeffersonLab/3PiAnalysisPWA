#include <cmath>
#include <cstdlib>
#include <cstring>


void printListOfKeys(TFile *file)
{
 TList:TObjArray *keylist = (TObjArray*) file->TFile::GetListOfKeys();
  keylist->SetOwner(kFALSE);
  keylist->Sort();
   for(int ii = 0; ii < keylist->GetEntries(); ii++)
     cout << "KEY #"<< ii << "\t\t" << keylist->At(ii)->GetTitle() << endl;
}


void loopdir() {
   TFile *f1 = TFile::Open("hsimple.root");
   TIter keyList(f1->GetListOfKeys());
   TKey *key;
   TCanvas c1;
   c1.Print("hsimple.ps[");
   while ((key = (TKey*)keyList())) {
      TClass *cl = gROOT->GetClass(key->GetClassName());
      if (!cl->InheritsFrom("TH1")) continue;
      TH1 *h = (TH1*)key->ReadObj();
      h->Draw();
      c1.Print("hsimple.ps");
   }
   c1.Print("hsimple.ps]");
}




void setstyle(TH1D *dhisto, TH1D *mchisto, TH1D *mcbghisto, TLegend *leg)
{

    leg->AddEntry(dhisto," Data","p");
        leg->AddEntry(mchisto," Thrown phsp","l");
        leg->AddEntry(mcbghisto," Thrown sig","l");
    //    leg->AddEntry(mchisto,"  1M with -t 10.0 1.0 (merged)","l");
    //    leg->AddEntry(mchisto,"  1M with -t 2.0 1.0 (merged)","l");
    //    leg->AddEntry(mcbghisto,"  2M with -t 10.0 0.5 -t 2.0 0.5","l");
    
    
    dhisto->Scale(1./dhisto->GetMaximum());
    mchisto->Scale(1./mchisto->GetMaximum());
    mcbghisto->Scale(1./mcbghisto->GetMaximum());


    
    
    Double_t height = 1.12*std::max(dhisto->GetMaximum(),mchisto->GetMaximum());//1.5
    dhisto->SetMaximum(height);
    mchisto->SetMaximum(height);
    mcbghisto->SetMaximum(height);
    
    
    dhisto->SetMarkerSize(0.6);
    dhisto->SetMarkerStyle(20);
    dhisto->SetTitleOffset(0.9,"X");
    dhisto->SetTitleSize(0.05,"X");
    dhisto->SetTitleOffset(1.2,"Y");
    dhisto->SetTitleSize(0.04,"Y"); 
    dhisto->SetLineColor(kBlack);
    dhisto->SetLineWidth(1);    

    mchisto->SetMarkerSize(0.5);
    mchisto->SetMarkerStyle(20);
    mchisto->SetMarkerColor(kCyan);
    mchisto->SetTitleOffset(0.9,"X");
    mchisto->SetTitleSize(0.05,"X");
    mchisto->SetTitleOffset(1.2,"Y");
    mchisto->SetTitleSize(0.04,"Y"); 
    mchisto->SetLineColor(kCyan);
    mchisto->SetLineWidth(2);    
    mchisto->SetFillStyle(3016);
    mchisto->SetFillColor(kCyan);

    mcbghisto->SetMarkerSize(0.5);
    mcbghisto->SetMarkerStyle(20);
    mcbghisto->SetMarkerColor(kAzure);
    mcbghisto->SetTitleOffset(0.9,"X");
    mcbghisto->SetTitleSize(0.05,"X");
    mcbghisto->SetTitleOffset(1.2,"Y");
    mcbghisto->SetTitleSize(0.04,"Y"); 
    mcbghisto->SetLineColor(kAzure);
    mcbghisto->SetLineWidth(2);    
    mcbghisto->SetFillStyle(3020);
    mcbghisto->SetFillColor(kAzure);

    
}







void print1DimCompDataMCbgInner(std::string mcfilename_arg  = "", std::string mcbgfilename_arg  = "", std::string tslope = "")
{
  
  const char *datafilename = "Histos2026/histos_data_spring2017_allcuts.root";
  const char *mcfilename = mcfilename_arg.c_str();
  const char *mcbgfilename = mcbgfilename_arg.c_str();

  //  const char *mcfilename   = "Histos2026/histos_mc_spring2017_allcuts.root";
  //  const char *mcbgfilename = "Histos2026/correlations_thrown_spring2017.root";

  
  //  const char *mcfilename = "GenAmpHistos/thrown_3PiDelta_36waves_pi0_a1_a2_pi2__tslope_12.0_0.6__6.0_0.2__3.0_0.2_histogrammed.root";
  


  Double_t posl,posr;
  
   
   TFile *datafile = TFile::Open(datafilename,"READ");
   TFile *mcfile = TFile::Open(mcfilename,"READ");
   TFile *mcbgfile = TFile::Open(mcbgfilename,"READ");
   if (!datafile || !mcfile || !mcbgfilename) return;

     std::string variables[21] = {"minustvar","Piminus1Proton_M","Piminus2Proton_M","PiplusLPiplusUPiminus1_M","PiplusLPiplusUPiminus2_M","ProtonPiplusUPiminus1_M","ProtonPiplusUPiminus2_M","ProtonPiplusLPiminus1_M","ProtonPiplusLPiminus2_M","ProtonPiplusUPiminus1Piminus2_M","ProtonPiplusLPiminus1Piminus2_M","ProtonPiplusLPiplusUPiminus1_M","ProtonPiplusLPiplusUPiminus2_M","FourPions_M","DeltaPhi_LV_UV","DeltaTheta_LV_UV","Miss_M2","PiplusL_P","Piminus2_Pz","PiplusLPiminus1_M","PiplusLPiminus2_M"};
   std::string outpdf;

   
   TH1D *var_data = new TH1D();
   TH1D *var_mc = new TH1D();
   TH1D *var_mcbg = new TH1D();



   
   for (int i=0; i<1; i++){
   
     const char *title = variables[i].c_str();
     
     outpdf = "Gen_PWs/pic_"+variables[i]+"_comp_"+tslope+"_tmp.pdf"; //Comp_DataMCbg5

     
     var_data = (TH1D*)datafile->TFile::Get(title);
     var_mc = (TH1D*)mcfile->TFile::Get(title);
     var_mcbg = (TH1D*)mcbgfile->TFile::Get(title);

     var_data->GetXaxis()->SetTitle(title);
     //     var_data->SetTitle("CORRECTED");

     
     ///Drawing   
     if (strncmp(title,"ProtonPiplusUPiminus1Piminus2_M",31)==0 || strncmp(title,"ProtonPiplusLPiminus1Piminus2_M",31)==0 ||
	 strncmp(title,"ProtonPiplusUPiminus1_M",23)==0 || strncmp(title,"ProtonPiplusLPiplusUPiminus1_M",30)==0){
       posl = 0.1;
       posr = 0.3;
     }
     else {
       posl = 0.7; 
       posr = 0.9;
     }	 
     

     //     posl = 0.45;
     posl = 0.68;

     
     TLegend *leg = new TLegend(posl,0.7,posr,0.9); //0.74, 0.75
     setstyle(var_data,var_mc,var_mcbg,leg);      
     leg->SetTextSize(0.04);
     gStyle->SetOptStat(0);

    TPaveText *ptw = new TPaveText(0.5,0.88,0.9,0.98,"NDC");
    ptw->AddText(Form("-mask 000 -tRange 0. 2. %s",tslope.c_str()));
    ptw->SetTextSize(0.04);
    ptw->SetBorderSize(0);
    ptw->SetFillStyle(0);

    
     TCanvas* canva = new TCanvas(title,title,15,10,400,300);
     //    gPad->SetLogy();
     //    var_data->SetMinimum(100.);
     var_data->Draw("P");
     var_mc->Draw("HIST F SAME");
     var_mcbg->Draw("HIST F SAME");
     leg->Draw("SAME");
     ptw->Draw("SAME");

     
     canva->Update();
     canva->Print(outpdf.c_str(),"pdf");    
     canva->Close();
     
}

    datafile->Close();
    mcfile->Close();
    mcbgfile->Close();
    
}




void print1DimCompDataMCbg()
{

  //  print1DimCompDataMCbgInner("GenAmpHistos/Diff_tslopes/thrown_3PiDelta_phsp__mask000_tslope_2.0_1.0_histogrammed.root","GenAmpHistos/Diff_tslopes/thrown_3PiDelta_36waves_pi0_a1_a2_pi2__mask000_tslope_2.0_1.0_histogrammed.root","-t 2.0 1.0");
  //  print1DimCompDataMCbgInner("GenAmpHistos/Diff_tslopes/thrown_3PiDelta_phsp__mask000_tslope_4.0_1.0_histogrammed.root","GenAmpHistos/Diff_tslopes/thrown_3PiDelta_36waves_pi0_a1_a2_pi2__mask000_tslope_4.0_1.0_histogrammed.root","-t 4.0 1.0");
  //  print1DimCompDataMCbgInner("GenAmpHistos/Diff_tslopes/thrown_3PiDelta_phsp__mask000_tslope_6.0_1.0_histogrammed.root","GenAmpHistos/Diff_tslopes/thrown_3PiDelta_36waves_pi0_a1_a2_pi2__mask000_tslope_6.0_1.0_histogrammed.root","-t 6.0 1.0");
  //  print1DimCompDataMCbgInner("GenAmpHistos/Diff_tslopes/thrown_3PiDelta_phsp__mask000_tslope_8.0_1.0_histogrammed.root","GenAmpHistos/Diff_tslopes/thrown_3PiDelta_36waves_pi0_a1_a2_pi2__mask000_tslope_8.0_1.0_histogrammed.root","-t 8.0 1.0");
  //  print1DimCompDataMCbgInner("GenAmpHistos/Diff_tslopes/thrown_3PiDelta_phsp__mask000_tslope_10.0_1.0_histogrammed.root","GenAmpHistos/Diff_tslopes/thrown_3PiDelta_36waves_pi0_a1_a2_pi2__mask000_tslope_10.0_1.0_histogrammed.root","-t 10.0 1.0");
  //  print1DimCompDataMCbgInner("GenAmpHistos/Diff_tslopes/thrown_3PiDelta_phsp__mask000_tslope_12.0_1.0_histogrammed.root","GenAmpHistos/Diff_tslopes/thrown_3PiDelta_36waves_pi0_a1_a2_pi2__mask000_tslope_12.0_1.0_histogrammed.root","-t 12.0 1.0");
  //  print1DimCompDataMCbgInner("GenAmpHistos/Diff_tslopes/thrown_3PiDelta_phsp__mask000_tslope_16.0_1.0_histogrammed.root","GenAmpHistos/Diff_tslopes/thrown_3PiDelta_36waves_pi0_a1_a2_pi2__mask000_tslope_16.0_1.0_histogrammed.root","-t 16.0 1.0");
  
  //  print1DimCompDataMCbgInner("GenAmpHistos/Diff_tslopes/test_tslope2_and_tslope10_histogrammed.root","GenAmpHistos/Diff_tslopes/test_tslopes_2_10__updated_histogrammed.root","combination of slopes");

    print1DimCompDataMCbgInner("GenAmpHistos/Diff_tslopes/thrown_3PiDelta_phsp__mask000_tslope_99.0__updated_histogrammed.root","GenAmpHistos/Diff_tslopes/thrown_3PiDelta_36waves_pi0_a1_a2_pi2__mask000_tslope_99.0__updated_histogrammed.root","combination of slopes");
  
}
