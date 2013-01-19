cmake_minimum_required(VERSION 2.6.0)
project(tag_localizer)

# Dependencies
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(OpenCV_COMPONENTS core imgproc calib3d highgui)
find_package(OpenCV REQUIRED ${OpenCV_COMPONENTS})


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
include_directories(${PROJECT_SOURCE_DIR}/gen)

# use new C++ standard, such as chrono library, and lots of new syntax
# also, turn off RTTI and exceptions
IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -fno-exceptions")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

# OpenMP support
find_package(OpenMP REQUIRED)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

# Main tag localization library
add_subdirectory(src)

# Image blobs
add_subdirectory(images)

# Example programs
add_subdirectory(examples)