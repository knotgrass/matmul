#! /bin/bash

version=1.14.0
workspaceFolder="$(dirname "$(dirname "$(realpath "$0")")")"
cd "$workspaceFolder"

# chuyển đến folder chứa thư viện
mkdir -p third_party
# git submodule add https://github.com/google/googletest.git third_party/googletest
cd third_party
# git clone https://github.com/google/googletest.git
cd googletest
git checkout v$version

mkdir -p build
cd build

# Install in /usr/local/ by default
# https://github.com/google/googletest/blob/main/googletest/README.md
# cmake ../

# https://github.com/google/googletest/issues/868#issuecomment-245593106
cmake ../ -DCMAKE_INSTALL_PREFIX=$workspaceFolder/Libs/googletest

make -j$(nproc)
make install
cd "$workspaceFolder"
