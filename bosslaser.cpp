#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "collision.h"
#include "bosslaser.h"


#define _USE_MATH_DEFINES
#include <math.h>

#define BOSSLASER_IMAGEMAX (100)
#define BOSSLASER_WIDTH	(32)
#define BOSSLASER_HEIGHT	(32)
#define BOSSLASER_SPEED	(32)
#define BOSSLASER_VISIBLE_FRAME 100


typedef struct
{
	float		x, y;				//�\���ʒu
	float		move_x, move_y;	//
	float		rotation;
	float		frame;
	bool		bEnable;

}LaserIMG;

typedef struct
{
	float		x, y;			//�\���ʒu
	float		move_x, move_y;	//
	float		rotation;
	CUPSULE		collision;
	LaserIMG	laserIMG[BOSSLASER_IMAGEMAX];
	float		frame;
	bool		bEnable;

}BossLaser;

BossLaser g_BossLaser[BOSSLASER_MAX];


void BossLaser_Initialize(void)
{
	//�z����̑S�Ă𖳌�������
	for (int i = 0; i < BOSSLASER_MAX; i++) {
		g_BossLaser[i].bEnable = false;
	}
}

void BossLaser_Update(void)
{
	for (int i = 0; i < BOSSLASER_MAX; i++) {

		if (g_BossLaser[i].bEnable) {

			//���W�X�V
			for (int n = 0; n < BOSSLASER_IMAGEMAX; n++) {
				if (g_BossLaser[i].laserIMG[n].bEnable)
				{
					g_BossLaser[i].laserIMG[n].x += g_BossLaser[i].laserIMG[n].move_x * BOSSLASER_SPEED;
					g_BossLaser[i].laserIMG[n].y += g_BossLaser[i].laserIMG[n].move_y * BOSSLASER_SPEED;

					++g_BossLaser[i].laserIMG[n].frame;
					if (g_BossLaser[i].laserIMG[n].frame > 40)
						g_BossLaser[i].laserIMG[n].bEnable = false;

				}
			}

			//�`��ǉ�
			for (int n = 0; n < BOSSLASER_IMAGEMAX; n++)
			{
				if (!g_BossLaser[i].laserIMG[n].bEnable)
				{
					g_BossLaser[i].laserIMG[n].x = g_BossLaser[i].x;
					g_BossLaser[i].laserIMG[n].y = g_BossLaser[i].y;
					g_BossLaser[i].laserIMG[n].move_x = g_BossLaser[i].move_x;
					g_BossLaser[i].laserIMG[n].move_y = g_BossLaser[i].move_y;
					g_BossLaser[i].laserIMG[n].rotation = g_BossLaser[i].rotation;
					g_BossLaser[i].laserIMG[n].frame = 0;
					g_BossLaser[i].laserIMG[n].bEnable = true;
					break;
				}
			}

			// �L���t���[���𒴂�����
			++g_BossLaser[i].frame;
			if (g_BossLaser[i].frame > BOSSLASER_VISIBLE_FRAME)
				g_BossLaser[i].bEnable = false;
		}
	}
}

void BossLaser_Draw(void)
{
	for (int i = 0; i < BOSSLASER_MAX; i++) {

		if (g_BossLaser[i].bEnable) {
			for (int n = 0; n < BOSSLASER_IMAGEMAX; n++) {
				if (g_BossLaser[i].laserIMG[n].bEnable) {
					int tw = Texture_GetWidth(TEXTURE_INDEX_BULLET);
					int th = Texture_GetHeight(TEXTURE_INDEX_BULLET);
					Sprite_Draw(TEXTURE_INDEX_LASER,
						g_BossLaser[i].laserIMG[n].x,
						g_BossLaser[i].laserIMG[n].y,
						0,
						0,
						tw,
						th,
						tw * 0.5f,
						th * 0.5f,
						1.0f,
						1.0f,
						g_BossLaser[i].laserIMG[n].rotation);
				}
			}
		}
	}
}

void BossLaser_Create(float x, float y, D3DXVECTOR2 dir)
{
	for (int i = 0; i < BOSSLASER_MAX; i++) {

		// �����e�����łɗL����������H
		if (g_BossLaser[i].bEnable) {
			continue;
		}

		// �e�̍��W�Ɉ����̒l��������
		g_BossLaser[i].x = x;
		g_BossLaser[i].y = y;

		// �e�̈ړ��������w�肷��
		D3DXVec2Normalize(&dir, &dir);
		g_BossLaser[i].move_x = dir.x;
		g_BossLaser[i].move_y = dir.y;

		// �e�̈ړ���������e�̉�]�p�x�����߂�
		g_BossLaser[i].rotation = atan2f(dir.y, dir.x) + M_PI / 2;

		// �J�v�Z���̐ݒ�
		g_BossLaser[i].collision.x = x;
		g_BossLaser[i].collision.y = y;
		g_BossLaser[i].collision.ex = dir.x * 1000;
		g_BossLaser[i].collision.ey = dir.y * 1000;
		g_BossLaser[i].collision.r = BOSSLASER_WIDTH * 0.8f;

		// �L���t���[����������
		g_BossLaser[i].frame = 0;

		for (int n = 0; n < BOSSLASER_IMAGEMAX; n++) {
			g_BossLaser[i].laserIMG[n].bEnable = false;
		}

		// �e��L���ɂ���
		g_BossLaser[i].bEnable = true;

		break;
	}
}

void BossLaser_Destroy(int index)
{
	g_BossLaser[index].bEnable = false;
}

bool BossLaser_IsEnable(int index)
{
	return g_BossLaser[index].bEnable;
}

const CUPSULE* BossLaser_GetCollision(int index)
{
	return &g_BossLaser[index].collision;
}