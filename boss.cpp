#include "boss.h"
#include "main.h"
#include "sprite.h"
#include "enemy_table.h"
#include "player.h"
#include "bossbullet.h"
#include "bosslaser.h"
#include "bossflame.h"
#include "bossthunder.h"
#include "explosion.h"
#include "enemy_hand.h"
#include "game.h"
#include "collisioncheck.h"


#define _USE_MATH_DEFINES
#include <math.h>

#define BOSS_WIDTH				(128)
#define BOSS_HEIGHT				(128)
#define ANIME_PATTERN_MAX		(3)
#define ANIME_PATTERN_SKIPFRAME (8)

#define BOSS_SEARCH_RADIUS		(200.0f)
#define BOSS_SEARCH_ANGLE		(0.85f)
#define BOSS_CHASE_SPEED		(1.0f)
#define BOSS_RETURN_SPEED		(1.0f)

#define BOSS_DESTROY_SPEED		(20.0f)



BOSS_DATA boss[BOSS_COUNT];

static PLAYER player;//player�\���̂̏��

D3DXVECTOR2 boss_dir;
float destroy_boss_pos_x[BOSS_COUNT];
float destroy_boss_pos_y[BOSS_COUNT];
int destroy_boss_dir;

typedef enum
{
	//ENEMY_STATE_INIT,		//������
	//ENEMY_STATE_SEARCH,		//���͂��T�[�`
	//ENEMY_STATE_FIND,		//�G�𔭌�
	//ENEMY_STATE_CHASE,		//�ǐ�
	//BOSS_STATE_SHOT,		//�V���b�g����
	BOSS_STATE_LASER,		//���[�U�[����
	BOSS_STATE_COOLDOWN,	//�V���b�g�I��
	BOSS_STATE_RETURN,		//��ʒu�ɖ߂�
	//ENEMY_STATE_DEAD,		//���S���
	BOSS_STATE_INIT,		//������
	BOSS_STATE_CHASE,		//�ǐ�
	BOSS_STATE_DEAD,		//���S���

	BOSS_STATE_MAX

}BOSS_STATE;

int bossatk_countdown;

//int Enemy_CalcMuki(D3DXVECTOR2 dir);
//void Enemy_StateSearch(int index);
//void Enemy_StateFind(int index);
//void Enemy_StateShot(int index);
//void Boss_StateLaser(int index);
//void Enemy_StateCooldown(int index);
//void Boss_StateReturn(int index);
void Boss_StateInit(int index);
void Boss_StateChase(int index);



void Boss_Initialize(void)
{
	destroy_boss_dir = 0.0f;
	boss_dir.x = 1.0f;
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
	player = GetPlayer();

	for (int i = 0; i < BOSS_COUNT; i++) {

		if (boss[i].hp <= 0)
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

	//�����蔻��p���W�̍X�V
	boss[0].colcol.r = BOSS_WIDTH * 0.6f;
	boss[0].colcol.s.p.x = boss[0].pos.x;
	boss[0].colcol.s.p.y = boss[0].pos.y - 50.0f;
	boss[0].colcol.s.v.x = 0.0f;
	boss[0].colcol.s.v.y = 100.0f;

	//�X�L�b�v����t���[���l�𒴂�����
	if (++boss[0].animeFrame > ANIME_PATTERN_SKIPFRAME)
	{
		//�A�j���p�^�[����i�߂�(�ő�l�𒴂����烊�Z�b�g)
		if (++boss[0].animePattern >= ANIME_PATTERN_MAX)
			boss[0].animePattern = 0;
		//�t���[���͌��ɖ߂�
		boss[0].animeFrame = 0;
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
	/*case BOSS_STATE_LASER:
		Boss_StateLaser(0);
		break;
		case ENEMY_STATE_SHOT:
			Enemy_StateShot(i);
			break;
		 case BOSS_STATE_RETURN:
			Boss_StateReturn(0);
			break;
		case ENEMY_STATE_COOLDOWN:
			Enemy_StateCooldown(i);
			break;*/
	default:
		break;
	}
}

void Boss_Draw(void)
{
	for (int i = 0; i < BOSS_COUNT; i++) {

		//�����t���O��TRUE�̓G�̂ݏ������s��
		if (!boss[i].enable) {
			continue;
		}

		Sprite_Draw(TEXTURE_INDEX_ZOMBIE,
			boss[i].pos.x,
			boss[i].pos.y,
			GetAnimTbl2(boss[i].color, boss[i].muki, boss[i].animePattern).x * 256,
			GetAnimTbl2(boss[i].color, boss[i].muki, boss[i].animePattern).y * 256,
			200,
			300,
			100,
			150,
			1.2f,
			1.2f,
			boss[i].rot);
		/*colcheck(boss[i].colcol);*/


	}
}

void Boss_Destroy(int index)
{
	boss[index].move = FALSE;

	if (destroy_boss_dir == 1)
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

const Capsule2D* Boss_GetCollision(int index)
{
	return &boss[index].colcol;
}

////���������
//int Enemy_CalcMuki(D3DXVECTOR2 dir)
//{
//	int muki = 0;
//
//	//�ړ��������L�����`�F�b�N
//	if (D3DXVec2Length(&dir) > 0.01f)
//	{
//		//X������Y�����̑傫�����ׂāA�ǂ���̕����傫���̂��𒲂ׂ�
//		//X�����̕����傫���ꍇ
//		if (fabs(dir.x) > fabs(dir.y))
//		{
//			//X�������v���X�̒l�Ȃ̂��}�C�i�X�̒l�Ȃ̂��𒲂ׂ�
//			//�}�C�i�X�����̏ꍇ�͍�����
//			if (dir.x < 0.0f)
//				muki = 1;
//			//��������Ȃ��i�v���X�����j�ꍇ�͉E����
//			else
//				muki = 2;
//		}
//		//Y�����̕����傫���ꍇ
//		else
//		{
//			//Y�������v���X�̒l�Ȃ̂��}�C�i�X�̒l�Ȃ̂��𒲂ׂ�
//			//�}�C�i�X�����̏ꍇ�͏����
//			if (dir.y < 0.0f)
//				muki = 3;
//			//��������Ȃ��i�v���X�����j�ꍇ�͏����
//			else
//				muki = 0;
//		}
//	}
//
//	return muki;
//}

void Boss_StateInit(int index)//�G�l�~�[�o�����
{
	/*enemy[index].pos.x = frand() * SCREEN_WIDTH;
	enemy[index].pos.y = frand() * SCREEN_HEIGHT;*/
	boss[index].rot = 0;
	boss[index].color = 0;
	boss[index].pos = { 1000,450 };
	boss[index].hp = 10;
	boss[index].t = 0;

	boss[index].muki = 1;
	boss[index].enable = TRUE;

	boss[index].colcol.s.p.x = boss[index].pos.x;
	boss[index].colcol.s.p.y = boss[index].pos.y;
	boss[index].colcol.r = BOSS_WIDTH * 0.8f;

	boss[index].dir_destroy = D3DXVECTOR2(0.0f, 0.0f);
	boss[index].frame = 0;
	boss[index].frame_attack = 0;
	boss[index].animeAttack = FALSE;
	boss[index].stay = FALSE;
	boss[index].bosslasermodestop = FALSE;
	boss[index].ready_attack = TRUE;
	boss[index].attack = FALSE;
	boss[index].laserattack = FALSE;
	boss[index].move = TRUE;
	//boss_dir.x = -1.0f;

	//�X�e�[�g���T�[�`��Ԃֈڍs
	//enemy[index].state = ENEMY_STATE_SEARCH;

	destroy_boss_pos_y[index] = boss[index].pos.y;

	boss[index].state = BOSS_STATE_CHASE;
}

//void Enemy_StateSearch(int index)
//{
//	//�t���[����i�߂�
//	enemy[index].frame++;
//
//	D3DXVECTOR2 dir;
//
//	//�����Ă�������̃x�N�g�������
//	switch (enemy[index].muki)
//	{
//	case 0://������
//		dir = D3DXVECTOR2( 0.0f,  1.0f);
//		break;
//	case 1://������
//		dir = D3DXVECTOR2(-1.0f,  0.0f);
//		break;
//	case 2://�E����
//		dir = D3DXVECTOR2( 1.0f,  0.0f);
//		break;
//	case 3://�����
//		dir = D3DXVECTOR2( 0.0f, -1.0f);
//		break;
//	}
//
//	//�v���C���[�̍��W���擾����
//	D3DXVECTOR2 pl_pos = D3DXVECTOR2(Player_GetCollision()->x, Player_GetCollision()->y);
//
//	//�G�ƃv���C���[�̋������v�Z����
//	D3DXVECTOR2 vLen = enemy[index].pos - pl_pos;
//	float length = D3DXVec2Length(&vLen);
//
//	//���肷�鋗���̒��ɓ����Ă��邩���`�F�b�N����
//	if (length < ENEMY_SEARCH_RADIUS)
//	{
//		//�G���猩�ăv���C���[�̍��W�ւ̕����x�N�g�������
//		D3DXVECTOR2 pl_dir = pl_pos - enemy[index].pos;
//		D3DXVec2Normalize(&pl_dir, &pl_dir);
//
//		//�G�̌����ƃv���C�����W�ւ̕����x�N�g���œ��ς����
//		float dot = D3DXVec2Dot(&dir, &pl_dir);
//
//		//���肷��p�x���ɓ����Ă��邩���`�F�b�N����
//		if (dot > ENEMY_SEARCH_ANGLE)
//		{
//			Explosion_Create(enemy[index].pos.x, enemy[index].pos.y);
//
//			//����ɒʂ�΃v���C���[�����X�e�[�g�Ɉڍs����
//			enemy[index].state = ENEMY_STATE_FIND;
//			enemy[index].frame = 0;
//
//			//�߂��ė�����W��ۑ�����
//			enemy[index].pos_return = enemy[index].pos;
//		}
//	}
//
//	//������ω�������
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
//	//�t���[����i�߂�
//	enemy[index].frame++;
//
//	//��莞�Ԍo������ǐՃX�e�[�g�ֈڍs
//	if (enemy[index].frame > 20){
//
//		enemy[index].frame = 0;
//
//		//�X�e�[�g���T�[�`��Ԃֈڍs
//		enemy[index].state = ENEMY_STATE_CHASE;
//	}
//}

void Boss_StateChase(int index)
{
	//�v���C���[�̍��W���擾����(takuadd)
	D3DXVECTOR2 pl_pos = D3DXVECTOR2(player.collision.s.p.x, player.collision.s.p.y);
	boss_dir = pl_pos - boss[index].pos;

	float muki = player.pos.x - boss[index].pos.x;
	D3DXVec2Normalize(&boss_dir, &boss_dir);


	if (boss[index].hp >= 1)
	{
		if (boss[index].nock == TRUE)
		{
			if (boss[index].t <= 1)
			{//�m�b�N�o�b�N
				boss[index].t = boss[index].t + 0.05;
			}

			if (boss[index].t > 1)
			{
				boss[index].t = 0;
				boss[index].nock = FALSE;
			}
		}
	}

	if (muki < 0)
	{//�v���C���[���G�l�~�[�̍��ɂ���Ȃ獶�Ɍ���
		boss[index].muki = 0;
	}
	else
	{//�v���C���[���G�l�~�[�̉E�ɂ���Ȃ�E�Ɍ���
		boss[index].muki = 1;
	}


	if (boss[index].bossstop || boss[index].bossleftstop) {
		if (boss[index].bossstop) {
			if (boss_dir.x > 0) {
				boss_dir.x = -0.5f;
				player.speed.x = 0.0f;
			}
		}
		else if (boss[index].bossleftstop) {
			if (boss_dir.x < 0) {
				boss_dir.x = 0.5f;
				player.speed.x = 0.0f;
			}
		}
	}
	boss[index].color = 0;  //�ʏ���

	if (player.pos.x + 150.0f > boss[index].pos.x&&player.pos.x - 150.0f < boss[index].pos.x)//�U���������[�V��������
	{
		boss[index].attack = TRUE;
	
	}

	
	if (boss[index].attack == TRUE)//�U������
	{
		//�t���[����i�߂�
		boss[index].frame++;

		boss[index].color = 0;
		boss[index].stay = TRUE;

		if (boss[index].frame > 100)
		{
			boss[index].animeAttack = TRUE;
			if (boss[index].ready_attack == TRUE)
			{
				boss[index].ready_attack = FALSE;
				Boss_Attack(index); //�r��U��
			}
		}

		if (boss[index].animeAttack == TRUE)
		{
			bossatk_countdown++;
			//�U�����[�V�����t���[����i�߂�
			boss[index].frame_attack++;

			if (boss[index].frame_attack < 50)
			{
				boss[index].color = 1;
			}

			if (boss[index].frame_attack >= 50)
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




	if (boss[index].move == TRUE)//�G�l�~�[��������΂���Ă��Ȃ�������
	{
		if (boss[index].stay == TRUE)
		{
			boss_dir.x = 0.0f;
		}
		else
		{
			if (boss[index].pos.x > 1000.0f)
			{
				boss_dir.x = -1.0f;
			}
			if (boss[index].pos.x < 150.0f)
			{
				boss_dir.x = 1.0f;
			}
		}

	
	}

	//�{�X�ړ��ł���
	if (boss[index].bosslasermodestop && !BossLaser_IsEnable(0)) {
		boss[index].bosslasermodestop = FALSE;
	}


	if (boss[index].bosslasermodestop == TRUE)
	{
		boss_dir.x = 0.0f;

	}

	if (boss[index].bossstop || boss[index].bossleftstop) {
		boss_dir.x = 0.0f;
		boss[index].bossstop = false;
		boss[index].bossleftstop = false;
	}

	boss[index].pos.x += boss[index].dir_destroy.x + boss[index].t*destroy_boss_dir*10.0f;

	if (boss[index].move == TRUE)
	{
		if (player.camerastop) {
			boss[index].pos.x += boss_dir.x;
		}
		else {
			boss[index].pos.x += boss_dir.x - player.speed.x;
		}
		boss[index].pos.y = destroy_boss_pos_y[index] - (3 * boss[index].t*((1 - boss[index].t)*(1 - boss[index].t)))*200.0f;
	}
	else
	{
		boss[index].pos.y += boss[index].dir_destroy.y;
		boss[index].rot += 2.0f;
	}
	if (boss[index].t > 1)
	{
		if (boss[index].pos.y < 450.0f)
		{
			boss[index].pos.y += 0.2f;
		}
	}


	boss[index].pos.x += boss_dir.x*BOSS_CHASE_SPEED + boss[index].dir_destroy.x + boss[index].t*destroy_boss_dir*15.0f;


	if (boss[index].move == TRUE)
	{
		boss[index].pos.y = destroy_boss_pos_y[index];
	}
	else
	{
		boss[index].pos.y += boss[index].dir_destroy.y;
	}
}
//
//void Boss_StateLaser(int index)
//{
//	if (boss[index].frame == 0) {
//		BossLaser_Create(boss[index].muki, boss[index].pos.x, boss[index].pos.y, boss[index].dir_shot);
//	}
//
//	//�t���[����i�߂�
//	boss[index].frame++;
//
//
//	//��莞�Ԍo������N�[���_�E���X�e�[�g�ֈڍs
//	if (boss[index].frame > 90) {
//
//		boss[index].frame = 0;
//
//		//�X�e�[�g���A�ҏ�Ԃֈڍs
//		boss[index].state = BOSS_STATE_COOLDOWN;
//	}
//}
//
//void BOSS_StateCooldown(int index)
//{
//	//�t���[����i�߂�
//	boss[index].frame++;
//
//	//��莞�Ԍo������A�҃X�e�[�g�ֈڍs
//	if (boss[index].frame > 30) {
//
//		boss[index].frame = 0;
//
//		//�X�e�[�g���A�ҏ�Ԃֈڍs
//		boss[index].state = BOSS_STATE_RETURN;
//	}
//}


void Boss_Attack(int index)
{
	D3DXVECTOR2 dir;		//�r�̌���
	D3DXVECTOR2 hand;

	//�e�𔭎�
	switch (boss[index].muki)
	{
	case 0://������
		dir.y = 1.0f;
		break;
	case 1://������
		dir.x = -1.0f;
		hand.x = boss[index].pos.x - 100.0f;
		break;
	case 2://�E����
		dir.x = 1.0f;
		hand.x = boss[index].pos.x + 100.0f;
		break;
	case 3://�����
		dir.y = -1.0f;
		break;
	}

	if (bossatk_countdown >= 250)	
	{
		boss[index].bosslasermodestop = TRUE;

		boss[index].pos = { 800,450 };

		bossatk_countdown = 0;

	BossLaser_Create(boss[index].muki,( boss[index].pos.x - (frand() * SCREEN_WIDTH))/3, boss[index].pos.y - 300.0f, boss_dir);

	BossThunder_IscooolTrue(index);

	BossThunder_Create(boss[index].muki, (boss[index].pos.x - (frand() * SCREEN_WIDTH))/3, boss[index].pos.y - 300.0f, boss_dir);

	BossFlame_IscooolTrue(index);

	BossFlame_Create(boss[index].muki, (boss[index].pos.x - (frand() * SCREEN_WIDTH))/3, boss[index].pos.y - 300.0f, boss_dir);
	}
	else if (bossatk_countdown >= 0 && bossatk_countdown < 250) {
		Bullet_IscoolTrue(index);
		BossBullet_Create(boss[index].muki, boss[index].pos.x, boss[index].pos.y, boss_dir);
		dir = D3DXVECTOR2(0.0f, 0.0f);
		
	}
	
	
	//Bullet_IscoolTrue(index);
	//BossBullet_Create(boss[index].muki, hand.x, boss[index].pos.y, boss_dir);



	/*  BossLaser_IscooolTrue(index);

		BossLaser_Create(boss[index].pos.x - (frand() * SCREEN_WIDTH) / 2, boss[index].pos.y - 300.0f, boss_dir);

		BossThunder_IscooolTrue(index);

		BossThunder_Create(boss[index].pos.x - (frand() * SCREEN_WIDTH) / 2, boss[index].pos.y - 300.0f, boss_dir);

		BossFlame_IscooolTrue(index);

		BossFlame_Create(boss[index].pos.x - (frand() * SCREEN_WIDTH) / 2, boss[index].pos.y - 300.0f, boss_dir);*/

}


int Boss_AddDamage(int damage)
{
	boss[0].hp -= damage;
	return boss[0].hp;

	if (boss[0].bosshitpoint < 0)
		boss[0].bosshitpoint = 0;
}



int Boss_GetHitPoint()
{
	return boss[0].bosshitpoint;
}

void Boss_NockBack(int index)
{
	boss[index].nock = TRUE;
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

BOSS_DATA GetBoss(int i) {
	return boss[i];
}

void BossInfoMatch(BOSS_DATA info, int i) {
	boss[i] = info;
}