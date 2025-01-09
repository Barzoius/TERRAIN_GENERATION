# TERRAIN_GENERATION

This project focuses on creating procedural terrain generation
using multiple height map techniques.
The goal is to produce diverse and realistic landscapes by combining 
different methods to simulate natural terrain features.

## Features

- Procedural HeightMap generation with:
    - Fault Formation
    - Fractal Brownian Noise
    - Midpoint Displacemant

- Procedural NormalMap generation with:
    - Sobel Operator
    - Scharr Operator
    - Prewitt Operator

- Triplanar texture mapping

- Texture Bombing

- Slope and Height based texture blending

- PBR

- LOD with tessellation shaders based on roughness



## HeightMap Generation

### Fault Formation

| FF16 | FF32 | FF64 |
|---|---|---|
| ![](TERRAIN_GENERATION/Resources/ForReadME/FF16.png)  | ![](TERRAIN_GENERATION/Resources/ForReadME/FF32.png)| ![](TERRAIN_GENERATION/Resources/ForReadME/FF64.png)  |

With mean filter:

| FF16 | FF32 | FF64 |
|---|---|---|
| ![](TERRAIN_GENERATION/Resources/ForReadME/FFB16.png)  | ![](TERRAIN_GENERATION/Resources/ForReadME/FFB32.png)| ![](TERRAIN_GENERATION/Resources/ForReadME/FFB64.png)  |

### Midpoint Displacemant

### Fractal Brownian Noise

| Value | Perlin |
|---|---|
| ![](TERRAIN_GENERATION/Resources/ForReadME/value.png)  | ![](TERRAIN_GENERATION/Resources/ForReadME/perlin.png)| 



## NormalMap Generation

| Prewitt | Sobel | Scharr |
|---|---|---|
| ![](TERRAIN_GENERATION/Resources/ForReadME/prewitt.png)  | ![](TERRAIN_GENERATION/Resources/ForReadME/sobel.png)| ![](TERRAIN_GENERATION/Resources/ForReadME/scharr.png)  |

## Triplanar Texture Mapping

Abrupt zone for better viewing of the effect:

| TTM Off | TTM On | 
|---|---|
| ![](TERRAIN_GENERATION/Resources/ForReadME/TriOff.png)  | ![](TERRAIN_GENERATION/Resources/ForReadME/TriOn.png)| 


# First Results

<img src="TERRAIN_GENERATION/Resources/ForReadME/Second.png" alt="Framework Diagram" style="width:100%;">

<img src="TERRAIN_GENERATION/Resources/ForReadME/third.png" alt="Framework Diagram" style="width:100%;">

<img src="TERRAIN_GENERATION/Resources/ForReadME/forth.png" alt="Framework Diagram" style="width:100%;">
