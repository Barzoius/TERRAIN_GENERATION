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

float random (in vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453123);
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

    vec2 uv = vec2(texel_coord) / resolution.xy ;
    float height = 0.0;

    height += ff(uv);

    //height += fbm(uv * 4.0);

    height = (height + 1.0) * 0.5;


    imageStore(hMap, texel_coord, vec4(height, height, height, 1.0));

}
