#!/usr/bin/env python

import sys
import os
import subprocess
import math
import pwd
from optparse import OptionParser

########################################################## MAIN ##########################################################
def main(argv):

    # SLURM INFO (see options at https://scicomp.jlab.org/scicomp/slurmJob/slurmInfo)
    PARTITION  = "gpu" #"gpu"
    GPUTYPE    = "A100" #T4
    TIMELIMIT  = "120:00:00"   # Max walltime
    nGPUs      = 1            # Number of GPU cards used by MPI
    nCPUs      = 8           # Number of CPUs used by MPI (only if nGPUs=0)

    MyFit = "deltasdmefit"                 #"deltaPlusPlus_b1Minus" gpu_WithbgrhosdmesnotfixedBWflexNewf2sDiffMassCuts
    MyFitType = "gpu_newdata_MomentsSPDWavesJcodespeedA100trigbit_withTOFchargedRFbunchcorr_hel" # writeConfigLoop.py used to define waveset gpu_1pb1free1mpiece10"
    MyNumRand = 40                      # set to 0 for likelihood scan 
    MyPrecision = "1e-13"               # set to negative for MINUIT determined precision
    #MyPrecision = "1e-13"               # set to negative for MINUIT determined precision

    # initial parameter seeds from previous fit: used for likelihood scan or bootstrap
    MyParamSeeds = "/home/jrsteven/volatile/ampToolsFits/deltaPlusPlus_b1Minus/allPeriods/PARA_0/gpu_1pb11mpiece6_data/Mass_1.100_1.300/t_0.15_0.30/param_seeds.txt"

    # Label for data (_data) or MC version (_ver03.1_mc)
    #MyDataInSample = ""

    # SET OPTIONS ON COMMAND LINE
    parser = OptionParser(usage = "submit.py --channel neutralb1 --fittype gpu_1p1miso")
    parser.add_option("-c","--channel", dest="channel", help="channel name")
    parser.add_option("-t","--fittype", dest="fittype", help="fit type")
    parser.add_option("-n","--nrand", dest="nrand", help="# randomized")
    #parser.add_option("-d","--data", dest="data", help="data label")
        
    (options, args) = parser.parse_args(sys.argv)
    if options.channel:
        MyFit = options.channel
    if options.fittype:
        MyFitType = options.fittype
    if options.nrand:
        MyNumRand = int(options.nrand)
    #if options.data:
        #MyDataInSample = options.data

    # JOB INFO
    MyEnv = "/work/halld/home/afzal/setup_gluex_fits_gpu.csh" #"/work/halld/home/afzal/version_fits.xml"
    MyCodeDir = "/work/halld/home/afzal/scripts/"
    SCRIPTFILE = MyCodeDir + "runFit_allcuts_allbeamtimes.csh" # GPU template uses bash
    if nGPUs == 0:
        MyEnv = "/work/halld/home/afzal/setup_gluex_fits_cpu.csh"#"/work/halld/home/afzal/setup_gluex_fits.csh"
        SCRIPTFILE = MyCodeDir + "runFit_allcutsrunFit_allcuts_allbeamtimes.csh"

    # extension name and directory of sample to fit (default: _data)
    dataInSampleDirs = ["/work/halld/home/afzal/data/Spring17-01-ver60/batch04","/work/halld/home/afzal/data/Spring18-01-ver15/batch04","/work/halld/home/afzal/data/Fall18-08-ver15/batch04"]
    # MyDataInSampleDir = "/work/halld/home/afzal/data/Spring17-01-ver60/batch04"
    #MyDataInSampleDir = "/work/halld/home/afzal/data/Fall18-08-ver15/batch04"

    MyBaryonRes = "1" #specify if baryon res. is used for DataReaderTEM

    # extension name and directory of phasespace sample
    #MyDataInPhasespace = "" 
    # MyDataInPhasespaceDir = "/work/halld/home/afzal/MC/signalMC/2018-01-ver15-batch01"
    dataInPhasespaceDirs = ["/work/halld/home/afzal/MC/2017-01-ver60-dalitz-100mio_withtslope_trigbit_withTOFchargedRFbunchcorr","/work/halld/home/afzal/MC/2018-01-ver15-dalitz-100mio_withtslope_trigbit_withTOFchargedRFbunchcorr","/work/halld/home/afzal/MC/2018-08-ver15-dalitz-100mio_withtslope_trigbit_withTOFchargedRFbunchcorr"]
    # MyDataInPhasespaceDir = "/work/halld/home/afzal/MC/2017-01-ver60-dalitz-30mio_withtslope"
    #MyDataInPhasespaceDir = "/work/halld/home/afzal/MC/2018-08-ver15-dalitz-100mio_withtslope"

    MyOutDir = "/volatile/halld/home/" + pwd.getpwuid( os.getuid() )[0]

    MyTemplate = open("slurm_template.txt","r")
    MyTemplateData = MyTemplate.read() 
    MyTemplate.close()

    # Specifc run period YYYY_MM or allPeriods
    periods = ["AllGlueXI"] #"allPeriods" #"2017_01" "2018_01" "2018_08"

    #pol. angles for the three beamtimes
    angle0 = [1.8, 4.1, 3.3] 
    angle45 = [47.9, 48.5, 48.3]
    angle90 = [94.5, 94.2, 92.9]
    angle135 = [138.4, 137.6, 137.9]

    #polvalues for the three beamtimes
    polval0 = [0.3537, 0.3458, 0.3563]
    polval45 = [0.3484, 0.3416, 0.3403]
    polval90 = [0.3472, 0.3448, 0.3430]
    polval135 = [0.3512, 0.3597, 0.3523]

    # Specific polarization orientation e.g. PARA_0 or ALL
    # names = ["PARA_0","PERP_45","PERP_90","PARA_135"]
    # angles = [0.0,45.0,90.0,135.0]
    # polvalues = [0.3519,0.3374,0.3303,0.3375]
    names = ["ALL"]#"PARA_0","PERP_45","PERP_90","PARA_135","comb_0_90"  "PARA_0","PERP_45","PERP_90","PARA_135","comb_0_90","comb_45_135", "ALL"
    #angles = [0.0]#,45.0,90.0,135.0, 0.0, 0.0,0.0
    #polvalues = [0.3519]#,0.3519,0.3416,0.3448,0.3597,0.3519,0.3519
        
    # Or put list of mass ranges in by hand
    chi2ndfhigh =  [8.7]    #8.7

    # masspipilow =  [1.100]
    # masspipihigh = [2.450]
    masspipilow =  [1.100]
    masspipihigh = [2.450]

    # masslow = [1.100,1.100,1.100,1.100]
    # masshigh = [1.315,1.330,1.370,1.400]
    masslow = [1.100]
    masshigh = [1.350]

    #masslow = [1.187]
    #masshigh = [1.280]

    # Specify list of -t ranges
    # tLow =  [0.025, 0.075,   0.1, 0.125, 0.150, 0.175,  0.2, 0.24, 0.28, 0.34, 0.40, 0.46, 0.54, 0.64, 0.85,1.05] 
    # tHigh = [0.075, 0.1, 0.125, 0.150, 0.175,   0.2, 0.24, 0.28, 0.34, 0.40, 0.46, 0.54, 0.64, 0.85,1.05, 1.4]
    tLow =  [0.150, 0.54] 
    tHigh = [0.175, 0.64] 

    # LOOP OVER THE 3 DIFFERENT BEAMTIMES
    for MyPeriod  in (periods):

        # LOOP OVER ANGLES/ORIENTATIONS TO FIT
        for MyFitName in (names):

            # LOOP OVER MASS RANGES TO FIT
            for MyChi2NDFHigh in (chi2ndfhigh):

                # LOOP OVER MASS RANGES TO FIT
                for MyMassPiPiLow,MyMassPiPiHigh in zip(masspipilow,masspipihigh):

                    # LOOP OVER MASS RANGES TO FIT
                    for MyMassLow,MyMassHigh in zip(masslow,masshigh):

                        # LOOP OVER T RANGES TO FIT
                        for MytLow,MytHigh in zip(tLow,tHigh):

                            # PREPARE DIRECTORIES AND CREATE THOSE THAT DON'T EXIST
                            MyRunningDir = "%s/TMPDIR/ampToolsFits/%s/%s/%s/%s/Cuts_chi2ndf_%0.1f_MPiPi_%0.3f_%0.3f_MPPiP_%0.3f_%0.3f/t_%0.3f_%0.3f/" % (MyOutDir,MyFit,MyPeriod,MyFitName,MyFitType,MyChi2NDFHigh,MyMassPiPiLow,MyMassPiPiHigh,MyMassLow,MyMassHigh,MytLow,MytHigh)
                            MyDataOutDir = "%s/ampToolsFits/%s/%s/%s/%s/Cuts_chi2ndf_%0.1f_MPiPi_%0.3f_%0.3f_MPPiP_%0.3f_%0.3f/t_%0.3f_%0.3f/" % (MyOutDir,MyFit,MyPeriod,MyFitName,MyFitType,MyChi2NDFHigh,MyMassPiPiLow,MyMassPiPiHigh,MyMassLow,MyMassHigh,MytLow,MytHigh)
                            
                            MyLogDir = MyDataOutDir + "/log/"
                
                            if not os.path.exists(MyRunningDir):
                                os.makedirs(MyRunningDir)
                            if not os.path.exists(MyDataOutDir):
                                os.makedirs(MyDataOutDir)
                            if not os.path.exists(MyLogDir):
                                os.makedirs(MyLogDir)	

                            print(MyFit,MyFitType,MyPeriod,MyChi2NDFHigh,MyMassPiPiLow,MyMassPiPiHigh,MyMassLow,MyMassHigh,MytLow,MytHigh)

                            # PREPARE NAMES
                            STUBNAME = MyFitName + "_Chi2NDF_%0.1f_MPiPi_%0.3f_%0.3f_M_%0.3f_%0.3f_t_%0.2f_%0.2f" % (MyChi2NDFHigh,MyMassPiPiLow,MyMassPiPiHigh,MyMassLow, MyMassHigh, MytLow, MytHigh)
                            JOBNAME = STUBNAME

                            # SCRIPT COMMAND
                            SCRIPT_COMMAND = SCRIPTFILE + " " + MyEnv  + " " + MyFit + " " + MyPeriod + " " + MyFitName + " " + MyFitType + " %d" % MyNumRand + " " + MyPrecision + " " + dataInSampleDirs[0] + " " + dataInSampleDirs[1] + " " + dataInSampleDirs[2]  + " " + dataInPhasespaceDirs[0] + " " + dataInPhasespaceDirs[1] + " " + dataInPhasespaceDirs[2] + " " + MyDataOutDir + " " + MyCodeDir + " %0.1f" % (MyChi2NDFHigh) +" %0.3f %0.3f" % (MyMassPiPiLow, MyMassPiPiHigh) +" %0.3f %0.3f" % (MyMassLow, MyMassHigh) + " %0.3f %0.3f " % (MytLow, MytHigh) + MyParamSeeds + " \n"

                            slurmOut = open("tempSlurm.txt",'w')
                            slurmOut.write("#!/bin/sh \n")
                            slurmOut.write("#SBATCH --nodes=1 \n")
                            slurmOut.write("#SBATCH --partition=%s \n" % PARTITION) 
                            # different requirements for GPU and CPU fits
                            if nGPUs > 0:
                                slurmOut.write("#SBATCH --gres=gpu:%s:%d \n" % (GPUTYPE,nGPUs))
                                slurmOut.write("#SBATCH --cpus-per-task=1 \n")
                                slurmOut.write("#SBATCH --ntasks=%d \n" % (nGPUs+1))
                                slurmOut.write("#SBATCH --mem=%dGB \n" % (nGPUs*40)) # multiplied by nGPUs in slurm? *20
                                slurmOut.write("#SBATCH --constraint=el7 \n") #centos79 general
                            else:
                                slurmOut.write("#SBATCH --cpus-per-task=1 \n")
                                slurmOut.write("#SBATCH --ntasks=%d \n" % (nCPUs+1))
                                slurmOut.write("#SBATCH --mem=%dGB \n" % (nCPUs*2)) #*2
                            slurmOut.write("#SBATCH --ntasks-per-core=1 \n")
                            slurmOut.write("#SBATCH --threads-per-core=1 \n")
                            slurmOut.write("#SBATCH --time=%s \n" % TIMELIMIT)
                                            
                            slurmOut.write("#SBATCH --chdir=%s \n" % MyRunningDir)
                            slurmOut.write("#SBATCH --error=%s/log.err \n" % MyLogDir)
                            slurmOut.write("#SBATCH --output=%s/log.out \n" % MyLogDir)
                            slurmOut.write("#SBATCH --job-name=%s \n" % JOBNAME)
                            slurmOut.write(SCRIPT_COMMAND)
                            slurmOut.close()
                            subprocess.call(["sbatch", "tempSlurm.txt"])
                                            
                            # remove temporary submission file
                            os.remove("tempSlurm.txt")
        

if __name__ == "__main__":
   main(sys.argv[1:])
   
