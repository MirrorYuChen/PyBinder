cmake_minimum_required(VERSION 3.1)
project(PyBind LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ----- Set build type -----
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "Release")
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

option(BUILD_SHARED_LIBS "Build Pybind as a shared library" ON)
option(BUILD_PYTHON "Build python wrapper" OFF)
if (BUILD_PYTHON)
  set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build stella_vslam as a static library" FORCE)
endif(BUILD_PYTHON)
set(BUILD_EXAMPLES OFF CACHE BOOL "Build examples")


# Note: add_compile_options() accepts only "simple" generator-expressions :-(
if(MSVC)
  # C4251: DLL export, C4244: floating->integer, C4305: double->float, C4267: size_t->any, C4127: constant condition
  add_compile_options(/W4 $<$<CONFIG:Debug>:/MTd>;$<$<CONFIG:Release>:/MT> /source-charset:utf-8 /execution-charset:utf-8 /wd4251 /wd4244 /wd4305 /wd4267 /wd4127 /bigobj)
  add_compile_options("$<$<CONFIG:Debug>:/Zo>;$<$<CONFIG:Release>:/O2>;$<$<CONFIG:None>:/O2>")
  add_link_options(/NODEFAULTLIB:library)
else()
  add_compile_options(-Wall -Wextra -fPIC)
  add_compile_options(
    "$<$<CONFIG:Debug>:-Og>"
    "$<$<CONFIG:Release>:-O3>"
    "$<$<CONFIG:None>:-O3>"
  )
endif()

include_directories(
  ${PROJECT_SOURCE_DIR}/source
)

# ----- Find dependencies -----
# Set standard installation directories
set(RUNTIME_DESTINATION bin)
set(LIBRARY_DESTINATION lib)
set(ARCHIVE_DESTINATION lib)
set(INCLUDES_DESTINATION include)

# ----- Build -----
add_subdirectory(source)

if (BUILD_PYTHON)
  add_subdirectory(3rdLibs/pybind11)
  add_subdirectory(python)
endif(BUILD_PYTHON)