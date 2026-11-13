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



# launch environment
source /home/gluex-software/gluex_top/gluex_env_boot.csh $BUILD_SCRIPTS 
source /home/gluex-software/gluex_top/gluex_env_local.csh $argEnv



# check executables and GPU cards availability 
if ( $argGPU > 0 ) then
    nvidia-smi
endif
if ( $argGPU == 1 ) then
    which fit
else
    which fitMPI
endif





# copy scripts to working directory
cp ${argCodeDir}/prepare_allcuts.py .
cp ${argCodeDir}/fulltemplate_3PiDelta.cfg .

# copy parameter seeds for likelihood scan or bootstrap
#cp $argParamSeeds seed.txt
#sed -i 's/parameter dsratio/#parameter dsratio/g' seed.txt

ls -lhtr



# Define subrange 
set subrange = `printf "tbin%d_m3pi_%.3f_%.3f" ${argtbin} ${argM3PiLow} ${argM3PiHigh}`

echo "Passed to the inner 'prepare_allcuts.py' script:"
echo " ${argDataDirA}  ${argMCphspDirA}  ${argFitResDir}  ${argLinksDir}  ${argRunRangeA} ${argtbin} ${argtLow} ${argtHigh}  ${argM3PiLow}  ${argM3PiHigh}  "


# prepare input trees for Spring-2017, Spring-2018 and Fall-2018 (use A,B,C for the beamtimes)
python prepare_allcuts.py $argDataDirA  $argMCphspDirA  $argFitResDir  $argLinksDir  $argRunRangeA $argtbin $argtLow $argtHigh  $argM3PiLow  $argM3PiHigh  
python prepare_allcuts.py $argDataDirB  $argMCphspDirB  $argFitResDir  $argLinksDir  $argRunRangeB $argtbin $argtLow $argtHigh  $argM3PiLow  $argM3PiHigh  
python prepare_allcuts.py $argDataDirC  $argMCphspDirC  $argFitResDir  $argLinksDir  $argRunRangeC $argtbin $argtLow $argtHigh  $argM3PiLow  $argM3PiHigh  

    
# prepare a config file using the template
set cfgTemplate = "fulltemplate_3PiDelta.cfg"
set cfgfile = "3PiDelta__${subrange}.cfg"
sed 's|$PATH|'${argLinksDir}'|g; s|$SUBRANGE|'${subrange}'|g' "${cfgTemplate}" > "${cfgfile}"
                

# Main command launching fit. Switch here between CPU and GPU commands
# use -s ${param_seeds} for a specific seeding 
# if run fitMPI interactively, add "-np 16" after "mpirun" 

if ( $argGPU == 1 ) then
    set command = "fit -c ${cfgfile} -m 100000"
else
    set command = "mpirun fitMPI -c ${cfgfile} -m 100000"
endif




if ( `awk 'BEGIN{print ('"$argPrecision"' > 0)}'` ) then
    set prec_str = `printf "%.1e" ${argPrecision}`
    set command = "${command} -e ${prec_str}"
endif

if ( $argNumRand > 0 ) then
    set command = "${command} -r ${argNumRand}"
endif

	    
        #########Add scanning by a certain parameter###
        ##Check if parScan is set in config file
        #parScan = False
        #parToScan = ""
        #if "parScan dsratio" in filedata:
        #        parScan = True
        #        parToScan = "dsratio"
        #if "parScan dphase" in filedata:
        #        parScan = True
        #        parToScan = "dphase"
        #if parScan:
        #        print("found parScan")
        #        if numRand == 0: # full parameter scan
        #                command += " -p %s" % parToScan
        #        else: # single parameter scan point
        #                command +=" -p %s__%d" % (parToScan,numRand)
        #elif numRand > 0: # randomize parameters
        #        command +=" -r %d" % numRand
        ##############################################


echo "  ${command}"
#exit        
eval $command


# cleanup auxiliary config script and failed fit outputs
rm -f ${cfgTemplate}
grep -l 'nan' ./fit_3PiDelta_${subrange}*.fit | xargs -r rm  



