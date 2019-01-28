#include <SDL2/SDL.h>
#include <glad/glad.h>
#include "stb_image.h"

typedef struct sprite
{
    int x;
    int y;
    int width;
    int height;
    unsigned char *pixels;
} sprite_t;

typedef struct assets
{
    sprite_t *sprites;

} assets_t;

sprite_t sprite_init(const char *filename);
void init(assets_t *assets);
void sprite_draw(sprite_t *sprite);