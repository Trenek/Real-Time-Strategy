set(SOURCE_DIRECTORIES
    "${CMAKE_SOURCE_DIR}/source"
    "${CMAKE_SOURCE_DIR}/source/camera"
    "${CMAKE_SOURCE_DIR}/source/graphics"
        "${CMAKE_SOURCE_DIR}/source/graphics/buffers"
        "${CMAKE_SOURCE_DIR}/source/graphics/setup"
            "${CMAKE_SOURCE_DIR}/source/graphics/setup/window"
            "${CMAKE_SOURCE_DIR}/source/graphics/setup/GraphicsSetup"
                "${CMAKE_SOURCE_DIR}/source/graphics/setup/GraphicsSetup/functions"
                    "${CMAKE_SOURCE_DIR}/source/graphics/setup/GraphicsSetup/functions/bufferOperations"
                    "${CMAKE_SOURCE_DIR}/source/graphics/setup/GraphicsSetup/functions/commonOperations"
                    "${CMAKE_SOURCE_DIR}/source/graphics/setup/GraphicsSetup/functions/imageOperations"
                    "${CMAKE_SOURCE_DIR}/source/graphics/setup/GraphicsSetup/functions/depthHelpers"
                "${CMAKE_SOURCE_DIR}/source/graphics/setup/GraphicsSetup/queueFamilyIndices"
                "${CMAKE_SOURCE_DIR}/source/graphics/setup/GraphicsSetup/swapChain"
                "${CMAKE_SOURCE_DIR}/source/graphics/setup/GraphicsSetup/swapChainSupportDetails"
            "${CMAKE_SOURCE_DIR}/source/graphics/setup/deltaTime"
            "${CMAKE_SOURCE_DIR}/source/graphics/setup/windowControl"
        "${CMAKE_SOURCE_DIR}/source/graphics/model"
            "${CMAKE_SOURCE_DIR}/source/graphics/model/builder"
                "${CMAKE_SOURCE_DIR}/source/graphics/model/builder/loader"
                "${CMAKE_SOURCE_DIR}/source/graphics/model/builder/type"
            "${CMAKE_SOURCE_DIR}/source/graphics/model/functions"
    "${CMAKE_SOURCE_DIR}/source/basicThings"
    "${CMAKE_SOURCE_DIR}/source/state"
    "${CMAKE_SOURCE_DIR}/source/button"
    "${CMAKE_SOURCE_DIR}/source/army"
        "${CMAKE_SOURCE_DIR}/source/army/cohord"
)

list(TRANSFORM SOURCE_DIRECTORIES APPEND "/*.c" OUTPUT_VARIABLE SOURCE_FILES)
file(GLOB SOURCE_FILES ${SOURCE_FILES})

set(SOURCE_FILES ${SOURCE_FILES} PARENT_SCOPE)
set(SOURCE_DIRECTORIES ${SOURCE_DIRECTORIES} PARENT_SCOPE)