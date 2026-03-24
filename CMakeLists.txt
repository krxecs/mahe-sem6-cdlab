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

add_subdirectory(lab-01)
add_subdirectory(lab-02)
add_subdirectory(lab-03)
add_subdirectory(lab-04)
add_subdirectory(lab-05)
add_subdirectory(midsem-exam)
add_subdirectory(lab-06)
add_subdirectory(lab-07)
add_subdirectory(lab-08)
add_subdirectory(lab-09)
