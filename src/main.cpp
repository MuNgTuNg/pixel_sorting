#include <shaders.hpp>
#include <gui.hpp>
#include <textures.hpp>
#include <vertices.hpp>
#include <buffers.hpp>
#include <error.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glad.c>

#include <glm.hpp>
#include <gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <windows.h>
#include <algorithm>

GLFWwindow* g_Window = nullptr;
std::string g_FileName = "";


int initGraphics(const std::string& windowTitle ){
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }
    //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); 
    g_Window = glfwCreateWindow(1,1, windowTitle.c_str(), NULL, NULL);
    if (g_Window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(g_Window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    
    initGuiOpenGL3(g_Window,"#version 330");
    stbi_set_flip_vertically_on_load(true);  
}

void mainLoop(){
    int guiWidth = 200;
    int guiHeight = 0;

    shb::ImageData imageData{g_FileName.c_str()};
    shb::ImageData imageDataUnsorted{g_FileName.c_str()};
    guiHeight = imageData.height();
    //stbi_image_free(imageData.m_Data); //we don't want to do this just yet

    glfwSetWindowSize(g_Window, imageData.width() + guiWidth, imageData.height());
    glViewport(100, 0, imageData.width() + guiWidth, imageData.height() + guiHeight);


    unsigned int textureHandle = 0;
    shb::loadTexture(imageData,&textureHandle);
    shb::ShaderProgram shaderProgramTexture{
        {
            "fragmentShaderTexture.frag", 
            "vertexShaderTexture.vert"
        }
    };

    shb::ShaderProgram shaderProgramBackground{
        {
            "fragmentShaderBackground.frag",
            "vertexShaderBackground.vert"
        }
    };

    shb::Buffer vertexBuffer{shb::defaultQuadVerts, GL_ARRAY_BUFFER};
    shb::Buffer indexBuffer{shb::defaultQuadIndices, GL_ELEMENT_ARRAY_BUFFER};
    shb::VertexArray vertexArray{};

    vertexArray.bind();
    vertexBuffer.bind();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);  
    vertexArray.unbind();
    vertexBuffer.unbind();

    bool demoWindowRender = false;
    double mousePosX, mousePosY;
    int resolutionWidth, resolutionHeight;

    while(!glfwWindowShouldClose(g_Window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwGetCursorPos(g_Window, &mousePosX, &mousePosY);
        glfwGetFramebufferSize(g_Window, &resolutionWidth, &resolutionHeight);

        styleGUI();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        //ImGui::DockSpaceOverViewport();
        ImGui::SetNextWindowSize(ImVec2(guiWidth, imageData.height()));
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::Begin("Controls", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
        g_ImGuiFrameBegin = true;

        

        shaderProgramBackground.use();
        float time = glfwGetTime();
        glUniform1f(shaderProgramBackground.uniformLoc("time"),time);
        glUniform2f(shaderProgramBackground.uniformLoc("resolution"),resolutionWidth,resolutionHeight);
        glViewport(0, 0, resolutionWidth, resolutionHeight);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureHandle);
       
        //drawQuad()
        vertexArray.bind();
        vertexBuffer.bind();
        indexBuffer.bind();
        glDrawElements(GL_TRIANGLES,indexBuffer.numElements(),GL_UNSIGNED_INT,0);
        
        glViewport(guiWidth, 0, imageData.width(), imageData.height());

        shaderProgramTexture.use();
        glUniform2f(shaderProgramTexture.uniformLoc("mousePosIn"),mousePosX,-mousePosY+resolutionHeight);
        glUniform1f(shaderProgramTexture.uniformLoc("time"),glfwGetTime());
        glUniform2f(shaderProgramTexture.uniformLoc("resolution"),resolutionWidth,resolutionHeight);
        glDrawElements(GL_TRIANGLES,indexBuffer.numElements(),GL_UNSIGNED_INT,0);
        
        glViewport(0, 0, imageData.width()+guiWidth, imageData.height());
        ImGui::Text("Image Height: %d\nImage width: %d\nChannels: %d\n",imageData.height(), imageData.width(),imageData.numChannels());
        if (ImGui::CollapsingHeader("Window"))
        {
            ImGui::Text("Mouse X: %.0f\nMouse Y: %.0f",mousePosX,mousePosY);
            ImGui::Text("Resolution: (%d,%d)",resolutionWidth,resolutionHeight);
            ImGui::Text("Time: %f\n",time);
        }
        if (ImGui::CollapsingHeader("Sorting"))
        {
            shb::pickSortingMethod();
            if (ImGui::Button("Sort pixels")) {
                shb::sortPixels(imageData);
                shb::loadTexture(imageData,&textureHandle);
            }
            if (ImGui::Button("Restore image")) {
                shb::loadTexture(imageDataUnsorted,&textureHandle);
            }
        }
        if (ImGui::Button("Reload shaders")) {
            shaderProgramBackground.reload();
            shaderProgramTexture.reload();
        }
    
        ImGui::Checkbox("Demo Window?",&demoWindowRender);
        ImGui::End();

        if(demoWindowRender){
            ImGui::ShowDemoWindow();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(g_Window);
        glfwPollEvents();    
    }
    //CHECK_GL_ERROR();

}

bool shutDownGraphics(){
    shutdownGUIOpenGL3();
    glfwDestroyWindow(g_Window);
    glfwTerminate();
}

int main(int argc, char* argv[]){
    if(argc == 0){
        printf("Please provide command line args\n");
        return -1;
    }
    else{
        g_FileName = argv[1];
    }
    initGraphics("Pixel Sorting");
    mainLoop();
    shutDownGraphics();
    return 0;
}