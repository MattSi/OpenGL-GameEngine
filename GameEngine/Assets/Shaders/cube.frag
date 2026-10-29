#version 330 core

#define MAX_POINT_LIGHTS  8
#define MAX_SPOT_LIGHTS   8

struct DirectionalLight {
    vec3 direction;
    float intensity;
    vec3 color;
    float pad0; // Align
};

struct PointLight {
    vec3 position;
    float intensity;
    vec3 color;
    float range;
    vec3 attenuation;   // x = constant, y = linear, z = quadratic
    float pad0;         // Align
};

struct SpotLight {
    vec3 position;
    float intensity;
    vec3 color;
    float range;
    vec3 direction;     // light direction
    float innerCutoff;  // cos(innerAngle)
    float outerCutoff;  // cos(outerAngle)
	float pad0;  //Align
	float pad1;		//Align
	float pad2;		//Align
};

uniform DirectionalLight uDirectionalLight;
uniform int uNumPointLights;
uniform PointLight uPointLights[MAX_POINT_LIGHTS];
uniform int uNumSpotLights;
uniform SpotLight uSpotLights[MAX_SPOT_LIGHTS];

uniform vec3 uViewPos; // Camera position

// ============= Material =========================
uniform vec3  uMaterialTint;            // tint
uniform float uMaterialShininess;       // shininess
uniform float uMaterialSpecularStrength;// specularStrength
uniform float uMaterialEmissiveStrength;// emissiveStrength

// =============== Texture samplers ================
uniform sampler2D uDiffuseMap;
uniform bool uHasDiffuseMap;

uniform sampler2D uSpecularMap;
uniform bool uHasSpecularMap;

uniform sampler2D uEmissionMap;
uniform bool uHasEmissionMap;

in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} fs_in;

out vec4 FragColor;

uniform vec3 uColor;   // Pass by C++

vec3 GetBaseColor()
{
	vec3 baseColor = uMaterialTint;
	if(uHasDiffuseMap)
	{
		baseColor *= texture(uDiffuseMap, fs_in.TexCoord).rgb;
	}
	return baseColor;
}

vec3 GetSpecularColor()
{
	vec3 specColor = vec3(1.0);
	if (uHasSpecularMap){
		specColor = texture(uSpecularMap, fs_in.TexCoord).rgb;
	}
	return specColor;
}

vec3 GetEmissiveColor(vec3 baseColor)
{
	vec3 emissive = baseColor * uMaterialEmissiveStrength;
	
	if (uHasEmissionMap){
		emissive += texture(uEmissionMap, fs_in.TexCoord).rgb * uMaterialEmissiveStrength;
	}
	return emissive;
}

// ================= Now Lighting ===========================
vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 baseColor, vec3 specColor)
{
	vec3 lightDir = normalize(-light.direction);
	float diff 	= max(dot(normal, lightDir),0.0);
	
	// Blinn - Phong
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), uMaterialShininess);
	
	vec3 diffuse = diff * baseColor *light.color;
	vec3 specular = spec * uMaterialSpecularStrength * specColor * light.color ;
	
	return light.intensity * (diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 baseColor, vec3 specColor)
{
	vec3 toLight   = light.position - fs_in.FragPos;
    float distance = length(toLight);
    vec3 lightDir  = normalize(toLight);
    
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec      = pow(max(dot(normal, halfwayDir), 0.0), uMaterialShininess);

    float attenuation =
        1.0 / (light.attenuation.x +
               light.attenuation.y * distance +
               light.attenuation.z * distance * distance);

    //
    float rangeFactor = smoothstep(light.range, 0.0, distance); // near-> 1，far ->0
    attenuation *= rangeFactor;

    vec3 diffuse  = diff  * baseColor * light.color;
    vec3 specular = spec * uMaterialSpecularStrength * specColor * light.color;

    return light.intensity * attenuation * (diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 baseColor, vec3 specColor)
{
    vec3 toFrag   = fs_in.FragPos - light.position;
    float distance = length(toFrag);
    vec3 lightDir  = normalize(-toFrag); 
    
    float theta   = dot(lightDir, normalize(-light.direction)); // 
    float epsilon = light.innerCutoff - light.outerCutoff;
    float spotIntensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

    if (spotIntensity <= 0.0)
        return vec3(0.0);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec      = pow(max(dot(normal, halfwayDir), 0.0), uMaterialShininess);

    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);
    float rangeFactor = smoothstep(light.range, 0.0, distance);
    attenuation *= rangeFactor;

    vec3 diffuse  = diff  * baseColor * light.color;
    vec3 specular = spec * uMaterialSpecularStrength * specColor * light.color;

    return light.intensity * attenuation * spotIntensity * (diffuse + specular);
}

// ===================== main =========================
void main()
{
	vec3 normal = normalize(fs_in.Normal);
	vec3 viewDir = normalize(uViewPos - fs_in.FragPos);
	vec3 baseColor = GetBaseColor();
	vec3 specColor = GetSpecularColor();
	vec3 emissive = GetEmissiveColor(baseColor);
	
	vec3 lighting = vec3(0.0);
	
	//Directional Light
	lighting += CalcDirectionalLight(uDirectionalLight, normal, viewDir, baseColor, specColor);
	
	// Point Lights
	for(int i=0; i< uNumPointLights; ++i)
	{
		lighting += CalcPointLight(uPointLights[i], normal, viewDir, baseColor, specColor);
	}
	
	// Spot Lights
	for(int i=0; i< uNumSpotLights; i++)
	{
		lighting += CalcSpotLight(uSpotLights[i], normal, viewDir, baseColor, specColor);
	}
	
	vec3 finalColor = lighting + emissive;
	
    FragColor = vec4(finalColor, 1.0);
}
