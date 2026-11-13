cmake_minimum_required(VERSION 3.21)

project(kotte LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

option(KOTTE_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)
option(KOTTE_BUILD_TESTS "Build the kotte test suite" ON)

include(cmake/sources.cmake)
include(cmake/external.cmake)

# Mirrors the WarningLevel4 / TreatWarningAsError settings used by the MSVC
# project files, translated to their GCC/Clang equivalents.
#function(kotte_set_project_warnings target)
#    target_compile_options(${target} PRIVATE
#        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra -Wpedantic>
#        $<$<AND:$<CXX_COMPILER_ID:GNU,Clang,AppleClang>,$<BOOL:${KOTTE_WARNINGS_AS_ERRORS}>>:-Werror>
#    )
#endfunction()

#Including the body

add_executable(${PROJECT_NAME} ${APP_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE
	"${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/include"
)

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

target_include_directories(${PROJECT_NAME} PRIVATE
	${PROJECT_SOURCE_DIR}/src
)
target_link_libraries(${PROJECT_NAME} PRIVATE
	${EXTERNAL_LINK_TARGETS}
)

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

target_link_libraries(${PROJECT_NAME} PRIVATE
	${EXTERNAL_INTERFACES}
)

#Warning levels

if(MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive- /utf-8)
else()
	target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
	if(MINGW)
		target_link_options(${PROJECT_NAME} PRIVATE -static)
	endif()
endif()

#exe path

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
