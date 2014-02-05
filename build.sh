#!/bin/sh

BUILD_DIR="build"
SRC_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
BUILD_TYPE=Release
ENABLE_TESTS=ON

set +x

mkdir -p $BUILD_DIR && \
cd $BUILD_DIR

echo "Running in $SRC_DIR :"
cmake -DWkCore_BUILD_TYPE=$BUILD_TYPE -DWkCore_ENABLE_TESTS=$ENABLE_TESTS $SRC_DIR

make

ctest

 
