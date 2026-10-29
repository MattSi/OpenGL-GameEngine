#version 330 core

in vec3 vTexDir;
out vec4 FragColor;

uniform samplerCube uSkybox;

// 1 = no cubemap textures were found; draw a procedural space sky instead.
uniform int uProcedural;

// ---------------------------------------------------------------------------
// Procedural space sky
// Everything is evaluated from the 3D view direction, so there are no seams
// between cube faces and no texture assets are needed.
// ---------------------------------------------------------------------------

// Integer hash (PCG-style), returns three values in [0, 1].
vec3 hash3(vec3 p)
{
    uvec3 v = uvec3(ivec3(floor(p)) + ivec3(1 << 20));
    v = v * 1664525u + 1013904223u;
    v.x += v.y * v.z; v.y += v.z * v.x; v.z += v.x * v.y;
    v ^= v >> 16u;
    v.x += v.y * v.z; v.y += v.z * v.x; v.z += v.x * v.y;
    return vec3(v) * (1.0 / 4294967295.0);
}

// Smooth 3D value noise in [0, 1].
float valueNoise(vec3 p)
{
    vec3 i = floor(p);
    vec3 f = fract(p);
    vec3 u = f * f * (3.0 - 2.0 * f);

    float n000 = hash3(i + vec3(0, 0, 0)).x;
    float n100 = hash3(i + vec3(1, 0, 0)).x;
    float n010 = hash3(i + vec3(0, 1, 0)).x;
    float n110 = hash3(i + vec3(1, 1, 0)).x;
    float n001 = hash3(i + vec3(0, 0, 1)).x;
    float n101 = hash3(i + vec3(1, 0, 1)).x;
    float n011 = hash3(i + vec3(0, 1, 1)).x;
    float n111 = hash3(i + vec3(1, 1, 1)).x;

    return mix(mix(mix(n000, n100, u.x), mix(n010, n110, u.x), u.y),
               mix(mix(n001, n101, u.x), mix(n011, n111, u.x), u.y), u.z);
}

// Fractal noise: 5 octaves, normalized to roughly [0, 1].
float fbm(vec3 p)
{
    float sum = 0.0;
    float amp = 0.5;
    for (int i = 0; i < 5; ++i)
    {
        sum += amp * valueNoise(p);
        p = p * 2.03 + vec3(1.7, 9.2, 3.1);
        amp *= 0.5;
    }
    return sum / 0.96875;
}

// One layer of stars: at most one star per grid cell, kept away from cell
// edges so it is never clipped. density = fraction of cells with a star.
float starLayer(vec3 dir, float scale, float density, float radius)
{
    vec3 p = dir * scale;
    vec3 cell = floor(p);
    vec3 h = hash3(cell);
    if (h.x > density)
        return 0.0;

    vec3 starPos = cell + 0.25 + 0.5 * hash3(cell + 17.0);
    float d = length(p - starPos);
    float brightness = 0.4 + 0.6 * h.y;
    return brightness * smoothstep(radius, 0.0, d);
}

vec3 proceduralSky(vec3 dir)
{
    // Large-scale nebula shape and a second field for color variation.
    float shape  = fbm(dir * 1.8);
    float detail = fbm(dir * 4.5 + vec3(5.2, 1.3, 7.7));
    float nebula = smoothstep(0.42, 0.78, shape * 0.75 + detail * 0.25);

    vec3 deepSpace = vec3(0.006, 0.007, 0.016);
    vec3 dust      = vec3(0.50, 0.26, 0.11);   // warm orange-brown
    vec3 violet    = vec3(0.20, 0.11, 0.36);

    vec3 color = deepSpace;
    color += mix(violet, dust, detail) * nebula * 0.55;
    color += dust * pow(nebula, 3.0) * detail * 0.45;   // brighter core

    // Dense faint stars plus sparse bright ones; dimmed inside thick nebula.
    float stars = starLayer(dir, 160.0, 0.30, 0.22) * 0.55
                + starLayer(dir, 55.0, 0.07, 0.16) * 1.00;
    color += vec3(stars) * (1.0 - 0.6 * nebula) * vec3(0.95, 0.97, 1.0);

    return color;
}

void main()
{
    vec3 dir = normalize(vTexDir);

    vec3 color;
    if (uProcedural == 1)
        color = proceduralSky(dir);
    else
        color = texture(uSkybox, dir).rgb;

    FragColor = vec4(color, 1.0);
}
