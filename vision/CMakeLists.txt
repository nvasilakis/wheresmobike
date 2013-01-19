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
# also, turn off RTTI and exceptions
IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

# OpenMP support
find_package(OpenMP REQUIRED)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

# library sources
add_subdirectory(lib_src)

# programs sources
add_subdirectory(bin_src)

