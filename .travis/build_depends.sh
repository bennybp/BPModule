#!/bin/sh
# This script expects two arguments compiler version and python version
PY_VER=$2
CXX_COMPILER=g++-$1
g++ --version
cd ${HOME}
#####################################
# Get Miniconda for python versions #
#####################################
wget https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh \
    -O miniconda.sh
bash miniconda.sh -b -p $HOME/miniconda
export PATH="$HOME/miniconda/bin:$PATH"
conda config --set always_yes yes --set changeps1 no
conda update -q conda
conda create -q -n test-environment python=${PY_VER}
PY_EXE=${HOME}/miniconda/envs/test-environment/bin/python
cd ${HOME}
###################################################
# Build Eigen because CMake doesn't support https #
###################################################
wget http://bitbucket.org/eigen/eigen/get/3.3.2.tar.bz2
tar -xf 3.3.2.tar.bz2
ls
cd eigen-eigen-da9b4e14c255
cmake -H. -Bbuild -DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
                  -DCMAKE_BUILD_TYPE=Release  \
                  -DCMAKE_INSTALL_PREFIX=${HOME} \
cd build && make install
cd ${HOME}
#####################################
#Build Pulsar-Meta for dependencies #
#####################################
git clone https://github.com/pulsar-chem/Pulsar-Meta.git
cd Pulsar-Meta
cmake -H. -Bbuild -DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
                  -DMPI_CXX_COMPILER=mpicxx -DMPI_C_COMPILER=mpicc \
                  -DCMAKE_BUILD_TYPE=Release \
                  -DCMAKE_INSTALL_PREFIX=${HOME} \
                  -DCMAKE_PREFIX_PATH=${HOME} \
                  -DPYTHON_EXECUTABLE=${PY_EXE} \
                  -DENABLE_PULSAR=False
cd build && make install
