cmake_minimum_required(VERSION 2.6)
project(INI)

include_directories("${INI_SOURCE_DIR}/include")

set(INI_SOURCE_FILES "${INI_SOURCE_DIR}/src/ini_pair.c"
                     "${INI_SOURCE_DIR}/src/ini_section.c"
                     "${INI_SOURCE_DIR}/src/ini_interpreter.c"
                     "${INI_SOURCE_DIR}/src/ini_iterator.c"
                     "${INI_SOURCE_DIR}/src/ini_util.c"
                     "${INI_SOURCE_DIR}/src/ini.c")

option(INI_BUILD_DYN "Build dynamic library (.ddl,.so)" OFF)

if (INI_BUILD_DYN)
  add_library(INI SHARED ${INI_SOURCE_FILES})
  target_compile_definitions(INI PRIVATE _INI_BUILD_DLL)
else()
  add_library(INI STATIC ${INI_SOURCE_FILES})
endif()

# Example
add_subdirectory("${INI_SOURCE_DIR}/src/examples" examples)

# Tests
add_subdirectory("${INI_SOURCE_DIR}/src/tests/" tests)
