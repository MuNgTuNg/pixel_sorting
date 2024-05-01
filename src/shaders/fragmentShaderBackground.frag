#version 330 core

out vec4 FragColor;
in vec2 texCoord;

uniform float time;
uniform vec2 resolution;
uniform sampler2D tex;


float noisej( vec3 x ) {
    vec3 p = floor(x),f = fract(x);

    f = f*f*(3.-2.*f);  // or smoothstep     // to make derivative continuous at borders

#define hash3(p)  fract(sin(1e3*dot(p,vec3(1,57,-13.7)))*4375.5453)        // rand
    
    return mix( mix(mix( hash3(p+vec3(0,0,0)), hash3(p+vec3(1,0,0)),f.x),       // triilinear interp
                    mix( hash3(p+vec3(0,1,0)), hash3(p+vec3(1,1,0)),f.x),f.y),
                mix(mix( hash3(p+vec3(0,0,1)), hash3(p+vec3(1,0,1)),f.x),       
                    mix( hash3(p+vec3(0,1,1)), hash3(p+vec3(1,1,1)),f.x),f.y), f.z);
}

#define noise(x) (noisej(x)+noisej(x+11.5)) / 2.0 // pseudoperlin improvement from foxes idea 

// float noise(vec3 x){
//     return noisekkk(x) + noisekkk(x+11.5)/2.0;
// }

void main()
{
    vec2 U = gl_FragCoord.xy;
    vec2 R = resolution;

    float n = noise(vec3(U * 8.0 / R.y, 0.1 * time));
    float v = sin(6.28 * 10.0 * n);
    float t = time;
    
    float k = abs(v)/fwidth(v);
    v = smoothstep(1.0, 0.0, 0.5 * k);

    vec4 sinColor = 0.5 + 0.5 * sin(12.0 * n + vec4(0.0, 2.1, -2.1, 0.0));
    vec4 texColor = texture(tex, (U + vec2(1.0, sin(t))) / R);
    FragColor = mix(exp(-33.0 / R.y) * texColor, sinColor, v);


    ivec2 dest = ivec2(floor(gl_FragCoord.x), floor(gl_FragCoord.y));
    if(mod(dest.x, 2) == 0 || mod(dest.y, 2) == 0){
       //FragColor *= vec4(0.1,0.3,0,1.f);
       
    }
    else{
       //FragColor =  vec4(0.0,0.0,0.0,1);
    }
    
    FragColor *= 0.5;
}