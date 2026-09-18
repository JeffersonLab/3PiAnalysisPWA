#Load shell environment
module use /usr/share/modulefiles
module load mpi/openmpi-x86_64

source /group/halld/Software/build_scripts/gluex_env_jlab.csh /work/halld/home/ilbelov/version_7.4.0_local.xml

#setenv PATH $ROOTSYS/bin:$PATH
#setenv AMPTOOLS_HOME $PWD/AmpTools/
#setenv AMPTOOLS $AMPTOOLS_HOME/AmpTools



#Load CUDA part
source /etc/profile.d/modules.csh

module use /cvmfs/oasis.opensciencegrid.org/jlab/scicomp/sw/el9/modulefiles
module load cuda

setenv CUDA_INSTALL_PATH /cvmfs/oasis.opensciencegrid.org/jlab/scicomp/sw/el9/cuda/12.2.2/
setenv GPU_ARCH sm_80


setenv AMPTOOLS_HOME $PWD/AmpTools/
setenv PATH $ROOTSYS/bin:$PATH

