
#include <stdio.h>
#include "collisioncheck.h"
#include "2D.h"
#include "player.h"
#include "enemy.h"
#include "sprite.h"
#include "texture.h"

void colcheck(Capsule2D col) {
	Sprite_Draw(TEXTURE_INDEX_CIRCLE, col.s.p.x, col.s.p.y, 0, 0, col.r,col.r);
	Sprite_Draw(TEXTURE_INDEX_CIRCLE, col.s.p.x + col.s.v.x, col.s.p.y + col.s.v.y, 0, 0, col.r,col.r);
}