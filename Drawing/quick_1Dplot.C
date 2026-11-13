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




void setstyle(TH1D *dhisto, TLegend *leg)
{

    leg->AddEntry(dhisto,"  Data","p");

    dhisto->SetTitleOffset(0.9,"X");
    dhisto->SetTitleSize(0.06,"X");
    dhisto->SetTitleOffset(0.9,"Y");
    dhisto->SetTitleSize(0.05,"Y"); 
    dhisto->GetXaxis()->SetLabelSize(0.04);
    dhisto->GetYaxis()->SetLabelSize(0.04);

    
    dhisto->SetMarkerSize(0.6);
    dhisto->SetMarkerStyle(20);
    dhisto->SetLineWidth(2);
    dhisto->SetFillStyle(3005);  //1001     
    //    dhisto->SetMarkerColor(kMagenta+2);
    //    dhisto->SetLineColor(kMagenta+2);    
    //    dhisto->SetFillColor(kMagenta-10);  
        dhisto->SetMarkerColor(kRed+2);
        dhisto->SetLineColor(kRed+2);    
        dhisto->SetFillColor(kRed-10);

}







void quick_1Dplot(std::string variable)
{

  const char *datafilename = "GenAmpHistos/Diff_tslopes/thrown_3PiDelta_36waves_pi0_a1_a2_pi2__mask000_tslope_99.0_symm__upd_histogrammed.root"; 

  std::string dirname = "Gen_PWs";    
   
   TFile *datafile = TFile::Open(datafilename,"READ");
   if (!datafile) return;

   
   std::string outpdf = dirname+"/pic_"+variable+"_symm_copy.pdf";  
   const char *title = variable.c_str();      

 
   TH1D *var_data = (TH1D*)datafile->TFile::Get(title);

   
     //write latex-style captions to the axis     
     if (variable=="minustvar")              var_data->GetXaxis()->SetTitle("-t  [GeV^{2}]");
     if (variable=="Mass_3pi"){              var_data->GetXaxis()->SetTitle("m(3#pi)  [GeV]"); var_data->GetXaxis()->SetRangeUser(0.5,2.5);}
     if (variable=="Mass_2pi"){              var_data->GetXaxis()->SetTitle("m(2#pi)  [GeV]"); var_data->GetXaxis()->SetRangeUser(0.0,1.8);}
     if (variable=="Mass_ppip"){             var_data->GetXaxis()->SetTitle("m(p#pi^{+})  [GeV]"); var_data->GetXaxis()->SetRangeUser(0.8,2.5);}

     
     if (variable=="FourPions_M")            var_data->GetXaxis()->SetTitle("m(4#pi)  [GeV]"); 
     if (variable=="PiplusLProton_M")        var_data->GetXaxis()->SetTitle("m(p#pi^{+})  [GeV]");
     if (variable=="PiplusUPiminus1_M")      var_data->GetXaxis()->SetTitle("m(2#pi)  [GeV]");
     if (variable=="PiplusUPiminus2_M")      var_data->GetXaxis()->SetTitle("m(2#pi)  [GeV]");
     if (variable=="PiplusUPiminus1Piminus2_M")  var_data->GetXaxis()->SetTitle("m(3#pi)  [GeV]");
        
 
     //     var_data->GetYaxis()->SetTitle("Events / 25 MeV");
     

     TLegend *leg = new TLegend(0.7,0.75,0.9,0.9); 
     setstyle(var_data,leg);      
     gStyle->SetOptStat(0);
     
     TCanvas* canva = new TCanvas(title,title,15,10,400,300);
     gPad->SetTopMargin(0.08);
     gPad->SetLeftMargin(0.08); //0.12
     gPad->SetBottomMargin(0.12);


     //    gPad->SetLogy();
     //    var_data->SetMinimum(100.);
     var_data->Draw("P"); 
     var_data->Draw("H SAME"); 
     //     leg->Draw("SAME");

     if (variable=="Mass_3pi")
     {
       gPad->Update();
       TLine *vline_a1 = new TLine(1.15,0.,1.15,gPad->GetUymax());
       TLine *vline_pi0 = new TLine(1.30,0.,1.30,gPad->GetUymax());
       TLine *vline_a2 = new TLine(1.32,0.,1.32,gPad->GetUymax());
       TLine *vline_pi2 = new TLine(1.72,0.,1.72,gPad->GetUymax());    
       vline_a1->SetLineColor(kRed); vline_a1->SetLineWidth(2); vline_a1->SetLineStyle(kDotted);
       vline_a2->SetLineColor(kRed); vline_a2->SetLineWidth(2); vline_a2->SetLineStyle(kDotted);
       vline_pi0->SetLineColor(kRed); vline_pi0->SetLineWidth(2); vline_pi0->SetLineStyle(kDotted);
       vline_pi2->SetLineColor(kRed); vline_pi2->SetLineWidth(2); vline_pi2->SetLineStyle(kDotted);   


       vline_pi0->Draw("SAME");
       vline_pi2->Draw("SAME");
       vline_a1->Draw("SAME");
       vline_a2->Draw("SAME");
     }
     
     
     canva->Update();
     canva->Print(outpdf.c_str(),"pdf");    
     canva->Close();
     datafile->Close();
    
}
