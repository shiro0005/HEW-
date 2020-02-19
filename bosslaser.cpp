#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "collision.h"
#include "bosslaser.h"
#include "2D.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include "collisioncheck.h"



int laserhint_countdown[LASERHINT_MAX];

BossLaser g_laserhint[LASERHINT_MAX];

BossLaser g_BossLaser[BOSSLASER_MAX];


void BossLaser_Initialize(void)
{

	g_BossLaser[0].coool = false;

	//�z����̑S�Ă𖳌�������
	for (int i = 0; i < BOSSLASER_MAX; i++) {
		g_BossLaser[i].bEnable = false;
		g_laserhint[i].bEnable = false;
		g_BossLaser[i].hit = false;
		laserhint_countdown[i] = 0;
	}
}

void BossLaser_Update(void)
{

	//		//���W�X�V
	//		for (int n = 0; n < BOSSLASER_IMAGEMAX; n++) {
	//			if (g_BossLaser[i].laserIMG[n].bEnable)
	//			{
	//				g_BossLaser[i].laserIMG[n].x += g_BossLaser[i].laserIMG[n].move_x * BOSSLASER_SPEED;
	//				g_BossLaser[i].laserIMG[n].y += g_BossLaser[i].laserIMG[n].move_y * BOSSLASER_SPEED;

	//				++g_BossLaser[i].laserIMG[n].frame;
	//				if (g_BossLaser[i].laserIMG[n].frame > 40)
	//					g_BossLaser[i].laserIMG[n].bEnable = false;

	//			}
	//		}

	//		//�`��ǉ�
	//		for (int n = 0; n < BOSSLASER_IMAGEMAX; n++)
	//		{
	//			if (!g_BossLaser[i].laserIMG[n].bEnable)
	//			{
	//				g_BossLaser[i].laserIMG[n].x = g_BossLaser[i].x;
	//				g_BossLaser[i].laserIMG[n].y = g_BossLaser[i].y;
	//				g_BossLaser[i].laserIMG[n].move_x = g_BossLaser[i].move_x;
	//				g_BossLaser[i].laserIMG[n].move_y = g_BossLaser[i].move_y;
	//				g_BossLaser[i].laserIMG[n].rotation = g_BossLaser[i].rotation;
	//				g_BossLaser[i].laserIMG[n].frame = 0;
	//				g_BossLaser[i].laserIMG[n].bEnable = true;
	//				break;

	//			}
	//		}

	//		// �L���t���[���𒴂�����
	//		++g_BossLaser[i].frame;
	//		if (g_BossLaser[i].frame > BOSSLASER_VISIBLE_FRAME)
	//			g_BossLaser[i].bEnable = false;
	//	}
	//}


	for (int i = 0; i < BOSSLASER_MAX; i++)
	{

		//laserhint_countdown++;
		//if (laserhint_countdown>5000)
		//{
		//	g_laserhint[i].bEnable = true;
		//}
		//if (laserhint_countdown > 10000)
		//{
		//	g_laserhint[i].bEnable = false;
		//}
		//if (laserhint_countdown > 15000)
		//{
		//	g_BossLaser[i].bEnable = true;
		//}
		//if(laserhint_countdown>20000){
		//	g_BossLaser[i].bEnable = false;
		//	laserhint_countdown = 0;
		//}

		/*if (flag_change == 1)
		{
			laserhint_countdown = 0;
		}*/


		if (g_BossLaser[i].bEnable)
		{

			laserhint_countdown[i]++;

			if (laserhint_countdown[i] >= 200)
			{
				g_BossLaser[i].frame++;
				if (g_BossLaser[i].frame % 4 == 0)
				{
					g_BossLaser[i].pattern++;
				}
				//�Ō�̃p�^�[�����\�����ꂽ��I�����鏈��
				if (g_BossLaser[i].pattern >= BOSSLASER_PATTERN_MAX)
				{
					g_BossLaser[i].bEnable = false;
					laserhint_countdown[i] = 0;
					BossLaser_IscooolFlase(0);
					g_BossLaser[0].hit = false;
				}
			}
		}
	}


	for (int i = 0; i < LASERHINT_MAX; i++) {
		if (g_laserhint[i].bEnable) {

			if (g_laserhint[i].frame % 4 == 0) {
				g_laserhint[i].pattern++;
			}

			//�Ō�̃p�^�[�����\�����ꂽ��I�����鏈��
			if (g_laserhint[i].pattern >= LASERHINT_PATTERN_MAX) {
				g_laserhint[i].pattern = 0;
			}
		}
	}
}

void BossLaser_Draw(void)
{

	for (int i = 0; i < LASERHINT_MAX; i++) {
		if (laserhint_countdown[i] > 0 && laserhint_countdown[i] < 200)
		{
			if (!g_laserhint[i].bEnable) {
				continue;
			}

			// ���ݕ\������ׂ��p�^�[���ԍ�����؂�����W���Z�o����
			int tx = LASERHINT_WIDTH * (g_laserhint[i].pattern % LASERHINT_PATTERN_H_MAX);
			int ty = LASERHINT_HEIGHT * (g_laserhint[i].pattern / LASERHINT_PATTERN_H_MAX);


			Sprite_Draw(TEXTURE_INDEX_HINT,
				g_BossLaser[i].x,
				SCREEN_HEIGHT / 2,
				tx,
				ty,
				LASERHINT_WIDTH,
				LASERHINT_HEIGHT,
				LASERHINT_WIDTH / 2,
				LASERHINT_HEIGHT / 2,
				1.5f,
				8.0f,
				0.0f);
		}
		else if (laserhint_countdown[i] >= 200)
		{

			for (int i = 0; i < BOSSLASER_MAX; i++) {

				if (!g_BossLaser[i].bEnable) {
					continue;
				}

				BossLaser_IscooolTrue(0);

				// ���ݕ\������ׂ��p�^�[���ԍ�����؂�����W���Z�o����
				int tx = BOSSLASER_WIDTH * (g_BossLaser[i].pattern % BOSSLASER_PATTERN_H_MAX);
				int ty = BOSSLASER_HEIGHT * (g_BossLaser[i].pattern / BOSSLASER_PATTERN_H_MAX);


				Sprite_Draw(TEXTURE_INDEX_LASER,
					g_BossLaser[i].x,
					SCREEN_HEIGHT / 2,
					tx,
					ty,
					BOSSLASER_WIDTH,
					BOSSLASER_HEIGHT,
					BOSSLASER_WIDTH / 2,
					BOSSLASER_HEIGHT / 2,
					3.0f,
					8.0f,
					0.0f);
				//colcheck(g_BossLaser[i].coco);
			}
		}

	}



}

void BossLaser_Create(int muki, float x, float y, D3DXVECTOR2 dir)
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
		/*D3DXVec2Normalize(&dir, &dir);
		g_BossLaser[i].move_x = dir.x;
		g_BossLaser[i].move_y = dir.y;


		// �e�̈ړ���������e�̉�]�p�x�����߂�
		g_BossLaser[i].rotation = atan2f(dir.y, dir.x) + M_PI / 2;
		*/
		// �J�v�Z���̐ݒ�
		g_BossLaser[i].coco.s.p.x = x;
		g_BossLaser[i].coco.s.p.y = y;
		g_BossLaser[i].coco.s.v.x = 0.0f;
		g_BossLaser[i].coco.s.v.y = +380.0f;
		g_BossLaser[i].coco.r = BOSSLASER_WIDTH * 0.8f;

		// �L���t���[����������
		g_BossLaser[i].frame = 0;

		g_BossLaser[i].pattern = 0;


		// �e��L���ɂ���
		g_BossLaser[i].bEnable = true;

		g_laserhint[i].bEnable = true;
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

const Capsule2D* BossLaser_GetCollision(int index)
{
	return &g_BossLaser[index].coco;
}


bool BossLaser_Iscoool(int index)
{
	return g_BossLaser[index].coool;
}

void BossLaser_IscooolFlase(int index)
{
	g_BossLaser[index].coool = false;
}

void BossLaser_IscooolTrue(int index)
{
	g_BossLaser[index].coool = true;
}

void Laserhit(bool f)
{
	g_BossLaser[0].hit = f;
}

bool Get_laserhit()
{
	return g_BossLaser[0].hit;
}