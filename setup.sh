#!/bin/bash

module load elysium/2026
#rub-deploy-spack-configs-2026
module load spack/2026
#module load perl-xml-simple
#module load perl-file-slurp

module load openmpi/5.0.9
module load cuda

#spack load root


export BUILD_SCRIPTS=/home/gluex-software/gluex_top/build_scripts

#export CUDA_INSTALL_PATH=$CUDA_HOME  #switch this off when use mpi instead of CUDA
export GPU_ARCH sm_90


export MPIGCC=$(spack location -i openmpi%gcc)
export TIRPC=$(spack location -i libtirpc)
export GSL=$(spack location -i gsl)


export MYAMPTOOLS=/home/beloviq2/AmpTools 
export MYHALLDSIM=/home/beloviq2/halld_sim


export PATH=$PATH:$(spack location -i apptainer)/bin:$MYAMPTOOLS:$MYHALLLDSIM


export APPTAINERENV_PATH=$PATH:$MPIGCC/bin:$TIRPC/bin:$GSL/bin
export APPTAINERENV_LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MPIGCC/lib:$TIRPC/lib:$GSL/lib
export APPTAINERENV_LIBRARY_PATH=$LIBRARY_PATH:$MPIGCC/lib:$TIRPC/lib:$GSL/lib










