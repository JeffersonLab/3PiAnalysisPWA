#!/usr/bin/env python3.6

import sys
import os
import subprocess
import math
from optparse import OptionParser
import ROOT



######################################################## PREPARE TREES ##################################################
#throwntree_gen_amp_V2_mask010__sum_030274_031057.root
#mctree_pippippimpim_phsp_mask010__sum_PARA_0_30274_31057_allcuts.root
#flattree_2pip2pim__sum_PARA_0_30274_31057_prompt_allcuts.root
#flattree_2pip2pim__sum_PARA_0_30274_31057_sideband_allcuts.root


def linkfiles(DataSampleDir,MCphspSampleDir,FitInputTreesDir,linksDir,runnums,tvar_low,tvar_high,m3pi_low,m3pi_high,m2pi_low,m2pi_high):

    DataSampleDir = DataSampleDir+"/POLS"
    MCphspSampleAccDir = MCphspSampleDir+"/POLS"
    MCphspSampleThrownDir = MCphspSampleDir+"/ThrownTrees"
    #subrange = "tvar_%.2f_%.2f_m3pi_%.2f_%.2f_m2pi_%.2f_%.2f" % (tvar_low,tvar_high,m3pi_low,m3pi_high,m2pi_low,m2pi_high)
    subrange = "tbin0_m3pi_%.2f_%.2f_m2pi_%.2f_%.2f" % (m3pi_low,m3pi_high,m2pi_low,m2pi_high)

    
    #tvar in the trees is always negative, and we pass only positive range values
    tvarcut = "tvar >= %.2f && tvar <= %.2f" % (-tvar_high,-tvar_low)
    m3picut = "PiplusUPiminus1Piminus2_M >= %.2f && PiplusUPiminus1Piminus2_M <= %.2f" % (m3pi_low,m3pi_high)
    m2picut = "PiplusUPiminus1_M >= %.2f  && PiplusUPiminus1_M <= %.2f && PiplusUPiminus2_M >= %.2f && PiplusUPiminus2_M <= %.2f" % (m2pi_low,m2pi_high,m2pi_low,m2pi_high)    
    cuts = tvarcut + " && " + m3picut

    
    if not os.path.exists(FitInputTreesDir+"/"+subrange):
        os.makedirs(FitInputTreesDir+"/"+subrange)

        
     #For the thrown tree selections, we use only the thrown -t range
     # We use the same Tree for any (m2pi, m3pi) subrange and either of the (00,45,90,135) polarization settings

    if not os.path.exists("%s/throwntree_gen_amp_V2_mask010__sum_%s.root" % (FitInputTreesDir,runnums)):      
            os.system("root -l -b -q  '/work/halld/home/ilbelov/skimTreeArgs.C(\"%s\",\"%s\",\"throwntree_gen_amp_V2_mask010__sum_%s.root\",\"%s\")'" % (MCphspSampleThrownDir,FitInputTreesDir,runnums,tvarcut))
            
    os.system("ln -sf %s/throwntree_gen_amp_V2_mask010__sum_%s.root %s/mcphsp_thrown.root" % (FitInputTreesDir,runnums,linksDir) )            

            
    for orientation in ["PARA_0", "PERP_45", "PERP_90", "PARA_135"]:                    
        if not os.path.exists("%s/%s/flattree_2pip2pim__sum_%s_%s_prompt_allcuts.root" % (FitInputTreesDir,subrange,orientation,runnums) ):
            os.system("root -l -b -q  '/work/halld/home/ilbelov/skimTreeArgs.C(\"%s\",\"%s/%s\",\"flattree_2pip2pim__sum_%s_%s_prompt_allcuts.root\",\"%s\")'" % (DataSampleDir,FitInputTreesDir,subrange,orientation,runnums,cuts))
        if not os.path.exists("%s/%s/flattree_2pip2pim__sum_%s_%s_sideband_allcuts.root" % (FitInputTreesDir,subrange,orientation,runnums) ):
            os.system("root -l -b -q  '/work/halld/home/ilbelov/skimTreeArgs.C(\"%s\",\"%s/%s\",\"flattree_2pip2pim__sum_%s_%s_sideband_allcuts.root\",\"%s\")'" % (DataSampleDir,FitInputTreesDir,subrange,orientation,runnums,cuts))
        if not os.path.exists("%s/%s/mctree_pippippimpim_phsp_mask010__sum_%s_%s_allcuts.root" % (FitInputTreesDir,subrange,orientation,runnums)):
            os.system("root -l -b -q  '/work/halld/home/ilbelov/skimTreeArgs.C(\"%s\",\"%s/%s\",\"mctree_pippippimpim_phsp_mask010__sum_%s_%s_allcuts.root\",\"%s\")'" % (MCphspSampleAccDir,FitInputTreesDir,subrange,orientation,runnums,cuts))

            
    for orientation in ["PARA_0", "PERP_45", "PERP_90", "PARA_135"]:            
        os.system("ln -sf %s/%s/flattree_2pip2pim__sum_%s_%s_prompt_allcuts.root %s/data_prompt_%s.root"        % (FitInputTreesDir,subrange,orientation,runnums,linksDir,orientation) )
        os.system("ln -sf %s/%s/flattree_2pip2pim__sum_%s_%s_sideband_allcuts.root %s/data_sideband_%s.root"    % (FitInputTreesDir,subrange,orientation,runnums,linksDir,orientation) )
        os.system("ln -sf %s/%s/mctree_pippippimpim_phsp_mask010__sum_%s_%s_allcuts.root %s/mcphsp_acc_%s.root" % (FitInputTreesDir,subrange,orientation,runnums,linksDir,orientation) )






########################################################## MAIN ##########################################################

def main(argv):
        global VERBOSE # so can modify here default values for local runs     
        
        numRand = 0
        precision = -1
        DataSampleDir = "/work/halld/home/ilbelov/DataSpring2017"
        MCphspSampleDir = "/work/halld/home/ilbelov/MCphspSpring2017/mask010"
        FitInputTreesDir = "/work/halld/home/ilbelov/TreesFitted"
        linksDir = "."
        runnums = "30274_31057"

        tvar_low =  0.00
        tvar_high = 0.25
        m3pi_low =  1.25
        m3pi_high = 1.30
        m2pi_low =  0.0
        m2pi_high = 2.0    
        subrange = "tbin0_m3pi_%.2f_%.2f_m2pi_%.2f_%.2f" % (m3pi_low,m3pi_high,m2pi_low,m2pi_high)

        
        parser_usage = "process_allcuts.py [13 parameters to overwrite the default variables]"
        parser = OptionParser(usage = parser_usage)
        (options, args) = parser.parse_args(argv)

        if len(args) == 13:
                numRand = int(args[0])
                precision = float(args[1])
                DataSampleDir = args[2]
                MCphspSampleDir = args[3]
                FitInputTreesDir = args[4]
                linksDir = args[5]
                runnums = args[6]
                tvar_low = float(args[7])
                tvar_high = float(args[8]) 
                m3pi_low = float(args[9])
                m3pi_high = float(args[10])
                m2pi_low = float(args[11])
                m2pi_high = float(args[12])

                
                
        # Config file
        cfgTemplate = "template_rhoIsoAmp.cfg"
        
        # Link files locally for fit
        linkfiles(DataSampleDir,MCphspSampleDir,FitInputTreesDir,linksDir,runnums,tvar_low,tvar_high,m3pi_low,m3pi_high,m2pi_low,m2pi_high)


        # Prepare a customized config file using the template        
        f = open(cfgTemplate,'r')
        filedata = f.read()
        f.close()

        filedata = filedata.replace("$PATH", linksDir)
        filedata = filedata.replace("$SUBRANGE", subrange)

        cfgfile = "rhoIsoAmp__%s.cfg" % (subrange)
        f = open(cfgfile,'w')
        f.write(filedata)
        f.close()

        
     	#Main command launching fit 
        os.system("which fitMPI") #which fit
        command = "mpirun --mca btl_openib_allow_ib 1 fitMPI -c " + cfgfile + " -m 100000"  # -np 16 to run it interactively with multiple processors


        
        #os.system("which fit")
        #command = "fit -c "+ cfgfile + "-m 1000000 -s param_seeds"

        if precision > 0:
            command += " -e %.1e" % precision

        if numRand > 0: # randomize parameters
            command +=" -r %d" % numRand
            
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


        print(command)
#        sys.exit()        
        os.system(command)

        
	# Call plotter
#        os.system("grep -l 'nan' ./fit_rhoIsoAmp_" + subrange + "*.fit | xargs rm") # delete nans
#        os.system("vecps3pi_plotter fit_rhoIsoAmp_" + subrange + ".fit -o vecps3pi_histos_" + subrange + ".root") 


        # link files in output directory for plotter
        # linkfiles(DataSampleDir,MCphspSampleDir,FitInputTreesDir,linksDir,runnums,tvar_low,tvar_high,m3pi_low,m3pi_high,m2pi_low,m2pi_high)


if __name__ == "__main__":
   main(sys.argv[1:])
