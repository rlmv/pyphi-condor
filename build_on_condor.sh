# fix interpreter paths


# Build a modern compiler
# Take a walk. This takes a while.
# GCC_URL=http://gcc.parentingamerica.com/releases/gcc-7.2.0/gcc-7.2.0.tar.gz
# GCC_SRC=gcc-7.2.0
# curl $GCC_URL | tar -xvz

# cd $GCC_SRC
# ./contrib/download_prerequisites
# cd ..
# mkdir objdir
# cd objdir
# $PWD/../$GCC_SRC/configure --prefix=$PWD/../GCC-7.2.0 --enable-languages=c,c++
# make
# make install


# MW
MW_URL=http://research.cs.wisc.edu/htcondor/mw/mw-0.3.0.tgz
curl $MW_URL | tar -xvz
cd mw

# Independent mode
# -fPIC has to do with the shared libraries
CXXFLAGS="-fPIC" ./configure --prefix=$(pwd)/../mw-build --without-pvm --without-mwfile --without-mpi --without-condor
make
make install

cd ..


# Python
tar -xvzf python-3.6.3-build.tar.gz
export PATH=$PWD/python/bin:$PATH

pip3 install cython
alias python=python3

# These were required last time?
export PYTHONHOME=/var/lib/condor/execute/slot1/dir_12676/python/
export PYTHONPATH=/var/lib/condor/execute/slot1/dir_12676/python/

git clone https://github.com/rlmv/pyphi-condor.git
