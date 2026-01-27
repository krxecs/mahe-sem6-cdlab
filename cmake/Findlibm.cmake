find_path(libm_INCLUDE_DIRS
  NAMES math.h
  PATHS /usr/include /usr/local/include /usr/local/bic/include
  NO_DEFAULT_PATH
)
find_library(libm_LIBRARIES m)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(libm DEFAULT_MSG libm_LIBRARIES libm_INCLUDE_DIRS)
mark_as_advanced(libm_INCLUDE_DIRS libm_LIBRARIES)

if(libm_FOUND)
  if(NOT TARGET libm::libm)
    add_library(libm::libm UNKNOWN IMPORTED)
    set_target_properties(libm::libm PROPERTIES
      IMPORTED_LOCATION "${libm_LIBRARIES}"
      INTERFACE_INCLUDE_DIRECTORIES "${libm_INCLUDE_DIRS}")
  endif()
endif()
