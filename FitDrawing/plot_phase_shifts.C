#include <cstdlib>
#include <cstring>
#include <string>
#include <algorithm>

#include <TVector2.h>
#include <TMath.h>

#include "parser.h"
#include "FitResults/fit_36waves_1000iters_fixedBWs.h"



void defstyle_red(TGraph *graph)
{
    graph->SetMarkerSize(1.5); //0.5
    graph->SetMarkerStyle(24);
    graph->SetMarkerColor(kRed+1);
    graph->GetXaxis()->SetTitleOffset(1.);
    graph->GetXaxis()->SetTitleSize(0.06);
    graph->GetXaxis()->SetLabelSize(0.07);
    graph->GetYaxis()->SetTitleOffset(1.);
    graph->GetYaxis()->SetTitleSize(0.06);
    graph->GetYaxis()->SetLabelSize(0.07);
    graph->SetLineColor(kRed+1);
    graph->SetLineWidth(1);
    graph->SetFillStyle(3005);
    graph->SetFillColor(kRed+1);
}


void defstyle_blue(TGraph *graph)
{
    graph->SetMarkerSize(1.5);//0.5
    graph->SetMarkerStyle(24);
    graph->SetMarkerColor(kBlue+2);
    graph->GetXaxis()->SetTitleOffset(1.);
    graph->GetXaxis()->SetTitleSize(0.06);
    graph->GetXaxis()->SetLabelSize(0.07);
    graph->GetYaxis()->SetTitleOffset(1.0);
    graph->GetYaxis()->SetTitleSize(0.06);
    graph->GetYaxis()->SetLabelSize(0.07);
    graph->SetLineColor(kBlue+2);
    graph->SetLineWidth(1);
    graph->SetFillStyle(3004);
    graph->SetFillColor(kBlue+2);
}



void plot_phase_shifts()
{

  std::string wave_pair,wave1,wave2,caption;
  const int nPhaseShifts =  sizeof(phase_shifts) / sizeof(phase_shifts[0]);
  float x_error[NBINS];
  float val_principle[NBINS], error_principle[NBINS];
  float val_principle_revert[NBINS], error_principle_revert[NBINS];

  
    for (int ii = 0; ii < NBINS; ii++)   x_error[ii] = 0.5 * BINWIDTH;

    gStyle->SetOptStat(0);
    gStyle->SetGridStyle(3);
    gStyle->SetGridWidth(1);
    gStyle->SetEndErrorSize(0); //1

    
    // ------------------------------------------------------------
    // 11 Canvases with 6 plots horizontally and 5 plots vertically
    // No delimiters within each canvas
    // ------------------------------------------------------------    
    TCanvas *canva[11];

    for (int jj=0; jj<11; jj++) {
      canva[jj] = new TCanvas(Form("phase_shifts_%d", jj+1),Form("phase_shifts_%d", jj+1),2800,1800);
      canva[jj]->Divide(6, 5, 0.0, 0.0);
    }

    //Loop over all saved wave pairings and draw 30 plots per canvas
    for (int ii = 0; ii < nPhaseShifts; ii++) {
      
      const auto& current_term = phase_shifts[ii];

      
       // --------------------------------------------------------
       // Convert every phase to the [-pi;pi] range and define an inverted phase
       // --------------------------------------------------------

      for (int ibin = 0; ibin < NBINS; ibin++) {

	val_principle[ibin] = std::atan2(std::sin(current_term.value[ibin]),std::cos(current_term.value[ibin]));
	error_principle[ibin] = std::atan2(std::cos(current_term.error[ibin]),std::cos(current_term.error[ibin]));

	
	val_principle_revert[ibin] = -val_principle[ibin];
	error_principle_revert[ibin] = error_principle[ibin];	
      }
      
        TGraph *current_graph = new TGraphErrors(NBINS,current_term.bin_center,val_principle,x_error,error_principle);
        TGraph *current_graph_revert = new TGraphErrors(NBINS,current_term.bin_center,val_principle_revert,x_error,error_principle_revert);
      

	if (startsWith(current_term.name, "PosRefl"))  {defstyle_red(current_graph); defstyle_red(current_graph_revert);}
	if (startsWith(current_term.name, "NegRefl"))  {defstyle_blue(current_graph); defstyle_blue(current_graph_revert);}

        // --------------------------------------------------------
        // Decode the names of two partial waves, erase ^{(#varepsilon)}
        // --------------------------------------------------------
	
	wave_pair = current_term.name.substr(8); 
	int delimiter = wave_pair.find("__");
	wave1 = wave_pair.substr(0, delimiter);
	wave2 = wave_pair.substr(delimiter+2);

	for (const auto& pointer : waveList_norefl) {
	  if (wave1 == pointer.first) {wave1 = pointer.second; wave1.erase(wave1.size()-17,16);}
	  if (wave2 == pointer.first) {wave2 = pointer.second; wave2.erase(wave2.size()-17,16);}
	}

        // --------------------------------------------------------
        // Drawing
        // --------------------------------------------------------

        current_graph->SetTitle("");
        current_graph_revert->SetTitle("");
        current_graph->GetXaxis()->SetLimits(650., 1950.);

        current_graph->SetMinimum(-3.5);//1.1*TMath::Pi());
        current_graph->SetMaximum(3.5);//1.1*TMath::Pi());

	
	canva[ii/30]->cd(ii%30 + 1);

	gPad->SetGridx();
	gPad->SetLeftMargin(0.00);
	gPad->SetTopMargin(0.00);
	gPad->SetRightMargin(0.00);
	gPad->SetBottomMargin(0.00);

	int icolumn = (ii%30) % 6;
	int irow    = (ii%30) / 6;
	
	if (irow < 4){
	    current_graph->GetXaxis()->SetLabelSize(0);
	    current_graph->GetXaxis()->SetTitleSize(0);
	    current_graph->GetXaxis()->SetTickLength(0);
	}
	else gPad->SetBottomMargin(0.25);

	  
	if (icolumn > 0){
	    current_graph->GetYaxis()->SetLabelSize(0);
	    current_graph->GetYaxis()->SetTitleSize(0);
	    current_graph->GetYaxis()->SetTickLength(0);
	  }
	else gPad->SetLeftMargin(0.20);
	  
	current_graph->Draw("AP");
	current_graph_revert->Draw("P SAME");


	TLine *zeroLine = new TLine(650., 0., 1950., 0.);
	zeroLine->SetLineStyle(3);
	zeroLine->Draw("SAME");
	
	
        TPaveText *ptw = new TPaveText(0.63,0.76,0.99,0.99,"NDC");
        ptw->SetTextSize(0.07);
        ptw->SetBorderSize(1);
        ptw->SetLineWidth(1);
        ptw->SetLineColor(kBlack);
        ptw->SetFillColor(kGray);
        ptw->AddText(wave1.c_str());
        ptw->AddText(wave2.c_str());
        ptw->Draw("SAME");
    }


    //Add common axes and print canvas
    for (int jj = 0; jj < 1; jj++) {    
    canva[jj]->cd();

    TLatex xLabel;
    xLabel.SetNDC();
    xLabel.SetTextSize(0.03);
    xLabel.SetTextAlign(22);
    xLabel.DrawLatex(0.5, 0.015, "m(3#pi)  [MeV]");

    TLatex yLabel;
    yLabel.SetNDC();
    yLabel.SetTextSize(0.03);
    yLabel.SetTextAngle(90);
    yLabel.SetTextAlign(22);
    yLabel.DrawLatex(0.012, 0.5, "#Delta#phi  [rad]");
    
    canva[jj]->Print(Form("phase_shifts_%d.pdf",jj+1), "pdf");
    }
}
