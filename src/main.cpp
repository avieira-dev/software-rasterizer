#include <SDL.h>
#include <vector>
#include <cstdint>

void draw_pixel(std::vector<uint32_t>& buffer, int x, int y, int width, int height, uint32_t color) {
    if(x >= 0 && x < width && y >= 0 && y < height) {
        buffer[y * width + x] = color;
    }
}

void draw_line(std::vector<uint32_t>& buffer, int x0, int y0, int x1, int y1, int width, int height, uint32_t color) {
    // Calculate the distances
    int dx = abs(x1 - x0);
    int dy = -abs(y1 - y0);

    // Direction of the steps
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    // Initial error
    int error = dx + dy;

    while(true) {
        // Draw the current pixel
        draw_pixel(buffer, x0, y0, width, height, color);

        if(x0 == x1 && y0 == y1) {
            break;
        }

        int error2 = 2 * error;

        // Walk on the X axis?
        if(error2 >= dy) {
            error += dy;
            x0 += sx;
        }

        // Walk on the Y axis?
        if(error2 <= dx) {
            error += dx;
            y0 += sy;
        }
    }
}

void draw_triangle(std::vector<uint32_t>& buffer, int x0, int y0, int x1, int y1, int x2, int y2, int width, int height, uint32_t color) {
    // Connect the 3 vertices to close the shape
    draw_line(buffer, x0, y0, x1, y1, width, height, color);
    draw_line(buffer, x1, y1, x2, y2, width, height, color);
    draw_line(buffer, x2, y2, x0, y0, width, height, color);
}

/*
void draw_horizontal_line(std::vector<uint32_t>& buffer, int fix_y, int max_x, int width, int height, uint32_t color) {
    if(max_x >= 0 && max_x < width && fix_y >= 0 && fix_y < height) {
        for(int x = 0; x <= max_x; x++) {
            draw_pixel(buffer, x, fix_y, width, height, color);
        }
    }
}

void draw_vertical_line(std::vector<uint32_t>& buffer, int fix_x, int max_y, int width, int height, uint32_t color) {
    if(max_y >= 0 && max_y < height && fix_x >= 0 && fix_x < width) {
        for(int y = 0; y <= max_y; y++) {
            draw_pixel(buffer, fix_x, y, width, height, color);
        }
    }
}
*/

void clear_screen(std::vector<uint32_t>& buffer, uint32_t color) {
    std::fill(buffer.begin(), buffer.end(), color);
}

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

    // Starting position
    int pixel_x = 0;
    int pixel_y = 300;
    uint32_t redColor = 0xFFFF0000;
    uint32_t blueColor = 0xFF0000FF;
    uint32_t greenColor = 0xFF00FF00;
    uint32_t yellowColor = 0xFFFFFF00;

    // Main loop
    while(running) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Clear screen
        clear_screen(framebuffer, 0xFF000000);

        draw_triangle(framebuffer, 400, 100, 200, 450, 600, 450, WIDTH, HEIGHT, greenColor);
        draw_triangle(framebuffer, 100, 50, 150, 250, 50, 200, WIDTH, HEIGHT, redColor);
        draw_triangle(framebuffer, 700, 50, 710, 500, 650, 480, WIDTH, HEIGHT, yellowColor);

        /* 
        draw_line(framebuffer, 400, 300, 700, 100, WIDTH, HEIGHT, blueColor);
        draw_line(framebuffer, 400, 300, 700, 500, WIDTH, HEIGHT, redColor);
        draw_line(framebuffer, 400, 300, 100, 300, WIDTH, HEIGHT, greenColor);
        draw_line(framebuffer, 400, 300, 400, 50, WIDTH, HEIGHT, yellowColor);
        */

        /*
        draw_horizontal_line(framebuffer, 300, 400, WIDTH, HEIGHT, blueColor);
        draw_vertical_line(framebuffer, 400, 300, WIDTH, HEIGHT, greenColor);
        /*

        /* Animation

        draw_pixel(framebuffer, pixel_x, pixel_y, WIDTH, HEIGHT, redColor);

        pixel_x++;

        if(pixel_x >= WIDTH) {
            pixel_x = 0;
        }

        */

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