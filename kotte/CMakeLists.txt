# CONFIGURE_DEPENDS makes CMake re-check this glob on every build and
# reconfigure automatically if files were added/removed under src/.
file(GLOB_RECURSE KOTTE_LIB_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)
list(REMOVE_ITEM KOTTE_LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

# Everything except main.cpp lives in a library so kotte.tests can link it
# too, without needing to know which .cpp files exist in kotte/src.
add_library(kotte_lib STATIC ${KOTTE_LIB_SOURCES})

target_include_directories(kotte_lib PUBLIC include)

target_link_libraries(kotte_lib PUBLIC raylib cpp_prngs)

# raylib's own headers (e.g. raymath.h) trigger -Wextra/-Wpedantic warnings;
# re-declaring them as SYSTEM (and PUBLIC, so it reaches kotte.tests too)
# keeps our own code held to the strict settings below without failing on
# vendored code we don't control.
target_include_directories(kotte_lib SYSTEM PUBLIC
    $<TARGET_PROPERTY:raylib,INTERFACE_INCLUDE_DIRECTORIES>
)

kotte_set_project_warnings(kotte_lib)

add_executable(kotte src/main.cpp)

target_link_libraries(kotte PRIVATE kotte_lib)

kotte_set_project_warnings(kotte)

set_target_properties(kotte PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
