#define STB_IMAGE_IMPLEMENTATION
#include "main.h"
#include <math.h>

const int SPRITE_NUM = 16;

assets_t *assets;

float counter = 0;
float position_x = 0;
float position_y = 0;

void __init__()
{
    sprite_load(assets, "pizza.jpg", 0);
    sprite_load(assets, "scooter.png", 1);
}

void __update__()
{
    counter += 0.0005;
}

void __draw__()
{
    sprite_draw(assets, 0, 64, 64);
    sprite_draw(assets, 1, 128, 128);

    // for (int i = 0; i < 40; i++)
    // {
    //     position_x = 256 + (sin(counter + (0.1 * i + i)) * 200);
    //     position_y = 256 + (cos(counter + (0.1 * i)) * 200);

    //     sprite_draw(assets, 0, position_x, position_y);
    // }
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

    init_geometry(SPRITE_NUM);

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

    /* END SDL STUFF */

    assets = assets_new(program, SPRITE_NUM);

    __init__();

    for (;;)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                return 0;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        __update__();
        __draw__();

        SDL_GL_SwapWindow(window);
    }

    return 0;
}