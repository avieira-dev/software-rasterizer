cmake_minimum_required(VERSION 3.10)

project(
    SoftwareRasterizer
    VERSION 0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL2 REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "src/*.cpp"
)

add_executable(software-rasterizer ${SOURCES})

target_include_directories(software-rasterizer
    PRIVATE include ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(software-rasterizer
    PRIVATE ${SDL2_LIBRARIES}
)