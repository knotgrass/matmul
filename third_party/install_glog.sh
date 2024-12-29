#! /bin/bash

version="0.7.0"
workspaceFolder="$(dirname "$(dirname "$(realpath "$0")")")"
cd "$workspaceFolder"
mkdir -p third_party
# git submodule add https://github.com/google/glog.git third_party/glog
cd third_party

#1
# git clone https://github.com/google/glog.git -b v0.7.0
cd glog
git checkout v$version
#2
cmake -S . -B build -G "Unix Makefiles" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=$workspaceFolder/libs/glog
#3
cmake --build build
#4
cmake --build build --target test
#5
cmake --build build --target install
cd $workspaceFolder
