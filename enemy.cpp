
#include "enemy.h"
#include "main.h"
#include "sprite.h"
#include "enemy_table.h"
#include "enemy_table2.h"
#include "player.h"
#include "explosion.h"
#include "explosion2.h"
#include "enemy_hand.h"
#include "game.h"
#include "collisioncheck.h"
#include "sound.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h> // rand関数

#define ENEMY_WIDTH				(64)
#define ENEMY_HEIGHT			(64)
#define ANIME_PATTERN_MAX		(3)
#define ANIME_PATTERN_SKIPFRAME (8)

#define ENEMY_SEARCH_RADIUS		(200.0f)
#define ENEMY_SEARCH_ANGLE		(0.85f)
#define ENEMY_CHASE_SPEED		(1.0f)
#define ENEMY_RETURN_SPEED		(1.0f)

#define ENEMY_DESTROY_SPEED		(20.0f)



ENEMY_DATA enemy[ENEMY_COUNT];

static PLAYER player;//player構造体の情報

D3DXVECTOR2 dir;		//腕の向き
float destroy_pos_x[ENEMY_COUNT];
float destroy_pos_y[ENEMY_COUNT];
float destroy_dir;
int enemycount;

typedef enum
{
	ENEMY_STATE_INIT,		//初期化
	ENEMY_STATE_CHASE,		//追跡
	ENEMY_STATE_DOWN,		//uekougekidedownsitatoki
	ENEMY_STATE_DEAD,		//死亡状態

	ENEMY_STATE_MAX

}ENEMY_STATE;


void Enemy_StateInit(int index, int hp);
void Enemy_StateChase(int index);
void Enemy_StateDown(int index);



void Enemy_Initialize(void)
{
	enemy[0].pos = { 0,450 };
	enemy[1].pos = { 1100,450 };
	enemy[2].pos = { 1000,450 };
	enemy[3].pos = { 1200,470 };
	enemy[4].pos = { 200,-100 };
	enemy[5].pos = { 0,450 };
	enemy[6].pos = { 1000,450 };
	enemy[7].pos = { 1200,470 };
	enemy[8].pos = { 1600,470 };
	enemy[9].pos = { 0,-100 };
	enemy[10].pos = { 800,-100 };
	enemy[11].pos = { 1000,-100 };
	enemy[12].pos = { 400,-100 };
	enemy[13].pos = { 401,-100 };
	enemy[14].pos = { 512,-130 };
	enemy[15].pos = { 823,-160 };
	enemy[16].pos = { 134,-190 };
	enemy[17].pos = { 645,-210 };
	enemy[18].pos = { 956,-190 };
	enemy[19].pos = { 567,-180 };
	enemy[20].pos = { 878,-140 };
	enemy[21].pos = { 489,-170 };
	enemy[22].pos = { 590,-120 };
	enemy[23].pos = { 711,-160 };
	enemy[24].pos = { 122,-120 };
	enemy[25].pos = { 233,-130 };
	enemy[26].pos = { 144,-170 };
	enemy[27].pos = { 455,-190 };
	enemy[28].pos = { 366,-150 };
	enemy[29].pos = { 577,-200 };
	enemy[30].pos = { 688,-230 };
	enemy[31].pos = { 1200,-100 };
	enemy[32].pos = { 1250,-100 };
	enemy[33].pos = { 1300,-100 };
	enemy[34].pos = { 1350,-100 };
	enemy[35].pos = { 1400,-100 };
	enemy[36].pos = { 1450,-100 };
	enemy[37].pos = { 1500,-100 };
	enemy[38].pos = { 1550,-100 };
	enemy[39].pos = { 1600,-100 };
	enemy[40].pos = { 1650,-100 };
	dir.x = 0.0f;
	destroy_dir = 0.0f;
	enemycount = 3;
	//敵の初期化
	for (int i = 0; i < ENEMY_COUNT; i++) {
		enemy[i].enable = FALSE;
	}
	for (int i = 0; i < enemycount; i++) {
		Enemy_StateInit(i, 3);
	}

}

void Enemy_Finalize(void)
{

}

void Enemy_Update(void)
{
	//player構造体の情報を取得
	player = GetPlayer();

	if (enemycount == 3)
	{
		if (enemy[enemycount].enable == FALSE)
		{
			if (player.fase == 1)
			{
				if (player.camerastop == false)
				{
					Enemy_StateInit(enemycount, 1);
					enemy[enemycount].ao = 1;
					enemycount += 1;
				}
			}
		}
	}

	for (int i = 0; i < 3; i++)
	{
		if (enemycount < 7)
		{
			if (enemy[enemycount].enable == FALSE)
			{
				if (player.fase == 1)
				{
					if (player.camerastop == true)
					{
						Enemy_StateInit(enemycount, 3);
						enemycount += 1;
					}
				}
			}
		}
	}

	if (enemycount < 9)
	{
		if (enemy[enemycount].enable == FALSE)
		{
			if (player.fase == 2)
			{
				if (player.camerastop == false)
				{
					Enemy_StateInit(enemycount, 1);
					enemy[enemycount].ao = 1;
					enemycount += 1;
				}
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (enemycount < 13)
		{
			if (enemy[enemycount].enable == FALSE)
			{
				if (player.fase == 2)
				{
					if (player.camerastop == true)
					{
						Enemy_StateInit(enemycount, 3);
						enemycount += 1;
					}
				}
			}
		}
	}

	for (int i = 0; i < 5; i++)
	{
		if (enemycount > 12 && !enemy[12].enable) {
			if (enemycount < 18)
			{
				if (enemy[enemycount].enable == FALSE)
				{
					if (player.fase == 2)
					{
						if (player.camerastop == true)
						{
							Enemy_StateInit(enemycount, 3);
							enemycount += 1;
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < 3; i++)
	{
		if (enemycount > 17 && !enemy[17].enable) {
			if (enemycount < 21)
			{
				if (enemy[enemycount].enable == FALSE)
				{
					if (player.fase == 2)
					{
						if (player.camerastop == true)
						{
							Enemy_StateInit(enemycount, 3);
							enemycount += 1;
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < 5; i++)
	{
		if (enemycount > 20 && !enemy[20].enable) {
			if (enemycount < 26)
			{
				if (enemy[enemycount].enable == FALSE)
				{
					if (player.fase == 2)
					{
						if (player.camerastop == true)
						{
							Enemy_StateInit(enemycount, 3);
							enemycount += 1;
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < 5; i++)
	{
		if (enemycount > 25 && !enemy[25].enable) {
			if (enemycount < 31)
			{
				if (enemy[enemycount].enable == FALSE)
				{
					if (player.fase == 2)
					{
						if (player.camerastop == true)
						{
							Enemy_StateInit(enemycount, 3);
							enemycount += 1;
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < 10; i++)
	{
		if (enemycount > 30 && !enemy[30].enable) {
			if (enemycount < 41)
			{
				if (enemy[enemycount].enable == FALSE)
				{
					if (player.fase == 3)
					{
						if (player.camerastop == false)
						{
							Enemy_StateInit(enemycount, 1);
							enemy[enemycount].ao = 1;
							enemycount += 1;
						}
					}
				}
			}
		}
	}


	for (int i = 0; i < ENEMY_COUNT; i++) {

		if (!Enemy_IsEnable(i)) {
			continue;
		}

		if (enemy[i].hp <= 0)
		{
			Enemy_Destroy(i);
		}

		if (enemy[i].move == FALSE)
		{

			if (enemy[i].pos.x < 0.0f)
			{
				enemy[i].enable = FALSE;
				Explosion_Create3(enemy[i].pos.x, enemy[i].pos.y, enemy[i].dir_destroy.x*-0.5f);
				PlaySound(SOUND_LABEL_SE_DEATH);
				Game_AddKillCount();
				break;
			}
			if (enemy[i].pos.y < 0.0f)
			{
				enemy[i].enable = FALSE;
				Explosion_Create3(enemy[i].pos.x, enemy[i].pos.y, enemy[i].dir_destroy.x);
				PlaySound(SOUND_LABEL_SE_DEATH);
				Game_AddKillCount();
				break;
			}

			if (enemy[i].pos.y > SCREEN_HEIGHT)
			{
				enemy[i].enable = FALSE;
				Explosion_Create3(enemy[i].pos.x, enemy[i].pos.y, enemy[i].dir_destroy.x* -1.0f);
				PlaySound(SOUND_LABEL_SE_DEATH);
				Game_AddKillCount();
				break;
			}
			if (enemy[i].pos.x > SCREEN_WIDTH)
			{
				enemy[i].enable = FALSE;
				Explosion_Create3(enemy[i].pos.x, enemy[i].pos.y, enemy[i].dir_destroy.x*-0.5f);
				PlaySound(SOUND_LABEL_SE_DEATH);
				Game_AddKillCount();
				break;
			}
		}
	}

	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		if (!Enemy_IsEnable(i)) {
			continue;
		}

		if (25.0f <= enemy[i].pos.x && enemy[i].pos.x <= (SCREEN_WIDTH - 25.0f))
		{
			enemy[i].spawn = true;
		}


		//当たり判定用座標の更新
		if (enemy[i].ao == 0)
		{
			enemy[i].colcol.r = ENEMY_WIDTH * 0.8f;
			enemy[i].colcol.s.p.x = enemy[i].pos.x;
			enemy[i].colcol.s.p.y = enemy[i].pos.y - 20.0f;
			enemy[i].colcol.s.v.x = 0.0f;
			enemy[i].colcol.s.v.y = 50.0f;
		}
		else
		{
			enemy[i].colcol.r = ENEMY_WIDTH * 0.5f;
			enemy[i].colcol.s.p.x = enemy[i].pos.x;
			enemy[i].colcol.s.p.y = enemy[i].pos.y - 20.0f;
			enemy[i].colcol.s.v.x = 0.0f;
			enemy[i].colcol.s.v.y = 50.0f;
		}


		if (++enemy[i].animeFrame > ANIME_PATTERN_SKIPFRAME)
		{
			//アニメパターンを進める(最大値を超えたらリセット)
			if (++enemy[i].animePattern >= ANIME_PATTERN_MAX)
				enemy[i].animePattern = 0;
			//フレームは元に戻す
			enemy[i].animeFrame = 0;
		}

		switch (enemy[i].state)
		{
		case ENEMY_STATE_INIT:
			Enemy_StateInit(i, 3);
			break;
		case ENEMY_STATE_CHASE:
			Enemy_StateChase(i);
			break;
		case ENEMY_STATE_DOWN:
			Enemy_StateDown(i);
			break;
		default:
			break;
		}
	}
}

void Enemy_Draw(void)
{
	for (int i = 0; i < ENEMY_COUNT; i++) {

		//生存フラグがTRUEの敵のみ処理を行う
		if (!enemy[i].enable) {
			continue;
		}

		if (enemy[i].ao == 0)
		{
			Sprite_Draw(TEXTURE_INDEX_ZOMBIE,
				enemy[i].pos.x + 10.0f,
				enemy[i].pos.y,
				GetAnimTbl2(enemy[i].color, enemy[i].muki, enemy[i].animePattern).x * 256,
				GetAnimTbl2(enemy[i].color, enemy[i].muki, enemy[i].animePattern).y * 256,
				200,
				300,
				100,
				150,
				0.5f,
				0.5f,
				enemy[i].rot, 0xff000000);

			Sprite_Draw(TEXTURE_INDEX_ZOMBIE,
				enemy[i].pos.x,
				enemy[i].pos.y,
				GetAnimTbl2(enemy[i].color, enemy[i].muki, enemy[i].animePattern).x * 256,
				GetAnimTbl2(enemy[i].color, enemy[i].muki, enemy[i].animePattern).y * 256,
				200,
				300,
				100,
				150,
				0.5f,
				0.5f,
				enemy[i].rot);
		}
		else
		{
			Sprite_Draw(TEXTURE_INDEX_AOZOMBIE,
				enemy[i].pos.x + 10.0f,
				enemy[i].pos.y,
				GetAnimTbl3(enemy[i].color, enemy[i].muki, enemy[i].animePattern).x * 256,
				GetAnimTbl3(enemy[i].color, enemy[i].muki, enemy[i].animePattern).y * 256,
				240,
				300,
				120,
				150,
				0.3f,
				0.3f,
				enemy[i].rot, 0xff000000);

			Sprite_Draw(TEXTURE_INDEX_AOZOMBIE,
				enemy[i].pos.x,
				enemy[i].pos.y,
				GetAnimTbl3(enemy[i].color, enemy[i].muki, enemy[i].animePattern).x * 256,
				GetAnimTbl3(enemy[i].color, enemy[i].muki, enemy[i].animePattern).y * 256,
				240,
				300,
				120,
				150,
				0.3f,
				0.3f,
				enemy[i].rot);

		}
	}
}

void Enemy_Destroy(int index)
{
	Enemy_StateChase(index);
	enemy[index].move = FALSE;

	if (destroy_dir == 1.0f)
	{
		enemy[index].dir_destroy.x = 2.0f;
	}
	else
	{
		enemy[index].dir_destroy.x = -2.0f;
	}
	enemy[index].dir_destroy.y = -1.5f;
	D3DXVec2Normalize(&enemy[index].dir_destroy, &enemy[index].dir_destroy);

	enemy[index].dir_destroy *= ENEMY_DESTROY_SPEED;
}

bool Enemy_IsEnable(int index)
{
	return enemy[index].enable;
}

const Capsule2D* Enemy_GetCollision(int index)
{
	return &enemy[index].colcol;
}

void Enemy_StateInit(int index, int hp)//エネミー出現情報
{
	/*enemy[index].pos.x = frand() * SCREEN_WIDTH;
	enemy[index].pos.y = frand() * SCREEN_HEIGHT;*/
	enemy[index].rot = 0;
	enemy[index].color = 0;
	enemy[index].hp = hp;
	enemy[index].t = 0;

	enemy[index].muki = 1;
	enemy[index].enable = TRUE;

	enemy[index].colcol.s.p.x = enemy[index].pos.x;
	enemy[index].colcol.s.p.y = enemy[index].pos.y;
	enemy[index].colcol.r = ENEMY_WIDTH * 0.8f;

	enemy[index].dir_destroy = D3DXVECTOR2(0.0f, 0.0f);
	enemy[index].frame = 0;
	enemy[index].korobuflame = 0;
	enemy[index].hitstop_frame = 0;
	enemy[index].frame_attack = 0;
	enemy[index].animeAttack = FALSE;
	enemy[index].stay = FALSE;
	enemy[index].nock = FALSE;
	enemy[index].ready_attack = TRUE;
	enemy[index].attack = FALSE;
	enemy[index].move = TRUE;
	enemy[index].korobu = false;
	enemy[index].spawn = false;
	enemy[index].damagecol = true;
	enemy[index].ao = 0;	//0...普通,1...青

	destroy_pos_y[index] = /*enemy[index].pos.y*/450.0f;
	enemy[index].enemystop = false;
	enemy[index].enemyleftstop = false;
	enemy[index].state = ENEMY_STATE_CHASE;
	//ステートをサーチ状態へ移行
	//enemy[index].state = ENEMY_STATE_SEARCH;


}

void Enemy_StateChase(int index)
{

	//プレイヤーの座標を取得する
	D3DXVECTOR2 pl_pos = D3DXVECTOR2(player.collision.s.p.x, player.collision.s.p.y);

	D3DXVECTOR2 dir = pl_pos - enemy[index].pos;
	float muki = pl_pos.x - enemy[index].pos.x;
	D3DXVec2Normalize(&dir, &dir);

	if (enemy[index].hp >= 1)
	{
		if (enemy[index].nock == TRUE)
		{
			if (enemy[index].t <= 1)
			{//ノックバック
				enemy[index].t = enemy[index].t + 0.05;
			}

			if (enemy[index].t > 1)
			{
				enemy[index].t = 0;
				enemy[index].nock = FALSE;
				if (enemy[index].korobu == true) {
					enemy[index].state = ENEMY_STATE_DOWN;
				}
			}
		}
	}

	if (muki < 0)
	{//プレイヤーがエネミーの左にいるなら左に向く
		enemy[index].muki = 0;
	}
	else
	{//プレイヤーがエネミーの右にいるなら右に向く
		enemy[index].muki = 1;
	}

	if (enemy[index].move == TRUE)//エネミーが吹っ飛ばされていなかったら
	{
		if (enemy[index].stay == FALSE)
		{
			dir *= ENEMY_CHASE_SPEED;
		}
		else
		{
			dir = dir * 0.0f;
		}
	}

	enemy[index].color = 0;  //通常状態

	if (pl_pos.x + 120.0f > enemy[index].pos.x&&pl_pos.x - 120.0f < enemy[index].pos.x)//攻撃準備モーション判定
	{
		if (enemy[index].spawn == true) {
			if (enemy[index].pos.y >= 430.0f) {
				enemy[index].attack = TRUE;
			}
		}
	}

	if (enemy[index].attack == TRUE)//攻撃準備
	{
		//フレームを進める
		enemy[index].frame++;

		enemy[index].color = 0;
		enemy[index].stay = TRUE;

		if (enemy[index].frame > 60)
		{
			enemy[index].animeAttack = TRUE;
			if (enemy[index].ready_attack == TRUE)
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
				enemy[index].color = 1;
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

	if (enemy[index].enemystop || enemy[index].enemyleftstop) {
		dir.x = 0.0f;
		enemy[index].enemystop = false;
		enemy[index].enemyleftstop = false;
	}

	enemy[index].pos.x += enemy[index].dir_destroy.x + enemy[index].t*destroy_dir*10.0f;

	if (enemy[index].move == TRUE)
	{
		if (player.camerastop) {
			enemy[index].pos.x += dir.x;
			if (enemy[index].spawn == true)
			{
				if (enemy[index].pos.x < 25.0f) {
					enemy[index].pos.x = 25.0f;
				}
				if (enemy[index].pos.x > SCREEN_WIDTH - 25.0f) {
					enemy[index].pos.x = SCREEN_WIDTH - 25.0f;
				}
			}
		}
		else {
			enemy[index].pos.x += dir.x - player.speed.x;
		}
		if (enemy[index].nock == TRUE)
		{
			enemy[index].pos.y = destroy_pos_y[index] - (3 * enemy[index].t*((1 - enemy[index].t)*(1 - enemy[index].t)))*200.0f;
		}
	}
	else
	{
		enemy[index].pos.y += enemy[index].dir_destroy.y;
		enemy[index].rot += 2.0f;
	}
	if (enemy[index].t == 0)
	{
		if (enemy[index].nock == FALSE)
		{
			if (enemy[index].move == TRUE)
			{
				if (enemy[index].ao == 0)
				{
					if (enemy[index].pos.y < 450.0f)
					{
						enemy[index].pos.y += 10.0f;
					}
				}
				else
				{
					if (enemy[index].pos.y < 470.0f)
					{
						enemy[index].pos.y += 10.0f;
					}
				}
			}
		}
	}
}

void Enemy_StateDown(int index)
{
	enemy[index].korobuflame++;

	if (!player.camerastop) {
		enemy[index].pos.x -= player.speed.x;
	}

	if (enemy[index].korobuflame <= 10)
	{
		if (enemy[index].muki == 0)
		{
			enemy[index].rot += 0.15f;
		}
		if (enemy[index].muki == 1)
		{
			enemy[index].rot -= 0.15f;
		}
	}

	if (enemy[index].korobuflame >= 200)
	{
		enemy[index].korobu = false;
		enemy[index].korobuflame = 0;
		enemy[index].state = ENEMY_STATE_CHASE;
		enemy[index].rot = 0.0f;
	}
}


void Enemy_Attack(int index)
{
	//弾を発射
	switch (enemy[index].muki)
	{
	case 0://左向き
		dir.x = -1.0f;
		break;
	case 1://右向き
		dir.x = 1.0f;
		break;
	}

	Bullet_IscoolTrue(index);
	Bullet_Create(enemy[index].muki, enemy[index].pos.x, enemy[index].pos.y, dir);
	dir = D3DXVECTOR2(0.0f, 0.0f);
}

void Enemy_AddDamage(/*int damage,*/int index)
{
	if (enemy[index].pos.x >= player.pos.x)
	{
		destroy_dir = 1.0f;
	}
	else
	{
		destroy_dir = -1.0f;
	}
	/*enemy[index].hp -= damage;
	return enemy[index].hp;*/

	//if (boss[0].bosshitpoint < 0)
	//	boss[0].bosshitpoint = 0;
}



int Enemy_GetHitPoint(int index)
{
	return enemy[index].hp;
}

void Enemy_NockBack(int index)
{
	enemy[index].nock = TRUE;
	if (enemy[index].hp >= 1) {
		enemy[index].t = 0;
	}
}

ENEMY_DATA GetEnemy(int i) {
	return enemy[i];
}

void EnemyInfoMatch(ENEMY_DATA info, int i) {
	enemy[i] = info;
}