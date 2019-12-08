#include "main.h"
#include "texture.h"
#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "collision.h"
#include "bossbullet.h"


#define _USE_MATH_DEFINES
#include <math.h>

#define BOSSBULLET_WIDTH  (32)
#define BOSSBULLET_HEIGHT (32)
#define BOSSBULLET_SPEED_X (3.0f)
#define BOSSBULLET_SPEED_Y (3.0f)
#define BOSSBULLET_VISIBLE_FRAME 300


typedef struct
{
	float x, y;				//�\���ʒu
	float move_x, move_y;	//
	float rotation;
	CIRCLE collision;
	float frame;
	bool bEnable;

}BossBullet;

static BossBullet g_BossBullets[BOSSBULLET_MAX];

void BossBullet_Initialize(void)
{
	//�z����̑S�Ă𖳌�������
	for (int i = 0; i < BOSSBULLET_MAX; i++) {
		g_BossBullets[i].bEnable = false;
	}
}

void BossBullet_Update(void)
{
	for (int i = 0; i < BOSSBULLET_MAX; i++) {

		if (g_BossBullets[i].bEnable) {
			g_BossBullets[i].x += g_BossBullets[i].move_x * BOSSBULLET_SPEED_X;
			g_BossBullets[i].y += g_BossBullets[i].move_y * BOSSBULLET_SPEED_Y;
			g_BossBullets[i].collision.cx = g_BossBullets[i].x;
			g_BossBullets[i].collision.cy = g_BossBullets[i].y;

			// ��ʊO�ɏo����
			int tw = Texture_GetWidth(TEXTURE_INDEX_BULLET);
			if (g_BossBullets[i].x - tw * 0.5f > SCREEN_WIDTH) {
				g_BossBullets[i].bEnable = false;
			}

			// �L���t���[���𒴂�����
			++g_BossBullets[i].frame;
			if (g_BossBullets[i].frame > BOSSBULLET_VISIBLE_FRAME) {
				g_BossBullets[i].bEnable = false;
			}
		}
	}
}

void BossBullet_Draw(void)
{
	for (int i = 0; i < BOSSBULLET_MAX; i++) {

		if (g_BossBullets[i].bEnable) {
			int tw = Texture_GetWidth(TEXTURE_INDEX_BULLET);
			int th = Texture_GetHeight(TEXTURE_INDEX_BULLET);
			Sprite_Draw(TEXTURE_INDEX_BULLET,
				g_BossBullets[i].x,
				g_BossBullets[i].y,
				0,
				0,
				tw,
				th,
				tw * 0.5f,
				th * 0.5f,
				1.0f,
				1.0f,
				g_BossBullets[i].rotation);
		}
	}
}

void BossBullet_Create(float x, float y, D3DXVECTOR2 dir)
{
	for (int i = 0; i < BOSSBULLET_MAX; i++) {

		// �����e�����łɗL����������H
		if (g_BossBullets[i].bEnable) {
			continue;
		}

		// �e�̍��W�Ɉ����̒l��������
		g_BossBullets[i].x = x;
		g_BossBullets[i].y = y;

		// �e�̈ړ��������w�肷��
		D3DXVec2Normalize(&dir, &dir);
		g_BossBullets[i].move_x = dir.x;
		g_BossBullets[i].move_y = dir.y;

		// �e�̈ړ���������e�̉�]�p�x�����߂�
		g_BossBullets[i].rotation = atan2f(dir.y, dir.x) + M_PI / 2;

		g_BossBullets[i].collision.cx = x;
		g_BossBullets[i].collision.cy = y;
		g_BossBullets[i].collision.r = BOSSBULLET_WIDTH * 0.8f;

		// �L���t���[����������
		g_BossBullets[i].frame = 0;

		// �e��L���ɂ���
		g_BossBullets[i].bEnable = true;

		break;
	}
}

void BossBullet_Destroy(int index)
{
	g_BossBullets[index].bEnable = false;
}

bool BossBullet_IsEnable(int index)
{
	return g_BossBullets[index].bEnable;
}

const CIRCLE* BossBullet_GetCollision(int index)
{
	return &g_BossBullets[index].collision;
}