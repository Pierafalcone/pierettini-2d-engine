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

void sprite_load(assets_t *assets, const char *filename, int id)
{
    sprite_t sprite;

    sprite.pixels = stbi_load(filename, &sprite.width, &sprite.height, 0, 4);

    glBindTexture(GL_TEXTURE_2D, assets->textures[id]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sprite.width, sprite.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, sprite.pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    assets->sprites[id] = sprite;
}

void sprite_draw(int sprite_id, int x, int y)
{
    glDrawArrays(GL_TRIANGLES, 0 + sprite_id, 6 + sprite_id);
}

int gfx_init(SDL_Window *window)
{
    return 0;
}

assets_t *assets_new(int sprite_num)
{
    assets_t *assets = malloc(sizeof(assets_t));
    assets->sprites = malloc(sizeof(sprite_t) * sprite_num);
    assets->textures = malloc(sizeof(GLuint) * sprite_num);
    glGenTextures(sprite_num, assets->textures);
    assets->sprite_len = sprite_num;
    assets->texture_len = sprite_num;
    return assets;
}