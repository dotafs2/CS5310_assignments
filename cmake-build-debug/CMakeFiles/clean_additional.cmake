# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/main_entry_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/main_entry_autogen.dir/ParseCache.txt"
  "CMakeFiles/test6a_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/test6a_autogen.dir/ParseCache.txt"
  "CMakeFiles/test6b_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/test6b_autogen.dir/ParseCache.txt"
  "main_entry_autogen"
  "test6a_autogen"
  "test6b_autogen"
  )
endif()
