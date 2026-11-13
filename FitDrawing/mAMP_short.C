// mAMP: modified Au-Morgan-Pennington pipi S-wave (arXiv:2305.11711, Table 2)
// Usage: root -l mAMP.C

#include <complex>
#include <cmath>
typedef std::complex<double> Cmplx;

const double kMPi = 0.13957018, kMK = 0.5*(0.493677+0.497611);
const double kMPi2 = kMPi*kMPi, kMK2 = kMK*kMK, k4MK2 = 4*kMK2;

const double s0 = -0.0074, s1 = 0.9828;
const double a11 = 0.1131, a22 = -0.3216;
const double c11[5] = {0.0337,-0.3185,-0.0942,-0.5927, 0.0};
const double c22[5] = {0.3010,-0.5140, 0.1176, 0.5204, 0.0};

Cmplx Rho(double s, double m2) {
    return 2.0*sqrt(Cmplx(s/4-m2,0))/sqrt(Cmplx(s,0));
}
double Poly(const double *c, double s) {
    double x = s/k4MK2-1;
    return c[0]+x*(c[1]+x*(c[2]+x*(c[3]+x*c[4])));
}

// M_12=0 => single channel: That11 = 1/(M11 - i*rho1)
Cmplx That11(double s) {
    double M11 = a11/(s-s0) + Poly(c11,s);
    return 1.0/(M11 - Cmplx(0,1)*Rho(s,kMPi2));
}
Cmplx T11(double s) { return Rho(s,kMPi2)*That11(s); }

double fThat(double *x,double*) { double s=x[0]*x[0]; return s<4*kMPi2?0:std::abs(That11(s)); }
double fThat2(double *x,double*){ double s=x[0]*x[0]; return s<4*kMPi2?0:std::norm(That11(s)); }
double fT2(double *x,double*)   { double s=x[0]*x[0]; return s<4*kMPi2?0:std::norm(T11(s)); }



void mAMP_short() {
    gStyle->SetOptStat(0);
    const int nPx=4000, N=5000;
    const double mMin=0, mMax=2.5, mKK=2*kMK;

    TCanvas *c = new TCanvas("c","mAMP",1200,400);
    c->Divide(3,1);


    // (a) Argand diagram
    c->cd(1); gPad->SetGrid();
    gPad->SetLeftMargin(0.13);
    gPad->SetBottomMargin(0.12);
    double re[5000], im[5000];
    for (int i=0;i<N;i++) {
        double m=mMin+(mMax-mMin)*i/(N-1.0);
        double s=m*m;
        if (s<4*kMPi2) { re[i]=0; im[i]=0; continue; }
        Cmplx t=T11(s); re[i]=t.real(); im[i]=t.imag();
    }
    TGraph *gArg = new TGraph(N,re,im);
    gArg->SetTitle("Argand T;Re T;Im T");
    gArg->SetLineColor(kBlue); gArg->SetLineWidth(2);
    gArg->GetYaxis()->SetTitleSize(0.05);
    gArg->GetXaxis()->SetTitleSize(0.06);
    gArg->Draw("AL");
    TEllipse *uc = new TEllipse(0,0.5,0.5,0.5);
    uc->SetLineColor(kGray+1); uc->SetLineStyle(2);
    uc->SetFillStyle(0); uc->Draw("same");


    
    // (b) |That11|^2
    c->cd(2); gPad->SetGrid();
    gPad->SetLeftMargin(0.13);
    gPad->SetBottomMargin(0.12);
    TF1 *f2 = new TF1("f2",fThat2,mMin,mMax,0);
    f2->SetNpx(nPx); f2->SetLineColor(kBlue); f2->SetLineWidth(2);
    f2->GetYaxis()->SetTitleSize(0.05);
    f2->GetXaxis()->SetTitleSize(0.06);
    f2->SetTitle("|#it{T}|^{2};m(#pi#pi) [GeV];|#it{T}|^{2}");
    f2->Draw();
    TLine *lk2 = new TLine(mKK,0,mKK,f2->GetMaximum());
    lk2->SetLineColor(kRed); lk2->SetLineStyle(2); lk2->Draw("same");

 

    // (c) |T11|^2
    /*
    c->cd(2); gPad->SetGrid();
    gPad->SetLeftMargin(0.13);
    gPad->SetBottomMargin(0.12);
    TF1 *f3 = new TF1("f3",fT2,mMin,mMax,0);
    f3->SetNpx(nPx); f3->SetLineColor(kBlue); f3->SetLineWidth(2);
    f3->GetYaxis()->SetTitleSize(0.05);
    f3->GetXaxis()->SetTitleSize(0.06);
    f3->SetTitle("|T_{11}|^{2};m(#pi#pi) [GeV];|T_{11}|^{2}");
    f3->Draw("");
    TLine *lk3 = new TLine(mKK,0,mKK,f3->GetMaximum());
    lk3->SetLineColor(kRed); lk3->SetLineStyle(2); lk3->Draw("same");
    */

    // (c) Phase shift (unwrapped)
    c->cd(3); gPad->SetGrid();
    gPad->SetLeftMargin(0.13);
    gPad->SetBottomMargin(0.12);
    double mA[5000], ph[5000]; double prev=0;
    for (int i=0;i<N;i++) {
        double m=mMin+(mMax-mMin)*i/(N-1.0); mA[i]=m;
        double s=m*m;
        if (s<4*kMPi2) { ph[i]=0; continue; }
        Cmplx S=1.0+2.0*Cmplx(0,1)*T11(s);
        double d=std::arg(S)/2*180/M_PI;
        if (i>0) while(d-prev<-90) d+=180;
        ph[i]=d; prev=d;
    }
    TGraph *gPh = new TGraph(N,mA,ph);
    gPh->SetTitle("#delta_{0};m(#pi#pi) [GeV];#delta [deg]"); //_{0}^{0}
    gPh->SetLineColor(kBlue); gPh->SetLineWidth(2);
    gPh->GetXaxis()->SetRangeUser(mMin,mMax);
    gPh->GetYaxis()->SetTitleSize(0.05);
    gPh->GetXaxis()->SetTitleSize(0.06);
    gPh->Draw("AL");
    TLine *l90 = new TLine(mMin,90,mMax,90);
    l90->SetLineColor(kGray); l90->SetLineStyle(3); l90->Draw("same");
    TLine *l180= new TLine(mMin,180,mMax,180);
    l180->SetLineColor(kGray); l180->SetLineStyle(3); l180->Draw("same");

    

    
    /*
    // (a) |That11|
    c->cd(1); gPad->SetGrid();
    TF1 *f1 = new TF1("f1",fThat,mMin,mMax,0);
    f1->SetNpx(nPx); f1->SetLineColor(kBlue); f1->SetLineWidth(2);
    f1->SetTitle("|#it{T}_{11}|;m(#pi#pi) [GeV];|#it{T}_{11}|");
    f1->Draw();
    TLine *lk1 = new TLine(mKK,0,mKK,f1->GetMaximum());
    lk1->SetLineColor(kRed); lk1->SetLineStyle(2); lk1->Draw("same");



    // (f) Info
    
    c->cd(4);
    TLatex tex; tex.SetTextFont(42); tex.SetTextSize(0.05);
    tex.DrawLatexNDC(0.05,0.90,"#bf{mAMP (arXiv:2305.11711)}");
    tex.SetTextSize(0.034);
    tex.DrawLatexNDC(0.05,0.82,"Single channel (K#bar{K} decoupled)");
    tex.DrawLatexNDC(0.05,0.74,"#hat{T}_{11} = 1/(M_{11} - i#rho_{1})");
    tex.DrawLatexNDC(0.05,0.64,"M_{11} = a_{11}/(s-s_{0}) + #Sigma c^{n}_{11} x^{n}");
    tex.DrawLatexNDC(0.05,0.54,"x = s/(4m_{K}^{2}) - 1,  n = 0..3");
    tex.DrawLatexNDC(0.05,0.42,"#eta #equiv 1 (elastic)");
    tex.DrawLatexNDC(0.05,0.32,"f_{0}(980), f_{0}(1500) added separately");
    */

    


    c->SaveAs("mAMP.pdf");
}
