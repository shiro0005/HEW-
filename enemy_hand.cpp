#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "collision.h"
#include "enemy_hand.h"
#include "2D.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define BULLET_WIDTH  (32)
#define BULLET_HEIGHT (32)
#define BULLET_SPEED_X (1.0f)
#define BULLET_SPEED_Y (1.0f)
#define BULLET_VISIBLE_FRAME 25


typedef struct Bullet_tag
{
	D3DXVECTOR2 pos;
	float x, y;				//表示位置
	float move_x, move_y;	//
	float rotation;
	Capsule2D   colcol;
	float frame;
	bool bEnable;
	bool cool;
} Bullet;

static Bullet g_Bullets[BULLET_MAX];

void Bullet_Initialize(void)
{
	//配列内の全てを無効化する
	for( int i = 0; i < BULLET_MAX; i++ ) {
		g_Bullets[i].bEnable = false;
		g_Bullets[i].cool = false;
	}
}

void Bullet_Update(void)
{
	for( int i = 0; i < BULLET_MAX; i++ ) {

		if( g_Bullets[i].bEnable ) {
			g_Bullets[i].pos.x += g_Bullets[i].move_x * BULLET_SPEED_X;
			g_Bullets[i].pos.y += g_Bullets[i].move_y * BULLET_SPEED_Y;
			g_Bullets[i].colcol.r = BULLET_WIDTH;
			g_Bullets[i].colcol.s.p.x = g_Bullets[i].pos.x - 32.0f;
			g_Bullets[i].colcol.s.p.y = g_Bullets[i].pos.y;
			g_Bullets[i].colcol.s.v.x = 64.0f;
			g_Bullets[i].colcol.s.v.y = 0.0f;
			//// 画面外に出たら
			//int tw = Texture_GetWidth(TEXTURE_INDEX_BULLET);
			//if( g_Bullets[i].x - tw * 0.5f > SCREEN_WIDTH ) {
			//	g_Bullets[i].bEnable = false;
			//}

			// 有効フレームを超えたら
			++g_Bullets[i].frame;
			if(g_Bullets[i].frame > BULLET_VISIBLE_FRAME){
				g_Bullets[i].bEnable = false;
			}
		}
	}
}

void Bullet_Draw(void)
{
	/*for( int i = 0; i < BULLET_MAX; i++ ) {

		if( g_Bullets[i].bEnable ) {
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
				1.0f,
				1.0f,
				g_Bullets[i].rotation);
		}
	}*/
	for (int i = 0; i < BULLET_MAX; i++) {

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
				2.0f,
				2.0f,
				g_Bullets[i].rotation);
		}
	}
}

void Bullet_Create(int muki, float x, float y, D3DXVECTOR2 dir)
{
	for( int i = 0; i < BULLET_MAX; i++ ) {

		// もし弾がすでに有効だったら？
		if( g_Bullets[i].bEnable ) {
			continue;
		}

		// 弾の座標に引数の値を代入する
		g_Bullets[i].pos.x = x;
		
		if (muki == 1)
		{
			g_Bullets[i].pos.x += -30.0f;
		}
		if (muki == 2)
		{
			g_Bullets[i].pos.x += 30.0f;
		}

		g_Bullets[i].pos.y = y - 20.0f;

		// 弾の移動方向を指定する
		D3DXVec2Normalize(&dir, &dir);
		g_Bullets[i].move_x = dir.x;
		g_Bullets[i].move_y = dir.y;

		// 弾の移動方向から弾の回転角度を求める
		g_Bullets[i].rotation = atan2f(dir.y, dir.x) + M_PI / 2;

		g_Bullets[i].colcol.s.p.x = g_Bullets[i].pos.x - 32.0f;
		g_Bullets[i].colcol.s.p.y = g_Bullets[i].pos.y;
		g_Bullets[i].colcol.s.v.x = 64.0f;
		g_Bullets[i].colcol.s.v.y = 0.0f;
		// 有効フレームを初期化
		g_Bullets[i].frame = 0;

		// 弾を有効にする
		g_Bullets[i].bEnable = true;
		g_Bullets[i].cool = true;

		break;
	}
}

void Bullet_Destroy(int index)
{
	g_Bullets[index].bEnable = false;
}

bool Bullet_IsEnable(int index )
{
	return g_Bullets[index].bEnable;
}

const Capsule2D* Bullet_GetCollision(int index)
{
	return &g_Bullets[index].colcol;
}

bool Bullet_Iscool(int index)
{
	return g_Bullets[index].cool;
}

void Bullet_IscoolFlase(int index)
{
	g_Bullets[index].cool=false;
}

void Bullet_IscoolTrue(int index)
{
	g_Bullets[index].cool = true;
}