#include <algorithm>
#include <cmath>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "parser.h"
#include "FitResults/fit_36waves_1000iters_fixedBWs.h"


// ROOT macro: one N x N matrix of intensities and phase shifts.
//
// Input wave names are suffixes used in the intensities/phase_shifts
// arrays, without the leading "PosRefl_" or "NegRefl_" prefix. For example:
//   plot_wave_matrix({"pipiIso_1+P-","pipiIso_1+P0","rhoIso_1+S0","rhoIso_2+D0"});

const double INTESITY_SCALE = 1.e-6;

// Canvas-level space reserved for captions (fractions of canvas dimensions)
const double CAPTION_LEFT   = 0.05;
const double CAPTION_TOP    = 0.04;
const double CAPTION_RIGHT  = 0.02;
const double CAPTION_BOTTOM = 0.04;

// Additional spacing between the plot frame and wave caption
const double CAPTION_X_OFFSET = 0.015;
const double CAPTION_Y_OFFSET = -0.01;

// Margins inside each plot
const double PAD_LEFT   = 0.09;
const double PAD_RIGHT  = 0.01;
const double PAD_TOP    = 0.01;
const double PAD_BOTTOM = 0.09;





//First auxiliary functions defining the style
void setStyleGraph(TGraphErrors* graph, Color_t color, Style_t markerStyle)
{
    graph->SetMarkerColor(color);
    graph->SetLineColor(color);
    graph->SetMarkerStyle(markerStyle);
    graph->SetMarkerSize(0.8);
    graph->SetLineWidth(1);
}


void configureCellAxes(TGraphErrors* graph, int row, int column, int n, double yMin,  double yMax)
{
    graph->SetTitle("");
    graph->GetXaxis()->SetLimits(0.650, 1.950);
    graph->SetMinimum(yMin);
    graph->SetMaximum(yMax);

    graph->GetXaxis()->SetLabelSize(row == n-1 ? 0.09 : 0.0);
    graph->GetXaxis()->SetTickLength(row == n-1 ? 0.05 : 0.0);
    graph->GetXaxis()->SetTitleSize(row == n-1 ? 0.08 : 0.0);

    graph->GetYaxis()->SetLabelSize(column == 0 ? 0.09 : 0.0);
    graph->GetYaxis()->SetTickLength(column == 0 ? 0.05 : 0.0);
    graph->GetYaxis()->SetTitleSize(column == 0 ? 0.08 : 0.0);

}


// Return a display label as defined in waveList_norefl
std::string displayWaveLabel(const std::string& wave)
{
    for (const auto& item : waveList_norefl) {
        if (wave == item.first) {
            std::string label = item.second;
            if (label.size() >= 17) label.erase(label.size() - 17, 16);
            return label;
        }
    }
    return wave;
}


void setWaveCaptions(const std::vector<std::string> &wave_list)
{
    //This routine calls drawing, therefore canvas must be open before calling this function
    TLatex xLabel;
    xLabel.SetNDC();
    xLabel.SetTextSize(0.03);
    xLabel.SetTextAlign(22);
    xLabel.DrawLatex(0.5, 0.015, "m(3#pi) [GeV]");

    TLatex yLabel;
    yLabel.SetNDC();
    yLabel.SetTextSize(0.03); 
    yLabel.SetTextAngle(90);
    yLabel.SetTextAlign(22);
    yLabel.DrawLatex(0.015, 0.5, "10^{-6} Yield / 25 [MeV]  or  #bf{#Delta#phi} [rad] / 25 [MeV]");


    const int n = wave_list.size();
    std::string label;

    TLatex latex;
    latex.SetNDC();
    latex.SetTextFont(42);
    latex.SetTextSize(0.02);
    latex.SetTextAlign(22);

    const double plotWidth  = 1.0 - CAPTION_LEFT - CAPTION_RIGHT;
    const double plotHeight = 1.0 - CAPTION_TOP - CAPTION_BOTTOM;

    for (int ii = 0; ii < n; ++ii) {
       label = displayWaveLabel(wave_list[ii]);

       // Column captions
        const double x = CAPTION_LEFT + (ii + 0.5)*plotWidth/n;
        latex.SetTextAngle(0);
        latex.DrawLatex(x, 1.0 - CAPTION_TOP/2. + CAPTION_Y_OFFSET, label.c_str());

        // Row captions
        const double y = 1.0 - CAPTION_TOP - (ii + 0.5)*plotHeight/n;
        latex.SetTextAngle(90);
        latex.DrawLatex(CAPTION_LEFT/2.0 + CAPTION_X_OFFSET, y, label.c_str());
    }
}




// Parse names of the form "PosRefl_waveA__waveB" or "NegRefl_waveA__waveB".
bool parsePhaseName(const std::string &fullName, std::string &reflectivity, std::string &waveA,  std::string &waveB)
{
    std::size_t prefixLength = 0;
    if (startsWith(fullName, "PosRefl_")) {
        reflectivity = "PosRefl";
        prefixLength = 8;
    } else if (startsWith(fullName, "NegRefl_")) {
        reflectivity = "NegRefl";
        prefixLength = 8;
    } else {
        return false;
    }

    const std::string pair = fullName.substr(prefixLength);
    const std::size_t delimiter = pair.find("__");
    if (delimiter == std::string::npos) return false;

    waveA = pair.substr(0, delimiter);
    waveB = pair.substr(delimiter + 2);
    return !waveA.empty() && !waveB.empty();
}

// Find a phase-shift entry for the requested reflectivity and wave pair (the order doesn't matter).
const JPLMeps* findPhaseShift(const std::string& reflectivity, const std::string& rowWave, const std::string& columnWave)
{
    const JPLMeps* directMatch = nullptr;
    const JPLMeps* reversedMatch = nullptr;

    for (const auto& item : phase_shifts) {
        std::string storedReflectivity, waveA, waveB;
        if (!parsePhaseName(item.name, storedReflectivity, waveA, waveB)) continue;
        if (storedReflectivity != reflectivity) continue;

        if (waveA == rowWave && waveB == columnWave) {directMatch = &item; break;}
        if (waveA == columnWave && waveB == rowWave) {reversedMatch = &item; break;}
    }

    return (directMatch) ? directMatch : reversedMatch;
}

  
TGraphErrors* makeGraph(const JPLMeps &entry, bool isPhase, double phaseSign = 1.0)
{
   double x[NBINS], y[NBINS], xError[NBINS] = {}, yError[NBINS];
    
    for (unsigned int ii = 0; ii < NBINS; ++ii) {

        x[ii] = 0.001*entry.bin_center[ii];  // MeV -> GeV
        xError[ii] = 0.0*0.001* BINWIDTH; //0.5*BINWIDTH
	yError[ii] = std::fabs(entry.error[ii]);
	y[ii] = entry.value[ii];
	
	if (isPhase) //in the case of the phase shift plot, recalculate from rad to deg
	  y[ii] = std::atan2(std::sin(phaseSign*y[ii]), std::cos(phaseSign*y[ii])); 
	else{ // in the case of intensity, myltiply by 10^(-6) factor
	  y[ii] *= INTESITY_SCALE; 
	  yError[ii] *= INTESITY_SCALE;
	}
    }

    return new TGraphErrors(NBINS, x, y, xError, yError);
}


float findMaxIntensity(const std::vector<std::string> &wave_list)
{
   float maximum = 0.0f;

    for (const auto& wave : wave_list) {
      for (const auto& entry : intensities){ 
	if (endsWith(entry.name,wave)){
	  for (unsigned int ii = 0; ii < NBINS; ++ii) 
	    maximum = std::max(maximum, entry.value[ii]);
	}
      }
    }
    return maximum;
}


void drawIntensityCell(const std::string &wave, int row, int column, int n, double cell_max)
{
   const JPLMeps *pos = nullptr;
   const JPLMeps *neg = nullptr; 

    for (const auto& item : intensities) {
        if (item.name == "PosRefl_" + wave) pos = &item;
        if (item.name == "NegRefl_" + wave) neg = &item;
    }
    
    if (!pos || !neg) {
        std::cerr << "Missing intensity entry for wave '" << wave << "'.\n";
        exit(1);
    }

    TGraphErrors* posGraph = makeGraph(*pos, false);
    TGraphErrors* negGraph = makeGraph(*neg, false);
    setStyleGraph(posGraph, kRed+1, 21);  //20
    setStyleGraph(negGraph, kBlue+2, 21); //20


    //Drawing goes further, therefore canvas must be open before calling this function
    configureCellAxes(posGraph, row, column, n, 0.0, 1.12*INTESITY_SCALE*cell_max);

    posGraph->Draw("AP");
    negGraph->Draw("P SAME");

    TLegend* legend = new TLegend(0.78, 0.8, 0.99, 0.99);
    legend->SetBorderSize(1);
    legend->SetFillColor(kWhite);
    legend->SetFillStyle(1001);
    legend->SetTextSize(0.06);
    legend->AddEntry(posGraph, "PosRefl", "lpf");
    legend->AddEntry(negGraph, "NegRefl", "lpf");
    legend->Draw();
}


void drawPhaseCell(const std::string &rowWave, const std::string &columnWave, const std::string &reflectivity, int row, int column, int n)
{
    const JPLMeps* item = findPhaseShift(reflectivity, rowWave, columnWave);

    if (!item) {
        std::cerr << "No " << reflectivity << " phase shift found for pair '"  << rowWave << "' / '" << columnWave << "'.\n";
        return;
    }

    TGraphErrors* graph = makeGraph(*item, true, 1.);
    TGraphErrors* graphRevert = makeGraph(*item, true, -1.);

    const Color_t color = (reflectivity == "PosRefl") ? kRed+1 : kBlue+2;
    setStyleGraph(graph, color, 24);
    setStyleGraph(graphRevert, color, 24);

    configureCellAxes(graph, row, column, n, -3.5, 3.5);
    graph->Draw("AP");
    
    TLine* zeroLine = new TLine(0.650, 0.0, 1.950, 0.0);
    zeroLine->SetLineStyle(3);
    zeroLine->SetLineWidth(1);
    zeroLine->SetLineColor(kGray+2);
    zeroLine->Draw("SAME");
    graph->Draw("P SAME");  
    graphRevert->Draw("P SAME");
    
}


void plot_wave_matrix(const std::vector<std::string> &wave_list, const char *outputFile = "wave_matrix.pdf")
{

  //arrange a list of unique waves, just in case user is not attentive
  std::vector<std::string> unique_wave_list;
  for (const auto& wave : wave_list){ 
    if (std::find(unique_wave_list.begin(),unique_wave_list.end(),wave) == unique_wave_list.end()) 
      unique_wave_list.push_back(wave);
  }
  if (unique_wave_list.empty()) exit(1);


  
  
  float maxIntensity = findMaxIntensity(unique_wave_list);
    
    gStyle->SetOptStat(0);
    gStyle->SetGridStyle(3);
    gStyle->SetGridWidth(1);
    gStyle->SetEndErrorSize(0);
    TGaxis::SetMaxDigits(3);

    
    const int n = static_cast<int>(unique_wave_list.size());
    int canvasWidth  = std::max(1200, 400*n+20*n);
    int canvasHeight = (3./4)*canvasWidth;

    TCanvas* canvas = new TCanvas("canva", "Wave intensity and phase-shift matrix",canvasWidth, canvasHeight);
    //    canvas->Divide(n, n, 0.0, 0.0);
    // Reserve space for captions, then divide the remaining area into equal pads.
    canvas->cd();

    TPad* matrixPad = new TPad("matrixPad", "",CAPTION_LEFT, CAPTION_BOTTOM, 1.0-CAPTION_RIGHT, 1.0-CAPTION_TOP);
    matrixPad->Draw();
    matrixPad->Divide(n, n, 0.0, 0.0);
    
    for (int row = 0; row < n; ++row) {
      for (int column = 0; column < n; ++column) {
        matrixPad->cd(row*n + column + 1);

        gPad->SetLeftMargin(PAD_LEFT);
        gPad->SetRightMargin(PAD_RIGHT);
        gPad->SetTopMargin(PAD_TOP);
        gPad->SetBottomMargin(PAD_BOTTOM);
        gPad->SetGridx();

        if (row == column) 
	  drawIntensityCell(unique_wave_list[row], row, column, n, maxIntensity);
	if (row < column)	  
	  drawPhaseCell(unique_wave_list[row],unique_wave_list[column],"NegRefl", row, column, n);
	if (row > column)	  
	  drawPhaseCell(unique_wave_list[row],unique_wave_list[column],"PosRefl", row, column, n);
      }
    }
    
    canvas->cd();
    setWaveCaptions(unique_wave_list);
    canvas->Print(outputFile, "pdf");
    canvas->Close();
    std::cout << "Wrote " << outputFile << "\n";
}








void plot_matrix()
{


  //    plot_wave_matrix({"rhoIso_1+S-","rhoIso_1+S0","rhoIso_1+S+","rhoIso_2+D-", "rhoIso_2+D0", "rhoIso_2+D+"},"matrix_6x6__1+S_2+D.pdf");
  //    plot_wave_matrix({"rhoIso_1+S-","rhoIso_1+S0","rhoIso_1+S+","f2Iso_2-S-", "f2Iso_2-S0","f2Iso_2-S+"},"matrix_6x6__1+S_2-S.pdf");
  //    plot_wave_matrix({"rhoIso_2+D-", "rhoIso_2+D0", "rhoIso_2+D+","f2Iso_2-S-", "f2Iso_2-S0","f2Iso_2-S+"},"matrix_6x6__2+D_2-S.pdf");
  //   plot_wave_matrix({"rhoIso_1+S+","rhoIso_2+D+","f2Iso_2-S+"},"matrix_3x3__m=1.pdf");
  //  plot_wave_matrix({"pipiIso_1+P+","rhoIso_1+S+","rhoIso_2+D+","f2Iso_2-S+"},"matrix_4x4__m=1.pdf");
  plot_wave_matrix({"pipiIso_0-S","rhoIso_0-P","pipiIso_1+P0","pipiIso_1+P+","rhoIso_1+S0","rhoIso_1+S+"},"matrix_6x6__lom_m3pi.pdf");
  
}
