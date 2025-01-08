#version 450 core

precision highp float;

layout (local_size_x = 16, local_size_y = 16) in;

layout (rgba32f, binding = 0) uniform image2D hMap;

layout (rgba32f, binding = 1) uniform image2D input_hMap;


uniform int alg;

uniform int seed;

uniform vec2 resolution;
uniform int iterations;

shared float workGroup[20][20]; //20

uniform int octaves;
uniform float lacunarity;
uniform float persistance;
uniform float noise_scale;
uniform float exponent;


uniform int stepSize;
uniform float scale;

uniform int stepType;



///-----------------------------RANDS-----------------------------///

vec2 hash(vec2 p) {
    p = vec2(dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3)));
    return fract(sin(p) * 43758.5453) * 2.0 - 1.0;
}

float random (in vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}

float rand(vec2 st) {
    return fract(sin(dot(st.xy + float(seed), vec2(12.9898, 78.233))) * 43758.5453);
}

///------------------------------FBM------------------------------///

// Based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
float noise (in vec2 st) {
    

    vec2 i = floor(st);
    vec2 f = fract(st);

    // Four corners in 2D of a tile
    float a = rand(i);
    float b = rand(i + vec2(1.0, 0.0));
    float c = rand(i + vec2(0.0, 1.0));
    float d = rand(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(a, b, u.x) +
            (c - a)* u.y * (1.0 - u.x) +
            (d - b) * u.x * u.y;
}

float fbn (in vec2 st) {
    // Initial values
    st = st * 2.0 - 1.0;

    float value = 0.0;
    float amplitude = .5;
    float frequency = 1.0;
    float weight = 0.0;


    for (int i = 0; i < 6; i++) {
        vec2 ss = (frequency * 2.0 * st);
        value += amplitude * (0.5 * noise(ss));

        weight += amplitude;
        frequency *= 2.0;
        amplitude *= .5;
    }
    float height = value / weight;
    return height;
}


float fbm( in vec2 x, in float H )
{    
    float t = 0.0;
    for(int i= 0; i < 16; i++ )
    {
        float f = pow( 2.0, float(i) );
        float a = pow( f, -H );
        t += a*noise(f*x);
    }
    return t;
}


///------------------------------FF------------------------------///

float ff(in vec2 uv)
{
    float height = 0;

    for (int i = 0; i < iterations; i++) 
    {
        vec2 faultPoint = hash(vec2(float(i), 0.0));

        vec2 direction = normalize(vec2(hash(vec2(float(i) + 1.0, 0.0)).x, 
                                        hash(vec2(float(i) + 2.0, 0.0)).y));

        float dist = dot(uv - faultPoint, direction);

        if (dist > 0.0) {
            height += 1.0 / float(iterations) ;
        } else {
            height -= 1.0 / float(iterations);
        }
    }

    return height;
}



vec4 mean_filter(in ivec2 pixel, in ivec2 kernelSize)
{
    ivec2 halfKernel = kernelSize / 2;

    vec4 sum = vec4(0.0);

    int size = kernelSize.x * kernelSize.y;

    for (int x = -halfKernel.x; x <= halfKernel.x; x++) 
    {
        for (int y = -halfKernel.y; y <= halfKernel.y; y++) 
        {
            // Reflective 
            ivec2 neighborCoord = pixel + ivec2(x, y);

            // corecr edges for clamping
            neighborCoord = clamp(neighborCoord, ivec2(0), ivec2(resolution.xy) - ivec2(1));

            sum += workGroup[neighborCoord.x][neighborCoord.y];
        }
    }

    vec4 mean = sum / float(size);

    return mean;
}


///-------------------------------MDD-------------------------------///

float quantize(float value, float step) {
    return floor(value / step + 0.5) * step;
}

void diamondStep(ivec2 coord, int stepSize, float scale) {
    int halfStep = stepSize / 2;

    float tl = imageLoad(hMap, coord).r;
    float tr = imageLoad(hMap, coord + ivec2(stepSize, 0)).r;
    float bl = imageLoad(hMap, coord + ivec2(0, stepSize)).r;
    float br = imageLoad(hMap, coord + ivec2(stepSize, stepSize)).r;

    float avg = (tl + tr + bl + br) * 0.25;
    float offset = (rand(vec2(coord)) * 2.0 - 1.0) * scale;

    float value = clamp(avg + offset, 0.0, 1.0);

    imageStore(hMap, coord + ivec2(halfStep, halfStep), vec4(value, value, value, 1.0));

    //return value;
}

void squareStep(ivec2 coord, int stepSize, float scale) {
    int halfStep = stepSize / 2;

    float t = imageLoad(hMap, coord + ivec2(0, -halfStep)).r;
    float b = imageLoad(hMap, coord + ivec2(0, halfStep)).r;
    float l = imageLoad(hMap, coord + ivec2(-halfStep, 0)).r;
    float r = imageLoad(hMap, coord + ivec2(halfStep, 0)).r;

    float avg = (t + b + l + r) * 0.25;

    float offset = (rand(vec2(coord)) * 2.0 - 1.0) * scale;

    float value = clamp(avg + offset, 0.0, 1.0);

    imageStore(hMap, coord, vec4(value, value, value, 1.0));

    //return value;

}

void ds(in ivec2 texel_coord)
{
    float height = 0.0;

    int halfStep = stepSize / 2;

    if(stepType == 0)
    {
        if ((texel_coord.x % stepSize == 0) && (texel_coord.y % stepSize == 0)) 
        {
            diamondStep(texel_coord, stepSize, scale);
        }
    }
    if(stepType == 1)
    {
        if ((texel_coord.x % halfStep == 0) && (texel_coord.y % stepSize == 0)) 
        {
            squareStep(texel_coord, stepSize, scale);
        }
        if ((texel_coord.x % stepSize == 0) && (texel_coord.y % halfStep == 0)) 
        {
            squareStep(texel_coord, stepSize, scale);
        }
    }

}

///------------------------------ENTRY------------------------------///

void main() 
{
    ivec2 texel_coord = ivec2(gl_GlobalInvocationID.xy);

    if(texel_coord.x >= resolution.x || texel_coord.y >= resolution.y) {
        return; 
    }

    vec2 uv = (gl_GlobalInvocationID.xy / resolution.xy);

    float height = 0.0;

    if(alg == 0)
    {
        height += fbn(uv);
        imageStore(hMap, texel_coord, vec4(vec3(height), 1.0));
    }
    else if(alg == 1)
    {

        if (texel_coord.x == 0 && texel_coord.y == 0) 
        {
            imageStore(hMap, ivec2(0, 0), vec4(rand(vec2(0.0)), 0.0, 0.0, 1.0));
            imageStore(hMap, ivec2(stepSize, 0), vec4(rand(vec2(1.0)), 0.0, 0.0, 1.0));
            imageStore(hMap, ivec2(0, stepSize), vec4(rand(vec2(2.0)), 0.0, 0.0, 1.0));
            imageStore(hMap, ivec2(stepSize, stepSize), vec4(rand(vec2(3.0)), 0.0, 0.0, 1.0));
        }
        
        ds(texel_coord);
  
    }
    else 
    {
        height += ff(uv);
        imageStore(hMap, texel_coord, vec4(vec3(height), 1.0));
    }


    

}
