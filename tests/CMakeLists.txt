project(Tester VERSION 1.0 LANGUAGES C)

include_directories(${SOURCE_DIRECTORIES})
add_executable(Tester ${SOURCE_FILES} test_main.c test_button.c test_cohord.c test_cohordModels.c test_displacement.c test_fightBuilder.c test_movement.c)

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