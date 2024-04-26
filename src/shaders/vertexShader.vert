#version 330 core

layout ( location = 0 ) in vec3 aPos;
layout ( location = 1 ) in vec2 aTex;
uniform vec2 mousePosIn;

out vec3 pos;
out vec2 mousePos;
out vec2 texCoord;
void main()
{   

    mousePos = mousePosIn;
    pos = aPos;
    texCoord = aTex;
    gl_Position = vec4(aPos,1.f);
}