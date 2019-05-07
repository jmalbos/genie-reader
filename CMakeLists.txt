### Check if cmake has the required version
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

### Set the project name
project(genie-reader)

###  Require C++11
#set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wno-overloaded-virtual -Wno-shadow")

### Append to the CMAKE_MODULE_PATH variable the path to the directory
### with additional CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

### Look for external dependencies
find_package(ROOT REQUIRED Geom EGPythia6 TreePlayer MathCore GenVector)
find_package(GENIE REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(Log4cpp REQUIRED)

include(${ROOT_USE_FILE})

add_subdirectory(src)
