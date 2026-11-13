/////// SET POLARIZATION VARIABLES

void definePolSet(FILE *f)
{
  const char *beamtime[3] = {"Spring2017","Spring2018","Fall2018"};
  const char *diamond[4] = {"PARA_0","PERP_45","PERP_90","PARA_135"};
  const float polangle[3][4] = {{1.8,47.9,94.5,138.4},{4.1,48.5,94.2,137.6},{3.3,48.3,92.9,137.9}};
  const float polfrac[3] = {0.35,0.35,0.35};

    for (int ii=0; ii<3; ii++)
	fprintf(f, "define polVal_%s %.2f\n",beamtime[ii],polfrac[ii]);    
    fprintf(f, "\n");
    for (int ii=0; ii<3; ii++){
      for (int jj=0; jj<4; jj++)      
	fprintf(f, "define polAng_%s_%s %.1f\n",beamtime[ii],diamond[jj],polangle[ii][jj]);
      fprintf(f, "\n");
    }
}



/////// INTRODUCE REACTIONS AND COHERENT SUMS

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

void writeGenDefault(FILE *f)
{
    fprintf(f, "#####################################\n");
    fprintf(f, "###  SET REACTION AND COHERENT SUMS  \n");
    fprintf(f, "#####################################\n");
    fprintf(f, "\nreaction reac3PiDelta Beam Proton Pi-%%a Pi-%%b Pi+%%a Pi+%%b\n");
    fprintf(f, "\n\nsum reac3PiDelta ImagNegSign\n");
    fprintf(f, "sum reac3PiDelta RealNegSign\n");
    fprintf(f, "sum reac3PiDelta ImagPosSign\n");
    fprintf(f, "sum reac3PiDelta RealPosSign\n");
    fprintf(f, "sum reac3PiDelta UniBG\n");
  
}



////// AMPLITUDES DEFINITION

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
    fprintf(f, "#amplitude 3PiDelta::sum_name::amp_name\n");
    fprintf(f, "#Iso_ps_refl(J,m,L,Re/Im,sign,angle,fraction,dalitz)\n");
}


void writeIsoPsAmp(FILE *f, const char *term, int S, int J, int M, int L){
  fprintf(f,"amplitude LOOPREAC::ImagNegSign::%s Iso_ps_refl %d %d %d %d -1 -1\n",term,S,J,M,L);
  fprintf(f,"amplitude LOOPREAC::RealNegSign::%s Iso_ps_refl %d %d %d %d +1 -1\n",term,S,J,M,L);
  fprintf(f,"amplitude LOOPREAC::ImagPosSign::%s Iso_ps_refl %d %d %d %d -1 +1\n",term,S,J,M,L);
  fprintf(f,"amplitude LOOPREAC::RealPosSign::%s Iso_ps_refl %d %d %d %d +1 +1\n",term,S,J,M,L);
}


void writeIsoPsAmpPol(FILE *f, const char *term, int S, int J, int M, int L){
  fprintf(f,"amplitude LOOPREAC::ImagNegSign::%s Iso_ps_refl %d %d %d %d -1 -1 LOOPPOLANG LOOPPOLVAL\n",term,S,J,M,L);
  fprintf(f,"amplitude LOOPREAC::RealNegSign::%s Iso_ps_refl %d %d %d %d +1 -1 LOOPPOLANG LOOPPOLVAL\n",term,S,J,M,L);
  fprintf(f,"amplitude LOOPREAC::ImagPosSign::%s Iso_ps_refl %d %d %d %d -1 +1 LOOPPOLANG LOOPPOLVAL\n",term,S,J,M,L);
  fprintf(f,"amplitude LOOPREAC::RealPosSign::%s Iso_ps_refl %d %d %d %d +1 +1 LOOPPOLANG LOOPPOLVAL\n",term,S,J,M,L);
}


void writeGenIsoPsAmpPol(FILE *f, const char *term, int S, int J, int M, int L){
  fprintf(f,"amplitude reac3PiDelta::ImagNegSign::%s Iso_ps_refl %d %d %d %d -1 -1 setPol\n",term,S,J,M,L);
  fprintf(f,"amplitude reac3PiDelta::RealNegSign::%s Iso_ps_refl %d %d %d %d +1 -1 setPol\n",term,S,J,M,L);
  fprintf(f,"amplitude reac3PiDelta::ImagPosSign::%s Iso_ps_refl %d %d %d %d -1 +1 setPol\n",term,S,J,M,L);
  fprintf(f,"amplitude reac3PiDelta::RealPosSign::%s Iso_ps_refl %d %d %d %d +1 +1 setPol\n",term,S,J,M,L);
}


void writeBW(FILE *f, const char *term, const char *masswidth, int S, int index1, int index2){
  fprintf(f,"\namplitude LOOPREAC::ImagNegSign::%s BreitWigner %s %d %d %d\n",term,masswidth,S,index1,index2);
  fprintf(f,"amplitude LOOPREAC::RealNegSign::%s BreitWigner %s %d %d %d\n",term,masswidth,S,index1,index2);
  fprintf(f,"amplitude LOOPREAC::ImagPosSign::%s BreitWigner %s %d %d %d\n",term,masswidth,S,index1,index2);
  fprintf(f,"amplitude LOOPREAC::RealPosSign::%s BreitWigner %s %d %d %d\n",term,masswidth,S,index1,index2);
}


void writeGenBW(FILE *f, const char *term, const char *masswidth, int S, int index1, int index2){
  fprintf(f,"\namplitude reac3PiDelta::ImagNegSign::%s BreitWigner %s %d %d %d\n",term,masswidth,S,index1,index2);
  fprintf(f,"amplitude reac3PiDelta::RealNegSign::%s BreitWigner %s %d %d %d\n",term,masswidth,S,index1,index2);
  fprintf(f,"amplitude reac3PiDelta::ImagPosSign::%s BreitWigner %s %d %d %d\n",term,masswidth,S,index1,index2);
  fprintf(f,"amplitude reac3PiDelta::RealPosSign::%s BreitWigner %s %d %d %d\n",term,masswidth,S,index1,index2);
}


void writeAMPK(FILE *f, const char *term, int index1, int index2){
  fprintf(f,"\namplitude LOOPREAC::ImagNegSign::%s PiPiSWaveAMPK %d %d\n",term,index1,index2);
  fprintf(f,"amplitude LOOPREAC::RealNegSign::%s PiPiSWaveAMPK %d %d\n",term,index1,index2);
  fprintf(f,"amplitude LOOPREAC::ImagPosSign::%s PiPiSWaveAMPK %d %d\n",term,index1,index2);
  fprintf(f,"amplitude LOOPREAC::RealPosSign::%s PiPiSWaveAMPK %d %d\n",term,index1,index2);
}

void writeGenAMPK(FILE *f, const char *term, int index1, int index2){
  fprintf(f,"\namplitude reac3PiDelta::ImagNegSign::%s PiPiSWaveAMPK %d %d\n",term,index1,index2);
  fprintf(f,"amplitude reac3PiDelta::RealNegSign::%s PiPiSWaveAMPK %d %d\n",term,index1,index2);
  fprintf(f,"amplitude reac3PiDelta::ImagPosSign::%s PiPiSWaveAMPK %d %d\n",term,index1,index2);
  fprintf(f,"amplitude reac3PiDelta::RealPosSign::%s PiPiSWaveAMPK %d %d\n",term,index1,index2);
}




////// CONSTRAINTS AND SCALING

void writeConstrainAmps(FILE *f, const char *term){
  fprintf(f,"\nconstrain LOOPREAC::ImagNegSign::%s LOOPREAC::RealPosSign::%s\n",term,term);
  fprintf(f,"constrain LOOPREAC::RealNegSign::%s LOOPREAC::ImagPosSign::%s\n",term,term);
}

void writeGenConstrainAmps(FILE *f, const char *term){
  fprintf(f,"\nconstrain reac3PiDelta::ImagNegSign::%s reac3PiDelta::RealPosSign::%s\n",term,term);
  fprintf(f,"constrain reac3PiDelta::RealNegSign::%s reac3PiDelta::ImagPosSign::%s\n",term,term);
}

void writeConstrainReactions(FILE *f, const char *firstreaction, const char *term){
  fprintf(f,"\nconstrain 3PiDelta_%s::RealNegSign::%s LOOPREAC::RealNegSign::%s\n",firstreaction,term,term);
  fprintf(f,"constrain 3PiDelta_%s::ImagNegSign::%s LOOPREAC::ImagNegSign::%s\n",firstreaction,term,term);
  fprintf(f,"constrain 3PiDelta_%s::RealPosSign::%s LOOPREAC::RealPosSign::%s\n",firstreaction,term,term);
  fprintf(f,"constrain 3PiDelta_%s::ImagPosSign::%s LOOPREAC::ImagPosSign::%s\n",firstreaction,term,term);
}


void writeScaling(FILE *f, const char *term){
  fprintf(f,"\nscale LOOPREAC::ImagNegSign::%s LOOPSCALE\n",term);
  fprintf(f,"scale LOOPREAC::RealNegSign::%s LOOPSCALE\n",term);
  fprintf(f,"scale LOOPREAC::ImagPosSign::%s LOOPSCALE\n",term);
  fprintf(f,"scale LOOPREAC::RealPosSign::%s LOOPSCALE\n",term);
}


////// INITIALIZATION

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

void writeGenInitialization(FILE *f, const char *term, float vmag1 = 100., float vphase1 = 100., float vmag2 = 100., float vphase2 = 100.){
  fprintf(f,"\ninitialize reac3PiDelta::ImagNegSign::%s polar %.1f %.1f fixed\n",term,vmag1,vphase1);
  fprintf(f,"initialize reac3PiDelta::RealNegSign::%s polar %.1f %.1f fixed\n",term,vmag2,vphase2);
}

