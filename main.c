#include <SDL2/SDL.h>
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void update()
{
    printf("Update\n");
}

void draw()
{
    printf("Draw\n");
}

int main()
{
    for (;;)
    {
        update();
        draw();
    }

    return 1;
}