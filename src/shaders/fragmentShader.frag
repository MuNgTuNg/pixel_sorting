#version 330 core

out vec4 FragColor;
in vec3 pos;
in vec2 mousePos;
in vec2 texCoord;

//uniform float time;
uniform vec2 resolution;

uniform sampler2D tex;

float circle(vec2 samplePosition, float radius){
    return length(samplePosition) - radius;
}


void main()
{
    vec2 normalizedCoords = (2.0 * gl_FragCoord.xy - resolution) / min(resolution.x, resolution.y);

   
    float distanceToMouse = length(gl_FragCoord.xy - mousePos);

    if(abs(distanceToMouse - 100) < 10){
        FragColor =  vec4(1,0,0,1);
    }
    else{
        FragColor = texture(tex,texCoord);
        
    }
    // ivec2 dest = ivec2(floor(gl_FragCoord.x), floor(gl_FragCoord.y));
    //if(mod(dest.x, 100) == 0 && mod(dest.y, 100) == 0){
    //    color = vec4(sin(pos.x),cos(pos.y),0,1.f);
    //}
    //else{
    //    FragColor =  vec4(sin(gl_FragCoord.x),cos(gl_FragCoord.y),1,0);
    //}

    //FragColor = color;
}