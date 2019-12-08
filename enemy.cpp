
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

D3DXVECTOR2 dir;		//�r�̌���

static PLAYER player;//player�\���̂̏��

typedef enum
{
	//ENEMY_STATE_INIT,		//������
	//ENEMY_STATE_SEARCH,		//���͂��T�[�`
	//ENEMY_STATE_FIND,		//�G�𔭌�
	//ENEMY_STATE_CHASE,		//�ǐ�
	//ENEMY_STATE_SHOT,		//�V���b�g����
	//ENEMY_STATE_LASER,		//���[�U�[����
	//ENEMY_STATE_COOLDOWN,	//�V���b�g�I��
	//ENEMY_STATE_RETURN,		//��ʒu�ɖ߂�
	//ENEMY_STATE_DEAD,		//���S���
	ENEMY_STATE_INIT,		//������
	ENEMY_STATE_CHASE,		//�ǐ�
	ENEMY_STATE_DEAD,		//���S���

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
	//�G�̏�����
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
	//player�\���̂̏����擾
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

		//�����蔻��p���W�̍X�V
		enemy[i].colcol.r = ENEMY_WIDTH * 0.8f;
		enemy[i].colcol.s.p.x = enemy[i].pos.x;
		enemy[i].colcol.s.p.y = enemy[i].pos.y - 25.0f;
		enemy[i].colcol.s.v.x = 0.0f;
		enemy[i].colcol.s.v.y = 50.0f;

		//������񂾎��̏���
		

		//�X�L�b�v����t���[���l�𒴂�����
		if (++enemy[i].animeFrame > ANIME_PATTERN_SKIPFRAME)
		{
			//�A�j���p�^�[����i�߂�(�ő�l�𒴂����烊�Z�b�g)
			if (++enemy[i].animePattern >= ANIME_PATTERN_MAX)
				enemy[i].animePattern = 0;
			//�t���[���͌��ɖ߂�
			enemy[i].animeFrame = 0;
			
			enemy[i].pos.x -= player.speed.x * 10;//�G�l�~�[���v���C���[�̈ړ��ƍ��킹��
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

		//�����t���O��TRUE�̓G�̂ݏ������s��
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

void Enemy_StateInit(int index)//�G�l�~�[�o�����
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


	//�X�e�[�g���T�[�`��Ԃֈڍs
	//enemy[index].state = ENEMY_STATE_SEARCH;


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

void Enemy_StateChase(int index)
{

	//�v���C���[�̍��W���擾����
	D3DXVECTOR2 pl_pos = D3DXVECTOR2(player.collision.s.p.x, player.collision.s.p.y);
	
	D3DXVECTOR2 dir = pl_pos - enemy[index].pos;
	float muki = pl_pos.x - enemy[index].pos.x;
	D3DXVec2Normalize(&dir, &dir);

	if (muki < 0)
	{//�v���C���[���G�l�~�[�̍��ɂ���Ȃ獶�Ɍ���
		enemy[index].muki = 1;
	}
	else
	{//�v���C���[���G�l�~�[�̉E�ɂ���Ȃ�E�Ɍ���
		enemy[index].muki = 2;
	}

	dir *= ENEMY_CHASE_SPEED;

	if (enemy[index].move == TRUE)//�G�l�~�[��������΂���Ă��Ȃ�������
	{
		if (enemy[index].stay == FALSE)
		{
			enemy[index].pos.x += dir.x;
		}
	}
	else
	{//������ԏ���
		enemy[index].pos +=	enemy[index].dir_destroy;
	}

	enemy[index].color = 0;  //�ʏ���

	if (pl_pos.x + 100.0f > enemy[index].pos.x&&pl_pos.x - 100.0f < enemy[index].pos.x)//�U���������[�V��������
	{
		enemy[index].attack = TRUE;
	}

	if (enemy[index].attack == TRUE)//�U������
	{
		//�t���[����i�߂�
		enemy[index].frame++;

		enemy[index].color = 1;
		enemy[index].stay = TRUE;

		if (enemy[index].frame > 100)
		{
			enemy[index].animeAttack = TRUE;
			if (enemy[index].ready_attack==TRUE)
			{
				enemy[index].ready_attack = FALSE;
				Enemy_Attack(index); //�r��U��
			}
		}

		if (enemy[index].animeAttack == TRUE)
		{
			//�U�����[�V�����t���[����i�߂�
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

//void Enemy_StateShot(int index)
//{
//	//�t���[����i�߂�
//	enemy[index].frame++;
//
//	float shot_rad = atan2(enemy[index].dir_shot.y, enemy[index].dir_shot.x);
//	shot_rad = (float)(M_PI * 2 / 20)*enemy[index].frame;
//
//	//�ˏo�p�x�N�g�����t���[���l�ŉ�]������
//	D3DXVECTOR2 shot_dir;
//	shot_dir.x = enemy[index].dir_shot.x*cosf(shot_rad) - enemy[index].dir_shot.y*sinf(shot_rad);
//	shot_dir.y = enemy[index].dir_shot.x*sinf(shot_rad) + enemy[index].dir_shot.y*cosf(shot_rad);
//
//	EnemyBullet_Create(enemy[index].pos.x, enemy[index].pos.y, shot_dir);
//
//	//��莞�Ԍo������N�[���_�E���X�e�[�g�ֈڍs
//	if (enemy[index].frame > 90){
//
//		enemy[index].frame = 0;
//
//		//�X�e�[�g���A�ҏ�Ԃֈڍs
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
//	//�t���[����i�߂�
//	enemy[index].frame++;
//
//
//	//��莞�Ԍo������N�[���_�E���X�e�[�g�ֈڍs
//	if (enemy[index].frame > 90){
//
//		enemy[index].frame = 0;
//
//		//�X�e�[�g���A�ҏ�Ԃֈڍs
//		enemy[index].state = ENEMY_STATE_COOLDOWN;
//	}
//}
//
//void Enemy_StateCooldown(int index)
//{
//	//�t���[����i�߂�
//	enemy[index].frame++;
//
//	//��莞�Ԍo������A�҃X�e�[�g�ֈڍs
//	if (enemy[index].frame > 30){
//
//		enemy[index].frame = 0;
//
//		//�X�e�[�g���A�ҏ�Ԃֈڍs
//		enemy[index].state = ENEMY_STATE_RETURN;
//	}
//}
//
//void Enemy_StateReturn(int index)
//{
//	//�t���[����i�߂�
//	enemy[index].frame++;
//
//	//�v���C���[�̍��W���擾����
//	D3DXVECTOR2 pl_pos = D3DXVECTOR2(Player_GetCollision()->x, Player_GetCollision()->y);
//
//	//�A�҂�����W�ւ̌����x�N�g�����v�Z����
//	D3DXVECTOR2 dir = enemy[index].pos_return - enemy[index].pos;
//
//	//�x�N�g���̒������v�Z����
//	float length = D3DXVec2Length(&dir);
//
//	D3DXVec2Normalize(&dir, &dir);
//	dir *= ENEMY_CHASE_SPEED;
//
//	enemy[index].pos += dir;
//
//	//�ړ�����������������
//	enemy[index].muki = Enemy_CalcMuki(dir);
//
//	//�A�҂�����W�ɓ���������
//	if (length <= ENEMY_CHASE_SPEED){
//
//		enemy[index].frame = 0;
//
//		//�X�e�[�g���T�[�`��Ԃֈڍs
//		enemy[index].state = ENEMY_STATE_SEARCH;
//	}
//}


void Enemy_Attack(int index)
{
	//�e�𔭎�
	switch (enemy[index].muki)
	{
	case 0://������
		dir.y = 1.0f;
		break;
	case 1://������
		dir.x = -1.0f;
		break;
	case 2://�E����
		dir.x = 1.0f;
		break;
	case 3://�����
		dir.y = -1.0f;
		break;
	}

	Bullet_IscoolTrue(index);
	Bullet_Create(enemy[index].muki,enemy[index].pos.x, enemy[index].pos.y, dir);
	dir = D3DXVECTOR2(0.0f, 0.0f);
}