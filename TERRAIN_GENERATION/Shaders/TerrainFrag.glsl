#version 450 core

in float Height;

out vec4 FragColor;

layout (binding = 0) uniform sampler2DArray ALBEDO;

layout (binding = 3) uniform sampler2DArray AO;

layout (binding = 4) uniform sampler2DArray NORMALS;

//layout (binding = 5) uniform sampler2DArray ROUGHNESS;


//layout (binding = 6) uniform sampler2DArray HEIGHTS;


uniform vec3 camOrigin;
uniform vec3 lightOrigin;

uniform bool triplanar;

in vec3 Position;

in vec2 TexCoords;

in vec3 Normal;
in vec3 Tangent;
in vec3 Bitangent;

mat3 TBN;

void vTBN()
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


vec4 TRIPLANAR_MAPPING(int texture)
{
    vec3 blend = abs(Normal);
    blend = normalize(max(blend,  0.00001));

    float b = blend.x + blend.y + blend.z;

    blend /= vec3(b,b,b);

    vec3 xTex = texture(ALBEDO, vec3(Position.yz * .8, texture)).rgb;
    vec3 yTex = texture(ALBEDO, vec3(Position.xz * .8, texture)).rgb;
    vec3 zTex = texture(ALBEDO, vec3(Position.xy * .8, texture)).rgb;

    vec4 Tex = vec4(xTex * blend.x + yTex * blend.y + zTex * blend.z, 1.0);

    return Tex;
}


vec4 TRIPLANAR_NORMAL_MAPPING(int texture)
{
    vec3 blend = abs(Normal);
    blend = normalize(max(blend,  0.00001));

    float b = blend.x + blend.y + blend.z;

    blend /= vec3(b,b,b);

    vec3 xN = 2.0 * texture(NORMALS, vec3(Position.yz * .6, texture)).rgb - 1.0;
    vec3 yN = 2.0 * texture(NORMALS, vec3(Position.xz * .6, texture)).rgb - 1.0;
    vec3 zN = 2.0 * texture(NORMALS, vec3(Position.xy * .6, texture)).rgb - 1.0;

    vec4 N = vec4(xN * blend.x + yN * blend.y + zN * blend.z, 1.0);

    return N;
}



vec3 DirectionLight(vec3 n) 
{
    // Normalize the normal
    vec3 normal = normalize(n);

    // Light direction (fixed for directional light)
    vec3 lightDir = normalize(Position + lightOrigin);

    float ambient = 0.20;
    vec3 ambientLight = ambient * vec3(1.0); // Assuming white ambient light

    // Diffuse lighting
    float diffuseStrength = max(dot(normal, lightDir), 0.0);
    vec3 diffuseLight = diffuseStrength * vec3(1.0); // Assuming white light


    vec3 eyeDir = normalize(camOrigin - Position); // View direction
    vec3 reflectedRay = reflect(-lightDir, normal); // Reflection direction
    float specularStrength = 0.50;
    float specPow = pow(max(dot(eyeDir, reflectedRay), 0.0), 16.0);
    vec3 specularLight = specularStrength * specPow * vec3(1.0); // Assuming white specular


    return ambientLight + diffuseLight ;
}


void main()
{

   vTBN();
   //mat3 TBN = mat3(Tangent, Bitangent, Normal);

	float h = (Height + 16)/64.0f;

     vec3 normal = normalize(Normal);


     normal = normalize(normal);



     vec4 albedo0 = texture(ALBEDO, vec3(Position.xz, 0));
     vec4 albedo1 = texture(ALBEDO, vec3(Position.xz, 1));
     vec4 albedo2 = texture(ALBEDO, vec3(Position.xz, 2));

     vec4 normal1 = TRIPLANAR_NORMAL_MAPPING(1);

     vec4 finalTex = vec4(0.0);

        

     float slope = 1.0 - normal.y;

     if(slope < 0.2)
     {
        float blendFactor = slope / 0.2;
        finalTex = mix(albedo2, albedo1, blendFactor);
     }

     if((slope < 0.7) && (slope >= 0.2f))
     {
        float blendFactor = (slope - 0.2) * (1.0 / (0.7 - 0.2));
        finalTex = mix(albedo0, albedo2, blendFactor);
     }
     if(slope >= 0.7) 
     {
        //float blendFactor = slope / 0.7;
        //finalTex = mix(albedo0, albedo1, blendFactor);;
        finalTex = albedo2;
     }


    vec4 albedoColor = texture(ALBEDO, vec3(Position.xz, 1));


    vec4 tt = vec4(0.0);

    if (triplanar == true)
    {
        tt = TRIPLANAR_MAPPING(1);
        //normal =  TBN * TRIPLANAR_NORMAL_MAPPING(1).rgb;
    }
    else
    {
        tt = texture(ALBEDO, vec3(Position.xz, 1));
        //normal = TBN * texture(NORMALS, vec3(Position.xz, 1)).rgb;
    }

 





    vec3 lighting = DirectionLight(normal);

    vec3 finalColor = tt.rgb * lighting;

    FragColor = vec4(finalColor, 1.0);

}