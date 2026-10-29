#version 330 core

in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uColorTex;
uniform sampler2D uDepthTex;
uniform vec2  uTexelSize;

uniform float uFocusDepth; // 0~1
uniform float uFocusRange; // 0~1
uniform float uMaxBlur;    // 

float ComputeBlurFactor(float depth)
{
    float d = abs(depth - uFocusDepth);
    float f = clamp(d / max(uFocusRange, 0.0001), 0.0, 1.0);
    return f;
}

void main()
{
    vec3 center = texture(uColorTex, vUV).rgb;
    float depth = texture(uDepthTex, vUV).r;

    float blurFactor = ComputeBlurFactor(depth); // 0 in focus, 1 max blur
    float radius = blurFactor * uMaxBlur;

    // 9-tap 简易模糊，半径按 depth 动态缩放
    vec2 offsets[9] = vec2[](
        vec2(-1.0,  1.0),
        vec2( 0.0,  1.0),
        vec2( 1.0,  1.0),

        vec2(-1.0,  0.0),
        vec2( 0.0,  0.0),
        vec2( 1.0,  0.0),

        vec2(-1.0, -1.0),
        vec2( 0.0, -1.0),
        vec2( 1.0, -1.0)
    );

    float kernel[9] = float[](
        1.0/16.0, 2.0/16.0, 1.0/16.0,
        2.0/16.0, 4.0/16.0, 2.0/16.0,
        1.0/16.0, 2.0/16.0, 1.0/16.0
    );

    vec3 blurCol = vec3(0.0);
    for (int i = 0; i < 9; ++i)
    {
        vec2 uv = vUV + offsets[i] * uTexelSize * radius;
        blurCol += texture(uColorTex, uv).rgb * kernel[i];
    }

    // in-focus 保持原色，out-of-focus 在 blurCol 和 center 之间混合
    vec3 finalColor = mix(center, blurCol, blurFactor);
    FragColor = vec4(finalColor, 1.0);
}
