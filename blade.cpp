#include"main.h"
#include"collision.h"
#include"blade.h"
#include "texture.h"
#include "sprite.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define BLADE_WIDTH  (32)
#define BLADE_HEIGHT (32)
#define BLADE_SPEED_X (5.0f)
#define BLADE_SPEED_Y (5.0f)

#define BLADE_VISIBLE_FRAME 10


typedef struct Blade_tag
{
	float x, y;				//�\���ʒu
	float move_x, move_y;	//
	float rotation;
	CIRCLE collision;
	float frame;
	bool bEnable;
} Blade;

static Blade g_Blade[BLADE_MAX];



void Blade_Initialize(void)
{
	//�z����̑S�Ă𖳌�������
	for (int i = 0; i < BLADE_MAX; i++) {
		g_Blade[i].bEnable = false;
	}
}


void Blade_Update(void)
{
	for (int i = 0; i < BLADE_MAX; i++) {

		if (g_Blade[i].bEnable) {
			g_Blade[i].x += g_Blade[i].move_x * BLADE_SPEED_X;
			g_Blade[i].y += g_Blade[i].move_y * BLADE_SPEED_Y;
			g_Blade[i].collision.cx = g_Blade[i].x;
			g_Blade[i].collision.cy = g_Blade[i].y;

			// ��ʊO�ɏo����
			int tw = Texture_GetWidth(TEXTURE_INDEX_BULLET);
			if (g_Blade[i].x - tw * 0.5f > SCREEN_WIDTH) {
				g_Blade[i].bEnable = false;
			}

			// �L���t���[���𒴂�����
			++g_Blade[i].frame;
			if (g_Blade[i].frame > BLADE_VISIBLE_FRAME) {
				g_Blade[i].bEnable = false;
			}
		}
	}
}

void Blade_Draw(void)
{
	for (int i = 0; i < BLADE_MAX; i++) {

		if (g_Blade[i].bEnable) {
			int tw = Texture_GetWidth(TEXTURE_INDEX_BULLET);
			int th = Texture_GetHeight(TEXTURE_INDEX_BULLET);
			Sprite_Draw(TEXTURE_INDEX_BULLET,
				g_Blade[i].x,
				g_Blade[i].y,
				0,
				0,
				tw,
				th,
				tw * 0.5f,
				th * 0.5f,
				1.0f,
				1.0f,
				g_Blade[i].rotation);
		}
	}
}


void Blade_Create(float x, float y, D3DXVECTOR2 dir)
{
	for (int i = 0; i < BLADE_MAX; i++) {


		// �������킪���Ɏ���������H
		if (g_Blade[i].bEnable) {
			continue;
		}
		
		// �u���C�h�Ɉ����̒l��������
		g_Blade[i].x = x;
		g_Blade[i].y = y;

		// �U���������w�肷��
		D3DXVec2Normalize(&dir, &dir);
		g_Blade[i].move_x = dir.x;
		g_Blade[i].move_y = dir.y;

		// �u���C�h���������߂�
		g_Blade[i].rotation = atan2f(dir.y, dir.x) + M_PI / 2;

		g_Blade[i].collision.cx = x;
		g_Blade[i].collision.cy = y;
		g_Blade[i].collision.r = BLADE_WIDTH * 0.8f;

		// �L���t���[����������
		g_Blade[i].frame = 0;

		// �u���C�h��L���ɂ���
		g_Blade[i].bEnable = true;

		break;
	}
}

void Blade_Destroy(int index)
{
	
	int bladedurability = 10;

	if (bladedurability=0)
	{
		g_Blade[index].bEnable = false;
	}

	
}

bool Blade_IsEnable(int index)
{
	return g_Blade[index].bEnable;
}

const CIRCLE* Blade_GetCollision(int index)
{
	return &g_Blade[index].collision;
}

