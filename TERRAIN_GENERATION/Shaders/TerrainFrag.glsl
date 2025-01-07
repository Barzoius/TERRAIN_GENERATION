#version 450 core

in float Height;

out vec4 FragColor;

layout (binding = 0) uniform sampler2DArray ALBEDO;

layout (binding = 3) uniform sampler2DArray AO;

layout (binding = 4) uniform sampler2DArray NORMALS;

layout (binding = 5) uniform sampler2DArray ROUGHNESS;

layout (binding = 6) uniform sampler2DArray METALLIC;


uniform vec3 camOrigin;
uniform vec3 lightOrigin;

uniform bool triplanar;

in vec3 Position;

in vec2 TexCoords;

in vec3 Normal;
in vec3 Tangent;
in vec3 Bitangent;

mat3 TBN;

const float PI = 3.14159265359;

// function taken from https://learnopengl.com/
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

///-------------------------TRIPLANAR_MAPPING_FUNCTIONS-------------------------///


///--------------------------------ALBEDO---------------------------------///

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

///----------------------------------AO-----------------------------------///

float TRIPLANAR_AO_MAPPING(int texture)
{
    vec3 blend = abs(Normal);
    blend = normalize(max(blend,  0.00001));

    float b = blend.x + blend.y + blend.z;

    blend /= vec3(b,b,b);

    float xTex = texture(AO, vec3(Position.yz * .8, texture)).r;
    float yTex = texture(AO, vec3(Position.xz * .8, texture)).r;
    float zTex = texture(AO, vec3(Position.xy * .8, texture)).r;

    float Tex = xTex * blend.x + yTex * blend.y + zTex * blend.z;

    return Tex;
}
///-----------------------------ROUGHNESS--------------------------------///

float TRIPLANAR_ROGHNESS_MAPPING(int texture)
{
    vec3 blend = abs(Normal);
    blend = normalize(max(blend,  0.00001));

    float b = blend.x + blend.y + blend.z;

    blend /= vec3(b,b,b);

    float xTex = texture(AO, vec3(Position.yz * .8, texture)).r;
    float yTex = texture(AO, vec3(Position.xz * .8, texture)).r;
    float zTex = texture(AO, vec3(Position.xy * .8, texture)).r;

    float Tex = xTex * blend.x + yTex * blend.y + zTex * blend.z;

    return Tex;
}

///-------------------------------METALLIC--------------------------------///

float TRIPLANAR_METALLIC_MAPPING(int texture)
{
    vec3 blend = abs(Normal);
    blend = normalize(max(blend,  0.00001));

    float b = blend.x + blend.y + blend.z;

    blend /= vec3(b,b,b);

    float xTex = texture(AO, vec3(Position.yz * .8, texture)).r;
    float yTex = texture(AO, vec3(Position.xz * .8, texture)).r;
    float zTex = texture(AO, vec3(Position.xy * .8, texture)).r;

    float Tex = xTex * blend.x + yTex * blend.y + zTex * blend.z;

    return Tex;
}

///-------------------------------NORMAL---------------------------------///

vec3 TRIPLANAR_NORMAL_MAPPING(int texture)
{
    vec3 blend = abs(Normal);
    blend = normalize(max(blend,  0.00001));

    float b = blend.x + blend.y + blend.z;

    blend /= vec3(b,b,b);

    vec3 xN = texture(NORMALS, vec3(Position.yz * .8, texture)).rgb;
    vec3 yN = texture(NORMALS, vec3(Position.xz * .8, texture)).rgb;
    vec3 zN = texture(NORMALS, vec3(Position.xy * .8, texture)).rgb;

    vec3 N = vec3(xN * blend.x + yN * blend.y + zN * blend.z);

    return N;
}

///-----------------------------------------------------------------------------///


///------------------------------PBR_COMPONENTS---------------------------------///

float TR_GGX_NDF(vec3 n, vec3 h, float rough) // Trowbridge-Reitz GGX 
{
    float a = rough * rough;

    float NDH2 = max(dot(n,h), 0.0) * max(dot(n,h), 0.0);

    return a / (PI * (NDH2 * (a - 1) + 1) * (NDH2 * (a - 1) + 1));

}


float S_GGX_GF(float nv, float rough) // Schlick-GGX
{
    float k = ((rough + 1) * (rough + 1)) / 8.0;

    return nv / (nv * (1 - k) + k);
}

float SMITH_GF(vec3 n, vec3 v, vec3 l, float rough) // the Smith method
{
    float ggx1 = S_GGX_GF(max(dot(n, v), 0.0), rough);
    float ggx2 = S_GGX_GF(max(dot(n, l), 0.0), rough);

    return ggx1 * ggx2;
}

float SCHLICK_FRESNEL(float cosAngle, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosAngle, 0.0, 1.0), 5.0);
}



///-----------------------------------------------------------------------------///
vec3 DirectionLight(vec3 n) 
{

    vec3 normal = normalize(n);


    vec3 lightDir = normalize(Position + lightOrigin);

    float ambient = 0.20;
    vec3 ambientLight = ambient * vec3(1.0);

    // Diffuse lighting
    float diffuseStrength = max(dot(normal, lightDir), 0.0);
    vec3 diffuseLight = diffuseStrength * vec3(1.0); 


    vec3 eyeDir = normalize(camOrigin - Position); 
    vec3 reflectedRay = reflect(-lightDir, normal); 
    float specularStrength = 0.50;
    float specPow = pow(max(dot(eyeDir, reflectedRay), 0.0), 16.0);
    vec3 specularLight = specularStrength * specPow * vec3(1.0); 


    return ambientLight + diffuseLight ;
}

float heightRanges[4] = float[](0.0, 0.2, 0.3, 1.0);



void main()
{

   vTBN();
   //mat3 TBN = mat3(Tangent, Bitangent, Normal);

	//float h = (Height + 16)/64.0;

    vec3 normal = normalize(Normal);

    ///---------------------SETING_TEXTURE_VECTORS---------------------//


     vec4 albedos[4];
     vec3 normals[4];

     float aos[4];
     float rough[4];
     float metallic[4];


 
     for(int i = 0; i <= 2; i++)
     {
         if(triplanar == true)
         {
            albedos[i] = TRIPLANAR_MAPPING(i);

            aos[i] = TRIPLANAR_AO_MAPPING(i);
            rough[i] = TRIPLANAR_ROGHNESS_MAPPING(i);
            metallic[i] = TRIPLANAR_METALLIC_MAPPING(i);

            vec3 tn = TRIPLANAR_NORMAL_MAPPING(i);
            normals[i] = normalize(TBN * tn);
         }
         else
         {

             albedos[i] = texture(ALBEDO, vec3(Position.xz, i));

             aos[i] = texture(AO, vec3(Position.xz, i)).r;
             rough[i] = texture(ROUGHNESS, vec3(Position.xz, i)).r;
             metallic[i] = texture(METALLIC, vec3(Position.xz, i)).r;

             vec3 tn = texture(NORMALS, vec3(Position.xz, i)).rgb;
             normals[i] = normalize(TBN * tn);
         }
     }

     if(triplanar == true)
     {
        albedos[3] = TRIPLANAR_MAPPING(2);

        aos[3] = TRIPLANAR_AO_MAPPING(2);
        vec3 tn = TRIPLANAR_NORMAL_MAPPING(2);
        normals[3] = normalize(TBN * tn);
        }
     else
     {
        albedos[3] = texture(ALBEDO, vec3(Position.xz, 2));
        aos[3] = texture(AO, vec3(Position.xz, 2)).r;
        vec3 tn = texture(NORMALS, vec3(Position.xz, 2)).rgb;
        normals[3] = normalize(TBN * tn);

      }

     ///----------------------------------------------------------------//


     vec4 finalAlbedo = vec4(0.0);
     vec3 finalNormal = vec3(0.0);
     float finalAO = 0.0;
     float finalRough = 0.0;
     float finalMetallic = 0.0;


     float h =  clamp(Height,  0.0, 1.0);

    if (h >= heightRanges[3]) {
        finalAlbedo = albedos[2];
        finalAO = aos[2];
    } else {
        for (int i = 0; i <= 2; ++i) {
         
            if (h >= heightRanges[i] && h < heightRanges[i + 1]) 
            {
            float t = (h - heightRanges[i]) / (heightRanges[i + 1] - heightRanges[i]); 
            finalAlbedo = mix(albedos[i], albedos[i + 1], t);
            finalNormal = mix(normals[i], normals[i+1], t);
            finalAO = mix(aos[i], aos[i + 1], t);
            finalRough = mix(rough[i], rough[i + 1], t);
            finalMetallic = mix(metallic[i], metallic[i + 1], t);
            }
        }
    }

     ///----------------------------------------------------------------//

    ///----FINAL LIGHTING---///

   // vec3 ambient = vec3(0.03) * finalAlbedo.rgb * finalAO; 

    vec3 lighting = DirectionLight(finalNormal);

    vec3 finalColor = finalAlbedo.rgb * lighting ;

    FragColor = vec4(finalColor, 1.0);

}