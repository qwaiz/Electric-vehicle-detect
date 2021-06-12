#!/bin/bash


#TARGET_ARCH_ABI=armv8 # for RK3399, set to default arch abi
#TARGET_ARCH_ABI=armv7hf # for Raspberry Pi 3B
#TARGET_ARCH_ABI=himix100 # for hi3559 
TARGET_ARCH_ABI=himix200 # for hi3516

# build
rm -rf build
mkdir build
cd build
cmake -DTARGET_ARCH_ABI=${TARGET_ARCH_ABI} ..
make


