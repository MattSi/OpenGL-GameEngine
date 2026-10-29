// color_by_pos.frag
#version 330 core

in vec3 vWorldPos;
in vec2 vTexCoord;

uniform sampler2D uDiffuseMap;

uniform float uColor;  

out vec4 FragColor;

void main()
{
    
	vec3 baseColor = texture(uDiffuseMap, vTexCoord).rgb;
    vec3 t = abs(vWorldPos) *0.2;   
    t = clamp(t, 0.0, 1.0);

    vec3 finalColor = baseColor * (0.0 + 0.7 * t);

    FragColor = vec4(finalColor, 1.0);
}
