cmake_minimum_required(VERSION 2.8.0)
project(wmb_vision)

# Dependencies
find_package(OpenCV REQUIRED)


# Help Eclipse CDT parse errors efficiently and automatically discover Include dirs
# http://www.cmake.org/Wiki/CMake:Eclipse_UNIX_Tutorial#Parsing_Errors_more_efficiently
IF(CMAKE_COMPILER_IS_GNUCC)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmessage-length=0")
ENDIF(CMAKE_COMPILER_IS_GNUCC)
IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmessage-length=0")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

#set the default path for includes
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/include)

# use new C++ standard, such as chrono library, and lots of new syntax
IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

# shared libs to cut back on having to re-link every recompilation
OPTION(BUILD_SHARED_LIBS "Build thelibraries shared." ON)
# library sources
add_subdirectory(lib_src)

# programs sources
add_subdirectory(bin_src)

