cmake_minimum_required(VERSION 2.6)
project(INI)

include_directories("${PROJECT_SOURCE_DIR}/include")

set(INI_SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/ini_pair.c"
                     "${PROJECT_SOURCE_DIR}/src/ini_section.c"
                     "${PROJECT_SOURCE_DIR}/src/ini_interpreter.c"
                     "${PROJECT_SOURCE_DIR}/src/ini_iterator.c"
                     "${PROJECT_SOURCE_DIR}/src/ini.c")

option(INI_BUILD_DYN "Build dynamic library (.ddl,.so)" OFF)

if (INI_BUILD_DYN)
  add_library(INI SHARED ${INI_SOURCE_FILES})
else()
  add_library(INI STATIC ${INI_SOURCE_FILES})
endif()

# Example
add_subdirectory("${PROJECT_SOURCE_DIR}/src/examples" examples)

# Tests
add_subdirectory("${PROJECT_SOURCE_DIR}/src/tests/" tests)
