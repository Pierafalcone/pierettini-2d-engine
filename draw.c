#include "draw.h"

GLuint compile_shader(GLenum shader_type, const char *filename)
{
    SDL_RWops *rw = SDL_RWFromFile(filename, "rb");
    if (!rw)
    {
        SDL_Log("unable to open file");
        exit(1);
    }

    size_t file_len = SDL_RWsize(rw);
    char *source = SDL_malloc(file_len + 1);
    if (!source)
    {
        SDL_Log("unable to allocate memory");
        exit(1);
    }

    if (SDL_RWread(rw, source, 1, file_len) != file_len)
    {
        SDL_Log("unable to read file");
        exit(1);
    }

    source[file_len] = 0;

    SDL_RWclose(rw);

    GLuint shader = glCreateShader(shader_type);
    // assign the shader code
    glShaderSource(shader, 1, (const GLchar **)&source, (GLint *)&file_len);
    // compile the shader
    glCompileShader(shader);

    SDL_free(source);

    GLint compile_status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
    if (compile_status == GL_FALSE)
    {
        GLint log_size;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_size);
        char *error_log = SDL_malloc(log_size + 1);
        if (!error_log)
        {
            SDL_Log("unable to allocate memory for log");
            exit(1);
        }
        glGetShaderInfoLog(shader, log_size, &log_size, error_log);
        error_log[log_size] = 0;
        SDL_Log("shader compile error: %s", error_log);
        SDL_free(error_log);
        exit(1);
    }

    return shader;
}

sprite_t sprite_init(const char *filename)
{
    sprite_t sprite;
    int colors;
    sprite.x = 0;
    sprite.y = 0;
    sprite.pixels = stbi_load(filename, &sprite.width, &sprite.height, &colors, 4);

    GLuint texture;

    glGenTextures(1, &texture);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sprite.width, sprite.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, sprite.pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return sprite;
}

void sprite_draw(sprite_t *sprite)
{
    // SDL_Log("%d", sprite->width);
    // SDL_Log("%d", sprite->height);
}