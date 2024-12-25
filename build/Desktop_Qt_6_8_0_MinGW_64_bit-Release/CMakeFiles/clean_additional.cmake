# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\Toriel-IDE_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Toriel-IDE_autogen.dir\\ParseCache.txt"
  "Toriel-IDE_autogen"
  )
endif()
