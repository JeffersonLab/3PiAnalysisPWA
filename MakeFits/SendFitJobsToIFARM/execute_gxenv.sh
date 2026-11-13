#!/bin/bash

module load elysium/2026
#rub-deploy-spack-configs-2026
module load spack/2026



#Load either cuda, or mpi, to prevent throwing errors about some shared libraries
if [ ${2} -eq 0 ]; then
    module load openmpi/5.0.9
elif [ ${2} -eq 1 ]; then
    module load cuda
else
    module load cuda
    module load openmpi/5.0.9
fi



#This is only to access git on a computing node, to prevent throwing errors for 'spack requires git'
export PATH=$PATH:/cluster/spack/opt/elysium_2026/linux-zen4/git-2.52.0-aruqslhh3laxjh7hfywripvdpqv64fxp/bin 


#export CUDA_INSTALL_PATH=$CUDA_HOME #switch this off when use mpi instead of CUDA
export GPU_ARCH sm_90

export MPIGCC=$(spack location -i openmpi%gcc)
export TIRPC=$(spack location -i libtirpc)
export GSL=$(spack location -i gsl)

export BUILD_SCRIPTS=/home/gluex-software/gluex_top/build_scripts


export MYAMPTOOLS=/home/beloviq2/AmpTools 
export MYHALLDSIM=/home/beloviq2/halld_sim


export PATH=$PATH:$(spack location -i apptainer)/bin:$(spack location -i git)/bin:$MYAMPTOOLS:$MYHALLLDSIM

export APPTAINERENV_PATH=$PATH:$MPIGCC/bin:$TIRPC/bin:$GSL/bin
export APPTAINERENV_LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MPIGCC/lib:$TIRPC/lib:$GSL/lib
export APPTAINERENV_LIBRARY_PATH=$LIBRARY_PATH:$MPIGCC/lib:$TIRPC/lib:$GSL/lib


#apptainer exec --nv --bind /lustre/beloviq2:/lustre/beloviq2,/home/beloviq2:/home/beloviq2,/home/gluex-software/gluex_top:/home/gluex-software/gluex_top,/usr/lib64/liblustreapi.so.1:/usr/lib64/liblustreapi.so.1,/usr/lib64/libnl-genl-3.so.200:/usr/lib64/libnl-genl-3.so.200,/usr/lib64/liblnetconfig.so.4:/usr/lib64/liblnetconfig.so.4,/usr/lib64/libnl-3.so.200:/usr/lib64/libnl-3.so.200,/cluster/spack:/cluster/spack  /home/gluex-software/gluex_almalinux_9_latest.sif csh /home/beloviq2/MakeFits/SendFitJobsToIFARM/runFit_allbeamtimes.csh "$@"


apptainer exec --nv --bind /lustre/beloviq2:/lustre/beloviq2,/home/beloviq2:/home/beloviq2,/home/gluex-software/gluex_top:/home/gluex-software/gluex_top,/usr/lib64/liblustreapi.so.1:/usr/lib64/liblustreapi.so.1,/usr/lib64/libnl-genl-3.so.200:/usr/lib64/libnl-genl-3.so.200,/usr/lib64/liblnetconfig.so.4:/usr/lib64/liblnetconfig.so.4,/usr/lib64/libnl-3.so.200:/usr/lib64/libnl-3.so.200,/cluster/spack:/cluster/spack  /home/gluex-software/gluex_almalinux_9_latest.sif csh /home/beloviq2/MakeFits/SendFitJobsToIFARM/runPlot.csh "$@"







