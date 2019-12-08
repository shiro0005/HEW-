
#include "enemy.h"
#include "main.h"
#include "sprite.h"
#include "enemy_table.h"
#include "player.h"
#include "explosion.h"
#include "enemy_hand.h"
#include "game.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define ENEMY_WIDTH				(64)
#define ENEMY_HEIGHT			(64)
#define ANIME_PATTERN_MAX		(3)
#define ANIME_PATTERN_SKIPFRAME (8)

#define ENEMY_SEARCH_RADIUS		(200.0f)
#define ENEMY_SEARCH_ANGLE		(0.85f)
#define ENEMY_CHASE_SPEED		(1.0f)
#define ENEMY_RETURN_SPEED		(1.0f)

#define ENEMY_DESTROY_SPEED		(10.0f)



ENEMY_DATA enemy[ENEMY_COUNT];

D3DXVECTOR2 dir;		//腕の向き

static PLAYER player;//player構造体の情報

typedef enum
{
	//ENEMY_STATE_INIT,		//初期化
	//ENEMY_STATE_SEARCH,		//周囲をサーチ
	//ENEMY_STATE_FIND,		//敵を発見
	//ENEMY_STATE_CHASE,		//追跡
	//ENEMY_STATE_SHOT,		//ショット発射
	//ENEMY_STATE_LASER,		//レーザー発射
	//ENEMY_STATE_COOLDOWN,	//ショット終了
	//ENEMY_STATE_RETURN,		//定位置に戻る
	//ENEMY_STATE_DEAD,		//死亡状態
	ENEMY_STATE_INIT,		//初期化
	ENEMY_STATE_CHASE,		//追跡
	ENEMY_STATE_DEAD,		//死亡状態

	ENEMY_STATE_MAX

}ENEMY_STATE;


//int Enemy_CalcMuki(D3DXVECTOR2 dir);
//void Enemy_StateSearch(int index);
//void Enemy_StateFind(int index);
//void Enemy_StateShot(int index);
//void Enemy_StateLaser(int index);
//void Enemy_StateCooldown(int index);
//void Enemy_StateReturn(int index);
void Enemy_StateInit(int index);
void Enemy_StateChase(int index);



void Enemy_Initialize(void)
{
	enemy[0].pos = { 0,450 };
	enemy[1].pos = { 1000,450 };
	enemy[2].pos = { 1500,450 };
	//敵の初期化
	for (int i = 0; i < ENEMY_COUNT; i++) {
		Enemy_StateInit(i);
		enemy[i].state = ENEMY_STATE_CHASE;
	}
	
}

void Enemy_Finalize(void)
{

}

void Enemy_Update(void)
{
	//player構造体の情報を取得
	player = GetPlayer();

	for (int i = 0; i<ENEMY_COUNT; i++){
		if (enemy[i].move == FALSE)
		{
			if (!Enemy_IsEnable(i)) {
				continue;
			}

			if (enemy[i].pos.y < 0.0f)
			{
				enemy[i].enable = FALSE;
				Game_AddKillCount();
				break;
			}
			if (enemy[i].pos.y < 0.0f)
			{
				enemy[i].enable = FALSE;
				Game_AddKillCount();
				break;
			}

			if (enemy[i].pos.y > SCREEN_HEIGHT + 50.0f)
			{
				enemy[i].enable = FALSE;
				Game_AddKillCount();
				break;
			}
			if (enemy[i].pos.y < -50.0f)
			{
				enemy[i].enable = FALSE;
				Game_AddKillCount();
				break;
			}
		}

		//当たり判定用座標の更新
		enemy[i].colcol.r = ENEMY_WIDTH * 0.8f;
		enemy[i].colcol.s.p.x = enemy[i].pos.x;
		enemy[i].colcol.s.p.y = enemy[i].pos.y - 25.0f;
		enemy[i].colcol.s.v.x = 0.0f;
		enemy[i].colcol.s.v.y = 50.0f;

		//吹っ飛んだ時の処理
		

		//スキップするフレーム値を超えたら
		if (++enemy[i].animeFrame > ANIME_PATTERN_SKIPFRAME)
		{
			//アニメパターンを進める(最大値を超えたらリセット)
			if (++enemy[i].animePattern >= ANIME_PATTERN_MAX)
				enemy[i].animePattern = 0;
			//フレームは元に戻す
			enemy[i].animeFrame = 0;
			
			enemy[i].pos.x -= player.speed.x * 10;//エネミーをプレイヤーの移動と合わせる
		}

		switch (enemy[i].state)
		{
		case ENEMY_STATE_INIT:
			Enemy_StateInit(i);
			break;
		/*case ENEMY_STATE_SEARCH:
			Enemy_StateSearch(i);
			break;
		case ENEMY_STATE_FIND:
			Enemy_StateFind(i);
			break;*/
		case ENEMY_STATE_CHASE:
			Enemy_StateChase(i);
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
}

void Enemy_Draw(void)
{
	for (int i = 0; i < ENEMY_COUNT; i++){

		//生存フラグがTRUEの敵のみ処理を行う
		if (!enemy[i].enable) {
			continue;
		}

		Sprite_Draw(TEXTURE_INDEX_ENEMY,
			enemy[i].pos.x,
			enemy[i].pos.y,
			GetAnimTbl2(enemy[i].color, enemy[i].muki, enemy[i].animePattern).x * 256,
			GetAnimTbl2(enemy[i].color, enemy[i].muki, enemy[i].animePattern).y * 256,
			32,
			32,
			16,
			16,
			4.0f,
			4.0f,
			enemy[i].rot);
	}
}

void Enemy_Destroy(int index)
{
	enemy[index].move = FALSE;
	D3DXVECTOR2 pl_pos = D3DXVECTOR2(player.collision.s.p.x, player.collision.s.p.y);

	if (pl_pos.x - enemy[index].pos.x < 0)
	{
		enemy[index].dir_destroy.x = 2.0f;
	}
	else
	{
		enemy[index].dir_destroy.x = -2.0f;
	}
	enemy[index].dir_destroy.y = -1.5f;
	D3DXVec2Normalize(&enemy[index].dir_destroy, &enemy[index].dir_destroy);

	enemy[index].dir_destroy*=ENEMY_DESTROY_SPEED;
}

bool Enemy_IsEnable(int index)
{
	return enemy[index].enable;
}

const Capsule2D* Enemy_GetCollision(int index)
{
	return &enemy[index].colcol;
}

////向きを作る
//int Enemy_CalcMuki(D3DXVECTOR2 dir)
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

void Enemy_StateInit(int index)//エネミー出現情報
{
	/*enemy[index].pos.x = frand() * SCREEN_WIDTH;
	enemy[index].pos.y = frand() * SCREEN_HEIGHT;*/
	enemy[index].rot = 0;
	enemy[index].color = 0;

	enemy[index].muki = 1;
	enemy[index].enable = TRUE;

	enemy[index].colcol.s.p.x = enemy[index].pos.x;
	enemy[index].colcol.s.p.y = enemy[index].pos.y;
	enemy[index].colcol.r = ENEMY_WIDTH * 0.8f;

	enemy[index].frame = 0;
	enemy[index].frame_attack = 0;
	enemy[index].animeAttack = FALSE;
	enemy[index].stay = FALSE;
	enemy[index].ready_attack =TRUE;
	enemy[index].attack = FALSE;
	enemy[index].move = TRUE;


	//ステートをサーチ状態へ移行
	//enemy[index].state = ENEMY_STATE_SEARCH;


}

//void Enemy_StateSearch(int index)
//{
//	//フレームを進める
//	enemy[index].frame++;
//
//	D3DXVECTOR2 dir;
//
//	//向いている方向のベクトルを作る
//	switch (enemy[index].muki)
//	{
//	case 0://下向き
//		dir = D3DXVECTOR2( 0.0f,  1.0f);
//		break;
//	case 1://左向き
//		dir = D3DXVECTOR2(-1.0f,  0.0f);
//		break;
//	case 2://右向き
//		dir = D3DXVECTOR2( 1.0f,  0.0f);
//		break;
//	case 3://上向き
//		dir = D3DXVECTOR2( 0.0f, -1.0f);
//		break;
//	}
//
//	//プレイヤーの座標を取得する
//	D3DXVECTOR2 pl_pos = D3DXVECTOR2(Player_GetCollision()->x, Player_GetCollision()->y);
//
//	//敵とプレイヤーの距離を計算する
//	D3DXVECTOR2 vLen = enemy[index].pos - pl_pos;
//	float length = D3DXVec2Length(&vLen);
//
//	//判定する距離の中に入っているかをチェックする
//	if (length < ENEMY_SEARCH_RADIUS)
//	{
//		//敵から見てプレイヤーの座標への方向ベクトルを作る
//		D3DXVECTOR2 pl_dir = pl_pos - enemy[index].pos;
//		D3DXVec2Normalize(&pl_dir, &pl_dir);
//
//		//敵の向きとプレイヤ座標への方向ベクトルで内積を取る
//		float dot = D3DXVec2Dot(&dir, &pl_dir);
//
//		//判定する角度内に入っているかをチェックする
//		if (dot > ENEMY_SEARCH_ANGLE)
//		{
//			Explosion_Create(enemy[index].pos.x, enemy[index].pos.y);
//
//			//判定に通ればプレイヤー発見ステートに移行する
//			enemy[index].state = ENEMY_STATE_FIND;
//			enemy[index].frame = 0;
//
//			//戻って来る座標を保存する
//			enemy[index].pos_return = enemy[index].pos;
//		}
//	}
//
//	//向きを変化させる
//	if (enemy[index].frame > 60)
//	{
//		enemy[index].muki = (enemy[index].muki + 1) % 4;
//		enemy[index].frame = 0;
//	}
//
//}
//
//void Enemy_StateFind(int index)
//{
//	//フレームを進める
//	enemy[index].frame++;
//
//	//一定時間経ったら追跡ステートへ移行
//	if (enemy[index].frame > 20){
//
//		enemy[index].frame = 0;
//
//		//ステートをサーチ状態へ移行
//		enemy[index].state = ENEMY_STATE_CHASE;
//	}
//}

void Enemy_StateChase(int index)
{

	//プレイヤーの座標を取得する
	D3DXVECTOR2 pl_pos = D3DXVECTOR2(player.collision.s.p.x, player.collision.s.p.y);
	
	D3DXVECTOR2 dir = pl_pos - enemy[index].pos;
	float muki = pl_pos.x - enemy[index].pos.x;
	D3DXVec2Normalize(&dir, &dir);

	if (muki < 0)
	{//プレイヤーがエネミーの左にいるなら左に向く
		enemy[index].muki = 1;
	}
	else
	{//プレイヤーがエネミーの右にいるなら右に向く
		enemy[index].muki = 2;
	}

	dir *= ENEMY_CHASE_SPEED;

	if (enemy[index].move == TRUE)//エネミーが吹っ飛ばされていなかったら
	{
		if (enemy[index].stay == FALSE)
		{
			enemy[index].pos.x += dir.x;
		}
	}
	else
	{//吹っ飛ぶ処理
		enemy[index].pos +=	enemy[index].dir_destroy;
	}

	enemy[index].color = 0;  //通常状態

	if (pl_pos.x + 100.0f > enemy[index].pos.x&&pl_pos.x - 100.0f < enemy[index].pos.x)//攻撃準備モーション判定
	{
		enemy[index].attack = TRUE;
	}

	if (enemy[index].attack == TRUE)//攻撃準備
	{
		//フレームを進める
		enemy[index].frame++;

		enemy[index].color = 1;
		enemy[index].stay = TRUE;

		if (enemy[index].frame > 100)
		{
			enemy[index].animeAttack = TRUE;
			if (enemy[index].ready_attack==TRUE)
			{
				enemy[index].ready_attack = FALSE;
				Enemy_Attack(index); //腕を振る
			}
		}

		if (enemy[index].animeAttack == TRUE)
		{
			//攻撃モーションフレームを進める
			enemy[index].frame_attack++;

			if (enemy[index].frame_attack < 50)
			{
				enemy[index].color = 2;
			}

			if (enemy[index].frame_attack >= 50)
			{
				enemy[index].frame = 0;
				enemy[index].frame_attack = 0;

				enemy[index].animeAttack = FALSE;
				enemy[index].stay = FALSE;
				enemy[index].ready_attack = TRUE;
				enemy[index].attack = FALSE;
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

//void Enemy_StateShot(int index)
//{
//	//フレームを進める
//	enemy[index].frame++;
//
//	float shot_rad = atan2(enemy[index].dir_shot.y, enemy[index].dir_shot.x);
//	shot_rad = (float)(M_PI * 2 / 20)*enemy[index].frame;
//
//	//射出角ベクトルをフレーム値で回転させる
//	D3DXVECTOR2 shot_dir;
//	shot_dir.x = enemy[index].dir_shot.x*cosf(shot_rad) - enemy[index].dir_shot.y*sinf(shot_rad);
//	shot_dir.y = enemy[index].dir_shot.x*sinf(shot_rad) + enemy[index].dir_shot.y*cosf(shot_rad);
//
//	EnemyBullet_Create(enemy[index].pos.x, enemy[index].pos.y, shot_dir);
//
//	//一定時間経ったらクールダウンステートへ移行
//	if (enemy[index].frame > 90){
//
//		enemy[index].frame = 0;
//
//		//ステートを帰還状態へ移行
//		enemy[index].state = ENEMY_STATE_COOLDOWN;
//	}
//}
//
//void Enemy_StateLaser(int index)
//{
//	if (enemy[index].frame == 0){
//		EnemyLaser_Create(enemy[index].pos.x, enemy[index].pos.y, enemy[index].dir_shot);
//	}
//
//	//フレームを進める
//	enemy[index].frame++;
//
//
//	//一定時間経ったらクールダウンステートへ移行
//	if (enemy[index].frame > 90){
//
//		enemy[index].frame = 0;
//
//		//ステートを帰還状態へ移行
//		enemy[index].state = ENEMY_STATE_COOLDOWN;
//	}
//}
//
//void Enemy_StateCooldown(int index)
//{
//	//フレームを進める
//	enemy[index].frame++;
//
//	//一定時間経ったら帰還ステートへ移行
//	if (enemy[index].frame > 30){
//
//		enemy[index].frame = 0;
//
//		//ステートを帰還状態へ移行
//		enemy[index].state = ENEMY_STATE_RETURN;
//	}
//}
//
//void Enemy_StateReturn(int index)
//{
//	//フレームを進める
//	enemy[index].frame++;
//
//	//プレイヤーの座標を取得する
//	D3DXVECTOR2 pl_pos = D3DXVECTOR2(Player_GetCollision()->x, Player_GetCollision()->y);
//
//	//帰還する座標への向きベクトルを計算する
//	D3DXVECTOR2 dir = enemy[index].pos_return - enemy[index].pos;
//
//	//ベクトルの長さを計算する
//	float length = D3DXVec2Length(&dir);
//
//	D3DXVec2Normalize(&dir, &dir);
//	dir *= ENEMY_CHASE_SPEED;
//
//	enemy[index].pos += dir;
//
//	//移動方向から向きを作る
//	enemy[index].muki = Enemy_CalcMuki(dir);
//
//	//帰還する座標に到着したら
//	if (length <= ENEMY_CHASE_SPEED){
//
//		enemy[index].frame = 0;
//
//		//ステートをサーチ状態へ移行
//		enemy[index].state = ENEMY_STATE_SEARCH;
//	}
//}


void Enemy_Attack(int index)
{
	//弾を発射
	switch (enemy[index].muki)
	{
	case 0://下向き
		dir.y = 1.0f;
		break;
	case 1://左向き
		dir.x = -1.0f;
		break;
	case 2://右向き
		dir.x = 1.0f;
		break;
	case 3://上向き
		dir.y = -1.0f;
		break;
	}

	Bullet_IscoolTrue(index);
	Bullet_Create(enemy[index].muki,enemy[index].pos.x, enemy[index].pos.y, dir);
	dir = D3DXVECTOR2(0.0f, 0.0f);
}