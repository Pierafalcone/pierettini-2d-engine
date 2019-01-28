#ifndef _DRAW_H_
#define _DRAW_H_

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include "stb_image.h"

GLuint compile_shader(GLenum shader_type, const char *filename);

#endif