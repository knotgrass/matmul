#! /bin/bash

workspaceFolder="$(dirname "$(dirname "$(realpath "$0")")")"
cd "$workspaceFolder"
echo "$workspaceFolder"

mkdir -p third_party && cd third_party
git clone https://github.com/google/ruy.git

cd ruy/third_party
git clone https://github.com/pytorch/cpuinfo.git
git clone https://github.com/google/googletest.git

cd ..
mkdir build && cd build
cmake ..
make
sudo make install
