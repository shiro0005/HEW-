#include "texture.h"
#include "sprite.h"
#include "number.h"


void Number_Draw(float x, float y, int n,float sx,float sy)
{
	if( n < 0 || n > 9 ) return;

	Sprite_Draw(TEXTURE_INDEX_NUMBER,
		x, 
		y,
		NUMBER_WIDTH * n,
		0,
		NUMBER_WIDTH,
		NUMBER_HEIGHT,
		NUMBER_WIDTH/2, 
		NUMBER_HEIGHT/2,
		sx,
		sy,
		0.0f);
}

