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




void setstyle(TH1D *dhisto, TH1D *dhistocut, TH1D *dhistocut2nd)
{
    

    dhisto->Scale(1./dhisto->GetMaximum());
    dhistocut->Scale(1./dhistocut->GetMaximum());
    dhistocut2nd->Scale(1./dhistocut2nd->GetMaximum());


  /*
    dhisto->Scale(1./dhisto->Integral());
    dhistocut->Scale(1./dhistocut->Integral());
    dhistocut2nd->Scale(1./dhistocut2nd->Integral());
  */
    

    Double_t height = 1.1*std::max(dhisto->GetMaximum(),dhistocut->GetMaximum());//1.5
    dhisto->SetMaximum(height);
    dhistocut->SetMaximum(height);

    
  
    dhisto->SetMarkerSize(0.5);
    dhisto->SetMarkerStyle(20);
    dhisto->SetMarkerColor(kRed+2);
    dhisto->SetTitleOffset(0.9,"X");
    dhisto->SetTitleSize(0.06,"X");
    dhisto->SetTitleOffset(1.2,"Y");
    dhisto->SetTitleSize(0.04,"Y"); 
    dhisto->SetLineColor(kRed+2);
    dhisto->SetLineWidth(2);    
    dhisto->SetFillColor(kRed+2);
    dhisto->SetFillStyle(0);

    
    
    dhistocut->SetMarkerSize(0.5);
    dhistocut->SetMarkerStyle(20);
    dhistocut->SetMarkerColor(kGreen+2);
    dhistocut->SetTitleOffset(1.0,"X");
    dhistocut->SetTitleSize(0.05,"X");
    dhistocut->SetTitleOffset(1.2,"Y");
    dhistocut->SetTitleSize(0.04,"Y"); 
    dhistocut->SetLineColor(kGreen+2);
    dhistocut->SetLineWidth(2);    
    dhistocut->SetFillColor(kGreen+2);
    dhistocut->SetFillStyle(0);//3002

    
    dhistocut2nd->SetMarkerSize(0.5);
    dhistocut2nd->SetMarkerStyle(20);
    dhistocut2nd->SetMarkerColor(kBlue-3);//kRed
    dhistocut2nd->SetTitleOffset(1.0,"X");
    dhistocut2nd->SetTitleSize(0.05,"X");
    dhistocut2nd->SetTitleOffset(1.2,"Y");
    dhistocut2nd->SetTitleSize(0.04,"Y"); 
    dhistocut2nd->SetLineColor(kBlue-3);
    dhistocut2nd->SetLineWidth(2);    
    dhistocut2nd->SetFillColor(kBlue-4);
    dhistocut2nd->SetFillStyle(3003);//3001


    
    
}







void print1DimBeamTimesComp()
{


  const char *data1filename = "Histos2026/histos_data_fall2018_allcuts.root";
  const char *data2filename = "Histos2026/histos_data_spring2018_allcuts.root";
  const char *data3filename = "Histos2026/histos_data_spring2017_allcuts.root";



  
  
  Double_t posl,posr;

  
   
   TFile *data1file = TFile::Open(data1filename,"READ");
   TFile *data2file = TFile::Open(data2filename,"READ");
   TFile *data3file = TFile::Open(data3filename,"READ");
   if (!data1file || !data2file || !data3file) return;

   
   std::string variables[26] = {"minustvar","Chi2Ndof","PiplusLProton_M","PiplusUPiminus1_M","PiplusUPiminus2_M","PiplusUPiminus1Piminus2_M","Piminus1Proton_M","Piminus2Proton_M","PiplusLPiplusUPiminus1_M","PiplusLPiplusUPiminus2_M","ProtonPiplusUPiminus1_M","ProtonPiplusUPiminus2_M","ProtonPiplusLPiminus1_M","ProtonPiplusLPiminus2_M","ProtonPiplusUPiminus1Piminus2_M","ProtonPiplusLPiminus1Piminus2_M","ProtonPiplusLPiplusUPiminus1_M","ProtonPiplusLPiplusUPiminus2_M","FourPions_M","DeltaPhi_LV_UV","DeltaTheta_LV_UV","Miss_M2","PiplusL_P","Piminus2_Pz","PiplusLPiminus1_M","PiplusLPiminus2_M"};
   std::string outpdf;

   
   TH1D *var_data = new TH1D();
   TH1D *var_datacut = new TH1D();
   TH1D *var_datacut2nd = new TH1D();

  
   for (int i=0; i<2; i++){
   
     const char *title = variables[i].c_str();      
     outpdf = "CompBeamTimes/pic_"+variables[i]+".pdf";

 
     var_data = (TH1D*)data1file->TFile::Get(title);
     var_datacut = (TH1D*)data2file->TFile::Get(title);
     var_datacut2nd = (TH1D*)data3file->TFile::Get(title);
 

     //write latex-style captions to the axis     
     if (variables[i]=="minustvar")                var_data->GetXaxis()->SetTitle("-t  [GeV^{2}]");
     if (variables[i]=="Chi2Ndof")                 var_data->GetXaxis()->SetTitle("#chi^{2} / N_{d.o.f.}");
     if (variables[i]=="Piminus1Proton_M")         var_data->GetXaxis()->SetTitle("m(p#pi^{-}_{fast})  [GeV]");
     if (variables[i]=="Piminus2Proton_M")         var_data->GetXaxis()->SetTitle("m(p#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="PiplusLPiplusUPiminus1_M") var_data->GetXaxis()->SetTitle("m(#pi^{+}#pi^{+}#pi^{-}_{fast})  [GeV]");
     if (variables[i]=="PiplusLPiplusUPiminus2_M")  var_data->GetXaxis()->SetTitle("m(#pi^{+}#pi^{+}#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="ProtonPiplusUPiminus1_M")  var_data->GetXaxis()->SetTitle("m(p#pi^{+}(uv)#pi^{-}_{fast})  [GeV]");
     if (variables[i]=="ProtonPiplusUPiminus2_M")  var_data->GetXaxis()->SetTitle("m(p#pi^{+}(uv)#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="ProtonPiplusLPiminus1_M")  var_data->GetXaxis()->SetTitle("m(p#pi^{+}(lv)#pi^{-}_{fast})  [GeV]");
     if (variables[i]=="ProtonPiplusLPiminus2_M")   var_data->GetXaxis()->SetTitle("m(p#pi^{+}(lv)#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="ProtonPiplusUPiminus1Piminus2_M"){
                                                    var_data->GetXaxis()->SetTitle("m(p#pi^{+}(uv)#pi^{-}#pi^{-})  [GeV]");
                                                    var_data->GetXaxis()->SetRangeUser(2.5,4.5);
     }
     if (variables[i]=="ProtonPiplusLPiminus1Piminus2_M") var_data->GetXaxis()->SetTitle("m(p#pi^{+}(lv)#pi^{-}#pi^{-})  [GeV]");
     if (variables[i]=="ProtonPiplusLPiplusUPiminus1_M"){
                                                     var_data->GetXaxis()->SetTitle("m(p#pi^{+}#pi^{+}#pi^{-}_{fast})  [GeV]");
                                                     var_data->GetXaxis()->SetRangeUser(1.5,4.5);
     }
     if (variables[i]=="ProtonPiplusLPiplusUPiminus2_M")  var_data->GetXaxis()->SetTitle("m(p#pi^{+}#pi^{+}#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="FourPions_M"){                    var_data->GetXaxis()->SetTitle("m(4#pi)  [GeV]");
                                                          var_data->GetXaxis()->SetRangeUser(1.0,3.5); 
                                      }
     if (variables[i]=="PiplusLPiminus1_M")      var_data->GetXaxis()->SetTitle("m(#pi^{+}(lv)#pi^{-}_{fast})  [GeV]");
     if (variables[i]=="PiplusLPiminus2_M")      var_data->GetXaxis()->SetTitle("m(#pi^{+}(lv)#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="DeltaPhi_LV_UV")         var_data->GetXaxis()->SetTitle("#Delta#phi(lv-uv)  [deg]");
     if (variables[i]=="DeltaTheta_LV_UV")       var_data->GetXaxis()->SetTitle("#Delta#Theta(lv-uv)  [deg]");
     if (variables[i]=="Miss_M2")                var_data->GetXaxis()->SetTitle("m_{miss}^{2}  [GeV^{2}]");
     if (variables[i]=="PiplusL_P")              var_data->GetXaxis()->SetTitle("|p(#pi^{+}(lv))|  [GeV]");
     if (variables[i]=="Piminus2_Pz")            var_data->GetXaxis()->SetTitle("p_{z}(#pi^{-}_{slow})  [GeV]");
     if (variables[i]=="PiplusLProton_M")        var_data->GetXaxis()->SetTitle("m(p#pi^{+}(lv))  [GeV]");
     if (variables[i]=="PiplusUPiminus1_M")      var_data->GetXaxis()->SetTitle("m(#pi^{+}(uv)#pi^{-}_{fast})  [GeV]");
     if (variables[i]=="PiplusUPiminus2_M")      var_data->GetXaxis()->SetTitle("m(#pi^{+}(uv)#pi^{-}_{slow})  [GeV]");
    if (variables[i]=="PiplusUPiminus1Piminus2_M")     var_data->GetXaxis()->SetTitle("m(#pi^{+}(uv)#pi^{-}#pi^{-})  [GeV]");
       
 


     ///Drawing   
     if (strncmp(title,"ProtonPiplusUPiminus1Piminus2_M",31)==0 || strncmp(title,"ProtonPiplusLPiminus1Piminus2_M",31)==0 ||
	 strncmp(title,"ProtonPiplusUPiminus1_M",23)==0 || strncmp(title,"ProtonPiplusLPiplusUPiminus1_M",30)==0){
       posl = 0.1;
       posr = 0.4;
     }
     else {
       posl = 0.7;
       posr = 0.9;
     }	 
     

     


     TLegend *leg = new TLegend(posl,0.7,posr,0.9); //0.74, 0.75
     setstyle(var_data,var_datacut,var_datacut2nd);      
     gStyle->SetOptStat(0);
     leg->AddEntry(var_datacut2nd,"  Spring-2017 ","lpf");
     leg->AddEntry(var_datacut,"  Spring-2018 ","lpf");
     leg->AddEntry(var_data,"  Fall-2018 ","lpf");



     
     TCanvas* canva = new TCanvas(title,title,15,10,400,300);
     //    gPad->SetLogy();
     //    var_data->SetMinimum(100.);
     gPad->SetBottomMargin(0.12);
     var_data->Draw("*H"); 
     var_datacut->Draw("*H F SAME");
     var_datacut2nd->Draw("*H F SAME");
     leg->Draw("SAME");


     
     canva->Update();
     canva->Print(outpdf.c_str(),"pdf");    
     canva->Close();
     
}

    data1file->Close();
    data2file->Close();
    data3file->Close();
    
}
