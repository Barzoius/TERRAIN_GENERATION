#version 330 core

uniform mat4 view;
uniform mat4 projection;

out vec3 TexCoords;

const vec3[36] positions = vec3[](
    // Front face
    vec3( 1.0,  1.0,  1.0), // Top-right
    vec3(-1.0,  1.0,  1.0), // Top-left
    vec3(-1.0, -1.0,  1.0), // Bottom-left

    vec3(-1.0, -1.0,  1.0), // Bottom-left
    vec3( 1.0, -1.0,  1.0), // Bottom-right
    vec3( 1.0,  1.0,  1.0), // Top-right

    // Back face
    vec3( 1.0,  1.0, -1.0), // Top-right
    vec3( 1.0, -1.0, -1.0), // Bottom-right
    vec3(-1.0, -1.0, -1.0), // Bottom-left

    vec3(-1.0, -1.0, -1.0), // Bottom-left
    vec3(-1.0,  1.0, -1.0), // Top-left
    vec3( 1.0,  1.0, -1.0), // Top-right

    // Left face
    vec3(-1.0,  1.0,  1.0), // Top-right
    vec3(-1.0,  1.0, -1.0), // Top-left
    vec3(-1.0, -1.0, -1.0), // Bottom-left

    vec3(-1.0, -1.0, -1.0), // Bottom-left
    vec3(-1.0, -1.0,  1.0), // Bottom-right
    vec3(-1.0,  1.0,  1.0), // Top-right

    // Right face
    vec3( 1.0,  1.0, -1.0), // Top-right
    vec3( 1.0,  1.0,  1.0), // Top-left
    vec3( 1.0, -1.0,  1.0), // Bottom-left

    vec3( 1.0, -1.0,  1.0), // Bottom-left
    vec3( 1.0, -1.0, -1.0), // Bottom-right
    vec3( 1.0,  1.0, -1.0), // Top-right

    // Top face
    vec3( 1.0,  1.0, -1.0), // Top-right
    vec3(-1.0,  1.0, -1.0), // Top-left
    vec3(-1.0,  1.0,  1.0), // Bottom-left

    vec3(-1.0,  1.0,  1.0), // Bottom-left
    vec3( 1.0,  1.0,  1.0), // Bottom-right
    vec3( 1.0,  1.0, -1.0), // Top-right

    // Bottom face
    vec3( 1.0, -1.0,  1.0), // Top-right
    vec3(-1.0, -1.0,  1.0), // Top-left
    vec3(-1.0, -1.0, -1.0), // Bottom-left

    vec3(-1.0, -1.0, -1.0), // Bottom-left
    vec3( 1.0, -1.0, -1.0), // Bottom-right
    vec3( 1.0, -1.0,  1.0)  // Top-right
);

void main()
{
    vec3 vertexPos = positions[gl_VertexID];
    TexCoords = vertexPos;
    gl_Position = projection * view * vec4(vertexPos, 1.0);
}   