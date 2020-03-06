#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "collision.h"
#include "bossthunder.h"
#include "2D.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include "collisioncheck.h"

#define BOSSTHUNDER_IMAGEMAX (100)

#define BOSSTHUNDER_MAX    (1)
#define BOSSTHUNDER_WIDTH  (64)
#define BOSSTHUNDER_HEIGHT (64)
#define BOSSTHUNDER_SPEED	(32)
#define BOSSTHUNDER_VISIBLE_FRAME 100
#define BOSSTHUNDER_PATTERN_FRAME (1)
#define BOSSTHUNDER_PATTERN_MAX   (22)
#define BOSSTHUNDER_PATTERN_H_MAX (4)

#define THUNDERHINT_MAX (512)
#define THUNDERHINT_WIDTH (64)
#define THUNDERHINT_HEIGHT (64)
#define THUNDERHINT_PATTERN_FRAME (1)
#define THUNDERHINT_PATTERN_MAX   (52)
#define THUNDERHINT_PATTERN_H_MAX (4)
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

}BossThunder;

int thunderhint_countdown;

BossThunder g_thunderhint[THUNDERHINT_MAX];

BossThunder g_BossThunder[BOSSTHUNDER_MAX];


void BossThunder_Initialize(void)
{
	//配列内の全てを無効化する
	for (int i = 0; i < BOSSTHUNDER_MAX; i++) {
		g_BossThunder[i].bEnable = false;
		g_BossThunder[i].hit = false;
		g_thunderhint[i].bEnable = false;
	}
}

void BossThunder_Update(void)
{
	for (int i = 0; i < BOSSTHUNDER_MAX; i++) 
	{


		if (g_BossThunder[i].bEnable) {

			thunderhint_countdown++;

			if (thunderhint_countdown >= 500) {
				g_BossThunder[i].frame++;
				if (g_BossThunder[i].frame % 4 == 0) {
					g_BossThunder[i].pattern++;
				}
				//最後のパターンが表示されたら終了する処理
				if (g_BossThunder[i].pattern >= BOSSTHUNDER_PATTERN_MAX) {
					g_BossThunder[i].bEnable = false;
					thunderhint_countdown = 0;
					g_BossThunder[0].hit = false;
				}
			}
		}
		
	}

	for (int i = 0; i < THUNDERHINT_MAX; i++) {
		if (g_thunderhint[i].bEnable) {

			if (g_thunderhint[i].frame % 4 == 0) {
				g_thunderhint[i].pattern++;
			}

			//最後のパターンが表示されたら終了する処理
			if (g_thunderhint[i].pattern >= THUNDERHINT_PATTERN_MAX) {
				g_thunderhint[i].bEnable = false;
			}
		}
	}
}
	//		//座標更新
	//		for (int n = 0; n < BOSSTHUNDER_IMAGEMAX; n++) {
	//			if (g_BossThunder[i].thunderIMG[n].bEnable)
	//			{
	//				g_BossThunder[i].thunderIMG[n].x += g_BossThunder[i].thunderIMG[n].move_x * BOSSTHUNDER_SPEED;
	//				g_BossThunder[i].thunderIMG[n].y += g_BossThunder[i].thunderIMG[n].move_y * BOSSTHUNDER_SPEED;

	//				++g_BossThunder[i].thunderIMG[n].frame;
	//				if (g_BossThunder[i].thunderIMG[n].frame > 40)
	//					g_BossThunder[i].thunderIMG[n].bEnable = false;

	//			}
	//		}

	//		//描画追加
	//		for (int n = 0; n < BOSSTHUNDER_IMAGEMAX; n++)
	//		{
	//			if (!g_BossThunder[i].thunderIMG[n].bEnable)
	//			{
	//				g_BossThunder[i].thunderIMG[n].x = g_BossThunder[i].x;
	//				g_BossThunder[i].thunderIMG[n].y = g_BossThunder[i].y;
	//				g_BossThunder[i].thunderIMG[n].move_x = g_BossThunder[i].move_x;
	//				g_BossThunder[i].thunderIMG[n].move_y = g_BossThunder[i].move_y;
	//				g_BossThunder[i].thunderIMG[n].rotation = g_BossThunder[i].rotation;
	//				g_BossThunder[i].thunderIMG[n].frame = 0;
	//				g_BossThunder[i].thunderIMG[n].bEnable = true;
	//				break;

	//			}
	//		}

	//		// 有効フレームを超えたら
	//		++g_BossThunder[i].frame;
	//		if (g_BossThunder[i].frame > BOSSTHUNDER_VISIBLE_FRAME)
	//			g_BossThunder[i].bEnable = false;
	//	}
	//}




void BossThunder_Draw(void)
{
	if (thunderhint_countdown > 0 && thunderhint_countdown < 500)
	{
		for (int i = 0; i < THUNDERHINT_MAX; i++) {

			if (!g_thunderhint[i].bEnable) {
				continue;
			}

			// 現在表示するべきパターン番号から切り取り座標を算出する
			int tx = THUNDERHINT_WIDTH * (g_thunderhint[i].pattern %  THUNDERHINT_PATTERN_H_MAX);
			int ty = THUNDERHINT_HEIGHT * (g_thunderhint[i].pattern / THUNDERHINT_PATTERN_H_MAX);


			Sprite_Draw(TEXTURE_INDEX_HINT,
				g_thunderhint[i].x,
				SCREEN_HEIGHT / 2,
				tx,
				ty,
				THUNDERHINT_WIDTH,
				THUNDERHINT_HEIGHT,
				THUNDERHINT_WIDTH / 2,
				THUNDERHINT_HEIGHT / 2,
				3.0f,
				8.0f,
				0.0f);
		}
	}
	else if (thunderhint_countdown >= 500)
	{
		for (int i = 0; i < BOSSTHUNDER_MAX; i++) {

			if (!g_BossThunder[i].bEnable) {
				continue;
			}

			// 現在表示するべきパターン番号から切り取り座標を算出する
			int tx = BOSSTHUNDER_WIDTH * (g_BossThunder[i].pattern % BOSSTHUNDER_PATTERN_H_MAX);
			int ty = BOSSTHUNDER_HEIGHT * (g_BossThunder[i].pattern / BOSSTHUNDER_PATTERN_H_MAX);

			Sprite_Draw(TEXTURE_INDEX_LASER,
				g_BossThunder[i].x,
				SCREEN_HEIGHT / 2,
				tx,
				ty,
				BOSSTHUNDER_WIDTH,
				BOSSTHUNDER_HEIGHT,
				BOSSTHUNDER_WIDTH / 2,
				BOSSTHUNDER_HEIGHT / 2,
				3.0f,
				8.0f,
				0.0f);
		}
	}
}

void BossThunder_Create(int muki, float x, float y, D3DXVECTOR2 dir)
{
	for (int i = 0; i < BOSSTHUNDER_MAX; i++) {

		// もし弾がすでに有効だったら？
		if (g_BossThunder[i].bEnable) {
			continue;
		}

		// 弾の座標に引数の値を代入する
		g_BossThunder[i].x = x;
		g_BossThunder[i].y = y;

		// 弾の移動方向を指定する
		D3DXVec2Normalize(&dir, &dir);
		g_BossThunder[i].move_x = dir.x;
		g_BossThunder[i].move_y = dir.y;


		// 弾の移動方向から弾の回転角度を求める
		g_BossThunder[i].rotation = atan2f(dir.y, dir.x) + M_PI / 2;

		// カプセルの設定
		g_BossThunder[i].coco.s.p.x = x;
		g_BossThunder[i].coco.s.p.y = y;
		g_BossThunder[i].coco.s.v.x = 0.0f;
		g_BossThunder[i].coco.s.v.y = +380.0f;
		g_BossThunder[i].coco.r = BOSSTHUNDER_WIDTH * 0.8f;

		// 有効フレームを初期化
		g_BossThunder[i].frame = 0;

		g_BossThunder[i].pattern = 0;

		//for (int n = 0; n < BOSSTHUNDER_IMAGEMAX; n++) {
		//	g_BossThunder[i].thunderIMG[n].bEnable = false;
		//}

		// 弾を有効にする
		g_BossThunder[i].bEnable = true;

		g_thunderhint[i].bEnable = true;

		break;
	}
}

void BossThunder_Destroy(int index)
{
	g_BossThunder[index].bEnable = false;
}

bool BossThunder_IsEnable(int index)
{
	return g_BossThunder[index].bEnable;
}

const Capsule2D* BossThunder_GetCollision(int index)
{
	return &g_BossThunder[index].coco;
}


bool BossThunder_Iscoool(int index)
{
	return g_BossThunder[index].coool;
}

void BossThunder_IscooolFlase(int index)
{
	g_BossThunder[index].coool = false;
}

void BossThunder_IscooolTrue(int index)
{
	g_BossThunder[index].coool = true;
}


void thunderhit(bool f)
{
	g_BossThunder[0].hit = f;
}

bool Get_Thunderhit()
{
	return g_BossThunder[0].hit;
}