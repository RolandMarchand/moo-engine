include_directories("${CMAKE_SOURCE_DIR}/src")
add_custom_target(tests DEPENDS
  test_vector
  test_level
)
add_custom_target(test_and_run COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS tests)

add_executable(test_vector test_vector.c)
target_link_libraries(test_vector PRIVATE structures)

add_executable(test_level test_level.c)
target_link_libraries(test_level PRIVATE structures)

add_test(Vector test_vector)
add_test(Level test_level)
