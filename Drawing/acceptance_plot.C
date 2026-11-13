#include <cstdlib>
#include <cstring>


TH2D* transposeTH2D(const TH2D* h, const char* name)
{
    const TAxis* ax = h->GetXaxis();
    const TAxis* ay = h->GetYaxis();

    TH2D* ht;

    if (ay->GetXbins()->GetSize() > 0 &&  ax->GetXbins()->GetSize() > 0) 
        ht = new TH2D(name, h->GetTitle(), ay->GetNbins(), ay->GetXbins()->GetArray(), ax->GetNbins(), ax->GetXbins()->GetArray());
    else if (ay->GetXbins()->GetSize() > 0) 
        ht = new TH2D(name, h->GetTitle(), ay->GetNbins(), ay->GetXbins()->GetArray(), ax->GetNbins(), ax->GetXmin(), ax->GetXmax());
    else if (ax->GetXbins()->GetSize() > 0) 
        ht = new TH2D(name, h->GetTitle(), ay->GetNbins(), ay->GetXmin(), ay->GetXmax(), ax->GetNbins(), ax->GetXbins()->GetArray());
    else 
        ht = new TH2D(name, h->GetTitle(), ay->GetNbins(), ay->GetXmin(), ay->GetXmax(), ax->GetNbins(), ax->GetXmin(), ax->GetXmax());
    
    for (int ix = 0; ix <= h->GetNbinsX() + 1; ++ix) {
        for (int iy = 0; iy <= h->GetNbinsY() + 1; ++iy) {
            ht->SetBinContent(iy, ix, h->GetBinContent(ix, iy));
            ht->SetBinError(iy, ix, h->GetBinError(ix, iy));
        }
    }

    ht->SetEntries(h->GetEntries());
    return ht;
}


void draw_2dim_acceptance(TH2D *histo, const char *title, const char *pdfname, const char *xtitle, const char *ytitle)
{

    TH2D *histo_transposed = transposeTH2D(histo, "histo_transposed"); //cos(theta) will be on the horizontal axis
    histo_transposed->GetXaxis()->SetTitle(xtitle);
    histo_transposed->GetYaxis()->SetTitle(ytitle);
    histo_transposed->SetTitle(title);

    histo_transposed->SetTitleOffset(0.9,"X");
    histo_transposed->SetTitleSize(0.05,"X");
    histo_transposed->SetTitleOffset(0.9,"Y");
    histo_transposed->SetTitleSize(0.05,"Y");    

    
    TCanvas *canva  = new TCanvas("c1","c1",15,10,480,300);
    histo_transposed->Draw("COLZ");
    canva->Print(pdfname, "pdf");
    canva->Close();
    delete canva;

    delete histo_transposed;
}


void draw_1dim_acceptance(TH1D *histo1, TH1D *histo2, TH1D *histo3, const char *xtitle, const char *pdfname)
{
    histo1->SetTitleOffset(0.9,"X");
    histo1->SetTitleSize(0.05,"X");
    histo1->SetTitleOffset(0.9,"Y");
    histo1->SetTitleSize(0.05,"Y");    
    
    histo1->GetXaxis()->SetTitle(xtitle);
    histo1->GetYaxis()->SetTitle("Acceptance / 15 MeV");
    histo1->SetMinimum(0.);
    histo1->SetMaximum(0.05);
    histo1->SetTitle("-t #leq 0.25 GeV^{2}");

    histo1->SetMarkerSize(0.5);
    histo1->SetMarkerStyle(20);
    histo1->SetLineColor(kBlack);
    histo1->SetLineWidth(2);    
    histo1->SetFillColor(kBlack);
    histo1->SetFillStyle(0); //3016

    histo2->SetMarkerColor(kRed+2);
    histo2->SetMarkerSize(0.5);
    histo2->SetMarkerStyle(20);
    histo2->SetLineColor(kRed+2);
    histo2->SetLineWidth(2);    
    histo2->SetFillColor(kRed+2);
    histo2->SetFillStyle(0); //3016

    histo3->SetMarkerColor(kGreen+3);
    histo3->SetMarkerSize(0.5);
    histo3->SetMarkerStyle(20);
    histo3->SetLineColor(kGreen+2);
    histo3->SetLineWidth(2);    
    histo3->SetFillColor(kGreen+3);
    histo3->SetFillStyle(0); //3016


    TLegend *legend = new TLegend(0.71,0.72,0.9,0.9); 
    legend->SetTextSize(0.05);
    legend->AddEntry(histo1, "Spring2017", "lpf");    
    legend->AddEntry(histo2, "Spring2018", "lpf");
    legend->AddEntry(histo3, "Fall2018", "lpf");    

    
    TCanvas *canva  = new TCanvas("c1","c1",15,10,480,300);
    histo1->GetXaxis()->SetRangeUser(0.3,2.3);
    histo1->Draw("HIST");      
    histo1->Draw("P SAME");      
    // histo1->Draw("HF SAME");      
    histo2->Draw("HIST SAME");      
    histo2->Draw("P SAME");      
    histo3->Draw("HIST SAME");      
    histo3->Draw("P SAME");      
    legend->Draw("SAME");
    canva->Print(pdfname, "pdf");
    canva->Close();
    delete canva;

    
}









void acceptance_plot()
{

  const Int_t NBINS_MINUST = 6;
  const double minust_left = 0., minust_right = 0.25;
  
   gROOT->SetBatch(kTRUE);  
   gStyle->SetOptStat(0);
   
   TFile *file_thrown1 = TFile::Open("Histos2026/correlations_thrown_spring2017.root","READ");
   TFile *file_reco1 = TFile::Open("Histos2026/correlations_mcphsp_spring2017_allcuts.root","READ");
   TFile *file_thrown2 = TFile::Open("Histos2026/correlations_thrown_spring2018.root","READ");
   TFile *file_reco2 = TFile::Open("Histos2026/correlations_mcphsp_spring2018_allcuts.root","READ");
   TFile *file_thrown3 = TFile::Open("Histos2026/correlations_thrown_fall2018.root","READ");
   TFile *file_reco3 = TFile::Open("Histos2026/correlations_mcphsp_fall2018_allcuts.root","READ");
   if (!file_thrown1 || !file_reco1 || !file_thrown2 || !file_reco2 || !file_thrown3 || !file_reco3) return;


   TH2D *h3d_thrown_anglesGJ_vs_m3pi_spring2017 = (TH2D*) file_thrown1->Get("PiplusUPiminusPiminus_mass_vs_minust");
   TH2D *h3d_reco_anglesGJ_vs_m3pi_spring2017 = (TH2D*) file_reco1->Get("PiplusUPiminusPiminus_mass_vs_minust");
   TH2D *h3d_thrown_anglesGJ_vs_m3pi_spring2018 = (TH2D*) file_thrown2->Get("PiplusUPiminusPiminus_mass_vs_minust");
   TH2D *h3d_reco_anglesGJ_vs_m3pi_spring2018 = (TH2D*) file_reco2->Get("PiplusUPiminusPiminus_mass_vs_minust");
   TH2D *h3d_thrown_anglesGJ_vs_m3pi_fall2018 = (TH2D*) file_thrown3->Get("PiplusUPiminusPiminus_mass_vs_minust");
   TH2D *h3d_reco_anglesGJ_vs_m3pi_fall2018 = (TH2D*) file_reco3->Get("PiplusUPiminusPiminus_mass_vs_minust");


   /*
   TH3D *h3d_thrown_anglesGJ_vs_minust = (TH3D*) file_thrown->Get("AnglesThrownGJ_vs_minust");
   TH3D *h3d_thrown_anglesHF_vs_minust = (TH3D*) file_thrown->Get("AnglesThrownHF_vs_minust");
   TH3D *h3d_reco_anglesGJ_vs_minust = (TH3D*) file_reco->Get("AnglesGJ_vs_minust");
   TH3D *h3d_reco_anglesHF_vs_minust = (TH3D*) file_reco->Get("AnglesHF_vs_minust");

   
   char const *trange[NBINS_MINUST] = {"0 #leq -t #leq 2 GeV^{2}","0 #leq -t #leq 0.25 GeV^{2}","0.25 GeV^{2} #leq -t #leq 0.5 GeV^{2}","0.5 GeV^{2} #leq -t #leq 0.75 GeV^{2}","0.75 GeV^{2} #leq -t #leq 1 GeV^{2}","1 GeV^{2} #leq -t #leq 2 GeV^{2}"};
   */
   

   //Plot acceptance as function of angles in bins by minust   
   //Either use dynamic memory for constructing multiple TH2D*, or define a static array of TH2D histograms  
   /*
   for (int jj=0; jj<1; jj++){

     TH2D* h2d_reco_GJ = (TH2D*)h3d_reco_anglesGJ_vs_minust->Project3D("xy");     
     TH2D* h2d_reco_HF = (TH2D*)h3d_reco_anglesHF_vs_minust->Project3D("xy");     
     TH2D* h2d_thrown_GJ = (TH2D*)h3d_thrown_anglesGJ_vs_minust->Project3D("xy");     
     TH2D* h2d_thrown_HF = (TH2D*)h3d_thrown_anglesHF_vs_minust->Project3D("xy");     

     
     h2d_reco_GJ->Divide(h2d_thrown_GJ);
     h2d_reco_HF->Divide(h2d_thrown_HF);

     draw_2dim_acceptance(&h2d_reco_GJ[jj],Form("%s",trange[jj]),Form("anglesGJ_%s.pdf",trange[jj]),"cos #theta (GJ)","#phi (GJ)  [deg]");
     draw_2dim_acceptance(&h2d_reco_HF[jj],Form("%s",trange[jj]),Form("anglesHF_%s.pdf",trange[jj]),"cos #theta (HF)","#phi (HF)  [deg]");     

     delete h2d_reco_GJ;
     delete h2d_reco_HF;
     delete h2d_thrown_GJ;
     delete h2d_thrown_HF;
     
   }
   */

   //Plot 1-dim acceptance across m(3pi)

   TH1D *h1d_reco_m3pi_spring2017 = h3d_reco_anglesGJ_vs_m3pi_spring2017->ProjectionX("h1d_m3pi_reco_spring2017",h3d_reco_anglesGJ_vs_m3pi_spring2017->GetYaxis()->FindBin(minust_left),h3d_reco_anglesGJ_vs_m3pi_spring2017->GetYaxis()->FindBin(minust_right));
   TH1D *h1d_thrown_m3pi_spring2017 = h3d_thrown_anglesGJ_vs_m3pi_spring2017->ProjectionX("h1d_m3pi_thrown_spring2017",h3d_thrown_anglesGJ_vs_m3pi_spring2017->GetYaxis()->FindBin(minust_left),h3d_thrown_anglesGJ_vs_m3pi_spring2017->GetYaxis()->FindBin(minust_right));
   TH1D *h1d_reco_m3pi_spring2018 = h3d_reco_anglesGJ_vs_m3pi_spring2018->ProjectionX("h1d_m3pi_reco_spring2018",h3d_reco_anglesGJ_vs_m3pi_spring2018->GetYaxis()->FindBin(minust_left),h3d_reco_anglesGJ_vs_m3pi_spring2018->GetYaxis()->FindBin(minust_right));
   TH1D *h1d_thrown_m3pi_spring2018 = h3d_thrown_anglesGJ_vs_m3pi_spring2018->ProjectionX("h1d_m3pi_thrown_spring2018",h3d_thrown_anglesGJ_vs_m3pi_spring2018->GetYaxis()->FindBin(minust_left),h3d_thrown_anglesGJ_vs_m3pi_spring2018->GetYaxis()->FindBin(minust_right));
   TH1D *h1d_reco_m3pi_fall2018 = h3d_reco_anglesGJ_vs_m3pi_fall2018->ProjectionX("h1d_m3pi_reco_fall2018",h3d_reco_anglesGJ_vs_m3pi_fall2018->GetYaxis()->FindBin(minust_left),h3d_reco_anglesGJ_vs_m3pi_fall2018->GetYaxis()->FindBin(minust_right));
   TH1D *h1d_thrown_m3pi_fall2018 = h3d_thrown_anglesGJ_vs_m3pi_fall2018->ProjectionX("h1d_m3pi_thrown_fall2018",h3d_thrown_anglesGJ_vs_m3pi_fall2018->GetYaxis()->FindBin(minust_left),h3d_thrown_anglesGJ_vs_m3pi_fall2018->GetYaxis()->FindBin(minust_right));

   h1d_reco_m3pi_spring2017->Divide(h1d_thrown_m3pi_spring2017);
   h1d_reco_m3pi_spring2018->Divide(h1d_thrown_m3pi_spring2018);
   h1d_reco_m3pi_fall2018->Divide(h1d_thrown_m3pi_fall2018);

   draw_1dim_acceptance(h1d_reco_m3pi_spring2017,h1d_reco_m3pi_spring2018,h1d_reco_m3pi_fall2018,"m(3#pi)  [GeV]","acc_m3pi.pdf");

   
   delete h1d_reco_m3pi_spring2017;
   delete h1d_thrown_m3pi_spring2018;
   delete h1d_thrown_m3pi_fall2018;
   
   file_thrown1->Close();
   file_reco1->Close();
   file_thrown2->Close();
   file_reco2->Close();
   file_thrown3->Close();
   file_reco3->Close();
    
}
