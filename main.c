#include <SDL2/SDL.h>
#include "main.h"

void update()
{
    printf("Update");
}

void draw()
{
    printf("Draw");
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