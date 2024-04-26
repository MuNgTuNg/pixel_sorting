#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glad.c>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm.hpp>
#include <gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <vector>
#include <windows.h>
#include <algorithm>

#include <shaders.hpp>
#include <gui_style.hpp>

GLFWwindow* window = nullptr;

GLuint imageTexture;
unsigned char* imageData = nullptr;
int imageWidth, imageHeight, nrChannels;


int guiWidth = 200;
int guiHeight = 0;

std::string fileName = "";
    

std::vector<float> defaultQuadVerts{
    // positions         // texture coords
    // positions         // texture coords
    -1.f,  1.f, 0.f,       0.f, 1.f, //top left front
    -1.f, -1.f, 0.f,       0.f, 0.f, //bottom left front
     1.f, -1.f, 0.f,       1.f, 0.f, //bottom right front
    -1.f,  1.f, 0.f,       0.f, 1.f, //top left front
     1.f, -1.f, 0.f,       1.f, 0.f, //bottom right front
     1.f,  1.f, 0.f,       1.f, 1.f,  //top right front 
};
std::vector<unsigned int> defaultQuadIndices{
    1,2,3,
    2,5,0
};


int initApp(){
    glfwInit();

    //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); 
    window = glfwCreateWindow(1,1, "Pixel Sorting", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    glGenTextures(1, &imageTexture);
    glBindTexture(GL_TEXTURE_2D, imageTexture);
   
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    stbi_set_flip_vertically_on_load(true);  

    imageData = stbi_load(fileName.c_str(), &imageWidth, &imageHeight, &nrChannels, 0);
    if (imageData) {
        if(nrChannels == 3){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData); 
        }
        if(nrChannels == 4){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData); 
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture " + fileName << std::endl;
    }
    //stbi_image_free(imageData); //we don't want to do this just yet

    glfwSetWindowSize(window, imageWidth + guiWidth, imageHeight);

    glViewport(100, 0, imageWidth + guiWidth, imageHeight + guiHeight);

    ImGui::CreateContext();
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    guiHeight = imageHeight;
    
}

bool shutDown(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}



void mainLoop(){
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

    unsigned int vertexBuffer = 0;
    glGenBuffers(1,&vertexBuffer);
    unsigned int indexBuffer = 0;
    glGenBuffers(1,&indexBuffer);

    unsigned int vertexArray;
    glGenVertexArrays(1, &vertexArray);  
    glBindVertexArray(vertexArray);

    glBindBuffer(GL_ARRAY_BUFFER,vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,defaultQuadVerts.size() * sizeof(float),&defaultQuadVerts[0],GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);  

    glBindBuffer(GL_ARRAY_BUFFER,0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,defaultQuadIndices.size() * sizeof(unsigned int),&defaultQuadIndices[0],GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    
   
    bool demoWindowRender = false;
    double mousePosX, mousePosY;
    int resolutionWidth, resolutionHeight;

    while(!glfwWindowShouldClose(window))
    {
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwGetCursorPos(window, &mousePosX, &mousePosY);
        glfwGetFramebufferSize(window, &resolutionWidth, &resolutionHeight);

        styleGUI();


        glViewport(0, 0, resolutionWidth, resolutionHeight);
        shaderProgramBackground.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, imageTexture);
        float time = glfwGetTime();
        glUniform1f(shaderProgramBackground.uniformLoc("time"),time);
        glUniform2f(shaderProgramBackground.uniformLoc("resolution"),resolutionWidth,resolutionHeight);
        
        glBindVertexArray(vertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glDrawElements(GL_TRIANGLES,defaultQuadIndices.size(),GL_UNSIGNED_INT,0);
        


        glViewport(guiWidth, 0, imageWidth, imageHeight);

        shaderProgramTexture.use();
        glUniform2f(shaderProgramTexture.uniformLoc("mousePosIn"),mousePosX,-mousePosY+resolutionHeight);
        glUniform1f(shaderProgramTexture.uniformLoc("time"),glfwGetTime());
        glUniform2f(shaderProgramTexture.uniformLoc("resolution"),resolutionWidth,resolutionHeight);
        glActiveTexture(GL_TEXTURE0);   
        glBindTexture(GL_TEXTURE_2D, imageTexture);

        glBindVertexArray(vertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glDrawElements(GL_TRIANGLES,defaultQuadIndices.size(),GL_UNSIGNED_INT,0);
        
        glViewport(0, 0, imageWidth+guiWidth, imageHeight);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        //ImGui::DockSpaceOverViewport();
        ImGui::SetNextWindowSize(ImVec2(guiWidth, imageHeight));
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::Begin("Controls", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
        if (ImGui::Button("Reload shaders")) {
            shaderProgramBackground.reload();
            shaderProgramTexture.reload();
        }
        ImGui::Checkbox("Demo Window?",&demoWindowRender);
        ImGui::Text("Mouse X: %.0f\nMouse Y: %.0f",mousePosX,mousePosY);
        ImGui::Text("Resolution: (%d,%d)",resolutionWidth,resolutionHeight);
        ImGui::Text("Time: %f\n",time);
        ImGui::End();

        if(demoWindowRender){
            ImGui::ShowDemoWindow();
        }

        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
    
    glDeleteBuffers(1,&vertexBuffer);
    glDeleteBuffers(1,&indexBuffer);
    glDeleteVertexArrays(1,&vertexArray);
  
}

int main(int argc, char* argv[]){
    if(argc == 0){
        printf("Please provide command line args\n");
        return -1;
    }
    else{
        fileName = argv[1];
    }

    initApp();
    mainLoop();
    shutDown();
    return 0;
}