#ifndef _DRAW_H_
#define _DRAW_H_

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

GLuint compile_shader(GLenum shader_type, const char *filename);
sprite_t sprite_init(const char *filename);
void sprite_draw(sprite_t *sprite);

#endif