### --------------------------------------------------------
### File     : src/CMakeLists.txt
### Author   : Justo Martin-Albo
### Creation : 5 May 2019
### --------------------------------------------------------

add_executable(HNLDecayBkgnd HNLDecayBkgnd.cxx)
target_include_directories(HNLDecayBkgnd PRIVATE ${GENIE_INCLUDE_DIRS})
target_link_libraries(HNLDecayBkgnd ${ROOT_LIBRARIES}
                                    ${GENIE_LIBRARIES}
                                    ${Log4cpp_LIBRARIES}
                                    ${LIBXML2_LIBRARIES})

############################################################
