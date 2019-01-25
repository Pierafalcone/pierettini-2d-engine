#include <SDL2/SDL.h>
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "main.h"

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

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 512, 512, SDL_WINDOW_OPENGL);
    if (!window)
    {
        SDL_Log("unable to open the window: %s", SDL_GetError());
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    // create opengl context with the major/minor specified with SetAttribute
    SDL_GLContext context = SDL_GL_CreateContext(window);
    // load GL symbols
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    // set the current clear color
    glClearColor(1, 0, 0, 1);

    // allocate 1 VAO (in GPU)
    GLuint vao;
    glGenVertexArrays(1, &vao);
    // bind the VAO (in GPU). now it is the active one
    glBindVertexArray(vao);

    GLuint vbo[2];
    glGenBuffers(2, vbo);

    float vertices[] = {
        // First triangle
        -0.5, -0.5, 0,
        0.5, 0.5, 0,
        -0.5, 0.5, 0,
        // Second triangle
        0.5, -0.5, 0,
        -0.5, -0.5, 0,
        0.5, 0.5, 0};
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

    // create a new program/pipeline (in GPU)
    GLuint program = glCreateProgram();

    GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, "vertex.glsl");
    GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER, "frag.glsl");

    // attach the sahders to the program
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    // linke the program (now is ready)
    glLinkProgram(program);

    glDetachShader(program, vertex_shader);
    glDetachShader(program, fragment_shader);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // set the currently active program/pipeline
    glUseProgram(program);

    GLuint texture;

    assets_t *assets;
    assets = malloc(sizeof(assets_t));

    init(assets);

    for (;;)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                return 0;
        }

        update(assets);
        draw(assets);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        SDL_GL_SwapWindow(window);
    }

    return 0;
}