#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "collision.h"
#include "enemybullet.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define ENEMYBULLET_WIDTH  (32)
#define ENEMYBULLET_HEIGHT (32)
#define ENEMYBULLET_SPEED_X (3.0f)
#define ENEMYBULLET_SPEED_Y (3.0f)
#define ENEMYBULLET_VISIBLE_FRAME 300

//
//typedef struct
//{
//	float x, y;				//�\���ʒu
//	float move_x, move_y;	//
//	float rotation;
//	CIRCLE collision;
//	float frame;
//	bool bEnable;
//
//}EnemyBullet;
//
//static EnemyBullet g_EnemyBullets[ENEMYBULLET_MAX];
//
//void EnemyBullet_Initialize(void)
//{
//	//�z����̑S�Ă𖳌�������
//	for( int i = 0; i < ENEMYBULLET_MAX; i++ ) {
//		g_EnemyBullets[i].bEnable = false;
//	}
//}
//
//void EnemyBullet_Update(void)
//{
//	for (int i = 0; i < ENEMYBULLET_MAX; i++) {
//
//		if (g_EnemyBullets[i].bEnable) {
//			g_EnemyBullets[i].x += g_EnemyBullets[i].move_x * ENEMYBULLET_SPEED_X;
//			g_EnemyBullets[i].y += g_EnemyBullets[i].move_y * ENEMYBULLET_SPEED_Y;
//			g_EnemyBullets[i].collision.cx = g_EnemyBullets[i].x;
//			g_EnemyBullets[i].collision.cy = g_EnemyBullets[i].y;
//
//			// ��ʊO�ɏo����
//			int tw = Texture_GetWidth(TEXTURE_INDEX_BULLET);
//			if (g_EnemyBullets[i].x - tw * 0.5f > SCREEN_WIDTH) {
//				g_EnemyBullets[i].bEnable = false;
//			}
//
//			// �L���t���[���𒴂�����
//			++g_EnemyBullets[i].frame;
//			if (g_EnemyBullets[i].frame > ENEMYBULLET_VISIBLE_FRAME){
//				g_EnemyBullets[i].bEnable = false;
//			}
//		}
//	}
//}
//
//void EnemyBullet_Draw(void)
//{
//	for (int i = 0; i < ENEMYBULLET_MAX; i++) {
//
//		if (g_EnemyBullets[i].bEnable) {
//			int tw = Texture_GetWidth(TEXTURE_INDEX_BULLET);
//			int th = Texture_GetHeight(TEXTURE_INDEX_BULLET);
//			Sprite_Draw(TEXTURE_INDEX_BULLET, 
//				g_EnemyBullets[i].x,
//				g_EnemyBullets[i].y,
//				0,
//				0,
//				tw,
//				th,
//				tw * 0.5f,
//				th * 0.5f,
//				1.0f,
//				1.0f,
//				g_EnemyBullets[i].rotation);
//		}
//	}
//}
//
//void EnemyBullet_Create(float x, float y, D3DXVECTOR2 dir)
//{
//	for (int i = 0; i < ENEMYBULLET_MAX; i++) {
//
//		// �����e�����łɗL����������H
//		if (g_EnemyBullets[i].bEnable) {
//			continue;
//		}
//
//		// �e�̍��W�Ɉ����̒l��������
//		g_EnemyBullets[i].x = x;
//		g_EnemyBullets[i].y = y;
//
//		// �e�̈ړ��������w�肷��
//		D3DXVec2Normalize(&dir, &dir);
//		g_EnemyBullets[i].move_x = dir.x;
//		g_EnemyBullets[i].move_y = dir.y;
//
//		// �e�̈ړ���������e�̉�]�p�x�����߂�
//		g_EnemyBullets[i].rotation = atan2f(dir.y, dir.x) + M_PI / 2;
//
//		g_EnemyBullets[i].collision.cx = x;
//		g_EnemyBullets[i].collision.cy = y;
//		g_EnemyBullets[i].collision.r = ENEMYBULLET_WIDTH * 0.8f;
//
//		// �L���t���[����������
//		g_EnemyBullets[i].frame = 0;
//
//		// �e��L���ɂ���
//		g_EnemyBullets[i].bEnable = true;
//
//		break;
//	}
//}
//
//void EnemyBullet_Destroy(int index)
//{
//	g_EnemyBullets[index].bEnable = false;
//}
//
//bool EnemyBullet_IsEnable(int index)
//{
//	return g_EnemyBullets[index].bEnable;
//}
//
//const CIRCLE* EnemyBullet_GetCollision(int index)
//{
//	return &g_EnemyBullets[index].collision;
//}

