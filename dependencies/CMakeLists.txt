add_library(munit STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/munit/munit.c"
)
target_compile_features(munit PRIVATE c_std_11)
target_include_directories(munit PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/munit"
)

add_library(unity STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/unity/unity.c"
)
target_include_directories(unity PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/unity"
)

set(INCLUDE_DIRECTORIES
    "${Vulkan_INCLUDE_DIRS}"
    "${CMAKE_CURRENT_SOURCE_DIR}/cglm/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/stb"
    "${CMAKE_CURRENT_SOURCE_DIR}/tinyobjloader"
    "${CMAKE_CURRENT_SOURCE_DIR}/cgltf"
    "${CMAKE_CURRENT_SOURCE_DIR}/munit"
    "${CMAKE_CURRENT_SOURCE_DIR}/unity"
)

set(LINK_LIBRARIES
    "${Vulkan_LIBRARIES}"
    munit
    unity
)

if (WIN32)
    set(INCLUDE_DIRECTORIES
        ${INCLUDE_DIRECTORIES}
        "${CMAKE_CURRENT_SOURCE_DIR}/glfw-3.4/include"
    )

    set(LINK_LIBRARIES
        ${LINK_LIBRARIES}
        "${CMAKE_CURRENT_SOURCE_DIR}/glfw-3.4/src/libglfw3.a"
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