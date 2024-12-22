#version 450 core

layout (local_size_x = 16, local_size_y = 16) in;

layout (rgba32f, binding = 0) uniform image2D hMap;
layout (rgba32f, binding = 1) uniform image2D nMap;


shared float holoed_WorkGroup[gl_WorkGroupSize.x+2][gl_WorkGroupSize.y+2];


uniform int operatorType;


void sobel()
{
    uvec2 workGroup_pixel = gl_LocalInvocation.xy+ uvec2(1, 1)

}


void scharr()
{
    uvec2 workGroup_pixel = gl_LocalInvocation.xy+ uvec2(1, 1)


}


void main()
{
    
    ivec2 hMap_pixel = gl_GlobalInvocationID.xy;
    uvec2 workGroup_pixel = gl_LocalInvocation.xy+ uvec2(1, 1);

    holoed_WorkGroup[workGroup_pixel.x][workGroup_pixel.y] = imageLoad(hMap, hMap_pixel.xy).r;

    ivec2 workGroupSize = ivec2(gl_WorkGroupSize.xy);
    ivec2 hMapSize = gl_WorkGroupSize.xy * gl_NumWorkGroups.xy;


    if(workGroup_pixel.x == 1) // LEFT HALO SIDE
    {

        ivec pixel = clamp(hMap_pixel + ivec(-1, 0), ivec(0), hMapSize  - 1);
        haloed_WorkGroup[workGroup_pixel.x - 1][workGroup_pixel.y] = imageLoad(hMap, pixel.xy).r;

        if(workGroup_pixel.y == 1) // BUTTOM CORNER
        {
            ivec2 pixel = clamp(hMap_pixel + ivec2(-1, -1), ivec(0), hMapSize - 1);
            haloed_WorkGroup[workGroup_pixel.x - 1][workGroup_pixel.y - 1] = imageLoad(hMap, pixel.xy).r;
        }

        if(workGroup_pixel.y == workGroupSize.y) // TOP CORNER
        {
            ivec2 pixel = clamp(hMap_pixel + ivec2(-1, 1), ivec(0), hMapSize - 1);
            haloed_WorkGroup[workGroup_pixel.x - 1][workGroup_pixel.y + 1] = imageLoad(hMap, pixel.xy).r;
        }
    }

    if(workGroup_pixel.x = workGroupSize.x) // RIGHT HALO SIDE
    {

        ivec pixel = clamp(hMap_pixel + ivec(1, 0), ivec(0), hMapSize  - 1);
        haloed_WorkGroup[workGroup_pixel.x + 1][workGroup_pixel.y] = imageLoad(hMap, pixel.xy).r

        if(workGroup_pixel.y == 1) // BUTTOM CORNER
        {
            ivec pixel = clamp(hMap_pixel + ivec(1, -1), ivec(0), hMapSize  - 1);
            haloed_WorkGroup[workGroup_pixel.x + 1][workGroup_pixel.y - 1] = imageLoad(hMap, pixel.xy).r
        }

        if(workGroup_pixel.y == workGroupSize.y) // TOP CORNER
        {
            ivec pixel = clamp(hMap_pixel + ivec(1, 1), ivec(0), hMapSize  - 1);
            haloed_WorkGroup[workGroup_pixel.x + 1][workGroup_pixel.y + 1] = imageLoad(hMap, pixel.xy).r
        }
    }

    if(workGroup_pixel.y == 1) // UPPER HALO SIDE
    {
        ivec pixel = clamp(hMap_pixel + ivec(0, -1), ivec(0), hMapSize  - 1);
        haloed_WorkGroup[workGroup_pixel.x][workGroup_pixel.y - 1] = imageLoad(hMap, pixel.xy).r
    }

    if(workGroup_pixel.y == workGroupSize.y) // BUTTOM HALO SIDE
    {
        ivec pixel = clamp(hMap_pixel + ivec(0, 1), ivec(0), hMapSize  - 1);
        haloed_WorkGroup[workGroup_pixel.x][workGroup_pixel.y + 1] = imageLoad(hMap, pixel.xy).r
    }

    barrier();

    if(operatorType == 1)
    {
        sobel();
    }
    else if(operatorType == 2)
    {
        scharr();
    }

  

}