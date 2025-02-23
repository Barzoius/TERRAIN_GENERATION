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

- LOD with tessellation shaders based on distance

- Height based texture blending

- Triplanar texture mapping

- Texture Bombing

- PBR





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
....

### Fractal Brownian Noise

| Value | Perlin |
|---|---|
| ![](TERRAIN_GENERATION/Resources/ForReadME/value.png)  | ![](TERRAIN_GENERATION/Resources/ForReadME/perlin.png)| 



## NormalMap Generation

To generate normals, I create normal maps directly from 
height maps using convolution kernels. 
These 3x3 matrices are convolved with the image 
to calculate the pixel intensity gradients, 
typically used for edge detection. 
Here, I calculate the gradient for each pixel, 
corresponding to the surface normals, via
compute shader in parallel.

### Prewitt Operator

The Prewitt operator uses uniform weights for differentiation. 
It is less robust against noise, which can lead to results that
 are more sensitive to unwanted local variations in the height map.

$$
G_x = \begin{bmatrix}
-1 & 0 & 1 \\
-1 & 0 & 1 \\
-1 & 0 & 1
\end{bmatrix}, \quad
G_y = \begin{bmatrix}
-1 & -1 & -1 \\
0 & 0 & 0 \\
1 & 1 & 1
\end{bmatrix}
$$


### Sobel Operator

The Sobel operator uses weighted smoothing to reduce noise 
and achieve sharper edges. It applies higher weights to the 
central pixels, which helps reduce the impact of noise.

$$
G_x = \begin{bmatrix}
1 & 0 & 1 \\
2 & 0 & 2 \\
1 & 0 & 1
\end{bmatrix}, \quad
G_y = \begin{bmatrix}
1 & 2 & 1 \\
0 & 0 & 0 \\
1 & 2 & 1
\end{bmatrix}
$$


### Scharr Operator

The Scharr operator is an improved version of 
the Sobel operator, 
offering better approximations for fine details.

$$
G_x = \begin{bmatrix}
3 & 0 & 3 \\
10 & 0 & 10 \\
3 & 0 & 3
\end{bmatrix}, \quad
G_y = \begin{bmatrix}
3 & 10 & 3 \\
0 & 0 & 0 \\
3 & 10 & 3
\end{bmatrix}
$$

| Prewitt | Sobel | Scharr |
|---|---|---|
| ![](TERRAIN_GENERATION/Resources/ForReadME/prewitt.png)  | ![](TERRAIN_GENERATION/Resources/ForReadME/sobel.png)| ![](TERRAIN_GENERATION/Resources/ForReadME/scharr.png)  |

## LOD with tessellation shaders based on distance

<img src="TERRAIN_GENERATION/Resources/ForReadME/tess.gif" alt="Framework Diagram" style="width:100%;">

## Texture Blending

<img src="TERRAIN_GENERATION/Resources/ForReadME/blend.png" alt="Framework Diagram" style="width:100%;">

## Triplanar Texture Mapping

| TTM Off | TTM On | 
|---|---|
| ![](TERRAIN_GENERATION/Resources/ForReadME/simple.png)  | ![](TERRAIN_GENERATION/Resources/ForReadME/triplanar.png)| 


## Texture Bombing

| Bombinb Off | Bombing On | 
|---|---|
| ![](TERRAIN_GENERATION/Resources/ForReadME/noBomb.png)  | ![](TERRAIN_GENERATION/Resources/ForReadME/withBomb.png)| 

# First Results

<img src="TERRAIN_GENERATION/Resources/ForReadME/Second.png" alt="Framework Diagram" style="width:100%;">

<img src="TERRAIN_GENERATION/Resources/ForReadME/third.png" alt="Framework Diagram" style="width:100%;">

<img src="TERRAIN_GENERATION/Resources/ForReadME/forth.png" alt="Framework Diagram" style="width:100%;">
