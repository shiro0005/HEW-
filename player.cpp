
#include "player.h"
#include "main.h"
#include "input.h"
#include "sprite.h"
#include "player_table.h"
#include "bullet.h"
#include "blade.h"
#include "collision.h"
#include "explosion2.h"
#include "math.h"
#include <time.h>
#include "2D.h"
#include "collisioncheck.h"
#include "enemy.h"
#include "sound.h"

static ENEMY_DATA enemy;

#define PLAYER_WIDTH	(32)
#define PLAYER_HEIGHT	(32)

#define ANIME_PATTERN_MAX		3
#define ANIME_PATTERN_SKIPFRAME 8

typedef enum {
	PLAYER_INIT,//�ҋ@���
	PLAYER_MOVE,//�ړ�
	PLAYER_FIRSTAT,//�����U��
	PLAYER_FRONTAT,//�O�U��
	PLAYER_DAMAGE,//��_���[�W
	PLAYER_DEATH,//���S

	PLAYER_MAX
}PLAYER_STATE;

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
	player.mode = 0;
	player.size = D3DXVECTOR2(300.0f, 400.0f);

	player.attackcol = true;
	player.stop = false;
	player.leftstop = false;
	player.color = 0;
	player.muki = 0;
	player.speed = D3DXVECTOR2(0.0f, 0.0f);
	player.collision.s.v.x = 5.0f;
	player.collision.s.v.y = 5.0f;
	player.collision.r = PLAYER_WIDTH * 0.1f;




	player.commbo = 0;
	player.hitpoint = 10;
	player.karacombo = true;
	olddmg = 10;
	player.allAT = 0;
	cnt = 0;
	player.com = 0;
	player.comboflame = 0;
	frame = 0;
	for (int i = 0; i < 10; i++) {
		stick.F[i] = false;
	}
	player.camerastop = true;
	player.kyori = 0.0f;
	player.fase = 0;
}

void Player_Finalize(void)
{

}


//�v���C���[�ړ�����
void Player_Update(void)
{
	if (player.fase == 1) {
		if (player.kyori > 2000.0f)
		{
			player.camerastop = true;
		}
	}

	player.speed = D3DXVECTOR2(0.0f, 0.0f);




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
	if (player.allAT == 1) {
		player.speed.x = 0;
	}
	else if (stick.rote[0]) {
		player.speed.x = 6.5f;
	}
	else if (stick.rote[1]) {
		player.speed.x = -6.5f;
	}
	else {
		player.speed.x = 0;
	}

	if (player.stop || player.leftstop) {
		if (player.stop&&player.leftstop) {
			player.speed.x = 0.0f;
			player.stop = false;
			player.leftstop = false;
		}
		else if (player.stop) {
			if (player.speed.x > 0) {
				player.speed.x = 0.0f;
			}
			else {
				player.stop = false;
			}
		}
		else if (player.leftstop) {
			if (player.speed.x < 0) {
				player.speed.x = 0.0f;
			}
			else {
				player.leftstop = false;
			}
		}
	}

	//allAT=0...��U����, 1...�����U��, 2...��U��, 3...���U��

	//�����U��
	if ((stick.rote[0] || stick.rote[1])) {//��]���Ȃ�
		if (player.com == 0) {
			if (!(player.com == 2)) {
				if (player.allAT == 0) {
					if (GamePad_IsTrigger(0, BUTTON_C)) {
						StopSound(SOUND_LABEL_SE_UE);
						StopSound(SOUND_LABEL_SE_SURA);
						StopSound(SOUND_LABEL_SE_SITA);
						PlaySound(SOUND_LABEL_SE_FIRST);
						player.comboflame = 0;
						player.karacombo = true;
						player.mode = 1;
						player.attackcol = true;
						player.com = 1;

						player.allAT = 1;
						frame = 0;
						for (int i = 0; i < ENEMY_COUNT; i++)
						{
							enemy = GetEnemy(i);

							enemy.damagecol = true;

							EnemyInfoMatch(enemy, i);
						}
					}
				}
			}
		}
	}

	if (player.com == 1) {
		frame++;
		if (frame % 40 == 0) {
			player.animePattern++;
		}
		if (stick.rote[0])
		{
			//�E��

			player.first[0].r = 60.0f;
			player.first[0].s.p.x = (player.pos.x) + 30.0f*cos(D3DX_PI / 2 - player.rotate);
			player.first[0].s.p.y = (player.pos.y) - 30.0f*sin(D3DX_PI / 2 - player.rotate);
			player.first[0].s.v.x = 80.0f*cos(D3DX_PI / 2 + player.rotate);
			player.first[0].s.v.y = 80.0f*sin(D3DX_PI / 2 + player.rotate);


			player.muki = 1;

		}
		else if (stick.rote[1])
		{
			//����
			player.first[1].r = 60.0f;
			player.first[1].s.p.x = (player.pos.x) - 30.0f*cos(D3DX_PI / 2 - player.rotate);
			player.first[1].s.p.y = (player.pos.y) - 30.0f*sin(D3DX_PI / 2 - player.rotate);
			player.first[1].s.v.x = 60.0f*cos(D3DX_PI / 2 + player.rotate);
			player.first[1].s.v.y = 60.0f*sin(D3DX_PI / 2 + player.rotate);

			player.muki = 0;
		}
		player.size = D3DXVECTOR2(200.0f, 400.0f);
		if (player.animePattern == 1) {
			player.animePattern = 0;
			player.allAT = 0;
			player.com = 0;
		}
	}

	//�͊ۂ̍��U��   ��U��
	if (player.commbo >= 1) {//��]���Ȃ�
		if (player.karacombo == false) {
			if (player.comboflame < 60) {
				if (!(player.allAT == 2)) {
					if (GamePad_IsPress(0, BUTTON_X)) {
						StopSound(SOUND_LABEL_SE_FIRST);
						StopSound(SOUND_LABEL_SE_SURA);
						StopSound(SOUND_LABEL_SE_SITA);
						PlaySound(SOUND_LABEL_SE_UE);
						player.comboflame = 0;
						player.karacombo = true;
						player.attackcol = true;
						frame = 0;
						player.mode = 2;
						player.animePattern = 2;
						player.allAT = 2;
						player.com = 2;

						Explosion_Create4(player.pos.x, player.pos.y);

						//enemy�U����������
						for (int i = 0; i < ENEMY_COUNT; i++)
						{
							enemy = GetEnemy(i);

							enemy.damagecol = true;
							enemy.attack == FALSE;
							enemy.frame = 0;
							enemy.stay = FALSE;

							EnemyInfoMatch(enemy, i);
						}
					}
				}
			}
		}
	}

	if (player.com == 2) {
		frame++;

		if (player.animePattern == 2)
		{

			//��U���̓����蔻��
			player.UpCol.r = PLAYER_WIDTH * 0.5f;
			player.UpCol.s.p.x = player.pos.x - 70.0f;
			player.UpCol.s.p.y = player.pos.y + 20.0f;
			player.UpCol.s.v.x = 150.0f;
			player.UpCol.s.v.y = -40.0f;

			player.pos.y -= 12.0f;


		}
		if (player.animePattern == 1)
		{
			player.pos.y -= 6.0f;
		}
		if (player.animePattern == 0)
		{
			player.pos.y += 18.0f;
		}

		if (frame % 10 == 0) {
			player.animePattern--;
		}

		player.muki = 0;
		player.size = D3DXVECTOR2(400.0f, 200.0f);
		player.rotate = 0.0f;
		if (player.animePattern == 1)
		{
			//��U���̓����蔻��
			player.UpCol.r = PLAYER_WIDTH * 0.5f;
			player.UpCol.s.p.x = player.pos.x + 10.0f;
			player.UpCol.s.p.y = player.pos.y + 70.0f;
			player.UpCol.s.v.x = -20.0f;
			player.UpCol.s.v.y = -140.0f;

			player.size = D3DXVECTOR2(200.0f, 400.0f);
		}

		if (player.animePattern == 0) {
			//��U���̓����蔻��
			player.UpCol.r = PLAYER_WIDTH * 0.5f;
			player.UpCol.s.p.x = player.pos.x + 90.0f;
			player.UpCol.s.p.y = player.pos.y + 25.0f;
			player.UpCol.s.v.x = -180.0f;
			player.UpCol.s.v.y = -55.0f;
		}
		player.speed.x *= 2;

		if (player.animePattern == -1) {
			player.animePattern = 0;
			player.allAT = 0;
			player.com = 0;
		}
	}

	//�͊ۂ̍��U��2�@���U��
	if (player.commbo >= 1) {//��]���Ȃ�
		if (!(player.com == 2)) {
			if (player.karacombo == false) {
				if (player.comboflame < 60) {
					if (!(player.allAT == 3)) {
						if (GamePad_IsPress(0, BUTTON_B)) {
							StopSound(SOUND_LABEL_SE_UE);
							StopSound(SOUND_LABEL_SE_FIRST);
							StopSound(SOUND_LABEL_SE_SURA);
							PlaySound(SOUND_LABEL_SE_SITA);
							player.comboflame = 0;
							player.karacombo = true;
							player.attackcol = true;
							frame = 0;
							player.mode = 3;
							player.animePattern = 0;
							player.allAT = 3;
							player.com = 3;

							//enemy�U����������
							for (int i = 0; i < ENEMY_COUNT; i++)
							{
								enemy = GetEnemy(i);

								enemy.damagecol = true;
								enemy.attack == FALSE;
								enemy.frame = 0;
								enemy.stay = FALSE;

								EnemyInfoMatch(enemy, i);
							}
						}
					}
				}
			}
		}
	}

	if (player.com == 3) {
		frame++;

		if (frame % 5 == 0) {
			player.animePattern++;
		}

		if (frame % 30 == 0) {
			player.attackcol = true;
			for (int i = 0; i < ENEMY_COUNT; i++)
			{
				enemy = GetEnemy(i);

				enemy.damagecol = true;

				EnemyInfoMatch(enemy, i);
			}
		}

		//���U���̓����蔻��
		player.DownCol.r = PLAYER_WIDTH * 0.5f;
		player.DownCol.s.p.x = player.pos.x + 80.0f;
		player.DownCol.s.p.y = player.pos.y - 25.0f;
		player.DownCol.s.v.x = -180.0f;
		player.DownCol.s.v.y = 60.0f;

		player.muki = 0;
		player.size = D3DXVECTOR2(400.0f, 200.0f);
		player.rotate = 0.0f;
		if (player.animePattern % 3 == 1)
		{
			//���U���̓����蔻��
			player.DownCol.r = PLAYER_WIDTH * 0.5f;
			player.DownCol.s.p.x = player.pos.x;
			player.DownCol.s.p.y = player.pos.y - 30.0f;
			player.DownCol.s.v.x = 0.0f;
			player.DownCol.s.v.y = 60.0f;

			player.size = D3DXVECTOR2(200.0f, 200.0f);
		}

		if (player.animePattern % 3 == 2) {
			//���U���̓����蔻��
			player.DownCol.r = PLAYER_WIDTH * 0.5f;
			player.DownCol.s.p.x = player.pos.x - 80.0f;
			player.DownCol.s.p.y = player.pos.y - 25.0f;
			player.DownCol.s.v.x = 180.0f;
			player.DownCol.s.v.y = 60.0f;
		}

		if (player.animePattern == 15) {
			StopSound(SOUND_LABEL_SE_SITA);
			player.animePattern = 0;
			player.allAT = 0;
			player.com = 0;
		}
	}

	//�͊ۂ̍��U��3�@�X���C�f�B���O
	if (player.commbo >= 1) {//��]���Ȃ�
		if (!(player.com == 2)) {
			if (player.karacombo == false) {
				if (player.comboflame < 60) {
					if (!(player.allAT == 4)) {
						if (GamePad_IsPress(0, BUTTON_A)) {
							StopSound(SOUND_LABEL_SE_UE);
							StopSound(SOUND_LABEL_SE_FIRST);
							StopSound(SOUND_LABEL_SE_SITA);
							PlaySound(SOUND_LABEL_SE_SURA);
							player.comboflame = 0;
							player.karacombo = true;
							player.attackcol = true;
							frame = 0;
							player.mode = 3;
							player.animePattern = 0;
							player.allAT = 4;
							//�e�X�g�p
							player.com = 4;

							//enemy�U����������
							for (int i = 0; i < ENEMY_COUNT; i++)
							{
								enemy = GetEnemy(i);

								enemy.damagecol = true;
								enemy.attack == FALSE;
								enemy.frame = 0;
								enemy.stay = FALSE;

								EnemyInfoMatch(enemy, i);
							}
						}
					}
				}
			}
		}
	}


	if (player.com == 4) {
		frame++;
		//�e�X�g�p
		player.speed.x *= 3.0f;
		player.size = D3DXVECTOR2(400.0f, 200.0f);
		player.rotate = 0.0f;
		player.muki = 0;
		if (stick.rote[0])
		{

			player.slidemuki = 0;
			//���U���̓����蔻��
			player.SlideCol[0].r = PLAYER_WIDTH * 0.5f;
			player.SlideCol[0].s.p.x = player.pos.x - 80.0f;
			player.SlideCol[0].s.p.y = player.pos.y - 25.0f;
			player.SlideCol[0].s.v.x = 180.0f;
			player.SlideCol[0].s.v.y = 60.0f;
			player.animePattern = 2;

		}
		else if (stick.rote[1])
		{
			player.slidemuki = 1;
			//���U���̓����蔻��
			player.SlideCol[1].r = PLAYER_WIDTH * 0.5f;
			player.SlideCol[1].s.p.x = player.pos.x + 80.0f;
			player.SlideCol[1].s.p.y = player.pos.y - 25.0f;
			player.SlideCol[1].s.v.x = -180.0f;
			player.SlideCol[1].s.v.y = 60.0f;

			player.animePattern = 0;
		}

		if (frame % 30 == 0) {
			player.animePattern++;
		}
		if (player.animePattern == 1 || player.animePattern == 3) {
			player.animePattern = 0;
			player.allAT = 0;
			player.com = 0;
		}
	}

	////�R���{����
	if (player.comboflame >= 60)
	{
		player.commbo = 0;
	}

	//�R���{��t���Ԃ�i�߂�
	player.comboflame++;

	//��퓬���ʏ�e�N�X�`��
	if (player.com == 0)
	{

		player.mode = 0;
		player.size = D3DXVECTOR2(300.0f, 400.0f);
	}

	// ���W�̍X�V����
	if (player.camerastop) {
		player.pos += player.speed;
		if (player.pos.x < 50.0f) {
			player.pos.x = 50.0f;
		}
		if (player.pos.x > SCREEN_WIDTH - 50.0f) {
			player.pos.x = SCREEN_WIDTH - 50.0f;
		}
	}

	if (!player.camerastop) {

		if (!(507.0f < player.pos.x && player.pos.x < 517.0f))
		{
			if (player.pos.x < SCREEN_WIDTH / 2)
			{
				player.pos.x += player.speed.x;
			}
			if (player.pos.x > SCREEN_WIDTH / 2)
			{
				if (player.speed.x >= 5.0f) {
					player.pos.x -= player.speed.x;
				}
				if (player.speed.x <= -5.0f) {
					player.pos.x += player.speed.x * 2;
				}
			}
		}
		else
		{
			if (player.speed.x < 0.0f)
			{
				player.pos.x += player.speed.x;
			}
		}
		if (player.pos.x < 50.0f) {
			player.pos.x = 50.0f;
			player.speed.x = 0.0f;
		}
		//�v���C���[�̐i�񂾋���
		player.kyori += player.speed.x;
	}
	//�����蔻��p���W�̍X�V

	//�̑S�̂̓����蔻��
	player.collision.r = PLAYER_WIDTH * 2.0f;
	player.collision.s.p.x = (player.pos.x) + 30.0f*cos(D3DX_PI / 2 - player.rotate);
	player.collision.s.p.y = (player.pos.y) - 30.0f*sin(D3DX_PI / 2 - player.rotate);
	player.collision.s.v.x = 80.0f*cos(D3DX_PI / 2 + player.rotate);
	player.collision.s.v.y = 80.0f*sin(D3DX_PI / 2 + player.rotate);


	//player.rotate = 0;
	olddmg = player.hitpoint;

}

void Player_Draw(void)
{

	Sprite_Draw(TEXTURE_INDEX_PLAYER,
		player.pos.x,
		player.pos.y,
		GetAnimTblp(player.mode,player.muki, player.animePattern%3).x * 256,
		GetAnimTblp(player.mode,player.muki, player.animePattern%3).y * 256,
		player.size.x,
		player.size.y,
		player.size.x/2,
		player.size.y/2,
		0.5f,
		0.5f,
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


//void Player_AddDamage(int damage)
//{
//	player.hitpoint -= damage;
//	if (player.hitpoint < 0)
//		player.hitpoint = 0;
//}

PLAYER GetPlayer() {
	return player;
}

void PlayerInfoMatch(PLAYER info) {
	player = info;
}

void ChengeCamerastop(bool f) {
	player.camerastop = f;
}

void FaseCount(int f)
{
	player.fase = f;
}