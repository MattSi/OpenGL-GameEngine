#version 330 core

in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uInputTex;
uniform bool uPrecise;

void main()
{
    vec3 color = texture(uInputTex, vUV).rgb;

    float gray;
    if (uPrecise)
    {
        gray = dot(color, vec3(0.2126, 0.7152, 0.0722));
    }
    else
    {
        gray = dot(color, vec3(0.299, 0.587, 0.114));
    }

    FragColor = vec4(gray, gray, gray, 1.0);
}
