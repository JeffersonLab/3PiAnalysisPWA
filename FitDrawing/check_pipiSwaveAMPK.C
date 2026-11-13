#include <TCanvas.h>
#include <TGraph.h>
#include <cmath>
#include <complex>
#include <vector>

#include "pipiSwaveAMPK.h"
using namespace std;


const double PI = 4.0 * atan(1.0);




void setstyle(TGraph *graph)
{
    graph->SetLineWidth(2);
    graph->SetLineColor(kBlue);
    graph->GetXaxis()->SetTitleOffset(1.0);
    graph->GetXaxis()->SetTitleSize(0.06);
    graph->GetYaxis()->SetTitleOffset(1.0);
    graph->GetYaxis()->SetTitleSize(0.06);
}




void check_pipiSwaveAMPK()
{

    const int N = 2000;
    double mMin = 0.28;   
    double mMax = 2.5;

    vector<double> x(N), ymod(N), ymod2(N), phase(N);
    complex<double> AMP(0,0), S(0,0), imag(0,1);

    
    for (int i = 0; i < N; i++) {

        x[i] = mMin + (mMax - mMin) * i / (N - 1.0);
        AMP = amp_pipiSwave(x[i]);
	S = 1.0 + 2.0*imag*amphat_pipiSwave(x[i]);
	
        ymod2[i] = (double) norm(AMP);  // |A|^2;
        ymod[i] = (double) abs(AMP);    // |A|;
        phase[i] = (double) 180*arg(S)/2./PI;      	
    }

    
    TCanvas* canva = new TCanvas("c", "PiPiSwave", 1200, 400);
    canva->Divide(3,1,0.001,0.001);

    
    TGraph* graph1 = new TGraph(N, &x[0], &ymod[0]);
    TGraph* graph2 = new TGraph(N, &x[0], &ymod2[0]);
    TGraph* graph3 = new TGraph(N, &x[0], &phase[0]);
    
    graph1->SetTitle("The #pi#pi S-wave parametrization; m(2#pi) [GeV]; |T_{11}|");    
    graph2->SetTitle("The #pi#pi S-wave parametrization; m(2#pi) [GeV]; |T_{11}|^{2}");
    graph3->SetTitle("The #pi#pi S-wave parametrization; m(2#pi) [GeV]; #delta^{0}_{0} [deg]");
    setstyle(graph1);
    setstyle(graph2);
    setstyle(graph3);
      
    
    canva->cd(1);
    gPad->SetLeftMargin(0.14);  
    gPad->SetBottomMargin(0.12);   
    gPad->SetGrid();    
    graph1->GetXaxis()->SetRangeUser(0.,2.5);
    graph1->Draw("AL");

    canva->cd(2);
    gPad->SetLeftMargin(0.14);  
    gPad->SetBottomMargin(0.12);   
    gPad->SetGrid();    
    graph2->GetXaxis()->SetRangeUser(0.,2.5);
    graph2->Draw("AL");

    canva->cd(3);
    gPad->SetLeftMargin(0.14);  
    gPad->SetBottomMargin(0.12);   
    gPad->SetGrid();    
    graph3->GetXaxis()->SetRangeUser(0.,2.5);
    graph3->Draw("AL");

    
    
    canva->Print("pipiSwave_AMPK.pdf","pdf");
    //    canva->Close();
    
}
