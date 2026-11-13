cmake_minimum_required(VERSION 3.25)

set(PROJECT_NAME kotte)
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

option(KOTTE_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)
option(KOTTE_BUILD_TESTS "Build the kotte test suite" ON)

include(cmake/external.cmake)

#Including submodules

foreach(dep IN LISTS EXTERNAL_DEPS)
	if(NOT EXISTS "${EXTERNAL_DIR}/${dep}/CMakeLists.txt")
		message(FATAL_ERROR
			"External dependency '${dep}' is missing. "
			"Run: git submodule update --init --recursive"
		)
	endif()

	add_subdirectory("${EXTERNAL_DIR}/${dep}" SYSTEM) # Because I am not touching the libraries
endforeach()

#Include interface modules

foreach(dep IN LISTS EXTERNAL_INTERFACES)
	if(NOT EXISTS "${EXTERNAL_DIR}/${dep}/include")
		message(FATAL_ERROR
			"External precompiled dependency '${dep}' is missing. "
		)
	endif()

	add_library(${dep} INTERFACE)
	target_include_directories(${dep} SYSTEM INTERFACE "${EXTERNAL_DIR}/${dep}/include")
endforeach()

#Including the body

add_subdirectory(kotte)

#Test

if(KOTTE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(kotte.tests)
endif()
