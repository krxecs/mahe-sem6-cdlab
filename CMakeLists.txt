cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(cd-lab)
include(cmake/DefaultConfig.cmake)

include(CPM)
CPMUsePackageLock(package-lock.cmake)

CPMGetPackage(uthash)

if (uthash_ADDED)
  FILE(GLOB uthash_sources ${uthash_SOURCE_DIR}/src/*.h)
  add_library(uthash INTERFACE)
  target_include_directories(uthash INTERFACE
    "${uthash_SOURCE_DIR}/src"
  )
  target_sources(uthash INTERFACE ${uthash_sources})
endif ()

CPMGetPackage(Format.cmake)

add_subdirectory(lab-1)
add_subdirectory(lab-2)
add_subdirectory(lab-3)
add_subdirectory(lab-4)
