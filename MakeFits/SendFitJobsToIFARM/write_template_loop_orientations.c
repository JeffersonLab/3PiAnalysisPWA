#include <cstdio>
#include <cstring>
#include <string>
#include "def_template.h"



//Define set of reactions and related constants
const char *subreaction[4] = {"PARA_0","PERP_45","PERP_90","PARA_135"};
const char *link[4] = {"PARA_0","PERP_45","PERP_90","PARA_135"};
const char *link_thrown = "link";

float polVal[4] = {0.3537,0.3484,0.3472,0.3512};
float polAng[4] = {1.8,47.9,94.5,138.4};



//This template will include the {[pipi]s pi| 0-S,1+P}, {rho pi| 0-P,1+S,2+D} and {f2 pi| 2-S} waves
const int NTERMS = 18;
const char *pwterm[NTERMS] = {"pipiIso_0-S","pipiIso_1+P-","pipiIso_1+P0","pipiIso_1+P+","rhoIso_0-P","rhoIso_1+S-","rhoIso_1+S0","rhoIso_1+S+","rhoIso_2+D--","rhoIso_2+D-","rhoIso_2+D0","rhoIso_2+D+","rhoIso_2+D++","f2Iso_2-S--","f2Iso_2-S-","f2Iso_2-S0","f2Iso_2-S+","f2Iso_2-S++"};
int signature[NTERMS][4] = {{0,0,0,0},{0,1,-1,1},{0,1,0,1},{0,1,1,1},{1,0,0,1},{1,1,-1,0},{1,1,0,0},{1,1,1,0},{1,2,-2,2},{1,2,-1,2},{1,2,0,2},{1,2,1,2},{1,2,2,2},{2,2,-2,0},{2,2,-1,0},{2,2,0,0},{2,2,1,0},{2,2,2,0}};
const char *pwname[NTERMS] = {"pipiIso: spin 0 parity -1","pipiIso: spin 1 parity +1","","","rhoIso: spin 0 parity -1","rhoIso: spin 1 parity +1","","","rhoIso: spin 2 parity +1","","","","","f2Iso: spin2 parity -1","","","",""};
const char *isobar[NTERMS] = {"pipi","pipi","pipi","pipi","rho","rho","rho","rho","rho","rho","rho","rho","rho","f2","f2","f2","f2","f2"};

float initialpoint[NTERMS][4] = {{100.,100.,100.,100.},{100.,100.,100.,100.},{100.,100.,100.,100.},{100.,100.,100.,100.},{1117.,100.,-1846.,100.},{1949.,-1873.,-1156.,35.},{1555.,-265.,-421.,-1430.},{1066.,762.,142.,1013.},{-209.,434.,-142.,-110.},{-1663.,-171.,1573.,819.},{540.,1211.,-354.,1801.},{403.,173.,156.,-328.},{-661.,-657.,-819.,264.},{100.,100.,100.,100.},{100.,100.,100.,100.},{100.,100.,100.,100.},{100.,100.,100.,100.},{100.,100.,100.,100.}};




//This template will include only the {[pipi]s pi| 0-S,1+P}, waves
/*
const int NTERMS = 4;
const char *pwterm[NTERMS] = {"pipiIso_0-S","pipiIso_1+P-","pipiIso_1+P0","pipiIso_1+P+"};
int signature[NTERMS][4] = {{0,0,0,0},{0,1,-1,1},{0,1,0,1},{0,1,1,1}};
const char *pwname[NTERMS] = {"pipiIso: spin 0 parity -1","pipiIso: spin 1 parity +1","",""};
const char *isobar[NTERMS] = {"pipi","pipi","pipi","pipi"};

float initialpoint[NTERMS][4] = {{100.,100.,100.,100.},{100.,100.,100.,100.},{100.,100.,100.,100.},{100.,100.,100.,100.}};
*/


//This template will include the {rho pi| 0-P,1+S,1+D,2+D} and {f2 pi| 2-S, 2-D} waves
/*
const int NTERMS = 22;
const char *pwterm[NTERMS] = {"rhoIso_0-P","rhoIso_1+S-","rhoIso_1+S0","rhoIso_1+S+","rhoIso_1+D-","rhoIso_1+D0","rhoIso_1+D+","rhoIso_2+D--","rhoIso_2+D-","rhoIso_2+D0","rhoIso_2+D+","rhoIso_2+D++","f2Iso_2-S--","f2Iso_2-S-","f2Iso_2-S0","f2Iso_2-S+","f2Iso_2-S++","f2Iso_2-D--","f2Iso_2-D-","f2Iso_2-D0","f2Iso_2-D+","f2Iso_2-D++"};
int signature[NTERMS][4] = {{1,0,0,1},{1,1,-1,0},{1,1,0,0},{1,1,1,0},{1,1,-1,2},{1,1,0,2},{1,1,1,2},{1,2,-2,2},{1,2,-1,2},{1,2,0,2},{1,2,1,2},{1,2,2,2},{2,2,-2,0},{2,2,-1,0},{2,2,0,0},{2,2,1,0},{2,2,2,0},{2,2,-2,2},{2,2,-1,2},{2,2,0,2},{2,2,1,2},{2,2,2,2}};
const char *pwname[NTERMS] = {"rhoIso: spin 0 parity -1","rhoIso: spin 1 parity +1","","","","","","rhoIso: spin 2 parity +1","","","","","f2Iso: spin2 parity -1","","","","","","","","",""};
const char *isobar[NTERMS] = {"rho","rho","rho","rho","rho","rho","rho","rho","rho","rho","rho","rho","f2","f2","f2","f2","f2","f2","f2","f2","f2","f2"};

float initialpoint[NTERMS][4] = {{1117.,0.,-1846.,0.},{1949.,-1873.,-1156.,35.},{1555.,-265.,-421.,-1430.},{1066.,762.,142.,1013.},{950.,-1131.,1146.,-866.},{437.,-676.,135.,-1000.},{1430.,-27.,-93.,-108.},{-209.,434.,-142.,-110.},{-1663.,-171.,1573.,819.},{540.,1211.,-354.,1801.},{403.,173.,156.,-328.},{-661.,-657.,-819.,264.},{100.,100.,100.,100.},{100.,100.,100.,100.},{100.,100.,100.,100.},{100.,100.,100.,100.},{100.,100.,100.,100.},{100.,100.,100.,100.},{100.,100.,100.,100.},{100.,100.,100.,100.},{100.,100.,100.,100.},{100.,100.,100.,100.}};
*/





int main() {

    FILE* file = fopen("template.cfg", "w");
    if (!file) return 1;


    // Write header
    fprintf(file, "#####################################\n");
    fprintf(file, "###     THIS IS A CONFIG FILE     ###\n");
    fprintf(file, "#####################################\n");
    fprintf(file, "\n\n\n");

    
    // Write global variables
    fprintf(file, "#####################################\n");
    fprintf(file, "###  GLOBAL VARIABLES                \n");
    fprintf(file, "#####################################\n");
    fprintf(file, "\n# Beam polarization definitions\n\n");


    for (int jj=0; jj<4;jj++) fprintf(file, "define polVal_%s %.4f\n",subreaction[jj],polVal[jj]);
    fprintf(file, "\n");
    
    for (int jj=0; jj<4;jj++) fprintf(file, "define polAng_%s %.1f\n",subreaction[jj],polAng[jj]);
    fprintf(file, "\n");
        
    fprintf(file, "\n\n");

    
    
    fprintf(file, "# Used masses and widths\n");
    fprintf(file, "parameter deltaMass 1.232 bounded  1.182 1.282\n");
    fprintf(file, "parameter deltaWidth 0.117 bounded 0.067 0.167\n");
    fprintf(file, "parameter rhoMass 0.775 bounded  0.725 0.825\n");
    fprintf(file, "parameter rhoWidth 0.147 bounded 0.097 0.197\n");
    fprintf(file, "parameter f2Mass 1.275 bounded   1.225 1.325\n");
    fprintf(file, "parameter f2Width 0.187 bounded   0.137 0.237\n");
    fprintf(file, "\ndefine delta [deltaMass] [deltaWidth]\n");
    fprintf(file, "define rho [rhoMass] [rhoWidth]\n");
    fprintf(file, "define f2 [f2Mass] [f2Width]\n");
    fprintf(file, "\n\nfit fit_3PiDelta_$SUBRANGE\n");
    fprintf(file, "\n\n\n");


    //write loop statements
    fprintf(file, "#####################################\n");
    fprintf(file, "###  LOOP STATEMENTS                 \n");
    fprintf(file, "#####################################\n");

    fprintf(file, "\nloop LOOPREAC");
    for (int jj=0; jj<4;jj++)  fprintf(file, " 3PiDelta_%s",subreaction[jj]);

    fprintf(file, "\n\nloop NIFILE");
    for (int jj=0; jj<4;jj++)  fprintf(file, " 3PiDelta_%s__$SUBRANGE.ni",subreaction[jj]);

    fprintf(file, "\n\nnormintfile LOOPREAC NIFILE\n#input\n");
    fprintf(file, "\n\n\n");
    
    fprintf(file, "loop LOOPDATA");
    for (int jj=0; jj<4;jj++)  fprintf(file, " $PATH/data_prompt_%s.root",link[jj]);


    fprintf(file, "\n\nloop LOOPBG");
    for (int jj=0; jj<4;jj++)  fprintf(file, " $PATH/data_sideband_%s.root",link[jj]);


    fprintf(file, "\n\nloop LOOPACCMC");
    for (int jj=0; jj<4;jj++)  fprintf(file, " $PATH/mcphsp_acc_%s.root",link[jj]);
    
    fprintf(file, "\n\nloop LOOPGENMC");
    for (int jj=0; jj<4;jj++)  fprintf(file, " $PATH/mcphsp_acc_%s.root",link[jj]);
	//fprintf(file, " $PATH/mcphsp_thrown_%s.root",link_thrown[ii]); // for the thrown trees

    
    fprintf(file, "\n\nloop LOOPSCALE");
    for (int jj=0; jj<4;jj++)  fprintf(file, " [parScale_%s]",subreaction[jj]);


    fprintf(file, "\n\nloop LOOPPOLANG");
    for (int jj=0; jj<4;jj++)  fprintf(file, " polAng_%s",subreaction[jj]);

    
    fprintf(file, "\n\nloop LOOPPOLVAL");
    for (int jj=0; jj<4;jj++)  fprintf(file, " polVal_%s",subreaction[jj]);
    fprintf(file, "\n\n\n\n");


    //Write Definitions for reactions and sums, and for the Iso_ps_refl amplitude
    writeDefault(file);
    fprintf(file, "\n\n\n");
    writeAmpDefinition(file);
    fprintf(file, "\n\n\n");

    
    //Write partial wave definitions
    fprintf(file, "########################### Uniform background  #####################################\n\n");
    fprintf(file, "amplitude LOOPREAC::UniBG::Flat Uniform\n");
    fprintf(file,"\n#####################################################################################");
    fprintf(file, "\n\n\n");



    for (int ii=0; ii<NTERMS; ii++){
      if (strlen(pwname[ii]) > 0) fprintf(file, "############################ %s ##################################\n\n",pwname[ii]);
      writeIsoPsAmpPol(file, pwterm[ii], signature[ii][0], signature[ii][1], signature[ii][2], signature[ii][3]); 
      if (strcmp(isobar[ii],"pipi") == 0) writeAMPK(file, pwterm[ii], 3, 4);    
      if (strcmp(isobar[ii],"rho") == 0) writeBW(file, pwterm[ii], "rho", 1, 3, 4);    
      if (strcmp(isobar[ii],"f2") == 0)  writeBW(file, pwterm[ii], "f2", 2, 3, 4);    
      writeBW(file, pwterm[ii], "delta", 1, 1, 5);    
      fprintf(file, "\n\n");
    }
      fprintf(file, "\n");



    // Write setting constraints
    fprintf(file, "#####################################\n");
    fprintf(file, "###     SET CONSTRAINTS           ###\n");
    fprintf(file, "#####################################\n");
    fprintf(file, "# Constrain same amplitudes in different incoherent sums\n");
    for (int ii=0; ii<NTERMS; ii++)
      writeConstrainAmps(file,pwterm[ii]);
    fprintf(file, "\n\n\n");
    fprintf(file, "# Constrain all other 'reactions' to the first one:\n");
    fprintf(file, "constrain 3PiDelta_%s::UniBG::Flat  LOOPREAC::UniBG::Flat\n",subreaction[0]);      
    for (int ii=0; ii<NTERMS; ii++)
      writeConstrainReactions(file,subreaction[0],pwterm[ii]);
    fprintf(file, "\n\n\n");
 

    // Write scaling
    fprintf(file, "#####################################\n");
    fprintf(file, "###     SETUP SCALING             ###\n");
    fprintf(file, "#####################################\n");

    fprintf(file, "\nparameter parScale_%s 1.0 fixed\n",subreaction[0]);
    for (int jj=1; jj<4;jj++)  fprintf(file, "parameter parScale_%s 1.0\n",subreaction[jj]);
    fprintf(file, "\n");
          
    

    fprintf(file, "\n\nscale LOOPREAC::UniBG::Flat LOOPSCALE\n");
    for (int ii=0; ii<NTERMS; ii++)
      writeScaling(file,pwterm[ii]);
    fprintf(file, "\n\n\n");


    // Write initializing amplitudes
    fprintf(file, "#####################################\n");
    fprintf(file, "###   INITIALIZE AMPLITUDES       ###\n");
    fprintf(file, "#####################################\n");
    fprintf(file, "\ninitialize LOOPREAC::UniBG::Flat cartesian 760. 0. real\n");
    writeInitializationReal(file,pwterm[0],1117.,-1846.);
    for (int ii=1; ii<NTERMS; ii++)
      writeInitialization(file,pwterm[ii],initialpoint[ii][0],initialpoint[ii][1],initialpoint[ii][2],initialpoint[ii][3]); 
      //      writeInitialization(file,pwterm[ii]); //default initialization, always at (100.,100.) 
    fprintf(file, "\n\n\n");


    
	    
    fclose(file);
    printf("Template file written\n");

    return 0;
}
