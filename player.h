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
	bool        firstAT;        //�����U��
	int			mode;           //�v���C���[���
	int         commbo;			//�v���C���[�R���{��
	Capsule2D   collision;		//�J�v�Z���̓����蔻��
	Capsule2D   foot[2];        //�����J�v�Z���œ����蔻������@[0]�E���@[1]����   
	int			hitpoint;		//�q�b�g�|�C���g
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
