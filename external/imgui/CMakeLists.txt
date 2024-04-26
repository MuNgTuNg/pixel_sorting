file(GLOB_RECURSE HEADER_FILES_C ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB_RECURSE HEADER_FILES_CPP ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE C_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)


add_library(imgui 
    imconfig.h
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_impl_opengl3_loader.h
    imgui_impl_opengl3.h
    imgui_impl_opengl3.cpp
    imgui_internal.h
    imgui_tables.cpp
    imgui_widgets.cpp
    imgui.cpp
    imgui.h
    imstb_rectpack.h
    imstb_textedit.h
    imstb_truetype.h
    imgui_impl_glfw.h
    imgui_impl_glfw.cpp

    # ${HEADER_FILES_C}
    # ${HEADER_FILES_CPP}
    # ${C_FILES}
    # ${CPP_FILES}
)


target_include_directories(imgui PRIVATE
    ${CMAKE_SOURCE_DIR}/external/glfw/include
)

