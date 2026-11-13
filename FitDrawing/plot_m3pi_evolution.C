#include <cstdlib>
#include <cstring>
#include <string>

#include "parser.h"
#include "FitResults/fit_36waves_200iters_floatIsoBWs.h"
                     



void defstyle_red(TGraph *graph)
{  
    graph->SetMarkerSize(0.5);  //0.1
    graph->SetMarkerStyle(25); //107
    graph->SetMarkerColor(kRed+1);
    graph->GetXaxis()->SetTitleOffset(0.8);
    graph->GetXaxis()->SetTitleSize(0.06);
    graph->GetYaxis()->SetTitleOffset(0.8);
    graph->GetYaxis()->SetTitleSize(0.06);
    graph->SetLineColor(kRed+1);
    graph->SetLineWidth(2);
    graph->SetFillStyle(3005);  //1001 
    graph->SetFillColor(kRed+1);

}


void defstyle_blue(TGraph *graph)
{
    graph->SetMarkerSize(0.5);  //0.2
    graph->SetMarkerStyle(24);
    graph->SetMarkerColor(kBlue+2);
    graph->GetXaxis()->SetTitleOffset(0.8);
    graph->GetXaxis()->SetTitleSize(0.06);
    graph->GetYaxis()->SetTitleOffset(1.0);
    graph->GetYaxis()->SetTitleSize(0.06);
    graph->SetLineColor(kBlue+2);
    graph->SetLineWidth(2);
    graph->SetFillStyle(3004);  //1001 
    graph->SetFillColor(kBlue+2);
    
}



void setstyle(TGraph *graph1, TGraph *graph2)
{

   defstyle_red(graph1);
   defstyle_blue(graph2);

   Double_t height = 1.1*std::max(graph1->GetMaximum(),graph2->GetMaximum());
   graph1->SetMaximum(height);
   graph2->SetMaximum(height);
  
}




   

void plot_m3pi_evolution()
{

  std::string partial_wave;
  float maximum,tmp;
  
  float x_error[NBINS];
  for (int ii=0; ii<NBINS; ii++) x_error[ii] = 0.5*BINWIDTH;

    
  //gStyle->SetOptStat(0);
  // gStyle->SetTextSize(0.05);
 
  //find the value of the highest intensity
  for (const auto& current_term : intensities){
    tmp = *std::max_element(current_term.value, current_term.value+NBINS); 
    maximum = (tmp > maximum) ? tmp : maximum;
  }

  
  for (const auto& current_term : intensities) {
    //    std::cout << current_term.name << std::endl;    
        TGraph *current_graph = new TGraphErrors(NBINS,current_term.bin_center,current_term.value,x_error,current_term.error);
    //    TGraph *current_graph = new TGraph(NBINS,current_term.bin_center,current_term.value);

    current_graph->SetTitle("");
    current_graph->GetXaxis()->SetTitle("m(3#pi)  [MeV]");
    current_graph->GetYaxis()->SetTitle("Yield / 25 [MeV]");

    current_graph->GetXaxis()->SetLimits(600.,2000.);    
    current_graph->SetMinimum(0.0);
    //    current_graph->SetMaximum(1.1*maximum);
    
    
    //Customize color and notation
    if (startsWith(current_term.name,"PosRefl")) defstyle_red(current_graph);
    if (startsWith(current_term.name,"NegRefl")) defstyle_blue(current_graph);

    
    //pick up the term, either partial_wave, or coherent sum
    bool coherent_sum = false;
    bool partial_wave = false;

    for (const auto& pointer : waveList) {
      if (current_term.name == pointer.first) {
	current_graph->SetName(pointer.second.c_str());
	partial_wave = true; break;
      }
    }

 
    for (const auto& pointer : sumList) {
      if (current_term.name == pointer.first) {
	current_graph->SetName(pointer.second.c_str());
	coherent_sum = true; break;
      }
    }


    // if (coherent_sum) continue;
    if (partial_wave) continue;

    
    //Skip everything but 2+D wave
    //     if (current_term.name.find("2-S") == std::string::npos) continue;
        
    //    if (current_term.name.find("1+P")) continue;
    
    if (endsWith(current_term.name,"2+D")) current_graph->SetMaximum(2.1e+6); 
    if (endsWith(current_term.name,"2-S")) current_graph->SetMaximum(0.25e+6);
    if (endsWith(current_term.name,"1+S")) current_graph->SetMaximum(0.9e+6); 
    if (endsWith(current_term.name,"1+P")) current_graph->SetMaximum(0.25e+6); 


  

    TCanvas* canva = new TCanvas("c","c",15,10,400,300);                                                                                                                                                
    gPad->SetTopMargin(0.08);                                                                                                   
    gPad->SetLeftMargin(0.12);
    gPad->SetBottomMargin(0.12);
    //    TGaxis::SetMaxDigits(3);
    
    current_graph->Draw("AP");
    
    //   TLegend *leg = new TLegend(0.7,0.7,0.9,0.9); 
    //   leg->AddEntry(current_graph, current_term.name.c_str(), "lpf");
    //   leg->Draw("SAME");

    TPaveText *ptw = new TPaveText(0.65,0.75,0.88,0.9,"NDC");
    ptw->SetTextSize(0.06);
    ptw->SetBorderSize(2);//1
    ptw->SetLineWidth(2);
    ptw->SetLineColor(kRed);
    ptw->SetFillColor(kGray);
    ptw->AddText(current_graph->GetName());
    ptw->Draw("SAME");  

   
    canva->Print((current_term.name+".pdf").c_str(), "pdf");
    canva->Close();

    
    
 }
      
    
}
