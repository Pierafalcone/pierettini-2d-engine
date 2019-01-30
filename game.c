#include "game.h"

/*
    Things to add in order of priority:
    * Input handling
    * Add 2D graphical primitives
    * Tilesheet support
    * Map support
    * Audio support
    * Music support
*/

float counter = 0;
float position_x = 0;
float position_y = 0;

PIERETTINI_INIT()

void __init__()
{
    SPRITE_LOAD(0, "pizza.jpg");
    SPRITE_LOAD(1, "scooter.png");
    SPRITE_LOAD(2, "ball.gif");
    SPRITE_LOAD(3, "bread.gif");
    SPRITE_LOAD(4, "mount.gif");
    SPRITE_LOAD(5, "sushi.gif");
}

void __update__()
{
    counter += 0.02;
}

void __draw__()
{
    // sprite_draw(assets, 0, 64, 64);
    // sprite_draw(assets, 1, 128, 128);

    for (int i = 0; i < 40; i++)
    {
        position_x = 256 + (sin(counter + (0.1 * i + i)) * 200);
        position_y = 256 + (cos(counter + (0.1 * i)) * 200);

        SPRITE(i % 6, position_x, position_y);
    }
}