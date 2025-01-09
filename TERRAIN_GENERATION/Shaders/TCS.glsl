#version 450 core

layout(vertices=4) out;

in vec2 TexCoord[];

out vec2 TextureCoord[];

uniform sampler2D heightMap;
uniform sampler2D normalMap; 

uniform mat4 model;
uniform mat4 view;

uniform float roughWeight;

const int MIN_TESS_LEVEL = 1;
const int MAX_TESS_LEVEL = 8;
const float MIN_DISTANCE = 10;
const float MAX_DISTANCE = 100;


float computeRoughness2(vec2 uv) {

    vec2 texelSize = 1 / vec2(256.0, 256.0);
    float heightL = texture(heightMap, uv + vec2(-1.0, 0.0) * texelSize).r;
    float heightR = texture(heightMap, uv + vec2(1.0, 0.0) * texelSize).r;
    float heightD = texture(heightMap, uv + vec2(0.0, -1.0) * texelSize).r;
    float heightU = texture(heightMap, uv + vec2(0.0, 1.0) * texelSize).r;

    float gradientX = abs(heightR - heightL);
    float gradientY = abs(heightU - heightD);

    float rough = sqrt(gradientX * gradientX + gradientY * gradientY);

    return rough;
}




void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    TextureCoord[gl_InvocationID] = TexCoord[gl_InvocationID];

    if(gl_InvocationID == 0)
    {
        
        vec4 viewSpace00 = view * model * gl_in[0].gl_Position;
        vec4 viewSpace01 = view * model * gl_in[1].gl_Position;
        vec4 viewSpace10 = view * model * gl_in[2].gl_Position;
        vec4 viewSpace11 = view * model * gl_in[3].gl_Position;

        float distance00 = clamp( (abs(viewSpace00.z) - MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0 );
        float distance01 = clamp( (abs(viewSpace01.z) - MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0 );
        float distance10 = clamp( (abs(viewSpace10.z) - MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0 );
        float distance11 = clamp( (abs(viewSpace11.z) - MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0 );

        float tessLevelDist0 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance10, distance00) );
        float tessLevelDist1 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance00, distance01) );
        float tessLevelDist2 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance01, distance11) );
        float tessLevelDist3 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance11, distance10) );


        //vec2 uv00 = TextureCoord[0];
        //vec2 uv01 = TextureCoord[1];
        //vec2 uv10 = TextureCoord[2];
        //vec2 uv11 = TextureCoord[3];

        //float roughness00 = computeRoughness2(uv00);
        //float roughness01 = computeRoughness2(uv01);
        //float roughness10 = computeRoughness2(uv10);
        //float roughness11 = computeRoughness2(uv11);

        //float tessLevelRough0 = mix( MIN_TESS_LEVEL, MAX_TESS_LEVEL, roughness00 );
        //float tessLevelRough1 = mix( MIN_TESS_LEVEL, MAX_TESS_LEVEL, roughness01 );
        //float tessLevelRough2 = mix( MIN_TESS_LEVEL, MAX_TESS_LEVEL, roughness10 );
        //float tessLevelRough3 = mix( MIN_TESS_LEVEL, MAX_TESS_LEVEL, roughness11 );


        //gl_TessLevelOuter[0] = mix(tessLevelDist0, tessLevelRough0, roughWeight);
        //gl_TessLevelOuter[1] = mix(tessLevelDist1, tessLevelRough1, roughWeight);
        //gl_TessLevelOuter[2] = mix(tessLevelDist2, tessLevelRough2, roughWeight);
        //gl_TessLevelOuter[3] = mix(tessLevelDist3, tessLevelRough3, roughWeight);

        gl_TessLevelOuter[0] = tessLevelDist0;
        gl_TessLevelOuter[1] = tessLevelDist1;
        gl_TessLevelOuter[2] = tessLevelDist2;
        gl_TessLevelOuter[3] = tessLevelDist3;

        gl_TessLevelInner[0] = max(gl_TessLevelOuter[1], gl_TessLevelOuter[3]);
        gl_TessLevelInner[1] = max(gl_TessLevelOuter[0], gl_TessLevelOuter[2]);

    }

}