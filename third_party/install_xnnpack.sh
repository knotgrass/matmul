#! /bin/bash

workspaceFolder="$(dirname "$(dirname "$(realpath "$0")")")"
cd "$workspaceFolder"

mkdir -p third_party

# git submodule add https://github.com/google/XNNPACK.git third_party/XNNPACK
cd third_party
# git clone https://github.com/google/XNNPACK.git
cd XNNPACK
git checkout 3bc4ef01bbdf488556c54584fc2419dd77c39c85

mkdir -p build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$workspaceFolder/Libs/XNNPACK
make all -j$(nproc)
make install

# cd .. && rm build
cd "$workspaceFolder"
