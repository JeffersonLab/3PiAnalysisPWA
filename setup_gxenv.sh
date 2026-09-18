#!/bin/bash

module load elysium/2026
#rub-deploy-spack-configs-2026
module load spack/2026
#module load perl-xml-simple
#module load perl-file-slurp


module load openmpi/5.0.9


export BUILD_SCRIPTS=/home/gluex-software/gluex_top/build_scripts

export MPIGCC=$(spack location -i openmpi%gcc)
export TIRPC=$(spack location -i libtirpc)
export GSL=$(spack location -i gsl)


export MYAMPTOOLS=/home/beloviq2/AmpTools 
export MYHALLDSIM=/home/beloviq2/halld_sim


export PATH=$PATH:$(spack location -i apptainer)/bin:$MYAMPTOOLS:$MYHALLLDSIM


export APPTAINERENV_PATH=$PATH:$MPIGCC/bin:$TIRPC/bin:$GSL/bin
export APPTAINERENV_LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MPIGCC/lib:$TIRPC/lib:$GSL/lib
export APPTAINERENV_LIBRARY_PATH=$LIBRARY_PATH:$MPIGCC/lib:$TIRPC/lib:$GSL/lib


apptainer exec --bind /lustre/beloviq2:/lustre/beloviq2,/home/beloviq2:/home/beloviq2,/home/gluex-software/gluex_top:/home/gluex-software/gluex_top,/usr/lib64/liblustreapi.so.1:/usr/lib64/liblustreapi.so.1,/cluster/spack:/cluster/spack  /home/gluex-software/gluex_almalinux_9_latest.sif bash


#/home/gluex-software/gluex_top/gluex_env_boot.sh
#/home/gluex-software/gluex_top/gluex_env_local.sh








