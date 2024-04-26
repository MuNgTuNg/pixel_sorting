cmake_minimum_required(VERSION 3.2)
project(pixel_sorting)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



add_executable(${PROJECT_NAME} src/main.cpp)

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(external)


target_include_directories(${PROJECT_NAME} PUBLIC
${CMAKE_SOURCE_DIR}/external/glfw/include
${CMAKE_SOURCE_DIR}/external/glad/include
${CMAKE_SOURCE_DIR}/external/glad/src
${CMAKE_SOURCE_DIR}/external/imgui
${CMAKE_SOURCE_DIR}/external/glm
${CMAKE_SOURCE_DIR}/external/stb
${CMAKE_SOURCE_DIR}/include
PUBLIC ${PARENT_DIR}../include
)


target_link_libraries(${PROJECT_NAME} PRIVATE
    includes 
    src
    stb
    imgui
    glfw
    glad
    glm
)

