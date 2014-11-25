#!/bin/bash

#### Script to install local development environment
#### to mimick the beaglebone black setup.

cd ~/
mkdir WALLE
cd WALLE
git clone https://kgcoe-git.rit.edu/bvp/develop.git
cd develop/
git checkout Device

# Libraries
# OpenCV package
sudo apt-get install libopencv-dev
# UDEV package
sudo apt-get install libudev-dev
# Lib USB package
sudo apt-get install libusb-1.0-0-dev
# Build packages from source
cd ~/
mkdir tmp
cd tmp
# Build OpenNI2
git clone https://github.com/occipital/openni2
cd openni2/
make
sudo mkdir /usr/local/include/OpenNI2
sudo cp -r Include/* /usr/local/include/OpenNI2
sudo cp Bin/*-Release/libOpenNI2.so /usr/local/lib/
# Build BBBIO_lib
cd ../
git clone https://github.com/VegetableAvenger/BBBIOlib
cd BBBIOlib/
make all
sudo mkdir /usr/local/include/BBBio_lib
sudo cp BBBio_lib/*.h /usr/local/include/BBBio_lib/
sudo cp libBBBio.a /usr/local/lib/
# Cleanup
cd ~/
rm -rf tmp/
echo "TADA... Done installing everything"
