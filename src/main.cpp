#include <core/framebuffer.h>
#include <raster/rasterizer.h>
#include <platform/sdl_context.h>

#include <SDL.h>
#include <cstdint>

int main(int argc, char *argv[]) {
    const int WIDTH = 800;
    const int HEIGHT = 600;

    SDLContext context(WIDTH, HEIGHT, "Software Rasterizer");

    if (!context.isValid()) {
        return 1;
    }

    Framebuffer framebuffer(WIDTH, HEIGHT);
    Rasterizer raster;

    bool running = true;

    uint32_t redColor = 0xFFFF0000;
    uint32_t blueColor = 0xFF0000FF;
    uint32_t greenColor = 0xFF00FF00;
    uint32_t yellowColor = 0xFFFFFF00;
    uint32_t blackColor = 0xFF000000;

    while (running) {
        context.processEvent(running);
        framebuffer.reset(blackColor);

        raster.drawTriangle(framebuffer, 780, 100, 350, 450, 750, 450, blueColor);
        raster.drawTriangle(framebuffer, 80, 80, 180, 280, 80, 280, greenColor);
        raster.drawTriangle(framebuffer, 200, 150, 300, 100, 400, 150, yellowColor);

        context.updateTexture(framebuffer.data());
        context.present();
    }
    
    return 0;
}