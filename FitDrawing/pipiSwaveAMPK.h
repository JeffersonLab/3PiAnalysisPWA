// -----------------------------
// Breakup momentum (complex)
// -----------------------------
complex<double> breakupMomentum(double m, double m1, double m2)
{
    complex<double> val = (m*m*m*m + m1*m1*m1*m1 + m2*m2*m2*m2
        - 2.0*m*m*m1*m1 - 2.0*m*m*m2*m2 - 2.0*m1*m1*m2*m2);
    return sqrt(val) / (2.0 * m);
}

// -----------------------------
// Main amplitude
// -----------------------------
complex<double> amp_pipiSwave(double m) {

    double mpi  = 0.13957039;
    double mpi0 = 0.13497680;
    double mK   = 0.49367700;
    double mK0  = 0.49761400;
    double mKmean = 0.5*(mK + mK0);

    double s = m*m;

    // --- parameters ---
    vector<double> sP  = {-0.0074, 0.9828};
    vector<double> a11 = {0.1131, 0.}; //-0.1968*0.1968};
    vector<double> a22 = {-0.3216, 0.}; //-0.0154*0.0154};

    vector<double> c11 = {0.0337, -0.3185, -0.0942, -0.5927}; //, 0.1957};
    vector<double> c22 = {0.3010, -0.5140, 0.1176, 0.5204};  //, -0.3977};

    // --- breakup momenta ---
    complex<double> qPiPi   = breakupMomentum(m, mpi, mpi);
    complex<double> qPi0Pi0 = breakupMomentum(m, mpi0, mpi0);
    complex<double> qKK     = breakupMomentum(m, mK, mK);
    complex<double> qK0K0   = breakupMomentum(m, mK0, mK0);

    complex<double> rho11 = (qPiPi + qPi0Pi0) / m;
    complex<double> rho22 = (qKK + qK0K0) / m;

    complex<double> i(0,1);
    complex<double> M11(0,0), M22(0,0);

    double scale = (s / (4*mKmean*mKmean)) - 1.0;

    // --- poles ---
    for (size_t k = 0; k < sP.size(); k++) {
        M11 += a11[k] / (s - sP[k]);
        M22 += a22[k] / (s - sP[k]);
    }

    // --- polynomial ---
    for (size_t k = 0; k < c11.size(); k++) {
        M11 += c11[k] * pow(scale, (int)k);
        M22 += c22[k] * pow(scale, (int)k);
    }

    // --- unitarity ---
    M11 -= i * rho11;
    M22 -= i * rho22;

    return 1.0 / M11;
}


complex<double> amphat_pipiSwave(double m)
{

    double mpi  = 0.13957039;
    double mpi0 = 0.13497680;
    double mK   = 0.49367700;
    double mK0  = 0.49761400;
    double mKmean = 0.5*(mK + mK0);

    // --- breakup momenta ---
    complex<double> qPiPi   = breakupMomentum(m, mpi, mpi);
    complex<double> qPi0Pi0 = breakupMomentum(m, mpi0, mpi0);
    complex<double> qKK     = breakupMomentum(m, mK, mK);
    complex<double> qK0K0   = breakupMomentum(m, mK0, mK0);

    complex<double> rho11 = (qPiPi + qPi0Pi0) / m;
    complex<double> rho22 = (qKK + qK0K0) / m;

    complex<double> M11 = amp_pipiSwave(m);
    
    return rho11 / M11;
}
