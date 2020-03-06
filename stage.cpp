#include "main.h"
#include "texture.h"
#include "sprite.h"
#include "stage.h"
#include "player.h"
#include <time.h>
#include <stdlib.h>


#define BGNUM 5
#define UISIZE 270
#define MAXCOUNT 3	//何ステージ進んだらゴール

/*float x1;//スクロールテクスチャ1の位置
float x2;//スクロールテクスチャ2の位置
bool flg1;//スクロールテクスチャ1を動かすフラグ
bool flg2;//スクロールテクスチャ2を動かすフラグ
bool flg_bgchange1, flg_bgchange2;
int bgnum1, bgnum2;*/

typedef struct
{
	D3DXVECTOR2 pos; //スクロールテクスチャの位置

}FIELD_DATA;

//グローバル変数
static float Playerpos;
static PLAYER player;


FIELD_DATA field[2];

/*

配列で背景の画像を管理して描画させる

*/


void Field1_Initialize(void)
{
	//初期位置を設定
	field[0].pos.x = 0;
	field[1].pos.x = 0;
	field[0].pos.y = 0;
	field[1].pos.y = 0;

	srand((unsigned int)time(NULL));
}


void Field1_Update(void)
{
	//player構造体の情報を取得
	player = GetPlayer();

	if (!player.camerastop) {
		if (!(507.0f < player.pos.x && player.pos.x < 517.0f))
		{
			if (player.pos.x > SCREEN_WIDTH / 2) {
				if (player.speed.x >= 5.0f) {
					player.speed.x *= 2;
				}
			}
			if (player.pos.x < SCREEN_WIDTH / 2) {
				if (player.speed.x <= -5.0f) {
					player.pos.x *= -1;
				}
			}
		}
		else
		{
			if (player.speed.x < 0) {
				player.speed.x = 0.0f;
			}
		}
			field[0].pos.x -= player.speed.x;//背景をプレイヤーの移動と合わせる
	}

	//if (field[0].pos.x >= -5.0f&&field[0].pos.x <= 5.0f&&!player.camerastop) {
	//	//player.camerastop = true;
	//	ChengeCamerastop(player.camerastop);
	//}
	

	if (field[0].pos.x != 0) {
		if (field[0].pos.x > 0) {
			field[1].pos.x = field[0].pos.x - 1024.0f;
			//field[1].pos.x -= Player_Getdir();
		}
		else {
			field[1].pos.x = field[0].pos.x + 1024.0f;
			//field[1].pos.x -= Player_Getdir();
		}
	}

	if (field[0].pos.x > 1024.0f) {
		field[0].pos.x = field[1].pos.x - 1024.0f;
	}
	else if (field[0].pos.x < -1024.0f) {
		field[0].pos.x = field[1].pos.x + 1024.0f;
	}

}

void Field1_Finalize(void)
{

}

void Field1_Draw(void)
{
	Sprite_Draw(TEXTURE_INDEX_BGMAIN, field[1].pos.x, 0.0f);

	Sprite_Draw(TEXTURE_INDEX_BGLOOP, field[0].pos.x, 0.0f);
}
