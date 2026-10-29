#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 vTexDir;

uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    mat4 viewNoTrans = mat4(mat3(uView));

    vTexDir = aPos;
	 

    vec4 clipPos = uProjection * viewNoTrans * vec4(aPos, 1.0);
    gl_Position = clipPos;
    gl_Position.z = gl_Position.w; 
}
