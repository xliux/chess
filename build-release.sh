#!/bin/bash -x
type=release
mkdir $type
ln -nfs $type build 

proj=$1

if [[ "$proj" == "" ]]; then
  echo "Error! empty project. Usage: $0 <proj-name>|all"
  exit -1
fi

if [[ $proj == "all" ]]; then
  proj=.
fi

TOOLCHAIN=
OS=`uname -s`
echo "OS=$OS"

if [[ $OS == "Linux" ]]; then
  TOOLCHAIN=../linux-toolchain.cmake
elif [[ "$OS" == "Darwin" ]]; then
  TOOLCHAIN=../macos-toolchain.cmake
else
  echo "Unknown OS"
  exit -1
fi

cd build && \
  cmake .. -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN -DCMAKE_BUILD_TYPE=$type && \
  make -f $proj/Makefile
