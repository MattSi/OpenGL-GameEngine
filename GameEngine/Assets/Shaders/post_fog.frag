#version 330 core

in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uColorTex;
uniform sampler2D uDepthTex;

uniform vec3  uFogColor;
uniform float uFogStart;  // 0~1
uniform float uFogEnd;    // 0~1

void main()
{
    vec3 color = texture(uColorTex, vUV).rgb;
    float depth = texture(uDepthTex, vUV).r; // 

   
    float fogFactor = clamp((depth - uFogStart) / max(uFogEnd - uFogStart, 0.0001), 0.0, 1.0);

    vec3 fogged = mix(color, uFogColor, fogFactor);
    FragColor = vec4(fogged, 1.0);
}
