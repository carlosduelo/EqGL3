
# Copyright (c) 2015 Carlos Duelo <cduelo@cesvima.upm.es>

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(EqGL3)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake
  ${PROJECT_SOURCE_DIR}/CMake/common)

include(GitExternal)
include(SubProject)

set(VERSION_MAJOR "0")
set(VERSION_MINOR "1")
set(VERSION_PATCH "0")
set(VERSION_ABI 0)

set(PROJECT_INCLUDE_NAME eqGL3)
include(Common)

include(FindPackages)

add_subdirectory(eqGL3)
