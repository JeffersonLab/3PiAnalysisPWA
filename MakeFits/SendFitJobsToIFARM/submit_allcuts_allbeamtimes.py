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
    ACCOUNT    = "afzalf2c_0000" 
    PARTITION  = "cpu" #"fat_cpu" #gpu
    GPUTYPE    = "nvidia_h100" #H100 #A30
    TIMELIMIT  = "48:00:00"  # Max walltime
    RAMLIMIT   = 180         #180 or #40 # Requested memory in GB
    nGPUs      = 0           # Number of GPU cards used 
    nCPUs      = 22          #45 # Number of CPUs used by MPI (only with nGPUs=0)

    
    # FIT INFO
    argFitName = "FloatResMasses_a1_pi2" 
    argFitType = "3PiFullFit"    #"3PiFitBinned25MeV"    # leave this 'type' for all fits using trees with 25MeV binning
    argNumRand = 2                      # set to 0 for likelihood scan 
    argPrecision = "1.e-13"             # set to negative for MINUIT determined precision    
    argParamSeeds = "nofile.txt" #/lustre/... # initial parameter seeds from previous fit: used for likelihood scan or bootstrap
    argGPU = nGPUs
    

    # SET OPTIONS ON COMMAND LINE
    parser = OptionParser(usage = "submit_allcuts.py --channel 3pi --fittype rho_f2_pipi_isobars")
    parser.add_option("-c","--channel", dest="channel", help="channel name")
    parser.add_option("-t","--fittype", dest="fittype", help="fit type")
    parser.add_option("-n","--nrand", dest="nrand", help="# randomized")
    #parser.add_option("-d","--data", dest="data", help="data label")
        
    (options, args) = parser.parse_args(sys.argv)
    if options.channel:
        argFitType = options.channel
    if options.fittype:
        argFitName = options.fittype
    if options.nrand:
        argNumRand = int(options.nrand)
    #if options.data:
        #argDataInSample = options.data


    # JOB INFO
    EXECUTE_SCRIPT = "/home/beloviq2/MakeFits/SendFitJobsToIFARM/execute_gxenv.sh" #should launch CUDA modules as well
    argVersion = "/home/gluex-software/gluex_top/halld_versions/version.xml" #default version 

    argInputDataDirA = "/lustre/beloviq2/3PiDeltaTrees/Spring2017"
    argInputMCphspDirA = "/lustre/beloviq2/3PiDeltaTrees/Spring2017"
    argPeriodA = "30274_31057"

    argInputDataDirB = "/lustre/beloviq2/3PiDeltaTrees/Spring2018"
    argInputMCphspDirB = "/lustre/beloviq2/3PiDeltaTrees/Spring2018"
    argPeriodB = "40856_42559"

    argInputDataDirC = "/lustre/beloviq2/3PiDeltaTrees/Fall2018"
    argInputMCphspDirC = "/lustre/beloviq2/3PiDeltaTrees/Fall2018"
    argPeriodC = "50685_51768"
    
    argCodeDir = "/home/beloviq2/MakeFits/SendFitJobsToIFARM"
    lustreDir = "/lustre/" + pwd.getpwuid( os.getuid() )[0]
    tmpDir = "/tmp/" + pwd.getpwuid( os.getuid() )[0]
    

    #use template with slurm commands if needed    
    template = open("slurm_template.txt","r")
    templateData = template.read() 
    template.close()

    
    # Specify list of ranges 
    tbin = [0] 
    t_low =  [0.00] 
    t_high = [0.25] 


    mass_3pi_low  = [1.200,1.225,1.250,1.275,1.300,1.325,1.350,1.375,1.400,1.425,1.450,1.475,1.500,1.525,1.550,1.575,1.600,1.625,1.650,1.675,1.700,1.725,1.750,1.775,1.800,1.825,0.750,0.775,0.800,0.825,0.850,0.875,0.900,0.925,0.950,0.975,1.000,1.025,1.050,1.075,1.100,1.125,1.150,1.175]
    mass_3pi_high = [1.225,1.250,1.275,1.300,1.325,1.350,1.375,1.400,1.425,1.450,1.475,1.500,1.525,1.550,1.575,1.600,1.625,1.650,1.675,1.700,1.725,1.750,1.775,1.800,1.825,1.850,0.775,0.800,0.825,0.850,0.875,0.900,0.925,0.950,0.975,1.000,1.025,1.050,1.075,1.100,1.125,1.150,1.175,1.200]

    mass_3pi_low   = [0.750]
    mass_3pi_high  = [1.850]
   

    
    # LOOP OVER T RANGES TO FIT
    for argtbin,argtLow,argtHigh in zip(tbin,t_low,t_high):

        # LOOP OVER 3PI MASS RANGES TO FIT
        for argMass3PiLow,argMass3PiHigh in zip(mass_3pi_low,mass_3pi_high):

            
            # PREPARE DIRECTORIES AND CREATE THOSE THAT DON'T EXIST
            subrange = "tbin%d_m3pi_%.3f_%.3f" % (argtbin,argMass3PiLow,argMass3PiHigh)

            argInputTreesDir = "%s/ampToolsTrees/%s" % (lustreDir,argFitType)            
            argRunningDir = "%s/ampToolsFits/%s/%s/%s" % (lustreDir,argFitType,argFitName,subrange)
            thisLogDir = argRunningDir + "/log" 
                
            if not os.path.exists(argRunningDir):
                os.makedirs(argRunningDir)
            if not os.path.exists(argInputTreesDir):
                os.makedirs(argInputTreesDir)
            if not os.path.exists(thisLogDir):
                os.makedirs(thisLogDir)	

                            
            # PREPARE NAMES
            STUBNAME = argFitType + "_%s" % (subrange)
            JOBNAME = STUBNAME

            # SCRIPT COMMANDS
            SCRIPT_COMMAND = EXECUTE_SCRIPT + " %s %d %s %d %s" % (argVersion,argGPU,argCodeDir,argNumRand,argPrecision) + " %s %s %s" % (argInputDataDirA,argInputMCphspDirA,argPeriodA) + " %s %s %s" % (argInputDataDirB,argInputMCphspDirB,argPeriodB) + " %s %s %s" % (argInputDataDirC,argInputMCphspDirC,argPeriodC)  + " %s %s" % (argInputTreesDir,argRunningDir) + " %d %.3f %.3f %.3f %.3f" % (argtbin,argtLow,argtHigh,argMass3PiLow,argMass3PiHigh) + " \n"
            

            slurmOut = open("tempSlurm.txt",'w')
            slurmOut.write("#!/bin/bash \n") #csh
            slurmOut.write("#SBATCH --account=%s \n" % ACCOUNT)
            slurmOut.write("#SBATCH --nodes=1 \n")
            slurmOut.write("#SBATCH --partition=%s \n" % PARTITION) 
                
            # different requirements for fits with GPUs and CPUs 
            if nGPUs == 0:
                slurmOut.write("#SBATCH --ntasks=%d \n" % (nCPUs+1))
                slurmOut.write("#SBATCH --cpus-per-task=1 \n")                          
            elif nGPUs == 1:
                slurmOut.write("#SBATCH --ntasks=1 \n")  
                slurmOut.write("#SBATCH --gpus=1 \n") 
                slurmOut.write("#SBATCH --cpus-per-gpu=6 \n") 
            else:
                slurmOut.write("#SBATCH --ntasks=%d \n" % (nGPUs+1))  
                slurmOut.write("#SBATCH --gpus=%d \n" % (nGPUs)) 
                slurmOut.write("#SBATCH --cpus-per-gpu=6 \n") 
                
                
#           slurmOut.write("#SBATCH --constraint=el9 \n") #not relevant on Elysium                
            slurmOut.write("#SBATCH --mem=%dGB \n" % RAMLIMIT)                  
            slurmOut.write("#SBATCH --time=%s \n" % TIMELIMIT)            
            slurmOut.write("#SBATCH --ntasks-per-core=1 \n")
            slurmOut.write("#SBATCH --threads-per-core=1 \n")
                                            
            slurmOut.write("#SBATCH --chdir=%s \n" % argRunningDir)
            slurmOut.write("#SBATCH --error=%s/log.err \n" % thisLogDir)
            slurmOut.write("#SBATCH --output=%s/log.out \n" % thisLogDir)
            slurmOut.write("#SBATCH --job-name=%s \n" % JOBNAME)
            slurmOut.write("export OMPI_MCA_orte_tmpdir_base=%s/orte_init \n" % tmpDir)                           
            slurmOut.write(SCRIPT_COMMAND)
            slurmOut.close()
            subprocess.call(["sbatch", "tempSlurm.txt"])
                                            
            # remove temporary submission file
#            os.remove("tempSlurm.txt")
        

if __name__ == "__main__":
   main(sys.argv[1:])
   
