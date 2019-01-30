#include "draw.h"

#define PIERETTINI_INIT() assets_t *assets;
#define SPRITE_LOAD(index, filename) sprite_load(assets, index, filename);

void __init__();
void __update__();
void __draw__();