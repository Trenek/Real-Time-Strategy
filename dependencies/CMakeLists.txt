add_library(munit STATIC
    "${CMAKE_SOURCE_DIR}/dependencies/munit/munit.c"
)
target_compile_features(munit PRIVATE c_std_11)
target_include_directories(munit PUBLIC "${CMAKE_SOURCE_DIR}/dependencies/munit")

set(INCLUDE_DIRECTORIES
    "${Vulkan_INCLUDE_DIRS}"
    "${CMAKE_SOURCE_DIR}/dependencies/cglm/include"
    "${CMAKE_SOURCE_DIR}/dependencies/stb"
    "${CMAKE_SOURCE_DIR}/dependencies/tinyobjloader"
    "${CMAKE_SOURCE_DIR}/dependencies/cgltf"
    "${CMAKE_SOURCE_DIR}/dependencies/munit"
)

set(LINK_LIBRARIES
    "${Vulkan_LIBRARIES}"
    munit
)

if (WIN32)
    set(INCLUDE_DIRECTORIES
        ${INCLUDE_DIRECTORIES}
        "${CMAKE_SOURCE_DIR}/dependencies/glfw-3.4/include"
    )

    set(LINK_LIBRARIES
        ${LINK_LIBRARIES}
        "${CMAKE_SOURCE_DIR}/dependencies/glfw-3.4/src/libglfw3.a"
        -lucrt
    )
else ()
    set(LINK_LIBRARIES
        ${LINK_LIBRARIES}
        glfw
        -lm
    )
endif()

set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} PARENT_SCOPE)
set(LINK_LIBRARIES ${LINK_LIBRARIES} PARENT_SCOPE)