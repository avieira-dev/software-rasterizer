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

        //raster.drawTriangle(framebuffer, 50, 50, 250, 50, 50, 250, greenColor);
        //raster.drawTriangle(framebuffer, 500, 50, 700, 50, 600, 250, yellowColor);
        //raster.drawTriangle(framebuffer, 150, 350, 50, 550, 250, 550, redColor);
        //raster.drawTriangle(framebuffer, 450, 350, 750, 350, 600, 550, blueColor);

        raster.drawFilledTriangle(framebuffer, 50, 50, 250, 50, 50, 250, greenColor);
        raster.drawFilledTriangle(framebuffer, 500, 50, 700, 50, 600, 250, yellowColor);
        raster.drawFilledTriangle(framebuffer, 150, 350, 50, 550, 250, 550, redColor);
        raster.drawFilledTriangle(framebuffer, 450, 350, 750, 350, 600, 550, blueColor);

        context.updateTexture(framebuffer.data());
        context.present();
    }
    
    return 0;
}