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
    PARTITION  = "ifarm" #"gpu"
    GPUTYPE    = "A100" #T4
    TIMELIMIT  = "24:00:00"   # Max walltime
    RAMLIMIT   = 33          # Requested memory in GB
    nGPUs      = 0            # Number of GPU cards used by MPI
    nCPUs      = 32           # Number of CPUs used by MPI (only if nGPUs=0)

    
    # FIT INFO
    argFitName = "3PiBinnedFit"              
    argFitType = "3PiFit_rhoBWIso_f2BWIso" 
    argNumRand = 0                      # set to 0 for likelihood scan 
    argPrecision = "1.e-13"             # set to negative for MINUIT determined precision    
    argParamSeeds = "nofile.txt" #/volatile/... # initial parameter seeds from previous fit: used for likelihood scan or bootstrap


    # SET OPTIONS ON COMMAND LINE
    parser = OptionParser(usage = "submit_allcuts.py --channel 3pi --fittype rhoIsobar")
    parser.add_option("-c","--channel", dest="channel", help="channel name")
    parser.add_option("-t","--fittype", dest="fittype", help="fit type")
    parser.add_option("-n","--nrand", dest="nrand", help="# randomized")
    #parser.add_option("-d","--data", dest="data", help="data label")
        
    (options, args) = parser.parse_args(sys.argv)
    if options.channel:
        argFitName = options.channel
    if options.fittype:
        argFitType = options.fittype
    if options.nrand:
        argNumRand = int(options.nrand)
    #if options.data:
        #argDataInSample = options.data


    # JOB INFO
    argEnv = "/work/halld/home/ilbelov/setup.csh" #"/work/halld/home/afzal/version_fits.xml"
    argPeriod = "30274_31057"
    argInputDataDir = "/work/halld/home/ilbelov/DataSpring2017"
    argInputMCphspDir = "/work/halld/home/ilbelov/MCphspSpring2017/mask010"
    argCodeDir = "/work/halld/home/ilbelov/MakeFits/SendFitJobsToIFARM"
    volatileDir = "/volatile/halld/home/" + pwd.getpwuid( os.getuid() )[0]
    SCRIPTFILE = argCodeDir + "/runFit_allcuts.csh" # GPU template uses bash

    if nGPUs > 0:
        argEnv = "/work/halld/home/ilbelov/setup_gluex_fits_gpu.csh" 
        SCRIPTFILE = argCodeDir + "/runFit_allcuts.csh"


    #use template with slurm commands if needed    
    template = open("slurm_template.txt","r")
    templateData = template.read() 
    template.close()

    
    # Specify list of ranges
    # tLow =  [0.025, 0.075,   0.1, 0.125, 0.150, 0.175,  0.2, 0.24, 0.28, 0.34, 0.40, 0.46, 0.54, 0.64, 0.85,1.05] 
    # tHigh = [0.075, 0.1, 0.125, 0.150, 0.175,   0.2, 0.24, 0.28, 0.34, 0.40, 0.46, 0.54, 0.64, 0.85,1.05, 1.4]

    t_low =  [0.] 
    t_high = [0.25] 

    mass_3pi_low  = [1.00,1.05,1.10,1.15,1.20,1.25,1.30,1.35,1.40,1.45,1.50,1.55,1.60,1.65,1.70,1.75,1.80]
    mass_3pi_high = [1.05,1.10,1.15,1.20,1.25,1.30,1.35,1.40,1.45,1.50,1.55,1.60,1.65,1.70,1.75,1.80,1.85]
    
    mass_2pi_low =  [0.0]
    mass_2pi_high = [2.0]



    
    # LOOP OVER T RANGES TO FIT
    for argtLow,argtHigh in zip(t_low,t_high):

        # LOOP OVER 3PI MASS RANGES TO FIT
        for argMass3PiLow,argMass3PiHigh in zip(mass_3pi_low,mass_3pi_high):

            # LOOP OVER 2PI MASS RANGES TO FIT
            for argMass2PiLow,argMass2PiHigh in zip(mass_2pi_low,mass_2pi_high):

                # PREPARE DIRECTORIES AND CREATE THOSE THAT DON'T EXIST
                subrange = "tbin0_m3pi_%.2f_%.2f" % (argMass3PiLow,argMass3PiHigh)

                argRunningDir = "%s/ampToolsFits/%s/%s" % (volatileDir,argFitName,subrange)
                argInputTreesDir = "%s/TreesForAmpTools/%s" % (volatileDir,argFitName)                            
                thisLogDir = argRunningDir + "/log" 
                
                if not os.path.exists(argRunningDir):
                    os.makedirs(argRunningDir)
                if not os.path.exists(argInputTreesDir):
                    os.makedirs(argInputTreesDir)
                if not os.path.exists(thisLogDir):
                    os.makedirs(thisLogDir)	

                            
                # PREPARE NAMES
                STUBNAME = argFitName + "_%s" % (subrange)
                JOBNAME = STUBNAME

                # SCRIPT COMMAND
                SCRIPT_COMMAND = SCRIPTFILE + " %s %s %s %s %s" % (argEnv,argFitName,argFitType,argParamSeeds,argCodeDir) + " %d %s" % (argNumRand,argPrecision) + " %s %s %s %s %s" % (argInputDataDir,argInputMCphspDir,argInputTreesDir,argRunningDir,argPeriod) + " %0.2f %0.2f %0.2f %0.2f %0.2f %0.2f" % (argtLow,argtHigh,argMass3PiLow,argMass3PiHigh,argMass2PiLow,argMass2PiHigh) + " \n"


                slurmOut = open("tempSlurm.txt",'w')
                slurmOut.write("#!/bin/sh \n")
                slurmOut.write("#SBATCH --nodes=1 \n")
                slurmOut.write("#SBATCH --partition=%s \n" % PARTITION) 
                
                # different requirements for GPU and CPU fits
                if nGPUs > 0:
                    slurmOut.write("#SBATCH --gres=gpu:%s:%d \n" % (GPUTYPE,nGPUs))
                    lurmOut.write("#SBATCH --cpus-per-task=1 \n")
                    slurmOut.write("#SBATCH --ntasks=%d \n" % (nGPUs+1))
                    slurmOut.write("#SBATCH --mem=%dGB \n" % (nGPUs*40)) # 
                    slurmOut.write("#SBATCH --constraint=el9 \n") #centos79 general
                else:
                    slurmOut.write("#SBATCH --cpus-per-task=1 \n")
                    slurmOut.write("#SBATCH --ntasks=%d \n" % (nCPUs+1))
                    slurmOut.write("#SBATCH --mem=%dGB \n" % RAMLIMIT) 

                slurmOut.write("#SBATCH --ntasks-per-core=1 \n")
                slurmOut.write("#SBATCH --threads-per-core=1 \n")
                slurmOut.write("#SBATCH --time=%s \n" % TIMELIMIT)
                                            
                slurmOut.write("#SBATCH --chdir=%s \n" % argRunningDir)
                slurmOut.write("#SBATCH --error=%s/log.err \n" % thisLogDir)
                slurmOut.write("#SBATCH --output=%s/log.out \n" % thisLogDir)
                slurmOut.write("#SBATCH --job-name=%s \n" % JOBNAME)
                slurmOut.write("export OMPI_MCA_orte_tmpdir_base=%s/TMPDIR/orte_init \n" % volatileDir)                
                slurmOut.write(SCRIPT_COMMAND)
                slurmOut.close()
                subprocess.call(["sbatch", "tempSlurm.txt"])
                                            
                # remove temporary submission file
                os.remove("tempSlurm.txt")
        

if __name__ == "__main__":
   main(sys.argv[1:])
   
