cmake_minimum_required(VERSION 3.10)

project(Test VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

find_package(Vulkan REQUIRED)

set(DIRECTORIES
    "${CMAKE_SOURCE_DIR}/source"
    "${CMAKE_SOURCE_DIR}/source/queueFamilyIndices"
    "${CMAKE_SOURCE_DIR}/source/setup"
    "${CMAKE_SOURCE_DIR}/source/swapChainSupportDetails"
    "${CMAKE_SOURCE_DIR}/source/functions"
)

include_directories(${DIRECTORIES})

list(TRANSFORM DIRECTORIES APPEND "/*.c" OUTPUT_VARIABLE SOURCE_FILES)
file(GLOB SOURCE_FILES ${SOURCE_FILES})

add_executable(Test ${SOURCE_FILES})

target_compile_options(Test PUBLIC
    -O3
    -g
    -W
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wunused
    -Werror
    -Wno-comment
    -pedantic
    -pedantic-errors
    -D_GLIBCXX_DEBUG
    -D_GLIBCXX_DEBUG_PEDANTIC
)

get_target_property(MAIN_CFLAGS Test COMPILE_OPTIONS)

add_custom_command(TARGET Test 
    POST_BUILD COMMAND echo built with the flags: ${MAIN_CFLAGS}
)

if (WIN32)
    target_include_directories(Test PRIVATE 
        "${Vulkan_INCLUDE_DIRS}"
        "${CMAKE_SOURCE_DIR}/dependencies/glfw-3.4/include"
    )

    target_link_libraries(Test
        "${Vulkan_LIBRARIES}"
        "${CMAKE_SOURCE_DIR}/dependencies/glfw-3.4/src/libglfw3.a"
    )
else ()
    target_include_directories(Test PRIVATE
        "${Vulkan_INCLUDE_DIRS}"
    )

    target_link_libraries(Test
        "${Vulkan_LIBRARIES}"
        glfw
    )
endif ()
