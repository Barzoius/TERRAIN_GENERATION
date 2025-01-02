#version 450 core

layout(quads, fractional_odd_spacing, ccw) in;

uniform sampler2D heightMap;
uniform sampler2D normalMap;


uniform mat4 model;           
uniform mat4 view;           
uniform mat4 projection;

in vec2 TextureCoord[];

out float Height;

out vec3 Position;

out vec2 TexCoords;

out vec3 Normal;
out vec3 Tangent;
out vec3 Bitangent;



void main()
{

    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec2 t00 = TextureCoord[0];
    vec2 t01 = TextureCoord[1];
    vec2 t10 = TextureCoord[2];
    vec2 t11 = TextureCoord[3];

    vec2 t0 = (t01 - t00) * u + t00;
    vec2 t1 = (t11 - t10) * u + t10;
    vec2 texCoord = (t1 - t0) * v + t0;

    Height = texture(heightMap, texCoord).r * 64.0 - 16.0;


    vec4 p00 = gl_in[0].gl_Position;
    vec4 p01 = gl_in[1].gl_Position;
    vec4 p10 = gl_in[2].gl_Position;
    vec4 p11 = gl_in[3].gl_Position;


    vec4 uVec = p01 - p00;
    vec4 vVec = p10 - p00;
    vec4 normal = normalize( vec4(cross(vVec.xyz, uVec.xyz), 0) );


    vec4 p0 = (p01 - p00) * u + p00;
    vec4 p1 = (p11 - p10) * u + p10;
    vec4 p = (p1 - p0) * v + p0;

     p.y = 0.5 * Height; 
    // p += normal * Height;

     vec4 pos = model * p;

     Position = (pos).xyz;
    
     TexCoords = texCoord;

     vec3 nRBG = texture(normalMap, texCoord).rbg;

     vec3 n = nRBG * 2.0 - 1.0;


     vec3 deltaE1 = uVec.xyz;
     vec3 deltaE2 = vVec.xyz;

     vec2 deltaU = t01 - t00;
     vec2 deltaV = t10 - t00;

     float f = 1.0 / (deltaU.x * deltaV.y - deltaU.y * deltaV.y);

     Tangent = normalize(f * (deltaV.y * deltaE1 - deltaU.y * deltaE2));
     Normal = normalize(n);
     Bitangent = -normalize(cross(normalize(Normal), Tangent));

     gl_Position = projection * view * model * p;

}
