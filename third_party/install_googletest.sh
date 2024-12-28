#! /bin/bash

# use lastest commit of brand of v1.15.2
commit=b514bdc898e2951020cbdca1304b75f5950d1f59
workspaceFolder="$(dirname "$(dirname "$(realpath "$0")")")"
cd "$workspaceFolder"

# chuyển đến folder chứa thư viện
mkdir -p third_party
# git submodule add https://github.com/google/googletest.git third_party/googletest
cd third_party
# git clone https://github.com/google/googletest.git
cd googletest
git checkout $commit

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
