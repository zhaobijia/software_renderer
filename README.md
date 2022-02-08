# software renderer
A real-time shader-based software renderer written in c++.
It is an ongoing learning project inspired by [tinyrenderer](https://github.com/ssloy/tinyrenderer) without using any Graphic APIs such as OpenGL, DirectX or Vulkan. The rendering takes place entirely on the CPU. My main goal is to get a better understanding of how mordern computer graphics work.

### Screenshots:
 

### Current Features:
- Minimal dependencies
- Image loading using [stb-image](https://github.com/nothings/stb/blob/master/stb_image.h)
- Orbiting camera controls
- Wavefront obj file parser
- Reverse Z-Buffering
- Back-face Culling
- Perspective projection
- Texture mapping
- Shader based

### In progress (Todo/Wishlist <3 ):
- Scene switching
- Lighting (directional/point/spotlight)
- Homogeneous clipping
- View Frustum culling
- Shadow mapping
- Ambient occlusion
- Metallic workflow
- PBR (Physically based rendering)
- IBL (Image-based lighting)
- Reflections using cubemaps
- Antialiasing
- Skeletal Animation
- Particle system
- Post-processing

### References:
- [Tiny renderer](https://github.com/ssloy/tinyrenderer)
- [Learn OpenGL](https://learnopengl.com/)
- [Games 101](https://www.bilibili.com/video/BV1X7411F744/)
- [CMU 15-462](https://www.youtube.com/watch?v=W6yEALqsD7k&list=PL9_jI1bdZmz2emSh0UQ5iOdT2xRHFHL7E&index=1&ab_channel=KeenanCrane)
- [Fundamentals of Computer Graphics](https://www.amazon.ca/Fundamentals-Computer-Graphics-Peter-Shirley/dp/1568814690)
- [Windows App Development](https://docs.microsoft.com/en-us/windows/win32/)