#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "collision.h"
#include "bossflame.h"
#include "2D.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include "collisioncheck.h"

#define BOSSFLAME_IMAGEMAX (100)
#define BOSSFLAME_SPEED	(32)
#define BOSSFLAME_VISIBLE_FRAME 100

#define BOSSFLAME_MAX    (1)
#define BOSSFLAME_WIDTH  (64)
#define BOSSFLAME_HEIGHT (64)
#define BOSSFLAME_PATTERN_FRAME (1)
#define BOSSFLAME_PATTERN_MAX   (22)
#define BOSSFLAME_PATTERN_H_MAX (4)

#define FLAMEHINT_MAX (256)
#define FLAMEHINT_WIDTH (64)
#define FLAMEHINT_HEIGHT (64)

typedef struct
{
	float		x, y;			//�\���ʒu
	float		move_x, move_y;	//
	float		rotation;
	Capsule2D		coco;
	bool		bEnable;
	bool coool;
	int frame;
	int pattern;
	bool hit;

}BossFlame;

int flamehint_countdown;

BossFlame g_flamehint[FLAMEHINT_MAX];

BossFlame g_BossFlame[BOSSFLAME_MAX];


void BossFlame_Initialize(void)
{
	//�z����̑S�Ă𖳌�������
	for (int i = 0; i < BOSSFLAME_MAX; i++) {
		g_BossFlame[i].bEnable = false;
		g_flamehint[i].bEnable = false;
		g_BossFlame[i].hit = false;
	}
}

void BossFlame_Update(void)
{
	for (int i = 0; i < BOSSFLAME_MAX; i++) {


		if (g_BossFlame[i].bEnable) {

			flamehint_countdown++;

			if (flamehint_countdown >= 500) {
				g_BossFlame[i].frame++;
				if (g_BossFlame[i].frame % 4 == 0) {
					g_BossFlame[i].pattern++;
				}
				//�Ō�̃p�^�[�����\�����ꂽ��I�����鏈��
				if (g_BossFlame[i].pattern >= BOSSFLAME_PATTERN_MAX) {
					g_BossFlame[i].bEnable = false;
					flamehint_countdown = 0;
					g_BossFlame[0].hit = false;
				}
			}
		}
	}

	//		//���W�X�V
	//		for (int n = 0; n < BOSSFLAME_IMAGEMAX; n++) {
	//			if (g_BossFlame[i].flameIMG[n].bEnable)
	//			{
	//				g_BossFlame[i].flameIMG[n].x += g_BossFlame[i].flameIMG[n].move_x * BOSSFLAME_SPEED;
	//				g_BossFlame[i].flameIMG[n].y += g_BossFlame[i].flameIMG[n].move_y * BOSSFLAME_SPEED;

	//				++g_BossFlame[i].flameIMG[n].frame;
	//				if (g_BossFlame[i].flameIMG[n].frame > 40)
	//					g_BossFlame[i].flameIMG[n].bEnable = false;

	//			}
	//		}

	//		//�`��ǉ�
	//		for (int n = 0; n < BOSSFLAME_IMAGEMAX; n++)
	//		{
	//			if (!g_BossFlame[i].flameIMG[n].bEnable)
	//			{
	//				g_BossFlame[i].flameIMG[n].x = g_BossFlame[i].x;
	//				g_BossFlame[i].flameIMG[n].y = g_BossFlame[i].y;
	//				g_BossFlame[i].flameIMG[n].move_x = g_BossFlame[i].move_x;
	//				g_BossFlame[i].flameIMG[n].move_y = g_BossFlame[i].move_y;
	//				g_BossFlame[i].flameIMG[n].rotation = g_BossFlame[i].rotation;
	//				g_BossFlame[i].flameIMG[n].frame = 0;
	//				g_BossFlame[i].flameIMG[n].bEnable = true;
	//				break;

	//			}
	//		}

	//		// �L���t���[���𒴂�����
	//		++g_BossFlame[i].frame;
	//		if (g_BossFlame[i].frame > BOSSFLAME_VISIBLE_FRAME)
	//			g_BossFlame[i].bEnable = false;
	//	}
	//}
}

void BossFlame_Draw(void)
{

	if (flamehint_countdown > 0 && flamehint_countdown < 500)
	{
	for (int i = 0; i < BOSSFLAME_MAX; i++) {

		if (!g_flamehint[i].bEnable) {
			continue;
		}

		// ���ݕ\������ׂ��p�^�[���ԍ�����؂�����W���Z�o����
		int tx = BOSSFLAME_WIDTH * (g_flamehint[i].pattern % BOSSFLAME_PATTERN_H_MAX);
		int ty = BOSSFLAME_HEIGHT * (g_flamehint[i].pattern / BOSSFLAME_PATTERN_H_MAX);


		Sprite_Draw(TEXTURE_INDEX_CIRCLE,
			g_BossFlame[i].x,
			SCREEN_HEIGHT / 2,
			0.0f,
			0.0f,
			720,
			720,
			360,
			360,
			0.3f,
			0.3f,
			0.0f);
	}
}
	else if (flamehint_countdown >= 500)
	{

		for (int i = 0; i < BOSSFLAME_MAX; i++) {

			if (!g_BossFlame[i].bEnable) {
				continue;
			}

			// ���ݕ\������ׂ��p�^�[���ԍ�����؂�����W���Z�o����
			int tx = BOSSFLAME_WIDTH * (g_BossFlame[i].pattern % BOSSFLAME_PATTERN_H_MAX);
			int ty = BOSSFLAME_HEIGHT * (g_BossFlame[i].pattern / BOSSFLAME_PATTERN_H_MAX);

			Sprite_Draw(TEXTURE_INDEX_LASER,
				g_BossFlame[i].x,
				SCREEN_HEIGHT / 2,
				tx,
				ty,
				BOSSFLAME_WIDTH,
				BOSSFLAME_HEIGHT,
				BOSSFLAME_WIDTH / 2,
				BOSSFLAME_HEIGHT / 2,
				3.0f,
				8.0f,
				0.0f);
			//colcheck(g_BossFlame[i].coco);
		}
	}
}

void BossFlame_Create(int muki, float x, float y, D3DXVECTOR2 dir)
{
	for (int i = 0; i < BOSSFLAME_MAX; i++) {

		// �����e�����łɗL����������H
		if (g_BossFlame[i].bEnable) {
			continue;
		}

		// �e�̍��W�Ɉ����̒l��������
		g_BossFlame[i].x = x;
		g_BossFlame[i].y = y;

		// �e�̈ړ��������w�肷��
		D3DXVec2Normalize(&dir, &dir);
		g_BossFlame[i].move_x = dir.x;
		g_BossFlame[i].move_y = dir.y;


		// �e�̈ړ���������e�̉�]�p�x�����߂�
		g_BossFlame[i].rotation = atan2f(dir.y, dir.x) + M_PI / 2;

		// �J�v�Z���̐ݒ�
		g_BossFlame[i].coco.s.p.x = x;
		g_BossFlame[i].coco.s.p.y = y;
		g_BossFlame[i].coco.s.v.x = 0.0f;
		g_BossFlame[i].coco.s.v.y = +380.0f;
		g_BossFlame[i].coco.r = BOSSFLAME_WIDTH * 0.8f;

		// �L���t���[����������
		g_BossFlame[i].frame = 0;

		g_BossFlame[i].pattern = 0;
		

		// �e��L���ɂ���
		g_BossFlame[i].bEnable = true;

		g_flamehint[i].bEnable = true;
		break;
	}
}

void BossFlame_Destroy(int index)
{
	g_BossFlame[index].bEnable = false;
}

bool BossFlame_IsEnable(int index)
{
	return g_BossFlame[index].bEnable;
}

const Capsule2D* BossFlame_GetCollision(int index)
{
	return &g_BossFlame[index].coco;
}


bool BossFlame_Iscoool(int index)
{
	return g_BossFlame[index].coool;
}

void BossFlame_IscooolFlase(int index)
{
	g_BossFlame[index].coool = false;
}

void BossFlame_IscooolTrue(int index)
{
	g_BossFlame[index].coool = true;
}

void Flamehit(bool f)
{
	g_BossFlame[0].hit = f;
}

bool Get_flamehit()
{
	return g_BossFlame[0].hit;
}