#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

uniform int uUseInstancing;
uniform int uInstancesPerRow;
uniform float uInstanceSpacing;
uniform vec3 uRandomMin;
uniform vec3 uRandomMax;
uniform int uRandomRotation;
uniform int uRandomScale;

out VS_OUT
{
    vec3 FragPos;   // world position
    vec3 Normal;    // world position normal
    vec2 TexCoord;  // UV
	mat3 TBN;
} vs_out;


uint hash_u(uint x){
	x += 0x6D2B79F5u;
	x = (x^(x>> 15)) * (x | 1u);
	x ^= x + (x ^ (x >> 7)) * (x | 61u);
	return x ^ (x >> 14);
}
float rand01(int seed){
	return float(hash_u(uint(seed))) * 2.3283064365387e-10;
}

void main()
{
	vec3 localPos  = aPosition;
	vec3 localN = aNormal;
	vec3 localT = aTangent;
	if(uUseInstancing == 1)
	{
		int instanceID = gl_InstanceID;
		int row = instanceID / uInstancesPerRow;
		int col = instanceID % uInstancesPerRow;

		float baseX = (float(col) - float(uInstancesPerRow) * 0.5) * uInstanceSpacing;
		float baseZ = (float(row) - float(uInstancesPerRow) * 0.5) * uInstanceSpacing;

		float jx = (rand01(instanceID*3) -0.5) * uInstanceSpacing;
		float jz = (rand01(instanceID*7) -0.5) * uInstanceSpacing;

		float addY = mix(uRandomMin.y, uRandomMax.y, rand01(instanceID*13));
		vec3 instanceOffset = vec3(baseX + jx, addY, baseZ + jz);

		float scale = 1.0;
		if(uRandomScale == 1)
		{
			scale = mix(0.8, 1.2, rand01(instanceID*17));
		}

		float angle = 0.0;
		if(uRandomRotation == 1)
		{
			angle =rand01(instanceID*23) * 6.28318530718; // 2*PI
		}

		float c = cos(angle);
		float s = sin(angle);
		mat2 rot = mat2(c, -s,s, c);

		vec3 p = localPos * scale;
		p.xz = rot * p.xz;
		localPos = p + instanceOffset;

		vec3 n = localN;
		n.xz = rot * n.xz;
		localN = normalize(n);

		vec3 t = localT;
		t.xz = rot * t.xz;	
		localT = normalize(t);

		//worldPosLocal += instanceOffset;

	}


	vec4 worldPos   = uModel * vec4(localPos , 1.0);
    vs_out.FragPos  = worldPos.xyz;
	
	mat3 normalMat  = transpose(inverse(mat3(uModel)));
	vec3 worldN = normalize(normalMat * localN);
	vec3 worldT = normalize(normalMat * localT);
	vec3 B = normalize(cross(worldN,worldT));
	vs_out.TBN = mat3(worldT, B, worldN);
	
    vs_out.Normal   = worldN;	
	vs_out.TexCoord = aTexCoord;
	
    gl_Position = uProjection * uView * worldPos;
}
