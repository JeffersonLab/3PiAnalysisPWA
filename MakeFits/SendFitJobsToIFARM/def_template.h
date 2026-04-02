void writeDefault(FILE *f)
{
    fprintf(f, "#####################################\n");
    fprintf(f, "###  SETUP INPUT, REACTIONS, SUMS    \n");
    fprintf(f, "#####################################\n");
    fprintf(f, "\nreaction LOOPREAC Beam Proton Pi- Pi- Pi+%%a Pi+%%b\n");
    fprintf(f, "\ngenmc LOOPREAC ROOTDataReader LOOPGENMC kin\n");
    fprintf(f, "accmc LOOPREAC ROOTDataReader LOOPACCMC kin\n");
    fprintf(f, "data  LOOPREAC ROOTDataReader LOOPDATA  kin\n");
    fprintf(f, "bkgnd LOOPREAC ROOTDataReader LOOPBG    kin\n");
    fprintf(f, "\n\nsum LOOPREAC ImagNegSign\n");
    fprintf(f, "sum LOOPREAC RealNegSign\n");
    fprintf(f, "sum LOOPREAC ImagPosSign\n");
    fprintf(f, "sum LOOPREAC RealPosSign\n");
    fprintf(f, "sum LOOPREAC UniBG\n");
  
}



void writeAmpDefinition(FILE *f)
{
    fprintf(f, "#####################################\n");
    fprintf(f, "# DEFINE AMPLITUDES\n");
    fprintf(f, "#####################################\n\n");

    fprintf(f, "#The decay amplitudes in reflectivity basis (usually written as Zim) are computed in the Iso_ps_refl.cc routine with the following inputs:\n");
    fprintf(f, "# argument 0 : isobar spin (S)\n");
    fprintf(f, "# argument 1 : spin (J)\n");
    fprintf(f, "# argument 2 : spin_proj (m)\n");
    fprintf(f, "# argument 3 : partial wave (L)\n");
    fprintf(f, "# argument 4 : real (+1) or imaginary (-1) part\n");
    fprintf(f, "# argument 5 : 1 + (+1/-1) * P_gamma\n");
    fprintf(f, "#The rest arguments are optional and always represent strings\n");
    fprintf(f, "# argument 6 : polarization type (string)\n");
    fprintf(f, "# argument 7 : polarization fraction (string)\n");
    fprintf(f, "# argument 8-11: may be dalitz parameters for a two-body vector decay\n\n");

    fprintf(f, "# Amplitude naming convention: [spin parity][L][spin_proj][refl]\n");
    fprintf(f, "#       [refl] = p or m\n");
    fprintf(f, "#       [spin parity] = 0-, 1+, 1-, 2+, 2-, etc.\n");
    fprintf(f, "#       [spin_proj] = -,0,+ //m2, m1, 0, p1, p2\n");
    fprintf(f, "#       [L] = S,P,D,F, etc.\n\n");

    fprintf(f, "#############################################################\n");
    fprintf(f, "#amplitude rhof2Iso::sum_name::amp_name\n");
    fprintf(f, "#Iso_ps_refl(J,m,L,Re/Im,sign,angle,fraction,dalitz)\n");
}


void writeIsoPsAmp(FILE *f, const char *term, int S, int J, int M, int L){
  fprintf(f,"amplitude LOOPREAC::ImagNegSign::%s Iso_ps_refl %d %d %d %d -1 -1 LOOPPOLANG LOOPPOLVAL\n",term,S,J,M,L);
  fprintf(f,"amplitude LOOPREAC::RealNegSign::%s Iso_ps_refl %d %d %d %d +1 -1 LOOPPOLANG LOOPPOLVAL\n",term,S,J,M,L);
  fprintf(f,"amplitude LOOPREAC::ImagPosSign::%s Iso_ps_refl %d %d %d %d -1 +1 LOOPPOLANG LOOPPOLVAL\n",term,S,J,M,L);
  fprintf(f,"amplitude LOOPREAC::RealPosSign::%s Iso_ps_refl %d %d %d %d +1 +1 LOOPPOLANG LOOPPOLVAL\n",term,S,J,M,L);
}


void writeBW(FILE *f, const char *term, const char *masswidth, int S, int index1, int index2){
  fprintf(f,"\namplitude LOOPREAC::ImagNegSign::%s BreitWigner %s %d %d %d\n",term,masswidth,S,index1,index2);
  fprintf(f,"amplitude LOOPREAC::RealNegSign::%s BreitWigner %s %d %d %d\n",term,masswidth,S,index1,index2);
  fprintf(f,"amplitude LOOPREAC::ImagPosSign::%s BreitWigner %s %d %d %d\n",term,masswidth,S,index1,index2);
  fprintf(f,"amplitude LOOPREAC::RealPosSign::%s BreitWigner %s %d %d %d\n",term,masswidth,S,index1,index2);
}


void writeConstrainAmps(FILE *f, const char *term){
  fprintf(f,"\nconstrain LOOPREAC::ImagNegSign::%s LOOPREAC::RealPosSign::%s\n",term,term);
  fprintf(f,"constrain LOOPREAC::RealNegSign::%s LOOPREAC::ImagPosSign::%s\n",term,term);
}

void writeConstrainReactions(FILE *f, const char *firstreaction, const char *term){
  fprintf(f,"\nconstrain rhof2Iso_%s::RealNegSign::%s LOOPREAC::RealNegSign::%s\n",firstreaction,term,term);
  fprintf(f,"constrain rhof2Iso_%s::ImagNegSign::%s LOOPREAC::ImagNegSign::%s\n",firstreaction,term,term);
  fprintf(f,"constrain rhof2Iso_%s::RealPosSign::%s LOOPREAC::RealPosSign::%s\n",firstreaction,term,term);
  fprintf(f,"constrain rhof2Iso_%s::ImagPosSign::%s LOOPREAC::ImagPosSign::%s\n",firstreaction,term,term);
}

void writeScaling(FILE *f, const char *term){
  fprintf(f,"\nscale LOOPREAC::ImagNegSign::%s LOOPSCALE\n",term);
  fprintf(f,"scale LOOPREAC::RealNegSign::%s LOOPSCALE\n",term);
  fprintf(f,"scale LOOPREAC::ImagPosSign::%s LOOPSCALE\n",term);
  fprintf(f,"scale LOOPREAC::RealPosSign::%s LOOPSCALE\n",term);
}


void writeInitializationReal(FILE *f, const char *term, float vreal1 = 100., float vreal2 = 100.){
  fprintf(f,"\ninitialize LOOPREAC::ImagNegSign::%s cartesian %.1f 0. real\n",term,vreal1);
  fprintf(f,"initialize LOOPREAC::RealNegSign::%s cartesian %.1f 0. real\n",term,vreal2);
  fprintf(f,"initialize LOOPREAC::ImagPosSign::%s cartesian %.1f 0. real\n",term,vreal2);
  fprintf(f,"initialize LOOPREAC::RealPosSign::%s cartesian %.1f 0. real\n",term,vreal1);
}


void writeInitialization(FILE *f, const char *term, float vreal1 = 100., float vimag1 = 100., float vreal2 = 100., float vimag2 = 100.){
  fprintf(f,"\ninitialize LOOPREAC::ImagNegSign::%s cartesian %.1f %.1f\n",term,vreal1,vimag1);
  fprintf(f,"initialize LOOPREAC::RealNegSign::%s cartesian %.1f %.1f\n",term,vreal2,vimag2);
  fprintf(f,"initialize LOOPREAC::ImagPosSign::%s cartesian %.1f %.1f\n",term,vreal2,vimag2);
  fprintf(f,"initialize LOOPREAC::RealPosSign::%s cartesian %.1f %.1f\n",term,vreal1,vimag1);
}



