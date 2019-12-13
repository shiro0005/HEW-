#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "collision.h"
#include "bossbullet.h"
#include "2D.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define BULLET_WIDTH  (64)
#define BULLET_HEIGHT (64)
#define BULLET_SPEED_X (1.0f)
#define BULLET_SPEED_Y (1.0f)
#define BULLET_VISIBLE_FRAME 25


typedef struct BossBullet_tag
{
	D3DXVECTOR2 pos;	//�\���ʒu
	float move_x, move_y;	//
	float rotation;
	Capsule2D   colcol;
	float frame;
	bool bEnable;
	bool cool;
} BossBullet;

static BossBullet g_Bullets[BOSSBULLET_MAX];

void BossBullet_Initialize(void)
{
	//�z����̑S�Ă𖳌�������
	for (int i = 0; i < BOSSBULLET_MAX; i++) {
		g_Bullets[i].bEnable = false;
		g_Bullets[i].cool = false;
	}
}

void BossBullet_Update(void)
{
	for (int i = 0; i < BOSSBULLET_MAX; i++) {

		if (g_Bullets[i].bEnable) {
			g_Bullets[i].pos.x += g_Bullets[i].move_x * BULLET_SPEED_X;
			g_Bullets[i].pos.y += g_Bullets[i].move_y * BULLET_SPEED_Y;
			g_Bullets[i].colcol.r = BULLET_WIDTH;
			g_Bullets[i].colcol.s.p.x = g_Bullets[i].pos.x - 32.0f;
			g_Bullets[i].colcol.s.p.y = g_Bullets[i].pos.y;
			g_Bullets[i].colcol.s.v.x = 64.0f;
			g_Bullets[i].colcol.s.v.y = 0.0f;
			//// ��ʊO�ɏo����
			//int tw = Texture_GetWidth(TEXTURE_INDEX_BULLET);
			//if( g_Bullets[i].x - tw * 0.5f > SCREEN_WIDTH ) {
			//	g_Bullets[i].bEnable = false;
			//}

			// �L���t���[���𒴂�����
			++g_Bullets[i].frame;
			if (g_Bullets[i].frame > BULLET_VISIBLE_FRAME) {
				g_Bullets[i].bEnable = false;
			}
		}
	}
}

void BossBullet_Draw(void)
{
	for (int i = 0; i < BOSSBULLET_MAX; i++) {

		if (g_Bullets[i].bEnable) {
			int tw = Texture_GetWidth(TEXTURE_INDEX_BULLET);
			int th = Texture_GetHeight(TEXTURE_INDEX_BULLET);
			Sprite_Draw(TEXTURE_INDEX_BULLET,
				g_Bullets[i].pos.x,
				g_Bullets[i].pos.y,
				0,
				0,
				tw,
				th,
				tw * 0.5f,
				th * 0.5f,
				4.0f,
				4.0f,
				g_Bullets[i].rotation);
		}
	}
}

void BossBullet_Create(int muki, float x, float y, D3DXVECTOR2 dir)
{
	for (int i = 0; i < BOSSBULLET_MAX; i++) {

		// �����e�����łɗL����������H
		if (g_Bullets[i].bEnable) {
			continue;
		}

		// �e�̍��W�Ɉ����̒l��������
		g_Bullets[i].pos.x = x;
		g_Bullets[i].pos.y = y - 20.0f;

		if (muki == 1)
		{
			g_Bullets[i].pos.x += -30.0f;
		}
		if (muki == 2)
		{
			g_Bullets[i].pos.x += 30.0f;
		}

		// �e�̈ړ��������w�肷��
		D3DXVec2Normalize(&dir, &dir);
		g_Bullets[i].move_x = dir.x;
		g_Bullets[i].move_y = dir.y;

		// �e�̈ړ���������e�̉�]�p�x�����߂�
		g_Bullets[i].rotation = atan2f(dir.y, dir.x) + M_PI / 2;

		g_Bullets[i].colcol.s.p.x = g_Bullets[i].pos.x - 32.0f;
		g_Bullets[i].colcol.s.p.y = g_Bullets[i].pos.y;
		g_Bullets[i].colcol.s.v.x = 64.0f;
		g_Bullets[i].colcol.s.v.y = 0.0f;
		// �L���t���[����������
		g_Bullets[i].frame = 0;

		// �e��L���ɂ���
		g_Bullets[i].bEnable = true;
		g_Bullets[i].cool = true;

		break;
	}
}

void BossBullet_Destroy(int index)
{
	g_Bullets[index].bEnable = false;
}

bool BossBullet_IsEnable(int index)
{
	return g_Bullets[index].bEnable;
}

const Capsule2D* BossBullet_GetCollision(int index)
{
	return &g_Bullets[index].colcol;
}

bool BossBullet_Iscool(int index)
{
	return g_Bullets[index].cool;
}

void BossBullet_IscoolFlase(int index)
{
	g_Bullets[index].cool = false;
}

void BossBullet_IscoolTrue(int index)
{
	g_Bullets[index].cool = true;
}
