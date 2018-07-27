cmake_minimum_required(VERSION 2.6)
project(INI)

include_directories("${PROJECT_SOURCE_DIR}/include")

set(INI_SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/ini_pair.c"
                     "${PROJECT_SOURCE_DIR}/src/ini_section.c"
                     "${PROJECT_SOURCE_DIR}/src/ini_interpreter.c"
                     "${PROJECT_SOURCE_DIR}/src/ini_iterator.c"
                     "${PROJECT_SOURCE_DIR}/src/ini.c")

option(INI_BUILD_DYN "Build dynamic library (.ddl,.so)" OFF)

#if (INI_BUILD_DYN)
#  add_library(INI SHARED ${INI_SOURCE_FILES})
#else
  add_library(INI STATIC ${INI_SOURCE_FILES})
#endif()

# Tests

include(CTest)

macro(add_program_test NAME)
    add_executable(test_${NAME}
        ${PROJECT_SOURCE_DIR}/src/tests/${NAME}.c
    )
    target_link_libraries(test_${NAME} INI)   
endmacro()

macro(add_program_status_test NAME)
    add_program_test(${NAME})
    add_test(test_${NAME} test_${NAME})
endmacro()

macro(do_test NAME ARGS RESULT)
    add_test(test_${NAME}_${ARGS} test_${NAME} ${ARGS})
    set_tests_properties(test_${NAME}_${ARGS}
        PROPERTIES PASS_REGULAR_EXPRESSION ${RESULT})
endmacro()

add_program_status_test(general)
add_program_status_test(write)
add_program_status_test(read_static)
add_program_status_test(read_static_err)
add_program_status_test(read_write_file)
add_program_status_test(iterator)
