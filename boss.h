#pragma once

#include "collision.h"
#include "2D.h"

typedef struct
{
	int			enable;	//�����t���O

	D3DXVECTOR2 pos;	//�ʒu
	float		rot;	//��]
	int			color;	//�F
	int			muki;	//�����i�ړ������j
	int			animePattern;	//�A�j���p�^�[��
	int			animeFrame;		//�p�^�[���؂�ւ��E�G�C�g
	int			hp;


	Capsule2D   colcol;

	int			frame;
	int			frame_attack;  //�U�����[�V�����t���[��
	bool		animeAttack;
	bool		stay;		//�U�����̈ړ�����
	bool		ready_attack;
	bool		attack;
	bool       laserattack;
	bool		move;		//������΂��ꂽ�ウ�˂݁[�̒ǔ��������~�߂�
	bool		nock;
	bool        bossstop;
	bool        bosslasermodestop;
	bool       bossleftstop;
	int			frame_alpha;
	int			state;
	D3DXVECTOR2 dir_destroy;		//������΂�������
	D3DXVECTOR2 pos_return;
	D3DXVECTOR2 dir_shot;

	int			bosshitpoint;		//�q�b�g�|�C���g
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