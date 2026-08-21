/* Software Rasterizer
 * SDL Context
 *
 * Copyright (c) 2026 Alexandre Vieira
 * Licensed under the MIT License.
 */

#pragma once

#include <cstdint>
#include <SDL.h>

/**
 * @brief Manages SDL initialization and rendering resources.
 *
 * Provides an abstraction layer over SDL window creation,
 * renderer management and texture updates.
 */
class SDLContext {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    int width;
    int height;

public:
    /**
     * @brief Creates an SDL rendering context.
     *
     * Initializes SDL and creates the window, renderer and texture
     * required for rendering.
     *
     * @param width Window width in pixels.
     * @param height Window height in pixels.
     * @param title Window title.
     */
    SDLContext(int width, int height, const char* title);

    /**
     * @brief Releases SDL resources.
     */
    ~SDLContext();

    /**
     * @brief Checks whether the SDL context was successfully created.
     *
     * @return true if initialized correctly.
     */
    bool isValid() const;

    /**
     * @brief Updates the internal texture with framebuffer data.
     *
     * @param pixels Pixel buffer in ARGB8888 format.
     */
    void updateTexture(const uint32_t* pixels);

    /**
     * @brief Presents the current frame on screen.
     */
    void present();

    /**
     * @brief Processes SDL events.
     *
     * @param running Application running state.
     */
    void processEvent(bool& running);
};