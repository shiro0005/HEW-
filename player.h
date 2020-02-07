#pragma once

#include "collision.h"
#include "2D.h"

typedef struct
{
	D3DXVECTOR2 pos;			//�ʒu
	int			muki;			//����
	int			color;			//�F
	D3DXVECTOR2 speed;          //�v���C���[�ړ�
	int			animePattern;	//�A�j���p�^�[��
	int			mode;           //�v���C���[���
	float         commbo;			//�v���C���[�R���{��

	int         slidemuki;      //slide�U��
	Capsule2D   collision;		//�J�v�Z���̓����蔻��
	Capsule2D   first[2];        //�����J�v�Z���œ����蔻������@[0]�E�@[1]�� 
	Capsule2D   UpCol;          //��U�������蔻��
	Capsule2D   DownCol;        //���U�������蔻��
	Capsule2D   SlideCol[2];       //�X���C�f�B���O�����蔻��
	Capsule2D   FCol;

	int			hitpoint;		//�q�b�g�|�C���g
	float       rotate;			//�v���C���[�̊p�x
	int         state;          //�v���C���[�̏��
	bool        stop;           //�E��~
	bool        leftstop;       //����~
	bool		attackcol;		//�U���̓����蔻������ǂݍ���
	bool        camerastop;     //���ꂪtrue�̎��J�����̈ړ����~�߂ăX�e�[�W�Œ�Ő�킹��
	int			com;			//�A���œ����U�����o�����Ȃ��ϐ�
	bool		karacombo;				//�󌂂��ōU�����Z�b�g
	int			allAT;			//����AT�̑���
	int			comboflame;		//combocommanduketukejikan
	D3DXVECTOR2		size;		//�e�N�X�`���̑傫��
	float		kyori;	//player�̐i�񂾋���
	int			fase;
}PLAYER;

typedef struct
{
	D3DXVECTOR2 vec;//�X�e�B�b�N�̂�������
	float angle;//�X�e�B�b�N�̊p�x�i�x�j
	bool F[10];//�X�e�B�b�N���̓t���O
	bool rote[2];//�E��]�Ȃ�rote[0]��true�A����]�Ȃ�rote[1]��true
	int data[2];//F�̃t���O�̃f�[�^���i�[����
	int cntdata;//�O�̃t���[���Ɠ������ʂ̎��ɉ��Z
}STICK;

void Player_Initialize(void);
void Player_Finalize(void);
void Player_Update(void);
void Player_Draw(void);
//void Player_AddDamage(int damage);
PLAYER GetPlayer();
void PlayerInfoMatch(PLAYER info);
void ChengeCamerastop(bool f);
void FaseCount(int f);
float Getkyori();
int GetFase();
//const Capsule2D* Player_GetCollision();
//const Capsule2D* Player_GetCollisionFR();
//const Capsule2D* Player_GetCollisionFL();
//bool GetfirstAT();
//float Player_Getdir();
//int Player_GetHitPoint();
