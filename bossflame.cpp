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
	float		x, y;			//表示位置
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
	//配列内の全てを無効化する
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
				//最後のパターンが表示されたら終了する処理
				if (g_BossFlame[i].pattern >= BOSSFLAME_PATTERN_MAX) {
					g_BossFlame[i].bEnable = false;
					flamehint_countdown = 0;
					g_BossFlame[0].hit = false;
				}
			}
		}
	}

	//		//座標更新
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

	//		//描画追加
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

	//		// 有効フレームを超えたら
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

		// 現在表示するべきパターン番号から切り取り座標を算出する
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

			// 現在表示するべきパターン番号から切り取り座標を算出する
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

		// もし弾がすでに有効だったら？
		if (g_BossFlame[i].bEnable) {
			continue;
		}

		// 弾の座標に引数の値を代入する
		g_BossFlame[i].x = x;
		g_BossFlame[i].y = y;

		// 弾の移動方向を指定する
		D3DXVec2Normalize(&dir, &dir);
		g_BossFlame[i].move_x = dir.x;
		g_BossFlame[i].move_y = dir.y;


		// 弾の移動方向から弾の回転角度を求める
		g_BossFlame[i].rotation = atan2f(dir.y, dir.x) + M_PI / 2;

		// カプセルの設定
		g_BossFlame[i].coco.s.p.x = x;
		g_BossFlame[i].coco.s.p.y = y;
		g_BossFlame[i].coco.s.v.x = 0.0f;
		g_BossFlame[i].coco.s.v.y = +380.0f;
		g_BossFlame[i].coco.r = BOSSFLAME_WIDTH * 0.8f;

		// 有効フレームを初期化
		g_BossFlame[i].frame = 0;

		g_BossFlame[i].pattern = 0;
		

		// 弾を有効にする
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