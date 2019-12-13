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
	int			enable;	//�����t���O

	D3DXVECTOR2 pos;	//�ʒu
	float		rot;	//��]
	int			color;	//�F
	int			muki;	//�����i�ړ������j

	int			animePattern;	//�A�j���p�^�[��
	int			animeFrame;		//�p�^�[���؂�ւ��E�G�C�g

	Capsule2D		collision;

	int			frame;
	int			frame_attack;  //�U�����[�V�����t���[��
	bool		animeAttack;
	bool		stay;		//�U�����̈ړ�����
	bool		ready_attack;
	bool		attack;
	bool		move;		//������΂��ꂽ�ウ�˂݁[�̒ǔ��������~�߂�
	int			frame_alpha;
	int			state;
	D3DXVECTOR2 dir_destroy;		//������΂�������
	D3DXVECTOR2 pos_return;

	int			bosshitpoint;		//�q�b�g�|�C���g
	int			t;
}BOSS_DATA;

//�O���[�o���ϐ�
BOSS_DATA boss[BOSS_COUNT];

float destroy_boss_pos_x[BOSS_COUNT];
float destroy_boss_pos_y[BOSS_COUNT];
int destroy_boss_dir;
static PLAYER player;

typedef enum
{
	//BOSS_STATE_INIT,		//������
	//BOSS_STATE_SEARCH,		//���͂��T�[�`
	//BOSS_STATE_FIND,		//�G�𔭌�
	//BOSS_STATE_CHASE,		//�ǐ�
	//BOSS_STATE_SHOT,		//�V���b�g����
	//BOSS_STATE_LASER,		//���[�U�[����
	//BOSS_STATE_COOLDOWN,	//�V���b�g�I��
	//BOSS_STATE_RETURN,		//��ʒu�ɖ߂�
	//BOSS_STATE_DEAD,		//���S���
	BOSS_STATE_INIT,		//������
	BOSS_STATE_CHASE,		//�ǐ�
	BOSS_STATE_DEAD,		//���S���

	BOSS_STATE_MAX

}BOSS_STATE;

//void Boss_StateSearch(int index);
void Boss_StateInit(int index);
void Boss_StateChase(int index);


void Boss_Initialize(void)
{
	//�G�̏�����
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
	//player�\���̂̏����擾
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
			{//�m�b�N�o�b�N
				boss[i].pos.x = destroy_boss_pos_x[i] + (boss[i].t*destroy_boss_dir*100.0f);
				boss[i].pos.y = destroy_boss_pos_y[i] - (3 * boss[i].t*((1 - boss[i].t)*(1 - boss[i].t)))*100.0f;
				boss[i].t = boss[i].t + 0.02;
			}
		}
	}

	//�����蔻��p���W�̍X�V
	boss[0].collision.r = BOSS_WIDTH * 0.8f;
	boss[0].collision.s.p.x = boss[0].pos.x;
	boss[0].collision.s.p.y = boss[0].pos.y - 25.0f;
	boss[0].collision.s.v.x = 0.0f;
	boss[0].collision.s.v.y = 50.0f;

	//�X�L�b�v����t���[���l�𒴂�����
	if (++boss[0].animeFrame > ANIME_PATTERN_SKIPFRAME)
	{
		//�A�j���p�^�[����i�߂�(�ő�l�𒴂����烊�Z�b�g)
		if (++boss[0].animePattern >= ANIME_PATTERN_MAX)
			boss[0].animePattern = 0;
		//�t���[���͌��ɖ߂�
		boss[0].animeFrame = 0;

		boss[0].pos.x -= player.speed.x * 10;//�G�l�~�[���v���C���[�̈ړ��ƍ��킹��
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

		//�����t���O��TRUE�̓G�̂ݏ������s��
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

//���������
//int Boss_CalcMuki(D3DXVECTOR2 dir)
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

void Boss_StateInit(int index)//�{�X�o�����
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


	//�X�e�[�g���T�[�`��Ԃֈڍs
	//enemy[index].state = ENEMY_STATE_SEARCH;

	boss[index].state = BOSS_STATE_CHASE;

}

//void Boss_StateSearch(int index)
//{
//	//�v���C���[�̍��W���擾����
//	D3DXVECTOR2 pl_pos = D3DXVECTOR2(player.collision.s.p.x, player.collision.s.p.y);
//
//	D3DXVECTOR2 dir = pl_pos - boss[index].pos;
//	float muki = pl_pos.x - boss[index].pos.x;
//	D3DXVec2Normalize(&dir, &dir);
//
//	if (muki < 0)
//	{//�v���C���[���G�l�~�[�̍��ɂ���Ȃ獶�Ɍ���
//		boss[index].muki = 1;
//	}
//	else
//	{//�v���C���[���G�l�~�[�̉E�ɂ���Ȃ�E�Ɍ���
//		boss[index].muki = 2;
//	}
//
//	dir *= BOSS_CHASE_SPEED;
//
//	if (boss[index].move == TRUE)//�G�l�~�[��������΂���Ă��Ȃ�������
//	{
//		if (boss[index].stay == FALSE)
//		{
//			boss[index].pos.x += dir.x;
//		}
//	}
//	else
//	{//������ԏ���
//		boss[index].pos += boss[index].dir_destroy;
//	}
//
//	boss[index].color = 0;  //�ʏ���
//
//	if (pl_pos.x + 200.0f > boss[index].pos.x&&pl_pos.x - 200.0f < boss[index].pos.x)//�U���������[�V��������
//	{
//		boss[index].attack = TRUE;
//	}
//
//	if (boss[index].attack == TRUE)//�U������
//	{
//		//�t���[����i�߂�
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
//				Boss_Attack(index); //�r��U��
//			}
//		}
//
//		if (boss[index].animeAttack == TRUE)
//		{
//			//�U�����[�V�����t���[����i�߂�
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
	//�t���[����i�߂�
	boss[index].frame++;

	//��莞�Ԍo������ǐՃX�e�[�g�ֈڍs
	if (boss[index].frame > 20) {

		boss[index].frame = 0;

		//�X�e�[�g���T�[�`��Ԃֈڍs
		boss[index].state = BOSS_STATE_CHASE;
	}
}

void Boss_StateChase(int index)
{
	//�t���[����i�߂�
	boss[index].frame++;

	//�v���C���[�̍��W���擾����
	D3DXVECTOR2 pl_pos = D3DXVECTOR2(player.collision.s.p.x, player.collision.s.p.y);

	D3DXVECTOR2 dir = pl_pos - boss[index].pos;
	D3DXVec2Normalize(&dir, &dir);
	dir *= BOSS_CHASE_SPEED;

	boss[index].pos += dir;

	//�ړ�����������������
	boss[index].muki = Boss_CalcMuki(dir);

	//��莞�Ԍo������e�ˏo�X�e�[�g�ֈڍs
	if (boss[index].frame > 120) {

		boss[index].frame = 0;

		//�ړ�������ۑ�
		boss[index].dir_shot = dir;

		//�����_���Łu�΂�T���e�v�������́u���[�U�[�v���ˏo
		if (rand() % 2)
		{
			//�X�e�[�g���T�[�`��Ԃֈڍs
			boss[index].state = BOSS_STATE_SHOT;
		}
		else {
			//�X�e�[�g���T�[�`��Ԃֈڍs
			boss[index].state = BOSS_STATE_LASER;
		}
	}
}

void Boss_StateShot(int index)
{
	//�t���[����i�߂�
	boss[index].frame++;

	float shot_rad = atan2(boss[index].dir_shot.y, boss[index].dir_shot.x);
	shot_rad = (float)(M_PI * 2 / 20)*boss[index].frame;

	//�ˏo�p�x�N�g�����t���[���l�ŉ�]������
	D3DXVECTOR2 shot_dir;
	shot_dir.x = boss[index].dir_shot.x*cosf(shot_rad) - boss[index].dir_shot.y*sinf(shot_rad);
	shot_dir.y = boss[index].dir_shot.x*sinf(shot_rad) + boss[index].dir_shot.y*cosf(shot_rad);

	//BossBullet_Create(boss[index].pos.x, boss[index].pos.y, shot_dir);

	//��莞�Ԍo������N�[���_�E���X�e�[�g�ֈڍs
	if (boss[index].frame > 90) {

		boss[index].frame = 0;

		//�X�e�[�g���A�ҏ�Ԃֈڍs
		boss[index].state = BOSS_STATE_COOLDOWN;
	}
}

void Boss_StateLaser(int index)
{
	if (boss[index].frame == 0) {
		//BossLaser_Create(boss[index].pos.x, boss[index].pos.y, boss[index].dir_shot);
	}

	//�t���[����i�߂�
	boss[index].frame++;


	//��莞�Ԍo������N�[���_�E���X�e�[�g�ֈڍs
	if (boss[index].frame > 90) {

		boss[index].frame = 0;

		//�X�e�[�g���A�ҏ�Ԃֈڍs
		boss[index].state = BOSS_STATE_COOLDOWN;
	}
}

void Boss_StateCooldown(int index)
{
	//�t���[����i�߂�
	boss[index].frame++;

	//��莞�Ԍo������A�҃X�e�[�g�ֈڍs
	if (boss[index].frame > 30) {

		boss[index].frame = 0;

		//�X�e�[�g���A�ҏ�Ԃֈڍs
		boss[index].state = BOSS_STATE_RETURN;
	}
}

void Boss_StateReturn(int index)
{
	//�t���[����i�߂�
	boss[index].frame++;

	//�v���C���[�̍��W���擾����
	D3DXVECTOR2 pl_pos = D3DXVECTOR2(player.collision.s.p.x, player.collision.s.p.y);

	//�A�҂�����W�ւ̌����x�N�g�����v�Z����
	D3DXVECTOR2 dir = boss[index].pos_return - boss[index].pos;

	//�x�N�g���̒������v�Z����
	float length = D3DXVec2Length(&dir);

	D3DXVec2Normalize(&dir, &dir);
	dir *= BOSS_CHASE_SPEED;

	boss[index].pos += dir;

	//�ړ�����������������
	boss[index].muki = Boss_CalcMuki(dir);

	//�A�҂�����W�ɓ���������
	if (length <= BOSS_CHASE_SPEED) {

		boss[index].frame = 0;

		//�X�e�[�g���T�[�`��Ԃֈڍs
		boss[index].state = BOSS_STATE_SEARCH;
	}
}
*/

void Boss_StateChase(int index)
{

	//�v���C���[�̍��W���擾����
	D3DXVECTOR2 pl_pos = D3DXVECTOR2(player.collision.s.p.x, player.collision.s.p.y);

	D3DXVECTOR2 dir = pl_pos - boss[index].pos;
	float muki = pl_pos.x - boss[index].pos.x;
	D3DXVec2Normalize(&dir, &dir);

	if (muki < 0)
	{//�v���C���[���G�l�~�[�̍��ɂ���Ȃ獶�Ɍ���
		boss[index].muki = 1;
	}
	else
	{//�v���C���[���G�l�~�[�̉E�ɂ���Ȃ�E�Ɍ���
		boss[index].muki = 2;
	}

	dir *= BOSS_CHASE_SPEED;

	if (boss[index].move == TRUE)//�G�l�~�[��������΂���Ă��Ȃ�������
	{
		if (boss[index].stay == FALSE)
		{
			boss[index].pos.x += dir.x;
		}
	}
	else
	{//������ԏ���
		boss[index].pos += boss[index].dir_destroy;
	}

	boss[index].color = 0;  //�ʏ���

	if (pl_pos.x + 200.0f > boss[index].pos.x&&pl_pos.x - 200.0f < boss[index].pos.x)//�U���������[�V��������
	{
		boss[index].attack = TRUE;
	}

	if (boss[index].attack == TRUE)//�U������
	{
		//�t���[����i�߂�
		boss[index].frame++;

		boss[index].color = 1;
		boss[index].stay = TRUE;

		if (boss[index].frame > 30)
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
			//�U�����[�V�����t���[����i�߂�
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
	////�ړ�����������������
	//enemy[index].muki = Enemy_CalcMuki(dir);

	////��莞�Ԍo������e�ˏo�X�e�[�g�ֈڍs
	//if (enemy[index].frame > 120){

	//	enemy[index].frame = 0;

	//	//�ړ�������ۑ�
	//	enemy[index].dir_shot = dir;

	//	//�����_���Łu�΂�T���e�v�������́u���[�U�[�v���ˏo
	//	if (rand() % 2)
	//	{
	//		//�X�e�[�g���T�[�`��Ԃֈڍs
	//		enemy[index].state = ENEMY_STATE_SHOT;
	//	}
	//	else{
	//		//�X�e�[�g���T�[�`��Ԃֈڍs
	//		enemy[index].state = ENEMY_STATE_LASER;
	//	}
	//}
}

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
