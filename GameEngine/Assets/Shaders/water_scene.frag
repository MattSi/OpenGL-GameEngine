#version 330 core

in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uInputTex;
uniform float uFrequency;
uniform float uAmplitude;
uniform float uTime;
uniform bool uTintBlueEnabled;

void main(){
	vec2 uv = vUV;
	
	float wave1 = sin(uv.y * uFrequency * 3.0 + uTime*1.5);
	float wave2 = cos(uv.x * uFrequency * 2.0 - uTime * 1.1);
	
	vec2 offset = vec2(wave1, wave2) * (uAmplitude*0.1 );
	
	vec2 distortedUV = uv + offset;
	distortedUV = clamp(distortedUV, 0.0, 1.0);
	
	vec3 col = texture(uInputTex, distortedUV).rgb;
	
	if(uTintBlueEnabled){
		vec3 blueTint = vec3(0.7, 0.8, 1.2);
		col *= blueTint;
	}
	FragColor = vec4(col, 1.0);
}