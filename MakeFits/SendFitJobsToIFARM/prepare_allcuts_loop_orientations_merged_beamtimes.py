#!/usr/bin/env python3.6

import sys
import os
import subprocess
import math
from optparse import OptionParser
import ROOT



######################################################## PREPARE TREES ##################################################
#throwntree_gen_amp_V2_mask010.root
#mctree_pippippimpim_phsp_mask010__PARA_0_allcuts.root
#flattree_2pip2pim__PARA_0_prompt_allcuts.root
#flattree_2pip2pim__PARA_0_sideband_allcuts.root


def linkfiles(DataSampleDir,MCphspSampleDir,FitInputTreesDir,linksDir,tvar_low,tvar_high,m3pi_low,m3pi_high):

    DataSampleDir = DataSampleDir 
    MCphspSampleAccDir = MCphspSampleDir 
    MCphspSampleThrownDir = MCphspSampleDir 
    #subrange = "tvar_%.3f_%.3f_m3pi_%.3f_%.3f" % (tvar_low,tvar_high,m3pi_low,m3pi_high)
    subrange = "tbin0_m3pi_%.3f_%.3f" % (m3pi_low,m3pi_high)

    
    #tvar in the trees is always negative, and we pass only positive range values
    tvarcut = "tvar >= %.3f && tvar <= %.3f" % (-tvar_high,-tvar_low)
    m3picut = "PiplusUPiminus1Piminus2_M >= %.3f && PiplusUPiminus1Piminus2_M <= %.3f" % (m3pi_low,m3pi_high)
    cuts = tvarcut + " && " + m3picut

    
    if not os.path.exists(FitInputTreesDir+"/"+subrange):
        os.makedirs(FitInputTreesDir+"/"+subrange)

        
    if not os.path.exists("%s/%s/throwntree_gen_amp_V2_mask010.root" % (FitInputTreesDir,subrange)):      
            os.system("root -l -b -q  '/home/beloviq2/skimTreeArgs.C(\"%s\",\"%s/%s\",\"throwntree_gen_amp_V2_mask010.root\",\"%s\")'" % (MCphspSampleThrownDir,FitInputTreesDir,subrange,cuts))
            
    os.system("ln -sf %s/%s/throwntree_gen_amp_V2_mask010.root %s/mcphsp_thrown.root" % (FitInputTreesDir,subrange,linksDir) )            





            
    for orientation in ["PARA_0", "PERP_45", "PERP_90", "PARA_135"]:                    
        if not os.path.exists("%s/%s/flattree_2pip2pim__%s_prompt_allcuts.root" % (FitInputTreesDir,subrange,orientation) ):
            os.system("root -l -b -q  '/home/beloviq2/skimTreeArgs.C(\"%s\",\"%s/%s\",\"flattree_2pip2pim__%s_prompt_allcuts.root\",\"%s\")'" % (DataSampleDir,FitInputTreesDir,subrange,orientation,cuts))
        if not os.path.exists("%s/%s/flattree_2pip2pim__%s_sideband_allcuts.root" % (FitInputTreesDir,subrange,orientation) ):
            os.system("root -l -b -q  '/home/beloviq2/skimTreeArgs.C(\"%s\",\"%s/%s\",\"flattree_2pip2pim__%s_sideband_allcuts.root\",\"%s\")'" % (DataSampleDir,FitInputTreesDir,subrange,orientation,cuts))
        if not os.path.exists("%s/%s/mctree_pippippimpim_phsp_mask010__%s_allcuts.root" % (FitInputTreesDir,subrange,orientation)):
            os.system("root -l -b -q  '/home/beloviq2/skimTreeArgs.C(\"%s\",\"%s/%s\",\"mctree_pippippimpim_phsp_mask010__%s_allcuts.root\",\"%s\")'" % (MCphspSampleAccDir,FitInputTreesDir,subrange,orientation,cuts))

            
    for orientation in ["PARA_0", "PERP_45", "PERP_90", "PARA_135"]:            
        os.system("ln -sf %s/%s/flattree_2pip2pim__%s_prompt_allcuts.root %s/data_prompt_%s.root"        % (FitInputTreesDir,subrange,orientation,linksDir,orientation) )
        os.system("ln -sf %s/%s/flattree_2pip2pim__%s_sideband_allcuts.root %s/data_sideband_%s.root"    % (FitInputTreesDir,subrange,orientation,linksDir,orientation) )
        os.system("ln -sf %s/%s/mctree_pippippimpim_phsp_mask010__%s_allcuts.root %s/mcphsp_acc_%s.root" % (FitInputTreesDir,subrange,orientation,linksDir,orientation) )






########################################################## MAIN ##########################################################

def main(argv):
        global VERBOSE # so can modify here default values for local runs     
        
        DataSampleDir = "/lustre/beloviq2/3PiDeltaTrees/PhaseI"
        MCphspSampleDir = "/lustre/beloviq2/3PiDeltaTrees/PhaseI"
        FitInputTreesDir = "/lustre/beloviq2/TestFolder"
        linksDir = "."

        tvar_low =  0.00
        tvar_high = 0.25
        m3pi_low =  0.825
        m3pi_high = 0.850

        
        parser_usage = "process_allcuts.py [9 parameters to overwrite the default variables]"
        parser = OptionParser(usage = parser_usage)
        (options, args) = parser.parse_args(argv)

        if len(args) == 9:
                DataSampleDir = args[0]
                MCphspSampleDir = args[1]
                FitInputTreesDir = args[2]
                linksDir = args[3]
                tvar_low = float(args[4])
                tvar_high = float(args[5]) 
                m3pi_low = float(args[6])
                m3pi_high = float(args[7])

        
        # For a single beamtime, create trees and links to them in the 'FitInputTreesDir' and 'linksDir' directories, respectively
        linkfiles(DataSampleDir,MCphspSampleDir,FitInputTreesDir,linksDir,tvar_low,tvar_high,m3pi_low,m3pi_high)



if __name__ == "__main__":
   main(sys.argv[1:])
