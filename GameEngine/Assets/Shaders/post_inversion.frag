#version 330 core

in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uInputTex;

void main()
{
   
    vec3 color = texture(uInputTex, vUV).rgb;

    color = vec3(1.0) - color;

    FragColor = vec4(color, 1.0);
}
