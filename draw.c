#include "draw.h"

const int WINDOW_WIDTH = 512;
const int WINDOW_HEIGHT = 512;
const int SPRITE_SIZE = 64;

GLint pos_uniform;
GLint window_size_uniform;
GLint scale_uniform;

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
    GLuint texture;
    sprite_t sprite;

    sprite.pixels = stbi_load(filename, &sprite.width, &sprite.height, 0, 4);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sprite.width, sprite.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, sprite.pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glUniform2f(window_size_uniform, WINDOW_WIDTH, WINDOW_HEIGHT);
    glUniform1f(scale_uniform, SPRITE_SIZE);

    glBindTexture(GL_TEXTURE_2D, 0);

    assets->sprites[id] = sprite;
    assets->sprites[id].texture = texture;
}

void sprite_draw(assets_t *assets, int sprite_id, int x, int y)
{
    glUniform2f(pos_uniform, x, y);

    glBindTexture(GL_TEXTURE_2D, assets->sprites[sprite_id].texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindTexture(GL_TEXTURE_2D, 0);
}

int gfx_init(SDL_Window *window)
{
    return 0;
}

assets_t *assets_new(GLint* program, int sprite_num)
{
    assets_t *assets = malloc(sizeof(assets_t));
    assets->sprites = malloc(sizeof(sprite_t) * sprite_num);
    assets->sprite_len = sprite_num;
    assets->program = program;

    get_uniforms(program);

    return assets;
}

void init_geometry(int sprite_num)
{
    // allocate 1 VAO (in GPU)
    GLuint vao;
    glGenVertexArrays(1, &vao);
    // bind the VAO (in GPU). now it is the active one
    glBindVertexArray(vao);

    GLuint vbo[2];
    glGenBuffers(2, vbo);

    float vertices[] = {
        // First triangle
        -1, -1, 0,
        1, 1, 0,
        -1, 1, 0,
        // Second triangle
        1, -1, 0,
        -1, -1, 0,
        1, 1, 0};
    float uvs[] = {
        // First triangle
        0, -1,
        -1, 0,
        0, 0,
        // Second triangle
        0, 0,
        1, 0,
        0, 1};

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void get_uniforms(GLuint program)
{
    pos_uniform = glGetUniformLocation(program, "position");
    window_size_uniform = glGetUniformLocation(program, "window_size");
    scale_uniform = glGetUniformLocation(program, "scale");
}