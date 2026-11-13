#!/bin/csh

echo $HOSTNAME


##The four diamond orientations and three beamtimes are added up in the config file

setenv argEnv ${1}
setenv argGPU ${2}
setenv argCodeDir ${3}
setenv argNumRand ${4}
setenv argPrecision ${5}


setenv argDataDirA ${6}
setenv argMCphspDirA ${7}
setenv argRunRangeA ${8}

setenv argDataDirB ${9}
setenv argMCphspDirB ${10}
setenv argRunRangeB ${11}

setenv argDataDirC ${12}
setenv argMCphspDirC ${13}
setenv argRunRangeC ${14}


setenv argFitResDir ${15}
setenv argLinksDir ${16}

setenv argtbin ${17}
setenv argtLow ${18}
setenv argtHigh ${19}
setenv argM3PiLow ${20}
setenv argM3PiHigh ${21}




#launch environment
source /home/gluex-software/gluex_top/gluex_env_boot.csh $BUILD_SCRIPTS
source /home/gluex-software/gluex_top/gluex_env_local.csh $argEnv


which isops_plotter

ls -lhtr *.fit *.ni



# Define subrange 
set subrange = `printf "tbin%d_m3pi_%.3f_%.3f" ${argtbin} ${argM3PiLow} ${argM3PiHigh}`
echo "Running plotter for the ${subrange} bin:"


#Call plotter for the best converged fit result
#isops_plotter fit_3PiDelta_${subrange}.fit -o isops_histos_${subrange}.root -t isops_fitpars_${subrange}.txt
isops_plotter fit_3PiDelta_${subrange}.fit -t isops_fitpars_${subrange}.txt



# Move root and txt files one folder up
mv isops_histos*.root  ../.
mv isops_fitpars*.txt  ../.






