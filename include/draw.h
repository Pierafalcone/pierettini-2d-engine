#ifndef _DRAW_H_
#define _DRAW_H_

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include "stb_image.h"

#define SPRITE(id, x, y) sprite_draw(assets, id, x, y);
#define BUTTON(id)
#define SOUND(id)
#define MUSIC(id)

typedef struct sprite
{
    int width;
    int height;
    unsigned char *pixels;
    GLuint texture;
} sprite_t;

typedef struct assets
{
    sprite_t *sprites;
    int sprite_len;
    GLuint *program;
} assets_t;

GLuint compile_shader(GLenum shader_type, const char *filename);
void sprite_load(assets_t *assets, int id, const char *filename);
void sprite_draw(assets_t *assets, int sprite_id, int x, int y);
int gfx_init(SDL_Window *window);
assets_t *assets_new(GLuint *program, int sprite_num);
void init_geometry(int sprite_num);
static void get_uniforms(GLuint program);

#endif