#version 330 core

in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uInputTex;
uniform vec2 uTexelSize;  // 1/width, 1/height
uniform float uStrength;  // 0~1

void main()
{
    vec3 centerColor = texture(uInputTex, vUV).rgb;

    float kernel[9] = float[](
        1.0/16.0, 2.0/16.0, 1.0/16.0,
        2.0/16.0, 4.0/16.0, 2.0/16.0,
        1.0/16.0, 2.0/16.0, 1.0/16.0
    );

    vec2 offsets[9] = vec2[](
        vec2(-1.0, -1.0),
        vec2( 0.0, -1.0),
        vec2( 1.0, -1.0),

        vec2(-1.0,  0.0),
        vec2( 0.0,  0.0),
        vec2( 1.0,  0.0),

        vec2(-1.0,  1.0),
        vec2( 0.0,  1.0),
        vec2( 1.0,  1.0)
    );

    vec3 blurColor = vec3(0.0);

    for (int i = 0; i < 9; ++i)
    {
        vec2 sampleUV = vUV + offsets[i] * uTexelSize*uStrength;
        blurColor += texture(uInputTex, sampleUV).rgb * kernel[i];
    }

    vec3 finalColor = mix(centerColor, blurColor, clamp(uStrength, 0.0, 1.0));
    FragColor = vec4(finalColor, 1.0);
}
