
double DeltaPhiLab(TLorentzVector &PiplusL, TLorentzVector &PiplusU, TLorentzVector &Piminus1, TLorentzVector &Piminus2, TLorentzVector &Proton)
{

  double res=0.;
  TLorentzVector LV_P4 = Proton + PiplusL;
  TLorentzVector UV_P4 = PiplusU + Piminus1 + Piminus2;

   res = TMath::RadToDeg()*(UV_P4.Vect().Phi()-LV_P4.Vect().Phi());
   return res;

}


double DeltaThetaCOM(TLorentzVector &PiplusL, TLorentzVector &PiplusU, TLorentzVector &Piminus1, TLorentzVector &Piminus2, TLorentzVector &Proton, double bz)
{

  TLorentzVector LV_P4, UV_P4;
  TLorentzVector piplusL(PiplusL), piplusU(PiplusU), piminus1(Piminus1), piminus2(Piminus2), proton(Proton);
  double res=0.;  

  //define a TLorentzRotation matrix instead of copying four-vectors and calling Boost.

    proton.Boost(0,0,bz);
    piplusL.Boost(0,0,bz);
    piplusU.Boost(0,0,bz);
    piminus1.Boost(0,0,bz);
    piminus2.Boost(0,0,bz);

  LV_P4 = proton + piplusL;
  UV_P4 = piplusU + piminus1 + piminus2;

  //  std::cout << "UV_Theta+LV_Theta = " << TMath::RadToDeg()*(UV_P4.Vect().Theta()+LV_P4.Vect().Theta()) << endl;

  
   res = TMath::RadToDeg()*(UV_P4.Vect().Theta()+LV_P4.Vect().Theta());
   return res;
    
}



double DeltaThetaCOM2(const TLorentzVector &PiplusL, const TLorentzVector &PiplusU, const TLorentzVector &Piminus1, const TLorentzVector &Piminus2, const TLorentzVector &Proton, double bz)
{

  TLorentzRotation boost_to_com(0,0,bz);
  double res=0.;  

  TLorentzVector LV_P4 = boost_to_com*(Proton + PiplusL);
  TLorentzVector UV_P4 = boost_to_com*(PiplusU + Piminus1 + Piminus2);

  std::cout << "UV_Theta+LV_Theta = " << TMath::RadToDeg()*(UV_P4.Vect().Theta()+LV_P4.Vect().Theta()) << endl;

  
   res = TMath::RadToDeg()*(UV_P4.Vect().Theta()+LV_P4.Vect().Theta());
   return res;
    
}




//Rewrite with the help of TLorentzRotation matrix, that will work faster.
// ***************************************
//  Three Body van Hove coordinates
//   return van Hove X, Y, and omega for particles P1, P2, and P3 (it is assumed that CM = P1 + P2 + P3)
// ***************************************

double vanHoveX(TLorentzVector &P1, TLorentzVector &P2, TLorentzVector &P3)
{
    TLorentzVector p1(P1);
    TLorentzVector p2(P2);
    TLorentzVector p3(P3);
    TLorentzVector cm = p1 + p2 + p3;

    p1.Boost(-1.0*cm.BoostVector());
    p2.Boost(-1.0*cm.BoostVector());

    double q1 = p1.Pz();
    double q2 = p2.Pz();

    return -1.0*(q1+2.0*q2) / TMath::Sqrt(2.0);
}

double vanHoveY(TLorentzVector &P1, TLorentzVector &P2, TLorentzVector &P3)
{
    TLorentzVector p1(P1);
    TLorentzVector p2(P2);
    TLorentzVector p3(P3);
    TLorentzVector cm = p1 + p2 + p3;
    p1.Boost(-1.0*cm.BoostVector());

    double q1 = p1.Pz();

    return TMath::Sqrt(3.0/2.0)*q1;
}


double vanHoveomega(TLorentzVector &P1, TLorentzVector &P2, TLorentzVector &P3)
{
    TLorentzVector p1(P1);
    TLorentzVector p2(P2);
    TLorentzVector p3(P3);
    TLorentzVector cm = p1 + p2 + p3;

    p1.Boost(-1.0*cm.BoostVector());
    p2.Boost(-1.0*cm.BoostVector());

    double q1 = p1.Pz();
    double q2 = p2.Pz();

    return TMath::ATan2(-1.0*TMath::Sqrt(3.0)*q1,q1+2*q2) + TMath::Pi();
}



// ***************************************
//  Four Body van Hove coordinates
//   return van Hove theta and phi for particles P1, P2, P3, and P4 (it is assumed that CM = P1 + P2 + P3 + P4)
// ***************************************

double vanHoveTheta(TLorentzVector &P1, TLorentzVector &P2, TLorentzVector &P3, TLorentzVector &P4)
{
    TLorentzVector p1(P1);
    TLorentzVector p2(P2);
    TLorentzVector p3(P3);
    TLorentzVector p4(P4);
    TLorentzVector cm = p1 + p2 + p3 + p4;
    p1.Boost(-1.0*cm.BoostVector());
    p2.Boost(-1.0*cm.BoostVector());
    p3.Boost(-1.0*cm.BoostVector());
    p4.Boost(-1.0*cm.BoostVector());

    double q1 = p1.Pz();
    double q2 = p2.Pz();
    double q3 = p3.Pz();

    double x = TMath::Sqrt(3.0/8.0)*(q3-q2);
    double y = (2.0*q1 + 3.0*q2 +3.0*q3) / TMath::Sqrt(8.0);
    double z = q1;

    double theta = TMath::ATan2(TMath::Sqrt(x*x+y*y),z);

    return theta;
}

double vanHovePhi(TLorentzVector &P1, TLorentzVector &P2, TLorentzVector &P3, TLorentzVector &P4)
{
    TLorentzVector p1(P1);
    TLorentzVector p2(P2);
    TLorentzVector p3(P3);
    TLorentzVector p4(P4);
    TLorentzVector cm = p1 + p2 + p3 + p4;
    p1.Boost(-1.0*cm.BoostVector());
    p2.Boost(-1.0*cm.BoostVector());
    p3.Boost(-1.0*cm.BoostVector());
    p4.Boost(-1.0*cm.BoostVector());

    double q1 = p1.Pz();
    double q2 = p2.Pz();
    double q3 = p3.Pz();

    double x = TMath::Sqrt(3.0/8.0)*(q3-q2);
    double y = (2.0*q1 + 3.0*q2 +3.0*q3) / TMath::Sqrt(8.0);

    double phi = TMath::ATan2(y,x);
    if( phi < 0 ) phi += 2.0*TMath::Pi();

    return phi;
}



/*
double cosThetaGJ(TLorentzVector &Beam, TLorentzVector &Proton, TLorentzVector &PiplusU, TLorentzVector &PiplusL, TLorentzVector &PiminusBachelor, TLorentzVector &PiminusIsobar)
{
  TLorentzVector X_P4,Isobar_P4,PiminusBachelor_P4,PiminusIsobar_P4,Recoil_P4,Photon_P4;

  TVector3 bX, bIso;
  TVector3 xGJ,yGJ,zGJ,xHF,yHF,zHF,anglesGJ,anglesHF;

  
  X_P4 = PiplusU + PiminusBachelor + PiminusIsobar; 
  Isobar_P4 = PiplusU + PiminusIsobar;
  PiminusBachelor_P4 = PiminusBachelor;
  PiminusIsobar_P4 = PiminusIsobar;
  Recoil_P4 = PiplusL + Proton;		
  Photon_P4 = Beam;

  //Define boost to the X rest frame		
  bX = -X_P4.BoostVector();

  

}
*/



		/////////////*********************Define angles in the GJ and HF frames; define auxiliary vectors*********************************
/*		
		Double_t CosTheta_Piminus1_GJ_U1, CosTheta_Piminus2_GJ_U1, Phi_Piminus1_GJ_U1, Phi_Piminus2_GJ_U1;
		Double_t CosTheta_Piminus1_HF_U1, CosTheta_Piminus2_HF_U1, Phi_Piminus1_HF_U1, Phi_Piminus2_HF_U1;


		TVector3 bX, bIso;
		TVector3 xGJ,yGJ,zGJ,xHF,yHF,zHF,anglesGJ,anglesHF;
		

		////////////////The 1st combination: Piplus1 in the Upper Vertex, Piminus1 is bachelor///////////////


		bIso = -Isobar_P4.BoostVector();



		//Go to the X rest frame
		Photon_P4.Boost(bX); 
		Isobar_P4.Boost(bX); 
		Recoil_P4.Boost(bX); 
		PiminusBachelor_P4.Boost(bX); 

		
		zGJ = Photon_P4.Vect().Unit();
		yGJ = Recoil_P4.Vect().Cross(zGJ).Unit();
		xGJ = yGJ.Cross(zGJ);
		anglesGJ.SetXYZ(PiminusBachelor_P4.Vect().Dot(xGJ),PiminusBachelor_P4.Vect().Dot(yGJ),PiminusBachelor_P4.Vect().Dot(zGJ));

	      
	        CosTheta_Piminus1_GJ_U1 = anglesGJ.CosTheta();
		Phi_Piminus1_GJ_U1 = TMath::RadToDeg()*anglesGJ.Phi();

		
		//Go to the Isobar rest frame
		PiminusIsobar_P4.Boost(bIso); 

		
		zHF = Isobar_P4.Vect().Unit();
		yHF = zGJ.Cross(zHF);
		xHF = yHF.Cross(zHF);
		anglesHF.SetXYZ(PiminusIsobar_P4.Vect().Dot(xHF),PiminusIsobar_P4.Vect().Dot(yHF),PiminusIsobar_P4.Vect().Dot(zHF));

		
		CosTheta_Piminus2_HF_U1 = anglesHF.CosTheta();
		Phi_Piminus2_HF_U1 = TMath::RadToDeg()*anglesHF.Phi();

		
		
		////////////////The 2nd combination: Piplus1 in the Upper Vertex, Piminus2 is bachelor///////////////
		X_P4 = Piplus1Piminus1Piminus2_P4; 
		Isobar_P4 = Piplus1Piminus1_P4;
		PiminusBachelor_P4 = locPiMinus2P4;
		PiminusIsobar_P4 = locPiMinus1P4;
		Recoil_P4 = Piplus2Proton_P4;		
		Photon_P4 = locBeamP4;

		
		//Define boosts to the X and Isobar rest frames		
		bX = -X_P4.BoostVector();
		bIso = -Isobar_P4.BoostVector();


		//Go to the X rest frame
		Photon_P4.Boost(bX); 
		Isobar_P4.Boost(bX); 
		Recoil_P4.Boost(bX); 
		PiminusBachelor_P4.Boost(bX); 

		
		zGJ = Photon_P4.Vect().Unit();
		yGJ = Recoil_P4.Vect().Cross(zGJ).Unit();
		xGJ = yGJ.Cross(zGJ);
		anglesGJ.SetXYZ(PiminusBachelor_P4.Vect().Dot(xGJ),PiminusBachelor_P4.Vect().Dot(yGJ),PiminusBachelor_P4.Vect().Dot(zGJ));

	      
	        CosTheta_Piminus2_GJ_U1 = anglesGJ.CosTheta();
		Phi_Piminus2_GJ_U1 = TMath::RadToDeg()*anglesGJ.Phi();

		
		//Go to the Isobar rest frame
		PiminusIsobar_P4.Boost(bIso); 

		
		zHF = Isobar_P4.Vect().Unit();
		yHF = zGJ.Cross(zHF);
		xHF = yHF.Cross(zHF);
		anglesHF.SetXYZ(PiminusIsobar_P4.Vect().Dot(xHF),PiminusIsobar_P4.Vect().Dot(yHF),PiminusIsobar_P4.Vect().Dot(zHF));

		
		CosTheta_Piminus1_HF_U1 = anglesHF.CosTheta();
		Phi_Piminus1_HF_U1 = TMath::RadToDeg()*anglesHF.Phi();
*/
