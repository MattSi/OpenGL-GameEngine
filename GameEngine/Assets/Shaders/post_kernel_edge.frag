#version 330 core

in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uInputTex;
uniform vec2  uTexelSize;
uniform float uStrength;   // 0 ~ 3

void main()
{
    vec3 center = texture(uInputTex, vUV).rgb;

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
           1,  1,  1,
			1, -8,  1,
			1,  1,  1
    );

    vec3 result = vec3(0.0);
    for (int i = 0; i < 9; ++i)
    {
        vec2 uv = vUV + offsets[i] * uTexelSize;
        result += texture(uInputTex, uv).rgb * kernel[i];
    }

   
    float s = clamp(uStrength, 0.0, 3.0);
    vec3 finalColor = mix(center, result, s);

    FragColor = vec4(finalColor, 1.0);
}
