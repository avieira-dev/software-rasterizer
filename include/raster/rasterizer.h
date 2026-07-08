/* Software Rasterizer
 * Rasterizer
 *
 * Copyright (c) 2026 Alexandre Vieira
 * Licensed under the MIT License.
 */

#ifndef RASTER_RASTERIZER_H
#define RASTER_RASTERIZER_H

#include <core/framebuffer.h>

#include <cstdint>

/**
 * @brief Provides software rasterization algorithms.
 *
 * Implements basic rendering primitives by writing pixels into a
 * framebuffer. The rasterizer is responsible for converting geometric
 * primitives into pixels.
 */
class Rasterizer {

public:
    /**
     * @brief Draws a single pixel.
     *
     * @param fb Destination framebuffer.
     * @param x Horizontal pixel coordinate.
     * @param y Vertical pixel coordinate.
     * @param color Pixel color in ARGB8888 format.
     */
    void drawPixel(Framebuffer& fb, int x, int y, uint32_t color);

    /**
     * @brief Draws a line using Bresenham's line algorithm.
     *
     * @param fb Destination framebuffer.
     * @param x0 Starting X coordinate.
     * @param y0 Starting Y coordinate.
     * @param x1 Ending X coordinate.
     * @param y1 Ending Y coordinate.
     * @param color Line color in ARGB8888 format.
     */
    void drawLine(Framebuffer& fb, int x0, int y0, int x1, int y1, uint32_t color);

    /**
     * @brief Draws the outline of a triangle.
     *
     * The triangle is rendered by drawing the three edges connecting
     * its vertices.
     *
     * @param fb Destination framebuffer.
     * @param x0 First vertex X coordinate.
     * @param y0 First vertex Y coordinate.
     * @param x1 Second vertex X coordinate.
     * @param y1 Second vertex Y coordinate.
     * @param x2 Third vertex X coordinate.
     * @param y2 Third vertex Y coordinate.
     * @param color Triangle color in ARGB8888 format.
     */
    void drawTriangle(Framebuffer& fb, int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
};

#endif