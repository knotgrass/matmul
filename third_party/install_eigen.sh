#! /bin/bash

workspaceFolder="$(dirname "$(dirname "$(realpath "$0")")")"
cd "$workspaceFolder"

mkdir -p third_party

# git submodule add https://gitlab.com/libeigen/eigen.git third_party/eigen
cd third_party
git clone https://gitlab.com/libeigen/eigen.git -b 3.4
cd eigen
# eigen lastest v3.4
git checkout e67c494cba7180066e73b9f6234d0b2129f1cdf5

mkdir -p build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$workspaceFolder/libs/eigen
make all
make install

cd "$workspaceFolder"
