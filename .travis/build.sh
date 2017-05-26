#!/bin/sh
#This script is the contents of the .travis.yml file as a bash script to be used
#for debuggin Travis in a Docker container.  You will have to run this script as
#sudo
git clone --depth=50 --branch=master https://github.com/pulsar-chem/Pulsar-Core.git pulsar-chem/Pulsar-Core
cd pulsar-chem/Pulsar-Core
sudo -E apt-add-repository -y "ppa:george-edison55/precise-backports"
sudo -E apt-add-repository -y "ppa:ubuntu-toolchain-r/test"
sudo -E apt-get -yq update &>> ~/apt-get-update.log
#Locally I get a 404 from the previous command.  Running it again skips the 404
#error and finishes the update
sudo -E apt-get -yq update &>> ~/apt-get-update.log
sudo -E apt-get -yq --no-install-suggests --no-install-recommends --force-yes install doxygen graphviz cmake cmake-data libdb-dev g++-4.9 libmpich2-dev
cd ${HOME}
wget https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh -O miniconda.sh
bash miniconda.sh -b -p $HOME/miniconda
export PATH="$HOME/miniconda/bin:$PATH"
hash -r
conda config --set always_yes yes --set changeps1 no
conda update -q conda
conda info -a
conda create -q -n test-environment python=3.5
source activate test-environment
conda install --yes  numpy
cd ${HOME}
wget http://bitbucket.org/eigen/eigen/get/3.3.2.tar.bz2
tar -xf 3.3.2.tar.bz2
cd eigen-eigen-da9b4e14c255
cmake -H. -Bbuild -DCMAKE_CXX_COMPILER=g++-4.9 -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=${HOME}
cd build && make install
cd ${HOME}
git clone https://github.com/pulsar-chem/Pulsar-Meta.git
cd Pulsar-Meta
cmake -H. -Bbuild -DCMAKE_CXX_COMPILER=g++-4.9 -DMPI_CXX_COMPILER=mpicxx -DMPI_C_COMPILER=mpicc \
   -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=${HOME} -DCMAKE_PREFIX_PATH=${HOME} \
  -DPYTHON_EXECUTABLE=${HOME}/miniconda/envs/test-environment/bin/python -DENABLE_PULSAR=False
cd build && make install
cd ${HOME}/pulsar-chem/Pulsar-Core
cmake -H. -Bbuild -DCMAKE_CXX_COMPILER=g++-4.9 -DCMAKE_BUILD_TYPE=Release -DMPI_CXX_COMPILER=mpicxx\
  -DMPI_C_COMPILER=mpicc -DPYTHON_EXECUTABLE=${HOME}/miniconda/envs/test-environment/bin/python \
  -DCMAKE_PREFIX_PATH=${HOME} -DCMAKE_INSTALL_RPATH=${HOME}/lib
cd build && make -j2
