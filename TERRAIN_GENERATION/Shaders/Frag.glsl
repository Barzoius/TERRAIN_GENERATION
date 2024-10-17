#version 330 core


out vec4 FragColor;

in vec3 FragPos; 

uniform vec3 Color;
uniform float SquareSize; 

void main()
{
    float scaledX = (FragPos.x + 1.0) / SquareSize;
    float scaledY = (FragPos.y + 1.0) / SquareSize;

    int checkX = int(floor(scaledX)) % 2;
    int checkY = int(floor(scaledY)) % 2;


    if (checkX == checkY)
        FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0); 
    else
        FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0); 
}