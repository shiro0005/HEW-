#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "collision.h"
#include "bossbullet.h"
#include "2D.h"
#include "collisioncheck.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define BOSSBULLET_WIDTH  (64)
#define BOSSBULLET_HEIGHT (64)
#define BOSSBULLET_SPEED_X (3.0f)
#define BOSSBULLET_SPEED_Y (0.8f)
#define BOSSBULLET_VISIBLE_FRAME 25


typedef struct BossBullet_tag
{
	D3DXVECTOR2 pos;
	float x, y;				//表示位置
	float move_x, move_y;	//
	float rotation;
	Capsule2D   colcol;
	float frame;
	bool bEnable;
	bool cool;
} BossBullet;

static BossBullet g_BossBullets[BOSSBULLET_MAX];

void BossBullet_Initialize(void)
{
	//配列内の全てを無効化する
	for (int i = 0; i < BOSSBULLET_MAX; i++) {
		g_BossBullets[i].bEnable = false;
		g_BossBullets[i].cool = false;
	}
}

void BossBullet_Update(void)
{
	for (int i = 0; i < BOSSBULLET_MAX; i++) {

		if (g_BossBullets[i].bEnable) {
			g_BossBullets[i].pos.x += g_BossBullets[i].move_x * BOSSBULLET_SPEED_X;
			g_BossBullets[i].pos.y += g_BossBullets[i].move_y * BOSSBULLET_SPEED_Y;
			g_BossBullets[i].colcol.r = BOSSBULLET_WIDTH*1.8f;
			g_BossBullets[i].colcol.s.p.x = g_BossBullets[i].pos.x - 20.0f;
			g_BossBullets[i].colcol.s.p.y = g_BossBullets[i].pos.y;
			g_BossBullets[i].colcol.s.v.x = 128.0f;
			g_BossBullets[i].colcol.s.v.y = 0.0f;
			//// 画面外に出たら
			//int tw = Texture_GetWidth(TEXTURE_INDEX_BULLET);
			//if( g_Bullets[i].x - tw * 0.5f > SCREEN_WIDTH ) {
			//	g_Bullets[i].bEnable = false;
			//}

			// 有効フレームを超えたら
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
				g_BossBullets[i].pos.x,
				g_BossBullets[i].pos.y,
				0,
				0,
				tw,
				th,
				tw * 0.5f,
				th * 0.5f,
				4.0f,
				4.0f,
				g_BossBullets[i].rotation);
		}
			if (g_BossBullets[i].bEnable) {
				colcheck(g_BossBullets[i].colcol);
			}
	}
}

void BossBullet_Create(int muki, float x, float y, D3DXVECTOR2 dir)
{
	for (int i = 0; i < BOSSBULLET_MAX; i++) {

		// もし弾がすでに有効だったら？
		if (g_BossBullets[i].bEnable) {
			continue;
		}

		// 弾の座標に引数の値を代入する
		g_BossBullets[i].pos.x = x;

		if (muki == 0)
		{
			g_BossBullets[i].pos.x -= 90.0f;
		}
		if (muki == 1)
		{
			g_BossBullets[i].pos.x += 60.0f;
		}

		g_BossBullets[i].pos.y = y - 60.0f;

		// 弾の移動方向を指定する
		D3DXVec2Normalize(&dir, &dir);
		g_BossBullets[i].move_x = dir.x;
		g_BossBullets[i].move_y = 2.0f;

		// 弾の移動方向から弾の回転角度を求める
		g_BossBullets[i].rotation = atan2f(dir.y, dir.x) + M_PI / 2;

		g_BossBullets[i].colcol.s.p.x = g_BossBullets[i].pos.x - 32.0f;
		g_BossBullets[i].colcol.s.p.y = g_BossBullets[i].pos.y;
		g_BossBullets[i].colcol.s.v.x = 64.0f;
		g_BossBullets[i].colcol.s.v.y = 0.0f;
		// 有効フレームを初期化
		g_BossBullets[i].frame = 0;

		// 弾を有効にする
		g_BossBullets[i].bEnable = true;
		g_BossBullets[i].cool = true;

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

const Capsule2D* BossBullet_GetCollision(int index)
{
	return &g_BossBullets[index].colcol;
}

bool BossBullet_Iscool(int index)
{
	return g_BossBullets[index].cool;
}

void BossBullet_IscoolFlase(int index)
{
	g_BossBullets[index].cool = false;
}

void BossBullet_IscoolTrue(int index)
{
	g_BossBullets[index].cool = true;
}

float GetHandPos(int i)
{
	return g_BossBullets[i].pos.x;
}