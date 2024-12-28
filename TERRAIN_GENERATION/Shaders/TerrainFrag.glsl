#version 450 core

in float Height;

out vec4 FragColor;

layout (binding = 0) uniform sampler2DArray ALBEDO;
//layout (binding = 2) uniform sampler2DArray AO;
//layout (binding = 3) uniform sampler2DArray ROUGHNESS;

//layout (binding = 4) uniform sampler2DArray NORMALS;
//layout (binding = 5) uniform sampler2DArray HEIGHTS;


uniform vec3 camOrigin;
uniform vec3 lightOrigin;

in vec3 Position;

in vec2 TexCoords;

in vec3 Normal;
in vec3 Tangent;
in vec3 Bitangent;


mat3 TBN;

void get_tbn()
{
    vec3 Q1  = dFdx(Position);
    vec3 Q2  = dFdy(Position);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);
    
    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    TBN = mat3(T, B, N);
}



void main()
{

    get_tbn();
   // mat3 TBN = mat3(Tangent, Bitangent, Normal);

	float h = (Height + 16)/64.0f;

    float ambient = 0.1f;

    vec3 normal = Normal;

    normal = normalize(TBN * normal);

    vec3 lightDir = normalize(lightOrigin - Position);

  
    float diffuse  = max(dot(normal, lightDir), 0.0f);

    float specLight = 0.50f;
    vec3 eyeDir = normalize(camOrigin - Position);
    vec3 reflectedRay = reflect(-lightDir, normal);

    float specIntensity =  pow(max(dot(eyeDir, reflectedRay), 0.0f), 3);

    float spec = specIntensity * specLight;


    vec4 albedoColor = texture(ALBEDO, vec3(Position.xz, 1));

    FragColor = vec4(albedoColor.rgba) * (diffuse + ambient + spec);

}