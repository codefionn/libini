# Tests

include(CTest)

macro(add_program_test NAME)
    add_executable(${NAME}
        "${PROJECT_SOURCE_DIR}/src/tests/${NAME}.c"
    )
    if (INI_BUILD_DYN)
      target_compile_definitions(${NAME} PRIVATE INI_BUILD_DLL)
    endif()
    target_link_libraries(${NAME} INI)
endmacro()

macro(add_program_status_test NAME)
    add_program_test(${NAME})
    add_test(test_${NAME} ${NAME})
endmacro()

macro(do_test NAME ARGS RESULT)
    add_test(test_${NAME}_${ARGS} ${NAME} ${ARGS})
    set_tests_properties(test_${NAME}_${ARGS}
        PROPERTIES PASS_REGULAR_EXPRESSION ${RESULT})
endmacro()

add_program_status_test(general)
add_program_status_test(write)
add_program_status_test(read_static)
add_program_status_test(read_static_err)
add_program_status_test(read_write_file)
add_program_status_test(iterator)
add_program_status_test(ignore_case)
