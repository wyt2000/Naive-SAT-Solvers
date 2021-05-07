# set cmake version
cmake_minimum_required(VERSION 3.16) 

# set the project name 
project(NaiveDPLL) 

# add include directory
include_directories(include)

# add the executable
file(GLOB NAIVEDPLLSRC src/*.cpp)
add_executable(NaiveDPLL ${NAIVEDPLLSRC})

# set c++11 support
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
