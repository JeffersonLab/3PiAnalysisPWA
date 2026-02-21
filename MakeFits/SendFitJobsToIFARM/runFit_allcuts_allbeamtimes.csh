#!/bin/csh

echo $HOSTNAME

setenv MyEnv $1
setenv MyFit $2 
setenv MyPeriod $3
setenv MyFitName $4
setenv MyFitType $5
setenv MyNumRand $6
setenv MyPrecision $7
setenv MyDataInSampleDirSp17 $8
setenv MyDataInSampleDirSp18 $9
setenv MyDataInSampleDirF18 ${10}
setenv MyDataInPhasespaceDirSp17 ${11}
setenv MyDataInPhasespaceDirSp18 ${12}
setenv MyDataInPhasespaceDirF18 ${13}
setenv MyDataOutDir ${14}
setenv MyCodeDir ${15}
setenv MyChi2NDFHigh ${16}
setenv MyMassPiPiLow ${17}
setenv MyMassPiPiHigh ${18}
setenv MyMassLow ${19}
setenv MyMassHigh ${20}
setenv MytLow ${21}
setenv MytHigh ${22}
setenv MyParamSeeds ${23}

echo $MyEnv
echo $MyFit
echo $MyPeriod
echo $MyFitName
echo $MyFitType
echo $MyNumRand
echo $MyPrecision
echo $MyDataInSampleDirSp17
echo $MyDataInSampleDirSp18
echo $MyDataInSampleDirF18
echo $MyDataInPhasespaceDirSp17
echo $MyDataInPhasespaceDirSp18
echo $MyDataInPhasespaceDirF18
echo $MyDataOutDir
echo $MyCodeDir
echo $MyChi2NDFHigh
echo $MyMassPiPiLow
echo $MyMassPiPiHigh
echo $MyMassLow
echo $MyMassHigh
echo $MytLow
echo $MytHigh
echo $MyParamSeeds

source ~/.cshrc 
source $MyEnv

pwd
ls -al
setenv MyCutTreeDir Cuts_chi2ndf_${MyChi2NDFHigh}_mpipi_${MyMassPiPiLow}_${MyMassPiPiHigh}_mpipp_${MyMassLow}_${MyMassHigh}

if ( ! -d "${MyDataInSampleDirSp17}/${MyCutTreeDir}") then
    mkdir ${MyDataInSampleDirSp17}/${MyCutTreeDir}
endif
ls -lthr ${MyDataInSampleDirSp17}/${MyCutTreeDir}

if ( ! -d "${MyDataInPhasespaceDirSp17}/${MyCutTreeDir}") then
    mkdir ${MyDataInPhasespaceDirSp17}/${MyCutTreeDir}
endif

if ( ! -d "${MyDataInSampleDirSp18}/${MyCutTreeDir}") then
    mkdir ${MyDataInSampleDirSp18}/${MyCutTreeDir}
endif
ls -lthr ${MyDataInSampleDirSp18}/${MyCutTreeDir}

if ( ! -d "${MyDataInPhasespaceDirSp18}/${MyCutTreeDir}") then
    mkdir ${MyDataInPhasespaceDirSp18}/${MyCutTreeDir}
endif

if ( ! -d "${MyDataInSampleDirF18}/${MyCutTreeDir}") then
    mkdir ${MyDataInSampleDirF18}/${MyCutTreeDir}
endif
ls -lthr ${MyDataInSampleDirF18}/${MyCutTreeDir}

if ( ! -d "${MyDataInPhasespaceDirF18}/${MyCutTreeDir}") then
    mkdir ${MyDataInPhasespaceDirF18}/${MyCutTreeDir}
endif

# copy scripts to working directory
cp $MyCodeDir/runOrientation_allcuts_allbeamtimes.py ./
# if ( ("$MyFitName" == "PARA_0") or ("$MyFitName" == "PERP_90") or ("$MyFitName" == "PARA_135") or ("$MyFitName" == "PERP_45"))  then
#     cp $MyCodeDir/template_${MyFit}.cfg ./
if ("$MyFitName" == "ALL") then
    cp $MyCodeDir/template_${MyFit}_all_allbeamtimes_polynomialMoments.cfg ./ #coherent
else if ("$MyFitName" == "AMO") then 
    cp $MyCodeDir/template_${MyFit}_all_allbeamtimes_polynomialMoments_amo.cfg ./
else if ("$MyFitName" == "comb_45_135") then
    cp $MyCodeDir/template_${MyFit}_45_135_allbeamtimes_polynomialMoments.cfg ./
else if ("$MyFitName" == "comb_0_90") then
    cp $MyCodeDir/template_${MyFit}_0_90_allbeamtimes_polynomialMoments.cfg ./
else if ("$MyFitName" == "PARA_0") then
    cp $MyCodeDir/template_${MyFit}_0_allbeamtimes_polynomialMoments.cfg ./
else if ("$MyFitName" == "PERP_90") then
    cp $MyCodeDir/template_${MyFit}_90_allbeamtimes_polynomialMoments.cfg ./
else if ("$MyFitName" == "PERP_45") then
    cp $MyCodeDir/template_${MyFit}_45_allbeamtimes_polynomialMoments.cfg ./
else if ("$MyFitName" == "PARA_135") then
    cp $MyCodeDir/template_${MyFit}_135_allbeamtimes_polynomialMoments.cfg ./
else 
    cp $MyCodeDir/template_${MyFit}_allbeamtimes.cfg ./
endif
#cp  $MyCodeDir/../fit_helicity.cfg fit_omegapi_amplitude_template.cfg

# copy parameter seeds for likelihood scan or bootstrap
#cp $MyParamSeeds seed.txt
#sed -i 's/parameter dsratio/#parameter dsratio/g' seed.txt

ls -al

# check GPU card is active
nvidia-smi

# run fit
#python3.6 runOrientation.py $MyFitName $MyAngle $MyPolVal $MyNumRand $MyPrecision $MyDataInSample $MyDataInSampleDir $MyDataInPhasespace $MyDataInPhasespaceDir $MyPeriod $MyMassLow $MyMassHigh $MytLow $MytHigh $MyBaryonRes $MyDataOutDir
python3.6 /work/halld/home/afzal/scripts/runOrientation_allcuts_allbeamtimes.py $MyFitName $MyNumRand $MyPrecision $MyDataInSampleDirSp17 $MyDataInSampleDirSp18 $MyDataInSampleDirF18 $MyDataInPhasespaceDirSp17 $MyDataInPhasespaceDirSp18 $MyDataInPhasespaceDirF18 $MyPeriod $MyChi2NDFHigh $MyMassPiPiLow $MyMassPiPiHigh $MyMassLow $MyMassHigh $MytLow $MytHigh $MyDataOutDir $MyCutTreeDir

ls -al

# move fit results to output directory
mv tbin_*.cfg $MyDataOutDir
mv tbin_*.fit $MyDataOutDir
mv tbin_*.ni $MyDataOutDir
#mv omegapi_ensemble.txt $MyDataOutDir
mv deltapp_fitPars.txt $MyDataOutDir
mv piDelta_plot.root $MyDataOutDir
#mv param_seeds*.txt $MyDataOutDir

# cleanup local running directory
#cd ../
#rm -rf Mass_${MyMassLow}_${MyMassHigh}
