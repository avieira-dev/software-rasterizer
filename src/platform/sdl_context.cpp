#include <platform/sdl_context.h>

#include <stdexcept>
#include <string>

SDLContext::SDLContext(int width, int height, const char* title) : window(nullptr), renderer(nullptr), texture(nullptr), width(width), height(height) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL initialization failed: %s", SDL_GetError());
        return;
    }

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Window creation failed: %s", SDL_GetError());
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Renderer creation failed: %s", SDL_GetError());
        return;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    if (!texture) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Texture creation failed: %s", SDL_GetError());
        return;
    }
}

SDLContext::~SDLContext() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }

    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }

    if (window) {
        SDL_DestroyWindow(window);
    }

    SDL_Quit();
}

bool SDLContext::isValid() const {
    return window && renderer && texture;
}

void SDLContext::updateTexture(const uint32_t* pixels) {
    SDL_UpdateTexture(texture, nullptr, pixels, width * sizeof(uint32_t));
}

void SDLContext::present() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void SDLContext::processEvent(bool& running) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
    }
}