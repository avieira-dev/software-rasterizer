# Software Rasterizer

> Software rasterizer in C++, focused on real-time rendering and graphics pipelines.

![status](https://img.shields.io/badge/status-in%20development-orange)
![language](https://img.shields.io/badge/language-C++-blue)
![build](https://img.shields.io/badge/build-CMake-red)
![license](https://img.shields.io/badge/license-MIT-blue)

> [!NOTE]  
> A real-time software rasterizer built from scratch in C++, focusing on the fundamentals of the graphics pipeline and memory manipulation.

## Table of Contents
- [Showcase](#showcase)
- [Core Technologies](#core-technologies)
- [Implementations](#implementations)
- [How to Run](#how-to-run)
- [Project Structure](#project-structure)
- [Roadmap](#roadmap)
- [Developer](#developer)
- [License](#license)

## Showcase
<p align="center">
    <em>Bresenham's line algorithm demonstrating various slopes and colors.</em><br>
    <img src="./screenshots/img-001.png" alt="Multiple colored lines rendered at different angles using Bresenham's algorithm" width="600">
    <br><br>
    <em>Wireframe triangles formed by connecting vertices with the line primitive.</em><br>
    <img src="./screenshots/img-002.png" alt="Three wireframe triangles in different shapes and colors" width="600">
</p>
## Core Technologies
- C++
- SDL2
- CMake

## Implementations
- ✅ **Direct Framebuffer Access**: Manual manipulation of a `std::vector<uint32_t>` as video memory.
- ✅ **Custom Drawing Primitives:**
    - `draw_pixel`: Includes boundary checks (clipping) to prevent memory access violations.
    - `clear_screen`: Efficient buffer clearing.
- ✅ **Bresenham's Line Algorithm**: High-performance line drawing using only integer arithmetic.
- ✅ **SDL2 Integration**: Window management and real-time texture streaming.

## How to Run
- C++ Compiler (GCC/Clang/MSVC)
- CMake (3.10+)
- SDL2 Library

### Building

```bash
# Clone the repository
git clone https://github.com/avieira-dev/software-rasterizer.git
cd software-rasterizer

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
make

# Run the executable
./SoftwareRasterizer
```

## Project Structure

```text
software-rasterizer/
├── screenshots/
├── include/
├── src/
├── .gitignore
├── CMakeLists.txt
├── LICENSE
└── README.md
```

## Roadmap
- ✅ Basic Framebuffer and SDL2 Setup
- ✅ Bresenham Line Algorithm
- ✅ Wireframe Triangles
- ❌ Triangle Filling (Scanline or Barycentric)
- ❌ 3D Wireframe Cube (Projections)
- ❌ Z-Buffer (Depth testing)

## Developer
**Alexandre Vieira**  
GitHub: [@avieira-dev](https://github.com/avieira-dev)

## License
Distributed under the license [MIT License](LICENSE). See the **LICENSE** file for more details.