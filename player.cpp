
#include "player.h"
#include "main.h"
#include "input.h"
#include "sprite.h"
#include "table.h"
#include "bullet.h"
#include "blade.h"
#include "collision.h"
#include "math.h"
#include <time.h>
#include "2D.h"


#define PLAYER_WIDTH	(32)
#define PLAYER_HEIGHT	(32)

#define ANIME_PATTERN_MAX		3
#define ANIME_PATTERN_SKIPFRAME 8



typedef struct
{
	D3DXVECTOR2 vec;//�X�e�B�b�N�̂�������
	float angle;//�X�e�B�b�N�̊p�x�i�x�j
	bool F[10];//�X�e�B�b�N���̓t���O
	bool rote[2];//�E��]�Ȃ�rote[0]��true�A����]�Ȃ�rote[1]��true
	int data[2];//F�̃t���O�̃f�[�^���i�[����
	int cntdata;//�O�̃t���[���Ɠ������ʂ̎��ɉ��Z
}STICK;

//�O���[�o���ϐ�
PLAYER player;
STICK stick;
static int cnt;
static int frame;
static int olddmg;//�ߋ��̃_���[�W
/*
����

�U����
�З�
�x�N�g��
�̐��l������

*/




void Player_Initialize(void)
{
	srand((unsigned int)time(NULL));
	//�v���C���[�̏�����
	player.pos.x = SCREEN_WIDTH / 2;
	player.pos.y = 430.0f;

	player.color = 0;
	player.muki = 0;
	player.speed = D3DXVECTOR2(0.0f, 0.0f);
	player.collision.s.v.x = 5.0f;
	player.collision.s.v.y = 5.0f;
	player.collision.r = PLAYER_WIDTH * 1.3f;

	//�E��
	player.foot[0].r = 5.0f;
	player.foot[0].s.p.x = player.pos.x;
	player.foot[0].s.p.y = player.pos.y;
	player.foot[0].s.v.x = 10;
	player.foot[0].s.v.y = 20;

	//����
	player.foot[1].r = 10.0f;
	player.foot[1].s.p.x = player.pos.x;
	player.foot[1].s.p.y = player.pos.y;
	player.foot[1].s.v.x = 10;
	player.foot[1].s.v.y = 20;
			
	player.hitpoint = 10;
	olddmg = 10;
	player.firstAT = false;
	cnt = 0;
	frame = 0;
	for (int i = 0; i < 10; i++) {
		stick.F[i] = false;
	}

}

void Player_Finalize(void)
{

}


//�v���C���[�ړ�����
void Player_Update(void)
{

	player.speed = D3DXVECTOR2(0.0f, 0.0f);

	/*if (Keyboard_IsPress(DIK_UP) || GamePad_IsPress(0, BUTTON_UP)){
		dir.y -= 1.0f;
		player.muki = 3;
	}
	if ((Keyboard_IsPress(DIK_UP) && Keyboard_IsPress(DIK_A)) || GamePad_IsPress(0, BUTTON_UP)&&GamePad_IsPress(0,BUTTON_Y)){
		dir.y -= 10.0f;
		player.muki = 3;
		player.rotate += 0.1f;
	}
	if (Keyboard_IsPress(DIK_DOWN) || GamePad_IsPress(0, BUTTON_DOWN)){
		dir.y += 1.0f;
		player.muki = 0;
	}
	if ((Keyboard_IsPress(DIK_DOWN )&& Keyboard_IsPress(DIK_A)) || GamePad_IsPress(0, BUTTON_DOWN) && GamePad_IsPress(0, BUTTON_Y)) {
		dir.y += 10.0f;
		player.muki = 0;
		player.rotate += 0.1f;
	}
	if (Keyboard_IsPress(DIK_LEFT) || GamePad_IsPress(0, BUTTON_LEFT)){
		dir.x -= 1.0f;
		player.muki = 1;
	}
	if ((Keyboard_IsPress(DIK_LEFT ) && Keyboard_IsPress(DIK_A)) || GamePad_IsPress(0, BUTTON_LEFT) && GamePad_IsPress(0, BUTTON_Y)) {
		dir.x -= 10.0f;
		player.muki = 1;
		player.rotate += 0.1f;
	}
	if (Keyboard_IsPress(DIK_RIGHT) || GamePad_IsPress(0, BUTTON_RIGHT)){
		dir.x += 1.0f;
		player.muki = 2;
	}
	if ((Keyboard_IsPress(DIK_RIGHT) && Keyboard_IsPress(DIK_A)) || GamePad_IsPress(0, BUTTON_RIGHT) && GamePad_IsPress(0, BUTTON_Y)) {
		dir.x += 10.0f;
		player.muki = 2;
		player.rotate += 0.1f;
	}*/

	/*//�X�y�[�X�������ꂽ��e�𔭎�
	if (Keyboard_IsTrigger(DIK_Z) || GamePad_IsPress(0, BUTTON_Y) && GamePad_IsTrigger(0, BUTTON_A)) {
		//�J�[�\���L�[���͂�����Ă��Ȃ��ꍇ�A�������甭�˕������쐬����
		if (D3DXVec2Length(&dir) < 0.01f) {
			switch (player.muki)
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
		}
		Bullet_Create(player.pos.x, player.pos.y, dir);
		dir = D3DXVECTOR2(0.0f, 0.0f);
	}

	//�u���C�h�ōU��
	if (Keyboard_IsTrigger(DIK_X) || GamePad_IsPress(0, BUTTON_Y) && GamePad_IsTrigger(0, BUTTON_B)) {
		//�J�[�\���L�[���͂�����Ă��Ȃ��ꍇ�A��������������쐬����
		if (D3DXVec2Length(&dir) < 0.01f) {
			switch (player.muki)
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
		}

		Blade_Create(player.pos.x, player.pos.y, dir);
		dir = D3DXVECTOR2(0.0f, 0.0f);
	}*/

	//�}�b�v�Ƃ̃q�b�g�`�F�b�N
	//Collision_HitCheck_TileMap(player.pos, &player.speed);

	//�X�e�B�b�N��X�AY�������擾
	stick.vec.x = GamePad_Stick().lX;
	stick.vec.y = GamePad_Stick().lY;

	//���������ƂɊp�x���擾�i���W�A���j
	player.rotate = -atan2f(stick.vec.x, stick.vec.y) + D3DX_PI;

	//�ϐ�angle�Ɋp�x�i�x�j����
	stick.angle = 180 * player.rotate / D3DX_PI;

	//�p�x�ɂ����F[�Ή����鐔]��true�ɂȂ�A��{�I�ɃA�P�R���Ɠ���
	//�ړ���������Ă���
	if (stick.vec == D3DXVECTOR2(0, 0)) {//�X�e�B�b�N����͂��ĂȂ��Ƃ�
		player.rotate = 0;
		stick.angle = 1000.0f;
		for (int j = 0; j < 10; j++) {
			stick.F[j] = false;
		}
		stick.F[5] = true;
		stick.rote[0] = false;
		stick.rote[1] = false;
	}
	if ((337.5f <= stick.angle && stick.angle <= 360.0f) || (stick.angle >= 0 && stick.angle < 22.5f)) {
		stick.F[8] = true;//���͔���

		//��]����
		if (stick.F[7]) {
			stick.rote[0] = true;//�E���
			stick.rote[1] = false;
			stick.F[7] = false;//������
		}
		else if (stick.F[9]) {
			stick.rote[1] = true;//�����
			stick.rote[0] = false;
			stick.F[9] = false;
		}
		else {//�X�e�B�b�N����]���͂����Ă��Ȃ�
			for (int i = 1; i < 7; i++) {//F[7]�`F[9]�܂Œ��ׂ�K�v���Ȃ�����
				if (stick.F[i]) {//��]�Ɋ֌W�Ȃ�F��true�Ȃ�
					stick.rote[0] = false;//�ړ������Ȃ�
					stick.rote[1] = false;
					for (int j = 0; j < 10; j++) {//�S�Ă�F��������
						stick.F[j] = false;
					}
				}
			}
		}

	}
	else if (22.5f <= stick.angle && stick.angle < 67.5f) {
		stick.F[9] = true;
		if (stick.F[8]) {
			stick.rote[0] = true;
			stick.rote[1] = false;
			stick.F[8] = false;
		}
		else if (stick.F[6]) {
			stick.rote[1] = true;
			stick.rote[0] = false;
			stick.F[6] = false;
		}
		else {
			for (int i = 1; i < 8; i++) {
				if (i == 6) {//����
					i++;
				}
				if (stick.F[i]) {
					stick.rote[0] = false;
					stick.rote[1] = false;
					for (int j = 0; j < 10; j++) {
						stick.F[j] = false;
					}
				}
			}
		}
	}
	else if (67.5f <= stick.angle && stick.angle < 112.5f) {
		stick.F[6] = true;
		if (stick.F[9]) {
			stick.rote[0] = true;
			stick.rote[1] = false;
			stick.F[9] = false;
		}
		else if (stick.F[3]) {
			stick.rote[1] = true;
			stick.rote[0] = false;
			stick.F[3] = false;
		}
		else {
			for (int i = 1; i < 9; i++) {
				if (i == 6) {//��������Ȃ��Ɩ��񂱂��ɊY������̂ŏ��������������Ȃ�
					i++;
				}
				if (stick.F[i]) {
					stick.rote[0] = false;
					stick.rote[1] = false;
					for (int j = 0; j < 10; j++) {
						stick.F[j] = false;
					}
				}
			}
		}
	}
	else if (112.5f <= stick.angle && stick.angle < 157.5f) {
		stick.F[3] = true;
		if (stick.F[6]) {
			stick.rote[0] = true;
			stick.rote[1] = false;
			stick.F[6] = false;
		}
		else if (stick.F[2]) {
			stick.rote[1] = true;
			stick.rote[0] = false;
			stick.F[2] = false;
		}
		else {
			for (int i = 1; i < 10; i++) {
				if (i == 3) {
					i++;
				}
				if (stick.F[i]) {
					stick.rote[0] = false;
					stick.rote[1] = false;
					for (int j = 0; j < 10; j++) {
						stick.F[j] = false;
					}
				}
			}
		}
	}
	else if (157.5f <= stick.angle && stick.angle < 202.5f) {
		stick.F[2] = true;
		if (stick.F[3]) {
			stick.rote[0] = true;
			stick.rote[1] = false;
			stick.F[3] = false;
		}
		else if (stick.F[1]) {
			stick.rote[1] = true;
			stick.rote[0] = false;
			stick.F[1] = false;
		}
		else {
			for (int i = 4; i < 10; i++) {
				if (stick.F[i]) {
					stick.rote[0] = false;
					stick.rote[1] = false;
					for (int j = 0; j < 10; j++) {
						stick.F[j] = false;
					}
				}
			}
		}
	}
	else if (202.5f <= stick.angle && stick.angle < 247.5f) {
		stick.F[1] = true;
		if (stick.F[2]) {
			stick.rote[0] = true;
			stick.rote[1] = false;
			stick.F[2] = false;
		}
		else if (stick.F[4]) {
			stick.rote[1] = true;
			stick.rote[0] = false;
			stick.F[4] = false;
		}
		else {
			for (int i = 3; i < 10; i++) {
				if (stick.F[i]) {
					stick.rote[0] = false;
					stick.rote[1] = false;
					for (int j = 0; j < 10; j++) {
						stick.F[j] = false;
					}
				}
			}
		}
	}
	else if (247.5f <= stick.angle && stick.angle < 292.5f) {
		stick.F[4] = true;
		if (stick.F[1]) {
			stick.rote[0] = true;
			stick.rote[1] = false;
			stick.F[1] = false;
		}
		else if (stick.F[7]) {
			stick.rote[1] = true;
			stick.rote[0] = false;
			stick.F[7] = false;
		}
		else {
			for (int i = 1; i < 10; i++) {
				if (i == 4) {
					i++;
				}
				if (stick.F[i]) {
					stick.rote[0] = false;
					stick.rote[1] = false;
					for (int j = 0; j < 10; j++) {
						stick.F[j] = false;
					}
				}
			}
		}
	}
	else if (292.5f <= stick.angle && stick.angle < 337.5f) {
		stick.F[7] = true;
		if (stick.F[4]) {
			stick.rote[0] = true;
			stick.rote[1] = false;
			stick.F[4] = false;
		}
		else if (stick.F[8]) {
			stick.rote[1] = true;
			stick.rote[0] = false;
			stick.F[8] = false;
		}
		else {
			for (int i = 1; i < 10; i++) {
				if (i == 7) {
					i++;
				}
				if (stick.F[i]) {
					stick.rote[0] = false;
					stick.rote[1] = false;
					for (int j = 0; j < 10; j++) {
						stick.F[j] = false;
					}
				}
			}
		}
	}

	for (int i = 0; i < 10; i++) {
		if (stick.F[i]) {//�t���O��true�ɂȂ��Ă���Ƃ�����i�[
			stick.data[cnt] = i;
		}
	}

	if (stick.data[0] == stick.data[1]) {//�O�̃t���[���Ɠ����Ƃ��낪true�Ȃ�
		stick.cntdata++;//�t���O�������������񐔁i�t���[�����j
		if (stick.cntdata >= 13) {//13�t���[������������

			stick.rote[0] = false;//��]
			stick.rote[1] = false;

			for (int i = 0; i < 10; i++) {
				stick.F[i] = false;//�S�Ă�F[]��������
			}
			player.rotate = 0;//�v���C���[�̊p�x��0�ɐݒ�

		}
	}
	else {
		stick.cntdata = 0;//������
	}

	cnt++;//�t���[���J�E���g
	if (cnt == 2) {
		cnt = 0;//������
	}



	//�ړ��l�ݒ�
	if (stick.rote[0]) {
		player.speed.x = 5.0f;
	}
	else if (stick.rote[1]) {
		player.speed.x = -5.0f;
	}
	else {
		player.speed.x = 0;
	}

	//�����U��
	if (stick.rote[0] || stick.rote[1]) {//��]���Ȃ�
		if (GamePad_IsPress(0, BUTTON_C)) {
			player.firstAT = true;
		}
	}

	if (player.firstAT) {
		frame++;
		if (frame % 5 == 0) {
			player.animePattern++;
		}
		if (player.animePattern == 8) {
			player.animePattern = 0;
			player.firstAT = false;
		}
	}


	if (player.hitpoint!=olddmg) {
		player.mode = 1;
	}
	if (player.mode == 1) {
		frame++;
		if (frame % 5 == 0) {
			player.animePattern++;
		}
		if (player.animePattern == 8) {
			player.animePattern = 0;
			player.mode = 0;
		}
	}


	// ���W�̍X�V����
	//player.pos += player.speed;

	//�����蔻��p���W�̍X�V

	//�̑S�̂̓����蔻��
	player.collision.r = PLAYER_WIDTH * 1.3f;
	player.collision.s.p.x = (player.pos.x) + 50.0f*cos(D3DX_PI / 2 - player.rotate);
	player.collision.s.p.y = (player.pos.y) - 50.0f*sin(D3DX_PI / 2 - player.rotate);
	player.collision.s.v.x = 100.0f*cos(D3DX_PI / 2 + player.rotate);
	player.collision.s.v.y = 100.0f*sin(D3DX_PI / 2 + player.rotate);

	//�E��
	player.foot[0].s.p.x = player.pos.x;
	player.foot[0].s.p.y = player.pos.y;
	player.foot[0].s.v.x = 60.0f*cos(D3DX_PI / 2 + player.rotate-0.7f);
	player.foot[0].s.v.y = 60.0f*sin(D3DX_PI / 2 + player.rotate-0.7f);

	//����
	player.foot[1].s.p.x = player.pos.x+10.0f;
	player.foot[1].s.p.y = player.pos.y+15.0f;
	player.foot[1].s.v.x = 70.0f*cos(D3DX_PI / 2 + player.rotate + 0.87f);
	player.foot[1].s.v.y = 70.0f*sin(D3DX_PI / 2 + player.rotate + 0.87f);

	//player.rotate = 0;
	olddmg = player.hitpoint;
	////�X�L�b�v����t���[���l�𒴂�����
	//if (++player.animeFrame > ANIME_PATTERN_SKIPFRAME)
	//{
	//	//�A�j���p�^�[����i�߂�(�ő�l�𒴂����烊�Z�b�g)
	//	if (++player.animePattern >= ANIME_PATTERN_MAX)
	//		player.animePattern = 0;
	//	//�t���[���͌��ɖ߂�
	//	player.animeFrame = 0;
	//}

}

void Player_Draw(void)
{
	Sprite_Draw(TEXTURE_INDEX_YUKIDARUMA,
		player.pos.x,
		player.pos.y,
		GetAnimTbl(player.mode, player.animePattern).x * 256,
		GetAnimTbl(player.mode, player.animePattern).y * 256,
		32,
		32,
		16,
		16,
		4.0f,
		6.0f,
		player.rotate);
}

//const Capsule2D* Player_GetCollision()
//{
//	return &player.collision;
//}
//const Capsule2D* Player_GetCollisionFR()
//{
//	return &player.foot[0];
//}
//const Capsule2D* Player_GetCollisionFL()
//{
//	return &player.foot[1];
//}
//float Player_Getdir() {
//	return player.speed.x;
//}
//
//bool GetfirstAT() {
//	return player.firstAT;
//}
//
//int Player_GetHitPoint()
//{
//	return player.hitpoint;
//}

void Player_AddDamage(int damage)
{
	player.hitpoint -= damage;
	if (player.hitpoint < 0)
		player.hitpoint = 0;
}

PLAYER GetPlayer() {
	return player;
}

