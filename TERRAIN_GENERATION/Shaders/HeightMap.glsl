#version 450 core

layout (local_size_x = 16, local_size_y = 16) in;

layout (rgba32f, binding = 0) uniform image2D hMap;


uniform vec2 resolution;
uniform int iterations;


uniform int octaves;
uniform float lacunarity;
uniform float persistance;
uniform float noise_scale;
uniform float exponent;



vec2 hash(vec2 p) {
    p = vec2(dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3)));
    return fract(sin(p) * 43758.5453) * 2.0 - 1.0;
}




float random (in vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}

// Based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
float noise (in vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(a, b, u.x) +
            (c - a)* u.y * (1.0 - u.x) +
            (d - b) * u.x * u.y;
}

float fbm (in vec2 st) {
    // Initial values
    float value = 0.0;
    float amplitude = .5;
    float frequency = 0.;
    //
    // Loop of octaves
    for (int i = 0; i < 8; i++) {
        value += amplitude * noise(st);
        st *= 2.;
        amplitude *= .5;
    }
    return value;
}



float ff(in vec2 uv)
{
    float height = 0;

    for (int i = 0; i < iterations; ++i) 
    {
        vec2 faultPoint = hash(vec2(float(i), 0.0));
        vec2 direction = normalize(vec2(hash(vec2(float(i) + 1.0, 0.0)).x, 
                                        hash(vec2(float(i) + 2.0, 0.0)).y));

        float dist = dot(uv - faultPoint, direction);

        if (dist > 0.0) {
            height += 1.0 / float(iterations);
        } else {
            height -= 1.0 / float(iterations);
        }
    }

    return height;
}



void main() {
    ivec2 texel_coord = ivec2(gl_GlobalInvocationID.xy);

    if (texel_coord.x >= resolution.x || texel_coord.y >= resolution.y) {
        return;
    }

    vec2 uv = vec2(gl_GlobalInvocationID.xy) / resolution.xy ;
    float height = 0.0;

    height += ff(uv);

    //height += fbm(uv * 3.0);

    height = (height + 1.0) * 0.5;


    imageStore(hMap, texel_coord, vec4(height, height, height, 1.0));

}
