#include <raster/rasterizer.h>

#include <cstdlib>
#include <algorithm>

namespace {
    int edgeFunction(int x0, int y0, int x1, int y1, int x, int y) {
        return (x1 - x0) * (y - y0) - (y1 - y0) * (x - x0);
    }
}

void Rasterizer::drawPixel(Framebuffer& fb, int x, int y, uint32_t color) {
    fb.setPixel(x, y, color);
}

/**
 * Draws a line using Bresenham's integer line rasterization algorithm.
 *
 * The algorithm incrementally determines the closest pixel to the ideal
 * line without performing floating-point calculations.
 */
void Rasterizer::drawLine(Framebuffer& fb, int x0, int y0, int x1, int y1, uint32_t color) {
    int x = x0;
    int y = y0;

    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);

    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    int px = 2 * dy - dx;
    int py = 2 * dx - dy;

    if (dx > dy) {
        for (int i = 0; i <= dx; i++) {
            drawPixel(fb, x, y, color);

            if (px > 0) {
                y += sy;
                px += 2 * dy - 2 * dx;
            } else {
                px += 2 * dy;
            }

            x += sx;
        }
    } else {
        for (int i = 0; i <= dy; i++) {
            drawPixel(fb, x, y, color);

            if (py > 0) {
                x += sx;
                py += 2 * dx - 2 * dy;
            } else {
                py += 2 * dx;
            }

            y += sy;
        }
    }
}

void Rasterizer::drawTriangle(Framebuffer& fb, int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
    drawLine(fb, x0, y0, x1, y1, color);
    drawLine(fb, x1, y1, x2, y2, color);
    drawLine(fb, x2, y2, x0, y0, color);
}

void Rasterizer::drawFilledTriangle(Framebuffer& fb, int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
    int minX = std::min({x0, x1, x2});
    int maxX = std::max({x0, x1, x2});
    int minY = std::min({y0, y1, y2});
    int maxY = std::max({y0, y1, y2});

    minX = std::max(minX, 0);
    maxX = std::min(maxX, static_cast<int>(fb.getWidth()) - 1);
    minY = std::max(minY, 0);
    maxY = std::min(maxY, static_cast<int>(fb.getHeight()) - 1);

    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            int w0 = edgeFunction(x0, y0, x1, y1, x, y);
            int w1 = edgeFunction(x1, y1, x2, y2, x, y);
            int w2 = edgeFunction(x2, y2, x0, y0, x, y);

            bool allPositive = (w0 >= 0) && (w1 >= 0) && (w2 >= 0);
            bool allNegative = (w0 <= 0) && (w1 <= 0) && (w2 <= 0);

            if (allPositive || allNegative) {
                fb.setPixel(x, y, color);
            }
        }
    }
}