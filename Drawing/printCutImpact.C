#include <cmath>
#include <cstdlib>
#include <cstring>


 const Int_t np = 99;                                                                                                                                                      
 Double_t xx[2*np+3], yy[2*np+3];



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




void setstyle(TH1D *dhisto, TH1D *dhisto2, TGraph *gred)
{
    
    
    dhisto->SetTitleOffset(0.9,"X");
    dhisto->SetTitleSize(0.06,"X");
    dhisto->SetTitleOffset(0.9,"Y");
    dhisto->SetTitleSize(0.05,"Y"); 
    dhisto->GetXaxis()->SetLabelSize(0.04);
    dhisto->GetYaxis()->SetLabelSize(0.04);


    dhisto->SetMarkerSize(0.5);
    dhisto->SetMarkerStyle(20);
    dhisto->SetLineWidth(2);
    dhisto->SetFillStyle(3005);  //1001     
    dhisto->SetMarkerColor(kMagenta+2);
    dhisto->SetLineColor(kMagenta+2);    
    dhisto->SetFillColor(kMagenta-10);
    
    
    dhisto2->SetTitleOffset(0.9,"X");
    dhisto2->SetTitleSize(0.06,"X");
    dhisto2->SetTitleOffset(0.9,"Y");
    dhisto2->SetTitleSize(0.05,"Y"); 
    dhisto2->GetXaxis()->SetLabelSize(0.04);
    dhisto2->GetYaxis()->SetLabelSize(0.04);


    dhisto2->SetMarkerSize(0.5);
    dhisto2->SetMarkerStyle(20);
    dhisto2->SetLineWidth(2);
    dhisto2->SetFillStyle(3004);  
    dhisto2->SetMarkerColor(kRed+2);
    dhisto2->SetLineColor(kRed+2);    
    dhisto2->SetFillColor(kRed-10);    
    
    
}







void printCutImpact()
{


  const char *data1filename = "GenAmpHistos/Diff_tslopes/thrown_3PiDelta_36waves_pi0_a1_a2_pi2__mask000_tslope_99.0__upd_histogrammed.root";
  const char *data2filename = "GenAmpHistos/Diff_tslopes/thrown_3PiDelta_36waves_pi0_a1_a2_pi2__mask000_tslope_99.0_symm__upd_histogrammed.root";

  
  Double_t posl,posr;
  
   
   TFile *data1file = TFile::Open(data1filename,"READ");
   TFile *data2file = TFile::Open(data2filename,"READ");
   if (!data1file || !data2file) return;

   
   std::string variables[28] = {"minustvar","Mass_3pi","Mass_2pi","Mass_ppip","Piminus1Proton_M","Piminus2Proton_M","PiplusLPiplusUPiminus1_M","PiplusLPiplusUPiminus2_M","ProtonPiplusUPiminus1_M","ProtonPiplusUPiminus2_M","ProtonPiplusLPiminus1_M","ProtonPiplusLPiminus2_M","ProtonPiplusUPiminus1Piminus2_M","ProtonPiplusLPiminus1Piminus2_M","ProtonPiplusLPiplusUPiminus1_M","ProtonPiplusLPiplusUPiminus2_M","DeltaPhi_LV_UV","DeltaTheta_LV_UV","Miss_M2","PiplusL_P","Piminus2_Pz","PiplusLPiminus1_M","PiplusLPiminus2_M","PiplusLProton_M","PiplusUPiminus1_M","PiplusUPiminus2_M","PiplusUPiminus1Piminus2_M","FourPions_M"};
   std::string outpdf;

   
   TH1D *var_data = new TH1D();
   TH1D *var_datacut = new TH1D();

  
   for (int i=0; i<4; i++){
   
     const char *title = variables[i].c_str();      
     //     outpdf = "ShowImpact/pic_"+variables[i]+".pdf"; 
     outpdf = "Gen_PWs/pic_"+variables[i]+".pdf"; 

     
 
     var_data = (TH1D*)data1file->TFile::Get(title);
     var_datacut = (TH1D*)data2file->TFile::Get(title);


     //write latex-style captions to the axis     
     if (variables[i]=="minustvar")              var_data->GetXaxis()->SetTitle("-t  [GeV^{2}]");
     if (variables[i]=="Mass_3pi"){              var_data->GetXaxis()->SetTitle("m(3#pi)  [GeV]"); var_data->GetXaxis()->SetRangeUser(0.5,2.5);}
     if (variables[i]=="Mass_2pi"){              var_data->GetXaxis()->SetTitle("m(2#pi)  [GeV]"); var_data->GetXaxis()->SetRangeUser(0.0,1.8);}
     if (variables[i]=="Mass_ppip"){             var_data->GetXaxis()->SetTitle("m(p#pi^{+})  [GeV]"); var_data->GetXaxis()->SetRangeUser(0.8,2.5);}


     if (variables[i]=="Piminus1Proton_M")         var_data->GetXaxis()->SetTitle("m(p#pi^{-}_{fast})  [GeV]");
     if (variables[i]=="Piminus2Proton_M")         var_data->GetXaxis()->SetTitle("m(p#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="PiplusLPiplusUPiminus1_M") var_data->GetXaxis()->SetTitle("m(#pi^{+}#pi^{+}#pi^{-}_{fast})  [GeV]");
     if (variables[i]=="PiplusLPiplusUPiminus2_M")  var_data->GetXaxis()->SetTitle("m(#pi^{+}#pi^{+}#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="ProtonPiplusUPiminus1_M")  var_data->GetXaxis()->SetTitle("m(p#pi^{+}(uv)#pi^{-}_{fast})  [GeV]");
     if (variables[i]=="ProtonPiplusUPiminus2_M")  var_data->GetXaxis()->SetTitle("m(p#pi^{+}(uv)#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="ProtonPiplusLPiminus1_M")  var_data->GetXaxis()->SetTitle("m(p#pi^{+}(lv)#pi^{-}_{fast})  [GeV]");
     if (variables[i]=="ProtonPiplusLPiminus2_M")   var_data->GetXaxis()->SetTitle("m(p#pi^{+}(lv)#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="ProtonPiplusUPiminus1Piminus2_M") var_data->GetXaxis()->SetTitle("m(p#pi^{+}(uv)#pi^{-}#pi^{-})  [GeV]");
     if (variables[i]=="ProtonPiplusLPiminus1Piminus2_M") var_data->GetXaxis()->SetTitle("m(p#pi^{+}(lv)#pi^{-}#pi^{-})  [GeV]");
     if (variables[i]=="ProtonPiplusLPiplusUPiminus1_M")  var_data->GetXaxis()->SetTitle("m(p#pi^{+}#pi^{+}#pi^{-}_{fast})  [GeV]");
     if (variables[i]=="ProtonPiplusLPiplusUPiminus2_M")  var_data->GetXaxis()->SetTitle("m(p#pi^{+}#pi^{+}#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="FourPions_M"){                    var_data->GetXaxis()->SetTitle("m(4#pi)  [GeV]");
                                                          var_data->GetXaxis()->SetRangeUser(1.,3.5); //1.5
                                                          var_datacut->GetXaxis()->SetRangeUser(1.,3.5); //1.5
                                      }
     if (variables[i]=="PiplusLPiminus1_M")      var_data->GetXaxis()->SetTitle("m(#pi^{+}(lv)#pi^{-}_{fast})  [GeV]");
     if (variables[i]=="PiplusLPiminus2_M")      var_data->GetXaxis()->SetTitle("m(#pi^{+}(lv)#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="DeltaPhi_LV_UV")         var_data->GetXaxis()->SetTitle("#Delta#phi(lv-uv)  [deg]");
     if (variables[i]=="DeltaTheta_LV_UV")       var_data->GetXaxis()->SetTitle("#Delta#Theta(lv-uv)  [deg]");
     if (variables[i]=="Miss_M2")                var_data->GetXaxis()->SetTitle("m_{miss}^{2}  [GeV^{2}]");
     if (variables[i]=="PiplusL_P")              var_data->GetXaxis()->SetTitle("|p(#pi^{+}(lv))|  [GeV]");
     if (variables[i]=="Piminus2_Pz")            var_data->GetXaxis()->SetTitle("p_{z}(#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="PiplusLProton_M")        var_data->GetXaxis()->SetTitle("m(p#pi^{+}_{lv})  [GeV]");
     if (variables[i]=="PiplusUPiminus1_M")      var_data->GetXaxis()->SetTitle("m(#pi^{+}_{uv}#pi^{-}_{fast})  [GeV]");
     if (variables[i]=="PiplusUPiminus2_M")      var_data->GetXaxis()->SetTitle("m(#pi^{+}_{uv}#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="PiplusUPiminus1Piminus2_M")     var_data->GetXaxis()->SetTitle("m(#pi^{+}_{uv}#pi^{-}#pi^{-})  [GeV]");
       
 


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
     

     
     TGraph *gfill = new TGraph(2*np+3,xx,yy);


     TLegend *leg = new TLegend(posl,0.75,posr,0.9); //0.74, 0.75
     leg->AddEntry(var_data,"  Symm off","lpf");
     leg->AddEntry(var_datacut,"  Symm on","lpf");


       
     setstyle(var_data,var_datacut,gfill);      
     gStyle->SetOptStat(0);

     
     
     TCanvas* canva = new TCanvas(title,title,15,10,400,300);
     //    gPad->SetLogy();
     //    var_data->SetMinimum(100.);
     gPad->SetBottomMargin(0.12);
     var_data->Draw("P");
     var_data->Draw("H SAME");
     var_datacut->Draw("P SAME");
     var_datacut->Draw("H SAME");
     leg->Draw("SAME");


     if (variables[i]=="Mass_3pi")
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

     
     //      if (variables[i]=="FourPions_M"){
     //      gPad->Update();	 
     //      TLine *cutrange = new TLine(1.8,0.,1.8,gPad->GetUymax());
     //      cutrange->SetLineColor(kRed); cutrange->SetLineWidth(3); cutrange->SetLineStyle(kDashed);
     //      cutrange->Draw("SAME");
     //      }

     //      gfill->Draw("same,lf");   


     
     canva->Update();
     canva->Print(outpdf.c_str(),"pdf");    
     canva->Close();
     
}

    data1file->Close();
    data2file->Close();
    
}
