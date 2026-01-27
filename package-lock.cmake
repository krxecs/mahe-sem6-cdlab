# CPM Package Lock
# This file should be committed to version control

# uthash
CPMDeclarePackage(uthash
  NAME uthash
  VERSION 2.3.0
  DOWNLOAD_ONLY YES
  GIT_REPOSITORY https://github.com/troydhanson/uthash
)
# Format.cmake
CPMDeclarePackage(Format.cmake
  VERSION 1.8.3
  GITHUB_REPOSITORY TheLartians/Format.cmake
  SYSTEM YES
  EXCLUDE_FROM_ALL YES
  OPTIONS
    # set to yes skip cmake formatting
    "FORMAT_SKIP_CMAKE YES"
    # path to exclude (optional, supports regular expressions)
    "CMAKE_FORMAT_EXCLUDE cmake/CPM.cmake"
)
