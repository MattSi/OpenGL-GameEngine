#version 330 core

in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uColorTex;
uniform sampler2D uDepthTex;

uniform float uNear;
uniform float uFar;

uniform vec3  uNearTint;
uniform vec3  uFarTint;
uniform float uDepthStart;  // 0~1
uniform float uDepthEnd;    // 0~1
uniform float uStrength;    // 0~1

// 把 [0,1] 深度转成线性深度 z
float LinearizeDepth(float depth, float nearPlane, float farPlane)
{
    float z_ndc = depth * 2.0 - 1.0;
    return (2.0 * nearPlane * farPlane) /
           (farPlane + nearPlane - z_ndc * (farPlane - nearPlane));
}

void main()
{
    vec3 color = texture(uColorTex, vUV).rgb;
    float depth = texture(uDepthTex, vUV).r;

    // 线性深度 → 再归一化到 0~1，方便和 uDepthStart/uDepthEnd 对齐
    float z = LinearizeDepth(depth, uNear, uFar);
    float zNorm = clamp((z - uNear) / max(uFar - uNear, 1e-4), 0.0, 1.0);

    // 在 [uDepthStart, uDepthEnd] 范围内做 0~1 插值
    float t = clamp((zNorm - uDepthStart) / max(uDepthEnd - uDepthStart, 1e-4), 0.0, 1.0);

    // 根据深度混合近景 / 远景颜色倾向（对原始颜色做乘法 tint）
    vec3 tint = mix(uNearTint, uFarTint, t);
    vec3 graded = color * tint;

    // 用 Strength 控制整体效果强度
    vec3 finalColor = mix(color, graded, clamp(uStrength, 0.0, 1.0));

    FragColor = vec4(finalColor, 1.0);
}
