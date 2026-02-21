// Note:  for info about possible function calls refer to this header file
#include "/work/halld/home/ilbelov/AmpTools/AmpTools/IUAmpTools/FitResults.h"
#include "/work/halld/home/ilbelov/AmpTools/AmpTools/IUAmpTools/ConfigurationInfo.h"

#include <iostream>
#include <string>

using namespace std;

// a helper function to fetch amplitude names from a vector of other objects
vector< string > namesFromAmpInfo( const vector< AmplitudeInfo* >& ampInfoVec );


int main( int argc, char* argv[] ){

  if( argc != 2 ){

    cout << "Usage:  displayFitResults [fitResultsFile]" << endl;
  }


  FitResults results( argv[1] );

  // make this function call here just to trigger the warning about free parameters so
  // it doesn't make a mess of the table formatting
  results.intensity();
   
  // hard code some lists of amplitudes we care about -- we'll want sum these up
  // over all sums and all reactions

  vector <string> FlatWave;
  FlatWave.push_back( "Flat" );
  
  vector<string> ZeroMinusWaves;
  ZeroMinusWaves.push_back( "0-P" );

  vector<string> OnePlusWaves;
  OnePlusWaves.push_back( "1+S-" );
  OnePlusWaves.push_back( "1+S0" );
  OnePlusWaves.push_back( "1+S+" );
  OnePlusWaves.push_back( "1+D-" );
  OnePlusWaves.push_back( "1+D0" );
  OnePlusWaves.push_back( "1+D+" );

  vector<string> TwoPlusWaves;
  TwoPlusWaves.push_back( "2+D--" );
  TwoPlusWaves.push_back( "2+D-" );
  TwoPlusWaves.push_back( "2+D0" );
  TwoPlusWaves.push_back( "2+D+" );
  TwoPlusWaves.push_back( "2+D++" );

  
  vector<string> allWaves = FlatWave;
  allWaves.insert(allWaves.end(), ZeroMinusWaves.begin(), ZeroMinusWaves.end());
  allWaves.insert(allWaves.end(), OnePlusWaves.begin(), OnePlusWaves.end());
  allWaves.insert(allWaves.end(), TwoPlusWaves.begin(), TwoPlusWaves.end());

  
  
  // PRINT SOME INFORMATION AT THE REACTION LEVEL
  
  vector< string > reactions = results.reactionList();
  cout << "====== REACTIONS ======" << endl;
  cout << " Number of reactions:  " << reactions.size() << endl;
  cout << "   (reaction name):  (observed signal events from fit)  [contribution to -2 ln L]" << endl;

  for( auto itr = reactions.begin(); itr != reactions.end(); ++itr ){

    pair< double, double > evts = results.intensity( results.ampList( *itr ), false );
    
    cout << "   " << *itr << ":  "  << evts.first << " +/- " << evts.second
	 << "  [" << results.likelihood( *itr ) << "]" << endl;
  }
  cout << "     TOTAL:  " << results.intensity(false).first << " +/- "
       << results.intensity(false).second << " [" << results.likelihood() << "]" << endl;


  // for some operations it helpful to use the functionality of the configuration info object
  const ConfigurationInfo* cfgInfo = results.configInfo();


  // PRINT INTENSITIES FOR EACH WAVE
  
  cout << "====== AMPLITUDES ======" << endl;
  cout << "   (amplitude name):  (acceptance corrected yield) (fit fraction) [detection efficiency]" << endl;

  for( auto itr = allWaves.begin(); itr != allWaves.end(); ++itr ){

    // first get the list of all amplitudes across sums and reactions
    vector< string > thisAmps = namesFromAmpInfo( cfgInfo->amplitudeList( "", "", *itr ) );

    pair< double, double > evtsObserved = results.intensity( thisAmps, false );
    pair< double, double > evtsCorrected = results.intensity( thisAmps );

    cout << "   " << *itr << "\t:  " << evtsCorrected.first << " +/- " << evtsCorrected.second << "  ("
	 << evtsCorrected.first/results.intensity().first << ") ["
	 << evtsObserved.first/evtsCorrected.first << "]" << endl;
  }

  

  // COMBINE ALL THE WAVES WITH A GIVEN J^{P} 

  cout << "====== COHERENT SUMS ======" << endl;
  cout << "   (amplitudes name):  (acceptance corrected yield) (fit fraction) [detection efficiency]" << endl;

  
  // get the list of full names for each m=-1,0,1 projection of the 1^{+} configuration and then add
  // them to the full list of all 1+ amplitudes  
  vector< string > fullOnePlusNames;
  for( auto itr = OnePlusWaves.begin(); itr != OnePlusWaves.end(); ++itr ){

    // get the list of all amplitudes across sums and reactions
    vector< string > thisAmps = namesFromAmpInfo( cfgInfo->amplitudeList( "", "", *itr ) );
    fullOnePlusNames.insert( fullOnePlusNames.end(), thisAmps.begin(), thisAmps.end() );
  }

  
  // now print out:
   pair< double, double > OnePlusObserved = results.intensity( fullOnePlusNames, false );
   pair< double, double > OnePlusCorrected = results.intensity( fullOnePlusNames );

   cout << "     ALL 1^{+}\t:  " <<  OnePlusCorrected.first << " +/- " << OnePlusCorrected.second << "  ("
	 << OnePlusCorrected.first/results.intensity().first << ") ["
	 << OnePlusObserved.first/OnePlusCorrected.first << "]" << endl;


  // get the list of full names for each m=-2,-1,0,1,2 projection of the 2^{+} configuration and then add
  // them to the full list of all 2+ amplitudes
  vector< string > fullTwoPlusNames;
  for( auto itr = TwoPlusWaves.begin(); itr != TwoPlusWaves.end(); ++itr ){

    // get the list of all amplitudes across sums and reactions
    vector< string > thisAmps = namesFromAmpInfo( cfgInfo->amplitudeList( "", "", *itr ) );
    fullTwoPlusNames.insert( fullTwoPlusNames.end(), thisAmps.begin(), thisAmps.end() );
  }
  
  // now print out:
   pair< double, double > TwoPlusObserved = results.intensity( fullTwoPlusNames, false );
   pair< double, double > TwoPlusCorrected = results.intensity( fullTwoPlusNames );

   cout << "     ALL 2^{+}\t:  " <<  TwoPlusCorrected.first << " +/- " << TwoPlusCorrected.second << "  ("
	 << TwoPlusCorrected.first/results.intensity().first << ") ["
	 << TwoPlusObserved.first/TwoPlusCorrected.first << "]" << endl;


  // get the list of full names of all the 0^{-}, 1^{+} and 2^{+} configurations and add
  // them to the full list of all 0-, 1+ & 2+ amplitudes  

   vector< string > fullZeroMinusOnePlusTwoPlusNames;
  for( auto itr = ZeroMinusWaves.begin(); itr != ZeroMinusWaves.end(); ++itr ){
    // get the list of all amplitudes across sums and reactions
    vector< string > thisAmps = namesFromAmpInfo( cfgInfo->amplitudeList( "", "", *itr ) );
    fullZeroMinusOnePlusTwoPlusNames.insert( fullZeroMinusOnePlusTwoPlusNames.end(), thisAmps.begin(), thisAmps.end() );
  }
   for( auto itr = OnePlusWaves.begin(); itr != OnePlusWaves.end(); ++itr ){
    // get the list of all amplitudes across sums and reactions
    vector< string > thisAmps = namesFromAmpInfo( cfgInfo->amplitudeList( "", "", *itr ) );
    fullZeroMinusOnePlusTwoPlusNames.insert( fullZeroMinusOnePlusTwoPlusNames.end(), thisAmps.begin(), thisAmps.end() );
  }
  for( auto itr = TwoPlusWaves.begin(); itr != TwoPlusWaves.end(); ++itr ){
    // get the list of all amplitudes across sums and reactions
    vector< string > thisAmps = namesFromAmpInfo( cfgInfo->amplitudeList( "", "", *itr ) );
    fullZeroMinusOnePlusTwoPlusNames.insert( fullZeroMinusOnePlusTwoPlusNames.end(), thisAmps.begin(), thisAmps.end() );
  }
  

  
  // now print out:
   pair< double, double > ZeroMinusOnePlusTwoPlusObserved = results.intensity( fullZeroMinusOnePlusTwoPlusNames, false );
   pair< double, double > ZeroMinusOnePlusTwoPlusCorrected = results.intensity( fullZeroMinusOnePlusTwoPlusNames );

   cout << "     ALL (0^{-},1^{+},2^{+})\t:  " <<  ZeroMinusOnePlusTwoPlusCorrected.first << " +/- " << ZeroMinusOnePlusTwoPlusCorrected.second << "  ("
	 << ZeroMinusOnePlusTwoPlusCorrected.first/results.intensity().first << ") ["
	 << ZeroMinusOnePlusTwoPlusObserved.first/ZeroMinusOnePlusTwoPlusCorrected.first << "]" << endl;


   

  cout << "====== MINUIT DIAGNOSTICS ======" << endl;
  cout << "  Error Matrix Status ( 3 = full/accurate ):  " << results.eMatrixStatus() << endl;
  cout << "  Last Command Status ( 0 = normal ):  " << results.lastMinuitCommandStatus() << endl;
  cout << "  Estimated Distance to Minimum:  " << results.estDistToMinimum() << endl;
}


vector< string >
namesFromAmpInfo( const vector< AmplitudeInfo* >& ampInfoVec ){

  vector< string > result;

  for( auto itr = ampInfoVec.begin(); itr != ampInfoVec.end(); ++itr ){

    result.push_back( (*itr)->fullName() );
  }

  return result;
}
