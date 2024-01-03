#! /bin/bash

workspaceFolder="$(dirname "$(dirname "$(realpath "$0")")")"
cd "$workspaceFolder"
echo "$workspaceFolder"

mkdir -p third_party
# git submodule add https://github.com/google/ruy third_party/ruy
cd third_party/ruy
git checkout cd7b92695b5d3f0c9ff65b865c2a1e19b99d766d

cd third_party
git clone https://github.com/pytorch/cpuinfo.git
cd cpuinfo && git checkout 082deffc80ce517f81dc2f3aebe6ba671fcd09c9 && cd ..
git clone https://github.com/google/googletest.git
cd googletest && git checkout 6c58c11d5497b6ee1df3cb400ce30deb72fc28c0 && cd ..

cd ..
mkdir -p build && cd build
cmake ..
make
sudo make install
