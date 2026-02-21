#!/bin/csh

echo $HOSTNAME


##The four diamond orientations are added up in the config file


setenv argEnv ${1}
setenv argFitName ${2}
setenv argFitType ${3}
setenv argParamSeeds ${4}
setenv argCodeDir ${5}



setenv argNumRand ${6}
setenv argPrecision ${7}
setenv argDataDir ${8}
setenv argMCphspDir ${9}
setenv argFitResDir ${10}
setenv argLinksDir ${11}
setenv argRunRange ${12}
setenv argtLow ${13}
setenv argtHigh ${14}
setenv argM3PiLow ${15}
setenv argM3PiHigh ${16}
setenv argM2PiLow ${17}
setenv argM2PiHigh ${18}





#launch environment
source ~/.cshrc 
source $argEnv



# copy scripts to working directory
cp ${argCodeDir}/process_allcuts.py .
cp ${argCodeDir}/template_rhoIsoAmp.cfg .


# copy parameter seeds for likelihood scan or bootstrap
#cp $argParamSeeds seed.txt
#sed -i 's/parameter dsratio/#parameter dsratio/g' seed.txt

ls -lhtr


# check GPU card is active
#nvidia-smi



echo "Passed to the inner 'process_allcut.py' script:"
echo " ${argNumRand}  ${argPrecision}  ${argDataDir}  ${argMCphspDir}  ${argFitResDir}  ${argLinksDir}  ${argRunRange}  ${argtLow} ${argtHigh}  ${argM3PiLow}  ${argM3PiHigh}  ${argM2PiLow}  ${argM2PiHigh} "


# run fit
python process_allcuts.py $argNumRand  $argPrecision  $argDataDir  $argMCphspDir  $argFitResDir  $argLinksDir  $argRunRange  $argtLow $argtHigh  $argM3PiLow  $argM3PiHigh  $argM2PiLow  $argM2PiHigh


# cleanup auxiliary scripts
rm template_rhoIsoAmp.cfg


ls -lhtr



