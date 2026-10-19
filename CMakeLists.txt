cmake_minimum_required(VERSION 3.21)

project(kotte LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

option(KOTTE_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)
option(KOTTE_BUILD_TESTS "Build the kotte test suite" ON)

# Mirrors the WarningLevel4 / TreatWarningAsError settings used by the MSVC
# project files, translated to their GCC/Clang equivalents.
function(kotte_set_project_warnings target)
    target_compile_options(${target} PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra -Wpedantic>
        $<$<AND:$<CXX_COMPILER_ID:GNU,Clang,AppleClang>,$<BOOL:${KOTTE_WARNINGS_AS_ERRORS}>>:-Werror>
    )
endfunction()

# --- vendor/cpp_prngs: header-only library -----------------------------
add_library(cpp_prngs INTERFACE)
target_include_directories(cpp_prngs INTERFACE ${CMAKE_SOURCE_DIR}/vendor/cpp_prngs/include)

# --- vendor/raylib -------------------------------------------------------
# The repository only ships prebuilt Windows binaries for raylib, so on
# Linux/macOS we use whatever raylib the system already provides (e.g.
# `brew install raylib` or a distro package) and fall back to building it
# from source if none is found.
set(KOTTE_RAYLIB_GIT_TAG "6.0" CACHE STRING "raylib git tag/commit to fetch when no system install is found")

find_package(raylib ${KOTTE_RAYLIB_GIT_TAG} QUIET)
if(NOT raylib_FOUND)
    message(STATUS "System raylib not found; fetching raylib ${KOTTE_RAYLIB_GIT_TAG} from source")

    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

    include(FetchContent)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG ${KOTTE_RAYLIB_GIT_TAG}
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(raylib)
endif()

add_subdirectory(kotte)

if(KOTTE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(kotte.tests)
endif()
