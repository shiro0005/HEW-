
#include <stdio.h>
#include "collisioncheck.h"
#include "2D.h"
#include "player.h"
#include "enemy.h"
#include "sprite.h"
#include "texture.h"

void colcheck(Capsule2D col) {
	//sx,sy�ŉ摜�̑傫���̔{����ݒ�@ex�Fsx=0.5,sy=0.5�ŉ摜�����̃T�C�Y�̔����̃T�C�Y��
	//cx,cy�͉摜�̒��S���W�A�e�N�X�`���̍��W�@ex�F�摜�T�C�Y��726*726�Ȃ�cx,cy��363�ƂȂ�

	Sprite_Draw(TEXTURE_INDEX_CIRCLE, col.s.p.x, col.s.p.y, 0, 0, 720, 720, 360, 360, col.r * 2 / 720, col.r * 2 / 720, 0);
	Sprite_Draw(TEXTURE_INDEX_CIRCLE, col.s.p.x + col.s.v.x, col.s.p.y + col.s.v.y, 0, 0, 720, 720, 360, 360, col.r * 2 / 720, col.r * 2 / 720, 0);
}