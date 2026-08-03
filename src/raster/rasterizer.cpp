#include <raster/rasterizer.h>

#include <cstdlib>
#include <algorithm>
#include <cmath>

namespace {
    struct BoundingBox {
        int minX;
        int maxX;
        int minY;
        int maxY;
    };

    /**
     * @brief Computes the signed edge function.
     *
     * Returns the signed area (twice the triangle area) formed by the
     * oriented edge AB and the test point P. The sign indicates on which
     * side of the edge the point lies.
     */
    int edgeFunction(int x0, int y0, int x1, int y1, int x, int y) {
        return (x1 - x0) * (y - y0) - (y1 - y0) * (x - x0);
    }

    BoundingBox calculateBounds(const Framebuffer& fb, int x0, int y0, int x1, int y1, int x2, int y2) {
        int minX = std::min({x0, x1, x2});
        int maxX = std::max({x0, x1, x2});
        int minY = std::min({y0, y1, y2});
        int maxY = std::max({y0, y1, y2});

        minX = std::max(minX, 0);
        maxX = std::min(maxX, static_cast<int>(fb.getWidth()) - 1);
        minY = std::max(minY, 0);
        maxY = std::min(maxY, static_cast<int>(fb.getHeight()) - 1);

        return {minX, maxX, minY, maxY};
    }

    uint8_t getAlpha(uint32_t color) {
        return (color >> 24) & 0xFF;
    }

    uint8_t getRed(uint32_t color) {
        return (color >> 16) & 0xFF;
    }

    uint8_t getGreen(uint32_t color) {
        return (color >> 8) & 0xFF;
    }

    uint8_t getBlue(uint32_t color) {
        return color & 0xFF;
    }

    uint32_t packColor(uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue) {
        return (static_cast<uint32_t>(alpha) << 24) | (static_cast<uint32_t>(red) << 16) | (static_cast<uint32_t>(green) << 8) | (static_cast<uint32_t>(blue));
    }
} // namespace

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
    BoundingBox boundingBox = calculateBounds(fb, x0, y0, x1, y1, x2, y2);

    for (int y = boundingBox.minY; y <= boundingBox.maxY; y++) {
        for (int x = boundingBox.minX; x <= boundingBox.maxX; x++) {
            int w0 = edgeFunction(x0, y0, x1, y1, x, y);
            int w1 = edgeFunction(x1, y1, x2, y2, x, y);
            int w2 = edgeFunction(x2, y2, x0, y0, x, y);

            bool allPositive = (w0 >= 0) && (w1 >= 0) && (w2 >= 0);
            bool allNegative = (w0 <= 0) && (w1 <= 0) && (w2 <= 0);

            if (allPositive || allNegative) {
                drawPixel(fb, x, y, color);
            }
        }
    }
}

/**
 * Draws a filled triangle with smoothly interpolated vertex colors.
 *
 * The rasterizer computes barycentric coordinates for every covered
 * pixel and uses them to linearly interpolate the red, green and blue
 * channels from the three vertex colors.
 */
void Rasterizer::drawColoredTriangle(Framebuffer& fb, int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color0, uint32_t color1, uint32_t color2) {
    BoundingBox boundingBox = calculateBounds(fb, x0, y0, x1, y1, x2, y2);

    float triangleArea = static_cast<float>((x1 - x0) * (y2 - y0) - (y1 - y0) * (x2 - x0));

    uint8_t r0 = getRed(color0);
    uint8_t g0 = getGreen(color0);
    uint8_t b0 = getBlue(color0);
    uint8_t r1 = getRed(color1);
    uint8_t g1 = getGreen(color1);
    uint8_t b1 = getBlue(color1);
    uint8_t r2 = getRed(color2);
    uint8_t g2 = getGreen(color2);
    uint8_t b2 = getBlue(color2);

    for (int y = boundingBox.minY; y <= boundingBox.maxY; y++) {
        for (int x = boundingBox.minX; x <= boundingBox.maxX; x++) {
            int w0 = edgeFunction(x0, y0, x1, y1, x, y);
            int w1 = edgeFunction(x1, y1, x2, y2, x, y);
            int w2 = edgeFunction(x2, y2, x0, y0, x, y);

            bool allPositive = (w0 >= 0) && (w1 >= 0) && (w2 >= 0);
            bool allNegative = (w0 <= 0) && (w1 <= 0) && (w2 <= 0);

            if (allPositive || allNegative) {
                float alpha = static_cast<float>(w1) / triangleArea;
                float beta = static_cast<float>(w2) / triangleArea;
                float gamma = static_cast<float>(w0) / triangleArea;

                float red = alpha * r0 + beta * r1 + gamma * r2;
                uint8_t redConv = static_cast<uint8_t>(std::clamp(std::round(red), 0.0f, 255.0f));

                float green = alpha * g0 + beta * g1 + gamma * g2;
                uint8_t greenConv = static_cast<uint8_t>(std::clamp(std::round(green), 0.0f, 255.0f));

                float blue = alpha * b0 + beta * b1 + gamma * b2;
                uint8_t blueConv = static_cast<uint8_t>(std::clamp(std::round(blue), 0.0f, 255.0f));

                uint32_t color = packColor(255, redConv, greenConv, blueConv);

                drawPixel(fb, x, y, color);
            }
        }
    }
}