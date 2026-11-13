#include <cstdlib>
#include <cstring>
#include <string>

#include "parser.h"
#include "FitResults/fit_36waves_200iters_floatIsoBWs.h"



double SETMAX = 1.e+6;


void defstyle_red(TGraph *graph)
{  
    graph->SetMarkerSize(0.6);  
    graph->SetMarkerStyle(20); //25
    graph->SetMarkerColor(kRed+1);

    graph->GetXaxis()->SetTitleOffset(0.9);
    graph->GetXaxis()->SetTitleSize(0.06);
    graph->GetXaxis()->SetLabelSize(0.05); 
    graph->GetYaxis()->SetTitleOffset(0.9); //0.8
    graph->GetYaxis()->SetTitleSize(0.06);
    graph->GetYaxis()->SetLabelSize(0.04);
    graph->SetLineColor(kRed+1);
    graph->SetLineWidth(2);
    graph->SetFillStyle(3005);  //1001 
    graph->SetFillColor(kRed+1);

    
}


void defstyle_blue(TGraph *graph)
{
    graph->SetMarkerSize(0.6);  
    graph->SetMarkerStyle(20);  //24
    graph->SetMarkerColor(kBlue+2);
    graph->GetXaxis()->SetTitleOffset(0.9);
    graph->GetXaxis()->SetTitleSize(0.06);
    graph->GetXaxis()->SetLabelSize(0.05); 
    graph->GetYaxis()->SetTitleOffset(0.8);
    graph->GetYaxis()->SetTitleSize(0.06);
    graph->GetYaxis()->SetLabelSize(0.04); 
    graph->SetLineColor(kBlue+2);
    graph->SetLineWidth(2);
    graph->SetFillStyle(3004);  //1001 
    graph->SetFillColor(kBlue+2);
    
}



void setstyle(TGraph *graph1, TGraph *graph2)
{

    graph1->SetTitle("");
    graph1->GetXaxis()->SetTitle("#bf{m(3#pi)  [GeV]}");
    graph1->GetYaxis()->SetTitle("Yield / 25 [MeV]");
    graph1->GetXaxis()->SetLimits(0.650,2.000);    
    graph1->SetMinimum(0.0);

    graph2->SetTitle("");
    graph2->GetXaxis()->SetTitle("m(3#pi)  [GeV]");
    graph2->GetYaxis()->SetTitle("Yield / 25 [MeV]");
    graph2->GetXaxis()->SetLimits(0.650,2.000);    
    graph2->SetMinimum(0.0);

    
   defstyle_red(graph1);
   defstyle_blue(graph2);
   
}




   

void plot_single_term(std::string plot_term = "2+D")
{

  std::string caption;
  float maximum=0.,tmp,lpos = 0.65,rpos = 0.89;
  
  float x_error[NBINS], x_GeV[NBINS], x_error_GeV[NBINS];
  for (int ii=0; ii<NBINS; ii++) x_error[ii] = 0.0*BINWIDTH; //0.5*BINWIDTH;

    
  //gStyle->SetOptStat(0);
  // gStyle->SetTextSize(0.05);
 
  //pick up the term, either amplitude, or coherent sum
  bool coherent_sum = false;
  bool partial_wave = false;


  TGraph *posGraph;
  TGraph *negGraph;  
  
  for (const auto& current_term : intensities) {
    //    std::cout << current_term.name << std::endl;    
                 
    // Skip plots for every m-projection in J^{P}L_{m}^{eps}   
    //     if (endsWith(current_term.name,"++") || endsWith(current_term.name,"+") || endsWith(current_term.name,"0") || endsWith(current_term.name,"-") || endsWith(current_term.name,"--")) continue;

    //Conversion from MeV to GeV
    for (int ibin = 0; ibin < NBINS; ibin++){
      x_GeV[ibin] = 0.001*current_term.bin_center[ibin];
      x_error_GeV[ibin] = 0.001*x_error[ibin];
    }
    
    
    //Skip everything but a given wave
    if (!endsWith(current_term.name,plot_term)) continue;

    //Pick up the graphs of the two reflectivities
    if (current_term.name.find("PosRefl") != std::string::npos){	
      posGraph = new TGraphErrors(NBINS,x_GeV,current_term.value,x_error_GeV,current_term.error);
      //      posGraph = new TGraphErrors(NBINS,current_term.bin_center,current_term.value,x_error,current_term.error);
      posGraph->SetName(current_term.name.c_str());
    }
    if (current_term.name.find("NegRefl") != std::string::npos){
      negGraph = new TGraphErrors(NBINS,x_GeV,current_term.value,x_error_GeV,current_term.error);
      //      negGraph = new TGraphErrors(NBINS,current_term.bin_center,current_term.value,x_error,current_term.error);
      negGraph->SetName(current_term.name.c_str());
    }

    tmp = *std::max_element(current_term.value, current_term.value+NBINS); 
    maximum = (tmp > maximum) ? tmp : maximum;
    
  }

  
  if (posGraph && negGraph) {

        
    cout << "FOUND\n";
    setstyle(posGraph, negGraph);   
    posGraph->SetMaximum(1.1*maximum);
    negGraph->SetMaximum(1.1*maximum);
    posGraph->SetMaximum(SETMAX);
    negGraph->SetMaximum(SETMAX);



    //Determine the right caption
    for (const auto& pointer : waveList_norefl) {
      if (endsWith(posGraph->GetName(),pointer.first)) {
	caption = pointer.second.c_str();
	partial_wave = true; break;
      }
    }
    
    for (const auto& pointer : sumList_norefl) {
      if (endsWith(posGraph->GetName(),pointer.first)) {
	caption = pointer.second.c_str();
	coherent_sum = true; break;
      }
    }

    //Shift the legend position to the left
    if (startsWith(plot_term,"2-S")){
      lpos -= 0.52;
      rpos -= 0.52;
    }
    
    TCanvas* canva = new TCanvas("c","c",15,10,400,300);
    
    gPad->SetTopMargin(0.08);
    gPad->SetLeftMargin(0.12);
    gPad->SetBottomMargin(0.12);

    gStyle->SetEndErrorSize(0);
    //        TGaxis::SetMaxDigits(3);

    posGraph->Draw("AP");
    negGraph->Draw("P SAME");
    
      

    TPaveText *ptw = new TPaveText(lpos,0.75,rpos,0.9,"NDC"); //0.65; 0.88
    ptw->SetTextSize(0.06);
    ptw->SetBorderSize(2);//1
    ptw->SetLineWidth(2);
    ptw->SetLineColor(kRed);
    ptw->SetFillColor(kGray);
    ptw->AddText(caption.c_str());
    ptw->Draw("SAME");  


 
    TLegend* leg = new TLegend(lpos+0.05,0.58,rpos,0.73); 
    leg->SetTextSize(0.05);
    //    leg->AddEntry(posGraph," #it{#varepsilon=+1}","lp");
    //    leg->AddEntry(negGraph," #it{#varepsilon=-1}","lp");
    leg->AddEntry(posGraph," PosRefl","lp");
    leg->AddEntry(negGraph," NegRefl","lp");

    leg->Draw();

    canva->Print((plot_term+".pdf").c_str(), "pdf");
    canva->Close();    
    
 }
          
}




void plot_m3pi_evolution_both()
{

  SETMAX = 2.5e+5; 
  plot_single_term("0-P");  

  

  SETMAX = 2.5e+5; 
  plot_single_term("0-S");  

  SETMAX = 2.1e+6; 
  plot_single_term("2+D");  
  
  plot_single_term("2+D++");
  plot_single_term("2+D+");
  plot_single_term("2+D0");
  plot_single_term("2+D-");
  plot_single_term("2+D--");
  
  SETMAX = 0.9e+6; 
  plot_single_term("1+S");
  
  plot_single_term("1+S+");
  plot_single_term("1+S0");
  plot_single_term("1+S-");
  
  SETMAX = 2.5e+5;
  plot_single_term("2-S");
  
  plot_single_term("2-S++");
  plot_single_term("2-S+");
  plot_single_term("2-S0");
  plot_single_term("2-S-");
  plot_single_term("2-S--");
  
  SETMAX = 3.e+5; 
  plot_single_term("1+P");
  
  plot_single_term("1+P+");
  plot_single_term("1+P0");
  plot_single_term("1+P-");

  
  
}
