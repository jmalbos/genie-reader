### Check if cmake has the required version
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

### Set the project name
project(GENIE-READER)

###  Require C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wno-overloaded-virtual -Wno-shadow")

### Append to the CMAKE_MODULE_PATH variable the path to the directory
### with additional CMake modules
set(CMAKE_MODULE_PATH "${GENIE-READER_SOURCE_DIR}/cmake" "${CMAKE_MODULE_PATH}")

### Look for external dependencies
find_package(ROOT REQUIRED Geom EGPythia6 TreePlayer)
find_package(GENIE REQUIRED)
find_package(LibXml2)
find_package(Log4cpp)

if(EXISTS ${ROOT_USE_FILE})
  ### ROOT found using the ROOTConfig.cmake module
  include(${ROOT_USE_FILE})
else()
  ### ROOT found using the FindROOT.cmake module
  include_directories(${ROOT_INCLUDE_DIRS})
endif()

include_directories(${GENIE_INCLUDE_DIRS})

add_executable(GenieReader GenieReader.cxx)
target_link_libraries(GenieReader ${ROOT_LIBRARIES}
				  ${GENIE_LIBRARIES}
				  ${Log4cpp_LIBRARIES}
				  ${LIBXML2_LIBRARIES} )
