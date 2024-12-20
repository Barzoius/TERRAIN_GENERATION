#version 450 core

in float Height;

out vec4 FragColor;

layout (binding = 0) uniform sampler2DArray ALBEDO;
//layout (binding = 2) uniform sampler2DArray AO;
//layout (binding = 3) uniform sampler2DArray ROUGHNESS;

//layout (binding = 4) uniform sampler2DArray NORMALS;
//layout (binding = 5) uniform sampler2DArray HEIGHTS;



void main()
{
	float h = (Height + 16)/64.0f;
   
     vec2 texCoords = gl_FragCoord.xy / vec2(64, 64);

    vec4 albedoColor = texture(ALBEDO, vec3(texCoords, 2));

    FragColor = vec4(albedoColor.rgba);
}