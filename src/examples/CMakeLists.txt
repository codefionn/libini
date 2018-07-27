# Examples

macro (add_example NAME)
  add_executable(example_${NAME}
      ${PROJECT_SOURCE_DIR}/src/examples/${NAME}.c
  )
  target_link_libraries(example_${NAME} INI)
endmacro()

add_example(parse_ini)
add_example(to_winnl)
