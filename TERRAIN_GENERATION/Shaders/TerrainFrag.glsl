#version 450 core

in float Height;

out vec4 FragColor;

layout (binding = 0) uniform sampler2DArray ALBEDO;
//layout (binding = 2) uniform sampler2DArray AO;
//layout (binding = 3) uniform sampler2DArray ROUGHNESS;

//layout (binding = 4) uniform sampler2DArray NORMALS;
//layout (binding = 5) uniform sampler2DArray HEIGHTS;


uniform vec3 camOrigin;

in vec3 Position;

in vec2 TexCoords;

void main()
{
	float h = (Height + 16)/64.0f;

    float ambient = 0.35f;

    vec3 lightOrigin = vec3(0.0, 30.0, 0.0);

    vec3 normal = normalize(vec3(0.0, 1.0, 0.0));
    vec3 lightDir = normalize(lightOrigin - Position);

    float diffuse  = max(dot(normal, lightDir), 0.0f);

    float specLight = 0.50f;
    vec3 eyeDir = normalize(camOrigin - Position);
    vec3 reflectedRay = reflect(-lightDir, normal);

    float specIntensity =  pow(max(dot(eyeDir, reflectedRay), 0.0f), 8);

    float spec = specIntensity * specLight;

    //FragColor = vec4(vec3(0.5, h, 0.7), 1.0)* (diffuse + ambient + spec);

	//vec2 texCoords = gl_FragCoord.xy / vec2(2048, 2048);

    vec4 albedoColor = texture(ALBEDO, vec3(TexCoords, 1));

    FragColor = vec4(albedoColor.rgba)* (diffuse + ambient + spec);
}