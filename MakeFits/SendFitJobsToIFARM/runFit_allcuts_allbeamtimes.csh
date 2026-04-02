#!/bin/csh

echo $HOSTNAME


##The four diamond orientations and three beamtimes are added up in the config file

setenv argEnv ${1}
setenv argCodeDir ${2}
setenv argParamSeeds ${3}
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
setenv argtLow ${17}
setenv argtHigh ${18}
setenv argM3PiLow ${19}
setenv argM3PiHigh ${20}
setenv argM2PiLow ${21}
setenv argM2PiHigh ${22}





#launch environment
source ~/.cshrc 
source $argEnv

which fitMPI


# copy scripts to working directory
cp ${argCodeDir}/prepare_allcuts.py .
cp ${argCodeDir}/template_rhoIso_f2Iso_amp.cfg .

# copy parameter seeds for likelihood scan or bootstrap
#cp $argParamSeeds seed.txt
#sed -i 's/parameter dsratio/#parameter dsratio/g' seed.txt

ls -lhtr


# check GPU card is active
#nvidia-smi


# Define subrange 
set subrange = `printf "tbin0_m3pi_%.2f_%.2f" ${argM3PiLow} ${argM3PiHigh}`

echo "Passed to the inner 'prepare_allcuts.py' script:"
echo " ${argDataDirA}  ${argMCphspDirA}  ${argFitResDir}  ${argLinksDir}  ${argRunRangeA}  ${argtLow} ${argtHigh}  ${argM3PiLow}  ${argM3PiHigh}  ${argM2PiLow}  ${argM2PiHigh} "


# prepare input trees for Spring-2017, Spring-2018 and Fall-2018 (use A,B,C for the beamtimes)
python prepare_allcuts.py $argDataDirA  $argMCphspDirA  $argFitResDir  $argLinksDir  $argRunRangeA  $argtLow $argtHigh  $argM3PiLow  $argM3PiHigh  $argM2PiLow  $argM2PiHigh
python prepare_allcuts.py $argDataDirB  $argMCphspDirB  $argFitResDir  $argLinksDir  $argRunRangeB  $argtLow $argtHigh  $argM3PiLow  $argM3PiHigh  $argM2PiLow  $argM2PiHigh
#python prepare_allcuts.py $argDataDirC  $argMCphspDirC  $argFitResDir  $argLinksDir  $argRunRangeC  $argtLow $argtHigh  $argM3PiLow  $argM3PiHigh  $argM2PiLow  $argM2PiHigh

    
# prepare a config file using the template
set cfgTemplate = "template_rhoIso_f2Iso_amp.cfg"
set cfgfile = "rhoIso_f2Iso_amp__${subrange}.cfg"
sed 's|$PATH|'${argLinksDir}'|g; s|$SUBRANGE|'${subrange}'|g' "${cfgTemplate}" > "${cfgfile}"
                

#Main command launching fit
set command = "mpirun --mca btl_openib_allow_ib 1 fitMPI -c ${cfgfile} -m 100000"

# add "-np 16" if running interactively on multiple processors
#set command = "fit -c ${cfgfile} -m 1000000 -s ${param_seeds}"


if ( `echo "$argPrecision > 0" | bc` ) then
    set prec_str = `printf "%.1e" ${argPrecision}`
    set command = "${command} -e ${prec_str}"
endif

if ( `echo "$argNumRand > 0" | bc` ) then
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

        
#Call plotter for succesfull fit outputs
grep -l 'nan' ./fit_rhof2IsoAmp_${subrange}*.fit | xargs rm  
isops_plotter fit_rhof2IsoAmp_${subrange}.fit -o isops_histos_${subrange}.root -t isops_fitpars_${subrange}.txt



# cleanup auxiliary scripts and move root files
rm template_rhoIso_f2Iso_amp.cfg
mv isops_histos*.root  ../.


ls -lhtr



