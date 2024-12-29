#! /bin/bash
# https://github.com/NVIDIA/cutlass/blob/main/media/docs/quickstart.md

workspaceFolder="$(dirname "$(dirname "$(realpath "$0")")")"
cd "$workspaceFolder"

mkdir -p third_party

# git submodule add https://github.com/NVIDIA/cutlass third_party/cutlass
cd third_party
# git clone https://github.com/NVIDIA/cutlass.git
cd cutlass
git checkout main
git checkout v3.6.0

export CUDACXX=/home/tz/anaconda3/envs/cxx/bin/nvcc

mkdir -p build && cd build
cmake .. \
    -DCMAKE_INSTALL_PREFIX=$workspaceFolder/libs/cutlass \
    -DCUTLASS_NVCC_ARCHS=75 \
    -DCUTLASS_ENABLE_TESTS=ON \
    -DCUTLASS_UNITY_BUILD_ENABLED=ON

make default_target -j$(nproc)
make install

cd "$workspaceFolder"
rm -rf third_party/cutlass/build

# make cutlass_profiler -j12

# https://github.com/NVIDIA/cutlass/blob/main/media/docs/profiler.md
# ./tools/profiler/cutlass_profiler --kernels=sgemm --m=4352 --n=4096 --k=4096
# ./tools/profiler/cutlass_profiler --operation=conv2d --n=8 --h=224 --w=224 --c=128 --k=128 --r=3 --s=3
# ./tools/profiler/cutlass_profiler --kernels=s1688fprop --n=8 --h=224 --w=224 --c=128 --k=128 --r=3 --s=3 --pad_h=1 --pad_w=1

# Build and run CUTLASS Unit Tests
# make test_unit -j
# make test_unit_gemm_warp -j
