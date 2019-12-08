#include "boss.h"
#include "main.h"
#include "sprite.h"
#include "table.h"
#include "player.h"
#include "bossbullet.h"
#include "bosslaser.h"
#include "explosion.h"


#define _USE_MATH_DEFINES
#include <math.h>

#define BOSS_WIDTH				(32)
#define BOSS_HEIGHT			(32)
#define ANIME_PATTERN_MAX		(3)
#define ANIME_PATTERN_SKIPFRAME (8)

#define BOSS_SEARCH_RADIUS		(200.0f)
#define BOSS_SEARCH_ANGLE		(0.85f)
#define BOSS_CHASE_SPEED		(3.0f)
#define BOSS_RETURN_SPEED		(1.0f)

typedef struct
{
	int			enable;	//�����t���O

	D3DXVECTOR2 pos;	//�ʒu
	float		rot;	//��]
	int			color;	//�F
	int			muki;	//�����i�ړ������j

	int			animePattern;	//�A�j���p�^�[��
	int			animeFrame;		//�p�^�[���؂�ւ��E�G�C�g

	CUPSULE		collision;

	int			frame;
	int			frame_alpha;
	int			state;
	D3DXVECTOR2 dir_shot;
	D3DXVECTOR2 pos_return;

	int			bosshitpoint;		//�q�b�g�|�C���g

}BOSS_DATA;

BOSS_DATA boss[BOSS_COUNT];


typedef enum
{
	BOSS_STATE_INIT,		//������
	BOSS_STATE_SEARCH,		//���͂��T�[�`
	BOSS_STATE_FIND,		//�G�𔭌�
	BOSS_STATE_CHASE,		//�ǐ�
	BOSS_STATE_SHOT,		//�V���b�g����
	BOSS_STATE_LASER,		//���[�U�[����
	BOSS_STATE_COOLDOWN,	//�V���b�g�I��
	BOSS_STATE_RETURN,		//��ʒu�ɖ߂�
	BOSS_STATE_DEAD,		//���S���

	BOSS_STATE_MAX

}ENEMY_STATE;


int Boss_CalcMuki(D3DXVECTOR2 dir);


void Boss_StateSearch(int index);
void Boss_StateFind(int index);
void Boss_StateChase(int index);
void Boss_StateShot(int index);
void Boss_StateLaser(int index);
void Boss_StateCooldown(int index);
void Boss_StateReturn(int index);


void Boss_Initialize(void)
{
	//�G�̏�����
	for (int i = 0; i < BOSS_COUNT; i++) {
//		Boss_StateInit(i);
		boss[i].enable = FALSE;
		boss[i].state = BOSS_STATE_SEARCH;

		boss[i].bosshitpoint = 10000;
	}
}

void Boss_Finalize(void)
{

}

void Boss_Update(void)
{
	for (int i = 0; i < BOSS_COUNT; i++) {

		//�����蔻��p���W�̍X�V
		boss[i].collision.x = boss[i].pos.x;
		boss[i].collision.y = boss[i].pos.y;

		//�X�L�b�v����t���[���l�𒴂�����
		if (++boss[i].animeFrame > ANIME_PATTERN_SKIPFRAME)
		{
			//�A�j���p�^�[����i�߂�(�ő�l�𒴂����烊�Z�b�g)
			if (++boss[i].animePattern >= ANIME_PATTERN_MAX)
				boss[i].animePattern = 0;
			//�t���[���͌��ɖ߂�
			boss[i].animeFrame = 0;
		}

		switch (boss[i].state)
		{
		case BOSS_STATE_INIT:
			Boss_StateInit(i);
			break;
		case BOSS_STATE_SEARCH:
			Boss_StateSearch(i);
			break;
		case BOSS_STATE_FIND:
			Boss_StateFind(i);
			break;
		case BOSS_STATE_CHASE:
			Boss_StateChase(i);
			break;
		case BOSS_STATE_SHOT:
			Boss_StateShot(i);
			break;
		case BOSS_STATE_LASER:
			Boss_StateLaser(i);
			break;
		case BOSS_STATE_COOLDOWN:
			Boss_StateCooldown(i);
			break;
		case BOSS_STATE_RETURN:
			Boss_StateReturn(i);
			break;
		default:
			break;
		}
	}
}

void Boss_Draw(void)
{
	for (int i = 0; i < BOSS_COUNT; i++) {

		//�����t���O��TRUE�̓G�̂ݏ������s��
		if (!boss[i].enable) {
			continue;
		}

		/*Sprite_Draw(TEXTURE_INDEX_YUKIDARUMA,
			boss[i].pos.x,
			boss[i].pos.y,
			GetAnimTbl(boss[i].color, boss[i].muki, boss[i].animePattern).x * 256,
			GetAnimTbl(boss[i].color, boss[i].muki, boss[i].animePattern).y * 256,
			32,
			32,
			16,
			16,
			8.0f,
			8.0f,
			boss[i].rot);*/
	}
}

void Boss_Destroy(int index)
{
	boss[index].state = BOSS_STATE_DEAD;
	boss[index].enable = false;
}

bool Boss_IsEnable(int index)
{
	return boss[index].enable;
}

const CUPSULE* Boss_GetCollision(int index)
{
	return &boss[index].collision;
}

//���������
int Boss_CalcMuki(D3DXVECTOR2 dir)
{
	int muki = 0;

	//�ړ��������L�����`�F�b�N
	if (D3DXVec2Length(&dir) > 0.01f)
	{
		//X������Y�����̑傫�����ׂāA�ǂ���̕����傫���̂��𒲂ׂ�
		//X�����̕����傫���ꍇ
		if (fabs(dir.x) > fabs(dir.y))
		{
			//X�������v���X�̒l�Ȃ̂��}�C�i�X�̒l�Ȃ̂��𒲂ׂ�
			//�}�C�i�X�����̏ꍇ�͍�����
			if (dir.x < 0.0f)
				muki = 1;
			//��������Ȃ��i�v���X�����j�ꍇ�͉E����
			else
				muki = 2;
		}
		//Y�����̕����傫���ꍇ
		else
		{
			//Y�������v���X�̒l�Ȃ̂��}�C�i�X�̒l�Ȃ̂��𒲂ׂ�
			//�}�C�i�X�����̏ꍇ�͏����
			if (dir.y < 0.0f)
				muki = 3;
			//��������Ȃ��i�v���X�����j�ꍇ�͏����
			else
				muki = 0;
		}
	}

	return muki;
}

void Boss_StateInit(int index)//�{�X�o�����
{
	boss[index].pos.x = frand() * SCREEN_WIDTH;
	boss[index].pos.y = frand() * SCREEN_HEIGHT;
	boss[index].rot = 0;
	boss[index].color = 1;

	boss[index].muki = rand() % 4;
	boss[index].enable = TRUE;

	boss[index].collision.x = boss[index].pos.x;
	boss[index].collision.y = boss[index].pos.y;
	boss[index].collision.r = BOSS_WIDTH * 0.8f;

	boss[index].frame = 0;

	//�X�e�[�g���T�[�`��Ԃֈڍs
	boss[index].state = BOSS_STATE_SEARCH;

}

void Boss_StateSearch(int index)
{
	//�t���[����i�߂�
	boss[index].frame++;

	D3DXVECTOR2 dir;

	//�����Ă�������̃x�N�g�������
	switch (boss[index].muki)
	{
	case 0://������
		dir = D3DXVECTOR2(0.0f, 1.0f);
		break;
	case 1://������
		dir = D3DXVECTOR2(-1.0f, 0.0f);
		break;
	case 2://�E����
		dir = D3DXVECTOR2(1.0f, 0.0f);
		break;
	case 3://�����
		dir = D3DXVECTOR2(0.0f, -1.0f);
		break;
	}

	//�v���C���[�̍��W���擾����
	D3DXVECTOR2 pl_pos = D3DXVECTOR2(Player_GetCollision()->s.p.x, Player_GetCollision()->s.p.y);

	//�G�ƃv���C���[�̋������v�Z����
	D3DXVECTOR2 vLen = boss[index].pos - pl_pos;
	float length = D3DXVec2Length(&vLen);

	//���肷�鋗���̒��ɓ����Ă��邩���`�F�b�N����
	if (length < BOSS_SEARCH_RADIUS)
	{
		//�G���猩�ăv���C���[�̍��W�ւ̕����x�N�g�������
		D3DXVECTOR2 pl_dir = pl_pos - boss[index].pos;
		D3DXVec2Normalize(&pl_dir, &pl_dir);

		//�G�̌����ƃv���C�����W�ւ̕����x�N�g���œ��ς����
		float dot = D3DXVec2Dot(&dir, &pl_dir);

		//���肷��p�x���ɓ����Ă��邩���`�F�b�N����
		if (dot > BOSS_SEARCH_ANGLE)
		{
			Explosion_Create(boss[index].pos.x, boss[index].pos.y);

			//����ɒʂ�΃v���C���[�����X�e�[�g�Ɉڍs����
			boss[index].state = BOSS_STATE_FIND;
			boss[index].frame = 0;

			//�߂��ė�����W��ۑ�����
			boss[index].pos_return = boss[index].pos;
		}
	}

	//������ω�������
	if (boss[index].frame > 60)
	{
		boss[index].muki = (boss[index].muki + 1) % 4;
		boss[index].frame = 0;
	}

}

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
	D3DXVECTOR2 pl_pos = D3DXVECTOR2(Player_GetCollision()->s.p.x, Player_GetCollision()->s.p.y);

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
	D3DXVECTOR2 pl_pos = D3DXVECTOR2(Player_GetCollision()->s.p.x, Player_GetCollision()->s.p.y);

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
