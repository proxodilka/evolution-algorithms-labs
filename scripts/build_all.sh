$BASE_DIR = $PWD
cd build
rm -r -f *

cmake ../ -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
make
cd $BASE_DIR
