
#include <stdio.h>
#include "collisioncheck.h"
#include "2D.h"
#include "player.h"
#include "enemy.h"
#include "sprite.h"
#include "texture.h"

void colcheck(Capsule2D col) {
	//sx,syで画像の大きさの倍率を設定　ex：sx=0.5,sy=0.5で画像を元のサイズの半分のサイズに
	//cx,cyは画像の中心座標、テクスチャの座標　ex：画像サイズが726*726ならcx,cyは363となる

	Sprite_Draw(TEXTURE_INDEX_CIRCLE, col.s.p.x, col.s.p.y, 0, 0, 720, 720, 360, 360, col.r * 2 / 720, col.r * 2 / 720, 0);
	Sprite_Draw(TEXTURE_INDEX_CIRCLE, col.s.p.x + col.s.v.x, col.s.p.y + col.s.v.y, 0, 0, 720, 720, 360, 360, col.r * 2 / 720, col.r * 2 / 720, 0);
}