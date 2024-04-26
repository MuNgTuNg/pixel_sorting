

add_library( includes INTERFACE
    shaders.hpp
    files.hpp
    gui.hpp
    textures.hpp
    vertices.hpp
    buffers.hpp
)   
include_directories(
    ${CMAKE_SOURCE_DIR}/external/glfw/include
    ${CMAKE_SOURCE_DIR}/external/glad/include
)