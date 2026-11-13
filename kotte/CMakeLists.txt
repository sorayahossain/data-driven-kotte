#Warning levels

function (set_warning_levels target)
	if(MSVC)
		target_compile_options(${target} PRIVATE /W4 /permissive- /utf-8)
		if(KOTTE_WARNINGS_AS_ERRORS)
			target_compile_options(${target} PRIVATE /WX)
		endif()
	else()
		target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
		if(KOTTE_WARNINGS_AS_ERRORS)
			target_compile_options(${target} PRIVATE -Werror)
		endif()
		if(MINGW)
			target_link_options(${target} PRIVATE -static)
		endif()
	endif()
endfunction()

# kotte_lib

include(cmake/sources.cmake)
list(REMOVE_ITEM KOTTE_LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

# Everything except main.cpp lives in a library so kotte.tests can link it
# too, without needing to know which .cpp files exist in kotte/src.
add_library(kotte_lib STATIC ${KOTTE_LIB_SOURCES})

target_include_directories(kotte_lib PUBLIC include)

target_link_libraries(kotte_lib PUBLIC raylib cpp_prngs)

#set_warning_levels(kotte_lib)

# kotte

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
	message(FATAL_ERROR
		"src/main.cpp is missing"
	)
else()
	add_executable(${PROJECT_NAME} src/main.cpp)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC include)
	
target_link_libraries(${PROJECT_NAME} PRIVATE kotte_lib)

set_warning_levels(${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
