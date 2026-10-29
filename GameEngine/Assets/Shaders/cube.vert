#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;


uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;


out VS_OUT
{
    vec3 FragPos;   // world position
    vec3 Normal;    // world position normal
    vec2 TexCoord;  // UV
} vs_out;

void main()
{
	vec4 worldPos   = uModel * vec4(aPosition, 1.0);
    vs_out.FragPos  = worldPos.xyz;
	
	mat3 normalMat  = transpose(inverse(mat3(uModel)));
    vs_out.Normal   = normalize(normalMat * aNormal);
	
	vs_out.TexCoord = aTexCoord;
    gl_Position = uProjection * uView * worldPos;
}
