# CONFIGURE_DEPENDS makes CMake re-check this glob on every build and
# reconfigure automatically if files were added/removed under src/.
file(GLOB_RECURSE KOTTE_TESTS_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(kotte_tests ${KOTTE_TESTS_SOURCES})

# Linking kotte_lib brings in kotte/include, raylib and cpp_prngs
# automatically, so entity_store.cpp (and anything else kotte_lib contains)
# never needs to be listed here by hand.
target_link_libraries(kotte_tests PRIVATE kotte_lib)

set_target_properties(kotte_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

add_test(NAME EntityStoreTests COMMAND kotte_tests)
