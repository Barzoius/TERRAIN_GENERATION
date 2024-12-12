#version 450 core

layout (local_size_x = 15, local_size_y = 16) in;

layout (rgba32f, binding = 0) uniform image2D hMap;

uniform vec2 resolution;
uniform int iterations;


vec2 hash(vec2 p) {
    p = vec2(dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3)));
    return fract(sin(p) * 43758.5453) * 2.0 - 1.0;
}

void main() {
    ivec2 texel_coord = ivec2(gl_GlobalInvocationID.xy);

    if (texel_coord.x >= resolution.x || texel_coord.y >= resolution.y) {
        return;
    }

    vec2 uv = vec2(texel_coord) / resolution;
    float height = 0.0;

    for (int i = 0; i < iterations; ++i) {
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

    // Normalize height to [0.0, 1.0]
    height = (height + 1.0) * 0.5;

   // imageStore(hMap, texel_coord, vec4(height, height, height, 1.0));

    imageStore(hMap, texel_coord, vec4(1.0, 0.0, 0.0, 1.0)); 
}
