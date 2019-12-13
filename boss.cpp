#include "boss.h"
#include "main.h"
#include "sprite.h"
#include "enemy_table.h"
#include "player.h"
#include "bossbullet.h"
#include "bosslaser.h"
#include "explosion.h"
#include "enemy_hand.h"
#include "game.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define BOSS_WIDTH				(128)
#define BOSS_HEIGHT			    (128)
#define ANIME_PATTERN_MAX		(3)
#define ANIME_PATTERN_SKIPFRAME (8)

#define BOSS_SEARCH_RADIUS		(200.0f)
#define BOSS_SEARCH_ANGLE		(0.85f)
#define BOSS_CHASE_SPEED		(3.0f)
#define BOSS_RETURN_SPEED		(1.0f)

#define BOSS_DESTROY_SPEED		(20.0f)


typedef struct
{
	int			enable;	//生存フラグ

	D3DXVECTOR2 pos;	//位置
	float		rot;	//回転
	int			color;	//色
	int			muki;	//向き（移動方向）

	int			animePattern;	//アニメパターン
	int			animeFrame;		//パターン切り替えウエイト

	Capsule2D		collision;

	int			frame;
	int			frame_attack;  //攻撃モーションフレーム
	bool		animeAttack;
	bool		stay;		//攻撃中の移動判定
	bool		ready_attack;
	bool		attack;
	bool		move;		//吹っ飛ばされた後えねみーの追尾処理を止める
	int			frame_alpha;
	int			state;
	D3DXVECTOR2 dir_destroy;		//吹っ飛ばされる方向
	D3DXVECTOR2 pos_return;

	int			bosshitpoint;		//ヒットポイント
	int			t;
}BOSS_DATA;

//グローバル変数
BOSS_DATA boss[BOSS_COUNT];

float destroy_boss_pos_x[BOSS_COUNT];
float destroy_boss_pos_y[BOSS_COUNT];
int destroy_boss_dir;
static PLAYER player;

typedef enum
{
	//BOSS_STATE_INIT,		//初期化
	//BOSS_STATE_SEARCH,		//周囲をサーチ
	//BOSS_STATE_FIND,		//敵を発見
	//BOSS_STATE_CHASE,		//追跡
	//BOSS_STATE_SHOT,		//ショット発射
	//BOSS_STATE_LASER,		//レーザー発射
	//BOSS_STATE_COOLDOWN,	//ショット終了
	//BOSS_STATE_RETURN,		//定位置に戻る
	//BOSS_STATE_DEAD,		//死亡状態
	BOSS_STATE_INIT,		//初期化
	BOSS_STATE_CHASE,		//追跡
	BOSS_STATE_DEAD,		//死亡状態

	BOSS_STATE_MAX

}BOSS_STATE;

//void Boss_StateSearch(int index);
void Boss_StateInit(int index);
void Boss_StateChase(int index);


void Boss_Initialize(void)
{
	//敵の初期化
	for (int i = 0; i < BOSS_COUNT; i++)
	{
		boss[i].enable = FALSE;
	}
}

void Boss_Finalize(void)
{

}

void Boss_Update(void)
{
	//player構造体の情報を取得
	player = GetPlayer();

	for (int i = 0; i < BOSS_COUNT; i++) {

		if (boss[i].bosshitpoint <= 0)
		{
			Boss_Destroy(i);
		}

		if (boss[i].move == FALSE)
		{
			if (!boss[i].enable) {
				continue;
			}

			if (boss[i].pos.y < 0.0f)
			{
				boss[i].enable = FALSE;
				Game_AddKillBossCount();
				break;
			}
			if (boss[i].pos.y < 0.0f)
			{
				boss[i].enable = FALSE;
				Game_AddKillBossCount();
				break;
			}

			if (boss[i].pos.y > SCREEN_HEIGHT + 50.0f)
			{
				boss[i].enable = FALSE;
				Game_AddKillBossCount();
				break;
			}
			if (boss[i].pos.y < -50.0f)
			{
				boss[i].enable = FALSE;
				Game_AddKillBossCount();
				break;
			}
		}
	}

	for (int i = 0; i < BOSS_COUNT; i++)
	{
		if (boss[i].bosshitpoint >= 1)
		{
			if (boss[i].t <= 1)
			{//ノックバック
				boss[i].pos.x = destroy_boss_pos_x[i] + (boss[i].t*destroy_boss_dir*100.0f);
				boss[i].pos.y = destroy_boss_pos_y[i] - (3 * boss[i].t*((1 - boss[i].t)*(1 - boss[i].t)))*100.0f;
				boss[i].t = boss[i].t + 0.02;
			}
		}
	}

	//当たり判定用座標の更新
	boss[0].collision.r = BOSS_WIDTH * 0.8f;
	boss[0].collision.s.p.x = boss[0].pos.x;
	boss[0].collision.s.p.y = boss[0].pos.y - 25.0f;
	boss[0].collision.s.v.x = 0.0f;
	boss[0].collision.s.v.y = 50.0f;

	//スキップするフレーム値を超えたら
	if (++boss[0].animeFrame > ANIME_PATTERN_SKIPFRAME)
	{
		//アニメパターンを進める(最大値を超えたらリセット)
		if (++boss[0].animePattern >= ANIME_PATTERN_MAX)
			boss[0].animePattern = 0;
		//フレームは元に戻す
		boss[0].animeFrame = 0;

		boss[0].pos.x -= player.speed.x * 10;//エネミーをプレイヤーの移動と合わせる
	}

	switch (boss[0].state)
	{
	case BOSS_STATE_INIT:
		Boss_StateInit(0);
		break;
		/*case ENEMY_STATE_SEARCH:
			Enemy_StateSearch(i);
			break;
		case ENEMY_STATE_FIND:
			Enemy_StateFind(i);
			break;*/
	case BOSS_STATE_CHASE:
		Boss_StateChase(0);
		break;
		/*case ENEMY_STATE_SHOT:
			Enemy_StateShot(i);
			break;
		case ENEMY_STATE_LASER:
			Enemy_StateLaser(i);
			break;
		case ENEMY_STATE_COOLDOWN:
			Enemy_StateCooldown(i);
			break;
		case ENEMY_STATE_RETURN:
			Enemy_StateReturn(i);
			break;*/
	default:
		break;
	}
}

void Boss_Draw(void)
{
	for (int i = 0; i < BOSS_COUNT; i++) {

		//生存フラグがTRUEの敵のみ処理を行う
		if (!boss[i].enable) {
			continue;
		}

		Sprite_Draw(TEXTURE_INDEX_ENEMY,
			boss[i].pos.x,
			boss[i].pos.y,
			GetAnimTbl2(boss[i].color, boss[i].muki, boss[i].animePattern).x * 256,
			GetAnimTbl2(boss[i].color, boss[i].muki, boss[i].animePattern).y * 256,
			32,
			32,
			16,
			16,
			8.0f,
			8.0f,
			boss[i].rot);
	}
}

void Boss_Destroy(int index)
{
	boss[index].move = FALSE;
	D3DXVECTOR2 pl_pos = D3DXVECTOR2(player.collision.s.p.x, player.collision.s.p.y);

	if (pl_pos.x - destroy_boss_pos_x[index] < 0)
	{
		boss[index].dir_destroy.x = 2.0f;
	}
	else
	{
		boss[index].dir_destroy.x = -2.0f;
	}
	boss[index].dir_destroy.y = -1.5f;
	D3DXVec2Normalize(&boss[index].dir_destroy, &boss[index].dir_destroy);

	boss[index].dir_destroy *= BOSS_DESTROY_SPEED;
}

bool Boss_IsEnable(int index)
{
	return boss[index].enable;
}

//const Capsule2D* Boss_GetCollision(int index)
//{
//	return &boss[index].collision;
//}

//向きを作る
//int Boss_CalcMuki(D3DXVECTOR2 dir)
//{
//	int muki = 0;
//
//	//移動方向が有効かチェック
//	if (D3DXVec2Length(&dir) > 0.01f)
//	{
//		//X成分とY成分の大きさを比べて、どちらの方が大きいのかを調べる
//		//X成分の方が大きい場合
//		if (fabs(dir.x) > fabs(dir.y))
//		{
//			//X成分がプラスの値なのかマイナスの値なのかを調べる
//			//マイナス成分の場合は左向き
//			if (dir.x < 0.0f)
//				muki = 1;
//			//そうじゃない（プラス成分）場合は右向き
//			else
//				muki = 2;
//		}
//		//Y成分の方が大きい場合
//		else
//		{
//			//Y成分がプラスの値なのかマイナスの値なのかを調べる
//			//マイナス成分の場合は上向き
//			if (dir.y < 0.0f)
//				muki = 3;
//			//そうじゃない（プラス成分）場合は上向き
//			else
//				muki = 0;
//		}
//	}
//
//	return muki;
//}

void Boss_StateInit(int index)//ボス出現情報
{
	/*enemy[index].pos.x = frand() * SCREEN_WIDTH;
	enemy[index].pos.y = frand() * SCREEN_HEIGHT;*/
	boss[index].rot = 0;
	boss[index].color = 0;
	boss[index].pos = { 1000,450 };
	boss[index].bosshitpoint = 10;
	boss[index].t = 2;

	boss[index].muki = 1;
	boss[index].enable = TRUE;

	boss[index].collision.s.p.x = boss[index].pos.x;
	boss[index].collision.s.p.y = boss[index].pos.y;
	boss[index].collision.r = BOSS_WIDTH * 0.8f;

	boss[index].frame = 0;
	boss[index].frame_attack = 0;
	boss[index].animeAttack = FALSE;
	boss[index].stay = FALSE;
	boss[index].ready_attack = TRUE;
	boss[index].attack = FALSE;
	boss[index].move = TRUE;


	//ステートをサーチ状態へ移行
	//enemy[index].state = ENEMY_STATE_SEARCH;

	boss[index].state = BOSS_STATE_CHASE;

}

//void Boss_StateSearch(int index)
//{
//	//プレイヤーの座標を取得する
//	D3DXVECTOR2 pl_pos = D3DXVECTOR2(player.collision.s.p.x, player.collision.s.p.y);
//
//	D3DXVECTOR2 dir = pl_pos - boss[index].pos;
//	float muki = pl_pos.x - boss[index].pos.x;
//	D3DXVec2Normalize(&dir, &dir);
//
//	if (muki < 0)
//	{//プレイヤーがエネミーの左にいるなら左に向く
//		boss[index].muki = 1;
//	}
//	else
//	{//プレイヤーがエネミーの右にいるなら右に向く
//		boss[index].muki = 2;
//	}
//
//	dir *= BOSS_CHASE_SPEED;
//
//	if (boss[index].move == TRUE)//エネミーが吹っ飛ばされていなかったら
//	{
//		if (boss[index].stay == FALSE)
//		{
//			boss[index].pos.x += dir.x;
//		}
//	}
//	else
//	{//吹っ飛ぶ処理
//		boss[index].pos += boss[index].dir_destroy;
//	}
//
//	boss[index].color = 0;  //通常状態
//
//	if (pl_pos.x + 200.0f > boss[index].pos.x&&pl_pos.x - 200.0f < boss[index].pos.x)//攻撃準備モーション判定
//	{
//		boss[index].attack = TRUE;
//	}
//
//	if (boss[index].attack == TRUE)//攻撃準備
//	{
//		//フレームを進める
//		boss[index].frame++;
//
//		boss[index].color = 1;
//		boss[index].stay = TRUE;
//
//		if (boss[index].frame > 30)
//		{
//			boss[index].animeAttack = TRUE;
//			if (boss[index].ready_attack == TRUE)
//			{
//				boss[index].ready_attack = FALSE;
//				Boss_Attack(index); //腕を振る
//			}
//		}
//
//		if (boss[index].animeAttack == TRUE)
//		{
//			//攻撃モーションフレームを進める
//			boss[index].frame_attack++;
//
//			if (boss[index].frame_attack < 30)
//			{
//				boss[index].color = 2;
//			}
//
//			if (boss[index].frame_attack >= 30)
//			{
//				boss[index].frame = 0;
//				boss[index].frame_attack = 0;
//
//				boss[index].animeAttack = FALSE;
//				boss[index].stay = FALSE;
//				boss[index].ready_attack = TRUE;
//				boss[index].attack = FALSE;
//			}
//		}
//	}
//}

/*
void Boss_StateFind(int index)
{
	//フレームを進める
	boss[index].frame++;

	//一定時間経ったら追跡ステートへ移行
	if (boss[index].frame > 20) {

		boss[index].frame = 0;

		//ステートをサーチ状態へ移行
		boss[index].state = BOSS_STATE_CHASE;
	}
}

void Boss_StateChase(int index)
{
	//フレームを進める
	boss[index].frame++;

	//プレイヤーの座標を取得する
	D3DXVECTOR2 pl_pos = D3DXVECTOR2(player.collision.s.p.x, player.collision.s.p.y);

	D3DXVECTOR2 dir = pl_pos - boss[index].pos;
	D3DXVec2Normalize(&dir, &dir);
	dir *= BOSS_CHASE_SPEED;

	boss[index].pos += dir;

	//移動方向から向きを作る
	boss[index].muki = Boss_CalcMuki(dir);

	//一定時間経ったら弾射出ステートへ移行
	if (boss[index].frame > 120) {

		boss[index].frame = 0;

		//移動方向を保存
		boss[index].dir_shot = dir;

		//ランダムで「ばら撒き弾」もしくは「レーザー」を射出
		if (rand() % 2)
		{
			//ステートをサーチ状態へ移行
			boss[index].state = BOSS_STATE_SHOT;
		}
		else {
			//ステートをサーチ状態へ移行
			boss[index].state = BOSS_STATE_LASER;
		}
	}
}

void Boss_StateShot(int index)
{
	//フレームを進める
	boss[index].frame++;

	float shot_rad = atan2(boss[index].dir_shot.y, boss[index].dir_shot.x);
	shot_rad = (float)(M_PI * 2 / 20)*boss[index].frame;

	//射出角ベクトルをフレーム値で回転させる
	D3DXVECTOR2 shot_dir;
	shot_dir.x = boss[index].dir_shot.x*cosf(shot_rad) - boss[index].dir_shot.y*sinf(shot_rad);
	shot_dir.y = boss[index].dir_shot.x*sinf(shot_rad) + boss[index].dir_shot.y*cosf(shot_rad);

	//BossBullet_Create(boss[index].pos.x, boss[index].pos.y, shot_dir);

	//一定時間経ったらクールダウンステートへ移行
	if (boss[index].frame > 90) {

		boss[index].frame = 0;

		//ステートを帰還状態へ移行
		boss[index].state = BOSS_STATE_COOLDOWN;
	}
}

void Boss_StateLaser(int index)
{
	if (boss[index].frame == 0) {
		//BossLaser_Create(boss[index].pos.x, boss[index].pos.y, boss[index].dir_shot);
	}

	//フレームを進める
	boss[index].frame++;


	//一定時間経ったらクールダウンステートへ移行
	if (boss[index].frame > 90) {

		boss[index].frame = 0;

		//ステートを帰還状態へ移行
		boss[index].state = BOSS_STATE_COOLDOWN;
	}
}

void Boss_StateCooldown(int index)
{
	//フレームを進める
	boss[index].frame++;

	//一定時間経ったら帰還ステートへ移行
	if (boss[index].frame > 30) {

		boss[index].frame = 0;

		//ステートを帰還状態へ移行
		boss[index].state = BOSS_STATE_RETURN;
	}
}

void Boss_StateReturn(int index)
{
	//フレームを進める
	boss[index].frame++;

	//プレイヤーの座標を取得する
	D3DXVECTOR2 pl_pos = D3DXVECTOR2(player.collision.s.p.x, player.collision.s.p.y);

	//帰還する座標への向きベクトルを計算する
	D3DXVECTOR2 dir = boss[index].pos_return - boss[index].pos;

	//ベクトルの長さを計算する
	float length = D3DXVec2Length(&dir);

	D3DXVec2Normalize(&dir, &dir);
	dir *= BOSS_CHASE_SPEED;

	boss[index].pos += dir;

	//移動方向から向きを作る
	boss[index].muki = Boss_CalcMuki(dir);

	//帰還する座標に到着したら
	if (length <= BOSS_CHASE_SPEED) {

		boss[index].frame = 0;

		//ステートをサーチ状態へ移行
		boss[index].state = BOSS_STATE_SEARCH;
	}
}
*/

void Boss_StateChase(int index)
{

	//プレイヤーの座標を取得する
	D3DXVECTOR2 pl_pos = D3DXVECTOR2(player.collision.s.p.x, player.collision.s.p.y);

	D3DXVECTOR2 dir = pl_pos - boss[index].pos;
	float muki = pl_pos.x - boss[index].pos.x;
	D3DXVec2Normalize(&dir, &dir);

	if (muki < 0)
	{//プレイヤーがエネミーの左にいるなら左に向く
		boss[index].muki = 1;
	}
	else
	{//プレイヤーがエネミーの右にいるなら右に向く
		boss[index].muki = 2;
	}

	dir *= BOSS_CHASE_SPEED;

	if (boss[index].move == TRUE)//エネミーが吹っ飛ばされていなかったら
	{
		if (boss[index].stay == FALSE)
		{
			boss[index].pos.x += dir.x;
		}
	}
	else
	{//吹っ飛ぶ処理
		boss[index].pos += boss[index].dir_destroy;
	}

	boss[index].color = 0;  //通常状態

	if (pl_pos.x + 200.0f > boss[index].pos.x&&pl_pos.x - 200.0f < boss[index].pos.x)//攻撃準備モーション判定
	{
		boss[index].attack = TRUE;
	}

	if (boss[index].attack == TRUE)//攻撃準備
	{
		//フレームを進める
		boss[index].frame++;

		boss[index].color = 1;
		boss[index].stay = TRUE;

		if (boss[index].frame > 30)
		{
			boss[index].animeAttack = TRUE;
			if (boss[index].ready_attack == TRUE)
			{
				boss[index].ready_attack = FALSE;
				Boss_Attack(index); //腕を振る
			}
		}

		if (boss[index].animeAttack == TRUE)
		{
			//攻撃モーションフレームを進める
			boss[index].frame_attack++;

			if (boss[index].frame_attack < 30)
			{
				boss[index].color = 2;
			}

			if (boss[index].frame_attack >= 30)
			{
				boss[index].frame = 0;
				boss[index].frame_attack = 0;

				boss[index].animeAttack = FALSE;
				boss[index].stay = FALSE;
				boss[index].ready_attack = TRUE;
				boss[index].attack = FALSE;
			}
		}
	}
	////移動方向から向きを作る
	//enemy[index].muki = Enemy_CalcMuki(dir);

	////一定時間経ったら弾射出ステートへ移行
	//if (enemy[index].frame > 120){

	//	enemy[index].frame = 0;

	//	//移動方向を保存
	//	enemy[index].dir_shot = dir;

	//	//ランダムで「ばら撒き弾」もしくは「レーザー」を射出
	//	if (rand() % 2)
	//	{
	//		//ステートをサーチ状態へ移行
	//		enemy[index].state = ENEMY_STATE_SHOT;
	//	}
	//	else{
	//		//ステートをサーチ状態へ移行
	//		enemy[index].state = ENEMY_STATE_LASER;
	//	}
	//}
}

void Boss_Attack(int index)
{
	D3DXVECTOR2 dir;		//腕の向き
	D3DXVECTOR2 hand;

	//弾を発射
	switch (boss[index].muki)
	{
	case 0://下向き
		dir.y = 1.0f;
		break;
	case 1://左向き
		dir.x = -1.0f;
		hand.x = boss[index].pos.x - 100.0f;
		break;
	case 2://右向き
		dir.x = 1.0f;
		hand.x = boss[index].pos.x + 100.0f;
		break;
	case 3://上向き
		dir.y = -1.0f;
		break;
	}

	Bullet_IscoolTrue(index);
	BossBullet_Create(boss[index].muki, hand.x, boss[index].pos.y, dir);
	dir = D3DXVECTOR2(0.0f, 0.0f);
}

int Boss_AddDamage(int damage)
{
	boss[0].bosshitpoint -= damage;
	return boss[0].bosshitpoint;

	//if (boss[0].bosshitpoint < 0)
	//	boss[0].bosshitpoint = 0;
}

int Boss_GetHitPoint()
{
	return boss[0].bosshitpoint;
}

void Boss_NockBack(int index)
{
	boss[index].t = 0;
	destroy_boss_pos_x[index] = boss[index].pos.x;
	destroy_boss_pos_y[index] = boss[index].pos.y;

	D3DXVECTOR2 pl_pos = D3DXVECTOR2(player.collision.s.p.x, player.collision.s.p.y);
	if (destroy_boss_pos_x[index] >= pl_pos.x)
	{
		destroy_boss_dir = 1;
	}
	else
	{
		destroy_boss_dir = -1;
	}
}
