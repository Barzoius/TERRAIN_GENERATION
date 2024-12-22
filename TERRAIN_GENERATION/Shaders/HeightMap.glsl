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

vec2 randomGradient(vec2 p) {
    // Generate a random gradient using hash and normalize
    float angle = random(p) * 6.28318; // Random angle (0 to 2*PI)
    return vec2(cos(angle), sin(angle));
}

float pnoise(vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);

    vec2 g00 = randomGradient(i);
    vec2 g10 = randomGradient(i + vec2(1.0, 0.0));
    vec2 g01 = randomGradient(i + vec2(0.0, 1.0));
    vec2 g11 = randomGradient(i + vec2(1.0, 1.0));

    vec2 d00 = f - vec2(0.0, 0.0);
    vec2 d10 = f - vec2(1.0, 0.0);
    vec2 d01 = f - vec2(0.0, 1.0);
    vec2 d11 = f - vec2(1.0, 1.0);

    float n00 = dot(g00, d00);
    float n10 = dot(g10, d10);
    float n01 = dot(g01, d01);
    float n11 = dot(g11, d11);

    vec2 u = smoothstep(0.0, 1.0, f);

    // Interpolate
    float nx0 = mix(n00, n10, u.x);
    float nx1 = mix(n01, n11, u.x);
    return mix(nx0, nx1, u.y);
}


float vnoise(vec2 p) {
    vec2 i = floor(p); 
    vec2 f = fract(p); 

    vec2 u = smoothstep(0.0, 1.0, f);

    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));


    return mix(
        mix(a, b, u.x),         
        mix(c, d, u.x),          
        u.y                      
    );
}


vec2 rotateCoords(vec2 p, float angle) {
    float s = sin(angle);
    float c = cos(angle);
    return mat2(c, -s, s, c) * p;
}

vec2 skewedCoords(vec2 p) {
    mat2 skew = mat2(1.0, 0.5, -0.5, 1.0); // Apply a skewing matrix
    return skew * p;
}

float fbm(vec2 st) {
    st = skewedCoords(st); 
    float value = 0.0;
    float amplitude = 0.5;
    float frequency = 1.0;

    for (int i = 0; i < 16; i++) {
        value += amplitude * pnoise(st * frequency);
        st *= 2.0;
        amplitude *= 0.5;
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

    vec2 uv = vec2(texel_coord) / resolution.xy ;
    float height = 0.0;

    //height += ff(uv);

    height += fbm(uv * 4.0);

    //height = (height + 1.0) * 0.5;


    imageStore(hMap, texel_coord, vec4(height, height, height, 1.0));

}
