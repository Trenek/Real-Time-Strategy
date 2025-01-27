project(Tester VERSION 1.0 LANGUAGES C)

include_directories(${SOURCE_DIRECTORIES})


set(T_SOURCE_DIRECTORIES
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

list(TRANSFORM T_SOURCE_DIRECTORIES APPEND "/*.c" OUTPUT_VARIABLE T_SOURCE_FILES)
file(GLOB T_SOURCE_FILES ${T_SOURCE_FILES})

add_executable(Tester ${SOURCE_FILES} ${T_SOURCE_FILES})

set(FLAGS
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
        -D_UCRT
        )

target_compile_options(Tester PUBLIC ${FLAGS})

target_include_directories(Tester PRIVATE ${INCLUDE_DIRECTORIES})
target_link_libraries(Tester ${LINK_LIBRARIES})
target_compile_features(Tester PRIVATE c_std_23)