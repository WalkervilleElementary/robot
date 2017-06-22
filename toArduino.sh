#!/usr/bin/env bash

if [ "$#" -ne 1 ]; then
  echo "We need the location of the arduino folder"
  exit 1
fi

arduino=$1
arduino_libs=$arduino"/libraries/walkerville"
arduino_sketch=$arduino"/sketches/robot"

echo "creating folder" $arduino_libs
mkdir -p $arduino_libs
echo "creating folder" $arduino_sketch
mkdir -p $arduino_sketch

echo "copying header files to " $arduino_libs
ln`find include/ -type f | grep -v configs.h` $arduino_libs

echo "copying cpp files to " $arduino_libs
ln `find src/ -type f | grep -v main.cpp` $arduino_libs

echo "converting configs.h file for device"
cat include/configs.h | sed -e "s/DEVICE() 0/DEVICE() 1/" > $arduino_libs/configs.h

echo "copying main to " $arduino_sketch
ln src/main.cpp $arduino_sketch/robot.ino


