#! /bin/bash

workspaceFolder="$(dirname "$(dirname "$(realpath "$0")")")"
cd "$workspaceFolder"

mkdir -p third_party

# git submodule add https://github.com/google/XNNPACK.git third_party/XNNPACK
cd third_party
# git clone https://github.com/google/XNNPACK.git
cd XNNPACK
git checkout 403565683d0b559e535a4f9b2fb5d3786e94ddae

mkdir -p build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$workspaceFolder/Libs/XNNPACK
make all -j$(nproc)
make install

# cd .. && rm build
cd "$workspaceFolder"
