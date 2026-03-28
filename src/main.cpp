#include <SDL.h>
#include <vector>
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
    window = SDL_CreateWindow(
        "Software Rasterizer",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        0
    );

    // Check that the window was successfully created
    if(!window) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());

        SDL_Quit();
        return 1;
    }

    // Renderer creation
    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    // Verify that the renderer was created successfully
    if(!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Unable to render: %s\n", SDL_GetError());

        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Texture creation
    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH,
        HEIGHT
    );

    // Check if the texture was created successfully
    if(!texture) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "It was not possible to create the texture: %s\n", SDL_GetError());

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Framebuffer creation
    std::vector<uint32_t> framebuffer(WIDTH * HEIGHT, 0xFF000000);

    bool running = true;
    SDL_Event event;

    // Main loop
    while(running) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Manually painting a pixel (center of the screen)
        int x = 400;
        int y = 300;
        
        uint32_t redColor = 0xFFFF0000;

        framebuffer[y * WIDTH + x] = redColor;

        // Array upload
        SDL_UpdateTexture(
            texture,
            NULL,
            framebuffer.data(),
            WIDTH * sizeof(uint32_t)
        );

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