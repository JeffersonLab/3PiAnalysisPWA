#!/bin/csh
module use /usr/share/modulefiles
module load mpi/openmpi-x86_64
#
#source /group/halld/Software/build_scripts/gluex_env_jlab.csh /work/halld/home/afzal/version_fits_gpu.xml
source /group/halld/Software/build_scripts/gluex_env_jlab.csh /work/halld/home/afzal/version_6.1.1.xml #version_5.18.2.xml 5.17.0.xml
#setenv AMPTOOLS $AMPTOOLS_HOME/AmpTools
setenv PATH $ROOTSYS/bin:$PATH

#module use /cvmfs/oasis.opensciencegrid.org/jlab/scicomp/sw/el9/modulefiles
#module load cuda
#setenv CUDA_INSTALL_PATH /cvmfs/oasis.opensciencegrid.org/jlab/scicomp/sw/el9/cuda/12.2.2
#setenv GPU_ARCH sm_86
