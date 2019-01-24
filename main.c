#include <SDL2/SDL.h>
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "main.h"

void update(assets_t *assets)
{
    //printf("Update\n");
}

void draw(assets_t *assets)
{
    sprite_draw(&assets->sprites[0]);
}

void init(assets_t *assets)
{
    assets->sprites = malloc(sizeof(assets_t) * 4);
    assets->sprites[0] = sprite_init("gatto.jpg");
}

sprite_t sprite_init(const char *filename)
{
    sprite_t sprite;
    int colors;
    sprite.x = 0;
    sprite.y = 0;
    sprite.pixels = stbi_load(filename, &sprite.width, &sprite.height, &colors, 4);
    return sprite;
}

void sprite_draw(sprite_t *sprite)
{
    SDL_Log("%d", sprite->width);
    SDL_Log("%d", sprite->height);
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 512, 512, SDL_WINDOW_OPENGL);
    if (!window)
    {
        SDL_Log("unable to open the window: %s", SDL_GetError());
        return -1;
    }

    // create opengl context with the major/minor specified with SetAttribute
    SDL_GLContext context = SDL_GL_CreateContext(window);
    // load GL symbols
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    // set the current clear color
    glClearColor(1, 0, 0, 1);

    assets_t *assets;
    assets = malloc(sizeof(assets_t));

    init(assets);

    for (;;)
    {
        update(assets);
        draw(assets);
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                return 0;
        }
    }

    return 0;
}