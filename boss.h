#pragma once

#include "collision.h"
#include "2D.h"

typedef struct
{
	int			enable;	//生存フラグ

	D3DXVECTOR2 pos;	//位置
	float		rot;	//回転
	int			color;	//色
	int			muki;	//向き（移動方向）
	int			animePattern;	//アニメパターン
	int			animeFrame;		//パターン切り替えウエイト
	int			hp;


	Capsule2D   colcol;

	int			frame;
	int			frame_attack;  //攻撃モーションフレーム
	bool		animeAttack;
	bool		stay;		//攻撃中の移動判定
	bool		ready_attack;
	bool		attack;
	bool       laserattack;
	bool		move;		//吹っ飛ばされた後えねみーの追尾処理を止める
	bool		nock;
	bool        bossstop;
	bool        bosslasermodestop;
	bool       bossleftstop;
	int			frame_alpha;
	int			state;
	D3DXVECTOR2 dir_destroy;		//吹っ飛ばされる方向
	D3DXVECTOR2 pos_return;
	D3DXVECTOR2 dir_shot;

	int			bosshitpoint;		//ヒットポイント
	float 		t;

}BOSS_DATA;


#define BOSS_COUNT 1

void Boss_Initialize(void);
void Boss_Finalize(void);
void Boss_Update(void);
void Boss_Draw(void);
void Boss_Destroy(int index);
bool Boss_IsEnable(int index);

int Boss_AddDamage(int damage);
int Boss_GetHitPoint();
const Capsule2D* Boss_GetCollision(int index);
void Boss_StateInit(int index);
void Boss_Attack(int index);
void Boss_NockBack(int index);

BOSS_DATA GetBoss(int);
void BossInfoMatch(BOSS_DATA info, int i);