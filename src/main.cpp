#include <core/framebuffer.h>
#include <raster/rasterizer.h>

#include <SDL.h>
#include <cstdint>

int main(int argc, char *argv[]) {

    const int WIDTH = 800;
    const int HEIGHT = 600;

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());

        return 1;
    }

    // Create an application window with the following settings
    window = SDL_CreateWindow("Software Rasterizer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);

    if(!window) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());

        SDL_Quit();

        return 1;
    }

    // Renderer creation
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Unable to render: %s\n", SDL_GetError());

        SDL_DestroyWindow(window);
        SDL_Quit();

        return 1;
    }

    // Texture creation
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);

    if (!texture) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "It was not possible to create the texture: %s\n", SDL_GetError());

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

        return 1;
    }

    // Framebuffer creation
    Framebuffer framebuffer(WIDTH, HEIGHT);

    // Rasterizer creation
    Rasterizer raster{};

    bool running = true;
    SDL_Event event;

    uint32_t redColor = 0xFFFF0000;
    uint32_t blueColor = 0xFF0000FF;
    uint32_t greenColor = 0xFF00FF00;
    uint32_t yellowColor = 0xFFFFFF00;

    // Main loop
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Reset
        framebuffer.reset(0xFF000000);

        raster.drawTriangle(framebuffer, 780, 100, 350, 450, 750, 450, blueColor);
        raster.drawTriangle(framebuffer, 80, 80, 180, 280, 80, 280, greenColor);
        raster.drawTriangle(framebuffer, 200, 150, 300, 100, 400, 150, redColor);
        raster.drawTriangle(framebuffer, 100, 50, 200, 8, 300, 50, yellowColor);

        // Array upload
        SDL_UpdateTexture(texture, NULL, framebuffer.data(), WIDTH * sizeof(uint32_t));

        // Draw the texture to the window
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    // Destruction and shutdown
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(texture);
    SDL_Quit();
    
    return 0;
}