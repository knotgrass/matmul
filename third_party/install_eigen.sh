#! /bin/bash

workspaceFolder="$(dirname "$(dirname "$(realpath "$0")")")"
cd "$workspaceFolder"

mkdir -p third_party

# git submodule add https://gitlab.com/libeigen/eigen.git third_party/eigen
cd third_party
# git clone https://gitlab.com/libeigen/eigen.git
cd eigen
# eigen lastest v3.4
git checkout 25270e35dbfb9d407175a321707a3b51a079588d

mkdir -p build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$workspaceFolder/Libs/eigen
make all
make install

cd "$workspaceFolder"
