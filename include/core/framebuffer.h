/* Software Rasterizer
 * Framebuffer
 *
 * Copyright (c) 2026 Alexandre Vieira
 * Licensed under the MIT License.
 */

#ifndef CORE_FRAMEBUFFER_H
#define CORE_FRAMEBUFFER_H

#include <cstdint>
#include <vector>

/**
 * @brief Represents a color framebuffer used by the software rasterizer.
 *
 * Stores a contiguous block of pixels in ARGB8888 format and provides
 * basic operations for pixel manipulation and buffer clearing.
 */
class Framebuffer {

private:
    size_t width;
    size_t height;
    std::vector<uint32_t> pixels;

public:
    /**
     * @brief Creates a framebuffer with the specified dimensions.
     *
     * @param w Framebuffer width in pixels.
     * @param h Framebuffer height in pixels.
     */
    Framebuffer(size_t w, size_t h);

    /**
     * @brief Returns a read-only pointer to the framebuffer data.
     *
     * The returned pointer can be used by the rendering backend
     * (e.g. SDL texture upload) without exposing write access.
     *
     * @return Pointer to the framebuffer pixel data.
     */
    const uint32_t* data() const;

    /**
     * @brief Returns the framebuffer width.
     *
     * @return Width in pixels.
     */
    size_t getWidth() const;

    /**
     * @brief Returns the framebuffer height.
     *
     * @return Height in pixels.
     */
    size_t getHeight() const;

    /**
     * @brief Writes a pixel to the framebuffer.
     *
     * If the specified coordinates are outside the framebuffer,
     * the operation fails and no pixel is written.
     *
     * @param x Horizontal pixel coordinate.
     * @param y Vertical pixel coordinate.
     * @param color Pixel color in ARGB8888 format.
     *
     * @return true if the pixel was written successfully; false otherwise.
     */
    bool setPixel(int x, int y, uint32_t color);

    /**
     * @brief Fills the entire framebuffer with a single color.
     *
     * @param color Fill color in ARGB8888 format.
     */
    void reset(uint32_t color);
};

#endif