#include <core/framebuffer.h>

Framebuffer::Framebuffer(size_t w, size_t h)
    : width(w), height(h), pixels(w * h, 0xFF000000) {
}

const uint32_t* Framebuffer::data() const {

    return pixels.data();
}

size_t Framebuffer::getWidth() const {

    return width;
}

size_t Framebuffer::getHeight() const {

    return height;
}

/**
 * Writes a pixel only if the coordinates lie inside the framebuffer.
 *
 * Negative coordinates are rejected before conversion to size_t to
 * prevent unsigned integer wraparound during index calculation.
 */
bool Framebuffer::setPixel(int x, int y, uint32_t color) {

    if (x < 0 || y < 0) {
        return false;
    }

    size_t posX = static_cast<size_t>(x);
    size_t posY = static_cast<size_t>(y);

    if (posX >= width || posY >= height ) {
        return false;
    }

    pixels[posY * width + posX] = color;

    return true;
}

void Framebuffer::reset(uint32_t color) {

    std::fill(pixels.begin(), pixels.end(), color);
}

