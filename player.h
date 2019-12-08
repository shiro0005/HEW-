#pragma once

#include "collision.h"
#include "2D.h"

typedef struct
{
	D3DXVECTOR2 pos;			//位置
	int			muki;			//向き
	int			color;			//色
	D3DXVECTOR2 speed;          //プレイヤー移動
	int			animePattern;	//アニメパターン
	bool        firstAT;        //初動攻撃
	int			mode;           //プレイヤー状態
	int         commbo;			//プレイヤーコンボ数
	Capsule2D   collision;		//カプセルの当たり判定
	Capsule2D   foot[2];        //足をカプセルで当たり判定を作る　[0]右足　[1]左足   
	int			hitpoint;		//ヒットポイント
	float rotate;

}PLAYER;



void Player_Initialize(void);
void Player_Finalize(void);
void Player_Update(void);
void Player_Draw(void);
void Player_AddDamage(int damage);
PLAYER GetPlayer();
void PlayerInfoMatch(PLAYER info);

//const Capsule2D* Player_GetCollision();
//const Capsule2D* Player_GetCollisionFR();
//const Capsule2D* Player_GetCollisionFL();
//bool GetfirstAT();
//float Player_Getdir();
//int Player_GetHitPoint();
