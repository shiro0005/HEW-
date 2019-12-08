#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "collision.h"
#include "enemylaser.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define ENAMYLASER_IMAGEMAX (100)
#define ENEMYLASER_WIDTH	(32)
#define ENEMYLASER_HEIGHT	(32)
#define ENEMYLASER_SPEED	(32)
#define ENEMYLASER_VISIBLE_FRAME 100


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
	LaserIMG	laserIMG[ENAMYLASER_IMAGEMAX];
	float		frame;
	bool		bEnable;

}EnemyLaser;

EnemyLaser g_EnemyLaser[ENEMYLASER_MAX];


void EnemyLaser_Initialize(void)
{
	//�z����̑S�Ă𖳌�������
	for( int i = 0; i < ENEMYLASER_MAX; i++ ) {
		g_EnemyLaser[i].bEnable = false;
	}
}

void EnemyLaser_Update(void)
{
	for (int i = 0; i < ENEMYLASER_MAX; i++) {

		if (g_EnemyLaser[i].bEnable) {

			//���W�X�V
			for (int n = 0; n < ENAMYLASER_IMAGEMAX; n++){
				if (g_EnemyLaser[i].laserIMG[n].bEnable)
				{
					g_EnemyLaser[i].laserIMG[n].x += g_EnemyLaser[i].laserIMG[n].move_x * ENEMYLASER_SPEED;
					g_EnemyLaser[i].laserIMG[n].y += g_EnemyLaser[i].laserIMG[n].move_y * ENEMYLASER_SPEED;

					++g_EnemyLaser[i].laserIMG[n].frame;
					if (g_EnemyLaser[i].laserIMG[n].frame > 40)
						g_EnemyLaser[i].laserIMG[n].bEnable = false;

				}
			}

			//�`��ǉ�
			for (int n = 0; n < ENAMYLASER_IMAGEMAX; n++)
			{
				if (!g_EnemyLaser[i].laserIMG[n].bEnable)
				{
					g_EnemyLaser[i].laserIMG[n].x = g_EnemyLaser[i].x;
					g_EnemyLaser[i].laserIMG[n].y = g_EnemyLaser[i].y;
					g_EnemyLaser[i].laserIMG[n].move_x = g_EnemyLaser[i].move_x;
					g_EnemyLaser[i].laserIMG[n].move_y = g_EnemyLaser[i].move_y;
					g_EnemyLaser[i].laserIMG[n].rotation = g_EnemyLaser[i].rotation;
					g_EnemyLaser[i].laserIMG[n].frame = 0;
					g_EnemyLaser[i].laserIMG[n].bEnable = true;
					break;
				}
			}

			// �L���t���[���𒴂�����
			++g_EnemyLaser[i].frame;
			if (g_EnemyLaser[i].frame > ENEMYLASER_VISIBLE_FRAME)
				g_EnemyLaser[i].bEnable = false;
		}
	}
}

void EnemyLaser_Draw(void)
{
	for (int i = 0; i < ENEMYLASER_MAX; i++) {

		if (g_EnemyLaser[i].bEnable) {
			for (int n = 0; n < ENAMYLASER_IMAGEMAX; n++){
				if (g_EnemyLaser[i].laserIMG[n].bEnable){
					int tw = Texture_GetWidth(TEXTURE_INDEX_BULLET);
					int th = Texture_GetHeight(TEXTURE_INDEX_BULLET);
					Sprite_Draw(TEXTURE_INDEX_LASER,
						g_EnemyLaser[i].laserIMG[n].x,
						g_EnemyLaser[i].laserIMG[n].y,
						0,
						0,
						tw,
						th,
						tw * 0.5f,
						th * 0.5f,
						1.0f,
						1.0f,
						g_EnemyLaser[i].laserIMG[n].rotation);
				}
			}
		}
	}
}

void EnemyLaser_Create(float x, float y, D3DXVECTOR2 dir)
{
	for (int i = 0; i < ENEMYLASER_MAX; i++) {

		// �����e�����łɗL����������H
		if (g_EnemyLaser[i].bEnable) {
			continue;
		}

		// �e�̍��W�Ɉ����̒l��������
		g_EnemyLaser[i].x = x;
		g_EnemyLaser[i].y = y;

		// �e�̈ړ��������w�肷��
		D3DXVec2Normalize(&dir, &dir);
		g_EnemyLaser[i].move_x = dir.x;
		g_EnemyLaser[i].move_y = dir.y;

		// �e�̈ړ���������e�̉�]�p�x�����߂�
		g_EnemyLaser[i].rotation = atan2f(dir.y, dir.x) + M_PI / 2;

		// �J�v�Z���̐ݒ�
		g_EnemyLaser[i].collision.x = x;
		g_EnemyLaser[i].collision.y = y;
		g_EnemyLaser[i].collision.ex = dir.x * 1000;
		g_EnemyLaser[i].collision.ey = dir.y * 1000;
		g_EnemyLaser[i].collision.r = ENEMYLASER_WIDTH * 0.8f;

		// �L���t���[����������
		g_EnemyLaser[i].frame = 0;	

		for (int n = 0; n < ENAMYLASER_IMAGEMAX; n++){
			g_EnemyLaser[i].laserIMG[n].bEnable = false;
		}

		// �e��L���ɂ���
		g_EnemyLaser[i].bEnable = true;

		break;
	}
}

void EnemyLaser_Destroy(int index)
{
	g_EnemyLaser[index].bEnable = false;
}

bool EnemyLaser_IsEnable(int index)
{
	return g_EnemyLaser[index].bEnable;
}

const CUPSULE* EnemyLaser_GetCollision(int index)
{
	return &g_EnemyLaser[index].collision;
}

