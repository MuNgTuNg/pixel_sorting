
add_subdirectory(glfw)
add_subdirectory(imgui)
add_subdirectory(glm)

add_library( glad INTERFACE
    glad/include/glad/glad.h
    glad/include/KHR/khrplatform.h
)
add_library( stb INTERFACE
    stb/stb_image.h
    stb/stb_image_write.h
)