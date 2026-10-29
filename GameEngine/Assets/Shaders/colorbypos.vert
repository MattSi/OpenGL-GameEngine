
// color_by_pos.vert
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;     
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aTangent;    

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec3 vWorldPos;
out vec2 vTexCoord;

void main()
{
    
    // vec3 _unusedTangent = aTangent;
    // vec3 _unusedNormal  = aNormal;

    vec4 worldPos = uModel * vec4(aPos, 1.0);
    vWorldPos = worldPos.xyz;
    vTexCoord = aTexCoord;

    gl_Position = uProjection * uView * worldPos;
}
