#version 330 core

in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uColorTex;
uniform sampler2D uDepthTex;

uniform vec2  uTexelSize;
uniform float uNear;
uniform float uFar;

// 物理参数（先保留接口）
uniform float uFocalLengthMm;   // 焦距（mm）
uniform float uFStop;           // f-number
uniform float uFocusDistance;   // 对焦距离（和场景单位一致）
uniform float uMaxBlurRadius;   // 最大模糊半径（像素）

// 把 [0,1] 深度转成线性深度（相机空间距离，单位≈场景单位）
float LinearizeDepth(float depth, float nearPlane, float farPlane)
{
    float z_ndc = depth * 2.0 - 1.0; // [0,1] -> [-1,1]
    return (2.0 * nearPlane * farPlane) /
           (farPlane + nearPlane - z_ndc * (farPlane - nearPlane));
}

// 这个函数返回 模糊半径（像素）
float ComputeBlurRadius(float z)
{
    // 与焦点距离的相对偏差：z = focus 时为 0，远离时增大
    float cocNorm = abs(z - uFocusDistance) / max(uFocusDistance, 1e-4);

    // 根据镜头参数稍微调一下强度（越大光圈，越容易虚）
    float lensScale = (uFocalLengthMm / 50.0) * (2.8 / uFStop);
    cocNorm *= lensScale;

    // 限制在 [0,1] 再映射到像素半径
    cocNorm = clamp(cocNorm, 0.0, 1.0);
    return cocNorm * uMaxBlurRadius;
}

// Poisson / ring 采样点（单位圆）
const int SAMPLE_COUNT = 8;
const vec2 samplePattern[SAMPLE_COUNT] = vec2[](
    vec2( 0.0,  0.0),
    vec2( 0.5,  0.1),
    vec2(-0.4,  0.3),
    vec2( 0.3, -0.5),
    vec2(-0.6, -0.2),
    vec2( 0.1,  0.7),
    vec2( 0.7, -0.6),
    vec2(-0.7,  0.5)
);

void main()
{
    vec3 centerColor = texture(uColorTex, vUV).rgb;
    float depth = texture(uDepthTex, vUV).r;

    // 1) 线性深度（单位≈世界空间的“米”/“单位”）
    float z = LinearizeDepth(depth, uNear, uFar);

    // 2) 基于 z 和 focusDistance 的模糊半径（像素）
    float blurRadius = ComputeBlurRadius(z);

    // 焦点附近：直接用原图
    if (blurRadius < 0.5)
    {
        FragColor = vec4(centerColor, 1.0);
        return;
    }

    // 3) 按 blurRadius 做散焦采样
    vec3 accum = centerColor;
    float weightSum = 1.0;

    for (int i = 1; i < SAMPLE_COUNT; ++i)
    {
        vec2 offset = samplePattern[i] * uTexelSize * blurRadius;
        vec3 c = texture(uColorTex, vUV + offset).rgb;

        float w = 1.0; // 可以以后再按距离调整权重
        accum += c * w;
        weightSum += w;
    }

    vec3 blurColor = accum / max(weightSum, 1e-4);

    // 4) 根据模糊半径在 [0, uMaxBlurRadius] 的位置决定混合程度
    float blend = clamp(blurRadius / uMaxBlurRadius, 0.0, 1.0);
    vec3 finalColor = mix(centerColor, blurColor, blend);

    FragColor = vec4(finalColor, 1.0);
	
}
