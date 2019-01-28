#define STB_IMAGE_IMPLEMENTATION
#include "main.h"

assets_t *assets;

void __init__()
{
    sprite_load(assets, "pizza.jpg", 0);
    sprite_load(assets, "scooter.jpg", 1);
}

void __update__()
{
    //printf("Update\n");
}

void __draw__()
{
    sprite_draw(0, 0, 0);
}

int main()
{
    /* START SDL STUFF */

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 512, 512, SDL_WINDOW_OPENGL);

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

    /* END SDL STUFF */

    assets = assets_new(16);

    __init__();

    for (;;)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                return 0;
        }

        __update__();
        __draw__();

        SDL_GL_SwapWindow(window);
    }

    return 0;
}