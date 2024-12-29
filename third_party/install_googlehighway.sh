#! /bin/bash

version=1.2.0
workspaceFolder="$(dirname "$(dirname "$(realpath "$0")")")"
cd $workspaceFolder
mkdir -p third_party
git submodule add https://github.com/google/highway.git third_party/highway
cd third_party

#1
# git clone https://github.com/google/highway.git
cd highway
git checkout $version

#2
sudo apt install cmake
sudo apt install libgtest-dev

mkdir -p build && cd build
cmake .. \
    -DCMAKE_INSTALL_PREFIX=$workspaceFolder/libs/highway

make -j2 && make test

make install

cd $workspaceFolder
