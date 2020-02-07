#include "input.h"
#include "scene.h"
#include "sprite.h"
#include "texture.h"
#include "fade.h"
#include "enemy.h"
#include "player.h"
#include <d3d9.h>
#include "sound.h"
#include "explosion2.h"

static bool g_bEnd = false;
static STICK sticktitle;
static float rote = 0;
static int cntt = 0;
void Title_Initialize(void)
{
	Fade_Start(false, 90, D3DCOLOR_RGBA(0, 0, 0, 0));
	g_bEnd = false;
	Explosion_Initialize2();
	Controller_Create(500.0f, 480.0f, 0.0f);
	PlaySound(SOUND_LABEL_TITLE);
}

void Title_Finalize(void)
{
	StopSound(SOUND_LABEL_TITLE);
}

void Title_Update(void)
{
	Explosion_Update2();

	//�X�e�B�b�N��X�AY�������擾
	sticktitle.vec.x = GamePad_Stick().lX;
	sticktitle.vec.y = GamePad_Stick().lY;

	//���������ƂɊp�x���擾�i���W�A���j
	rote = -atan2f(sticktitle.vec.x, sticktitle.vec.y) + D3DX_PI;

	//�ϐ�angle�Ɋp�x�i�x�j����
	sticktitle.angle = 180 * rote / D3DX_PI;

	//�p�x�ɂ����F[�Ή����鐔]��true�ɂȂ�A��{�I�ɃA�P�R���Ɠ���
	//�ړ���������Ă���
	if (sticktitle.vec == D3DXVECTOR2(0, 0)) {//�X�e�B�b�N����͂��ĂȂ��Ƃ�
		rote = 0;
		sticktitle.angle = 1000.0f;
		for (int j = 0; j < 10; j++) {
			sticktitle.F[j] = false;
		}
		sticktitle.F[5] = true;
		sticktitle.rote[0] = false;
		sticktitle.rote[1] = false;
	}
	if ((337.5f <= sticktitle.angle && sticktitle.angle <= 360.0f) || (sticktitle.angle >= 0 && sticktitle.angle < 22.5f)) {
		sticktitle.F[8] = true;//���͔���

		//��]����
		if (sticktitle.F[7]) {
			sticktitle.rote[0] = true;//�E���
			sticktitle.rote[1] = false;
			sticktitle.F[7] = false;//������
		}
		else if (sticktitle.F[9]) {
			sticktitle.rote[1] = true;//�����
			sticktitle.rote[0] = false;
			sticktitle.F[9] = false;
		}
		else {//�X�e�B�b�N����]���͂����Ă��Ȃ�
			for (int i = 1; i < 7; i++) {//F[7]�`F[9]�܂Œ��ׂ�K�v���Ȃ�����
				if (sticktitle.F[i]) {//��]�Ɋ֌W�Ȃ�F��true�Ȃ�
					sticktitle.rote[0] = false;//�ړ������Ȃ�
					sticktitle.rote[1] = false;
					for (int j = 0; j < 10; j++) {//�S�Ă�F��������
						sticktitle.F[j] = false;
					}
				}
			}
		}

	}
	else if (22.5f <= sticktitle.angle && sticktitle.angle < 67.5f) {
		sticktitle.F[9] = true;
		if (sticktitle.F[8]) {
			sticktitle.rote[0] = true;
			sticktitle.rote[1] = false;
			sticktitle.F[8] = false;
		}
		else if (sticktitle.F[6]) {
			sticktitle.rote[1] = true;
			sticktitle.rote[0] = false;
			sticktitle.F[6] = false;
		}
		else {
			for (int i = 1; i < 8; i++) {
				if (i == 6) {//����
					i++;
				}
				if (sticktitle.F[i]) {
					sticktitle.rote[0] = false;
					sticktitle.rote[1] = false;
					for (int j = 0; j < 10; j++) {
						sticktitle.F[j] = false;
					}
				}
			}
		}
	}
	else if (67.5f <= sticktitle.angle && sticktitle.angle < 112.5f) {
		sticktitle.F[6] = true;
		if (sticktitle.F[9]) {
			sticktitle.rote[0] = true;
			sticktitle.rote[1] = false;
			sticktitle.F[9] = false;
		}
		else if (sticktitle.F[3]) {
			sticktitle.rote[1] = true;
			sticktitle.rote[0] = false;
			sticktitle.F[3] = false;
		}
		else {
			for (int i = 1; i < 9; i++) {
				if (i == 6) {//��������Ȃ��Ɩ��񂱂��ɊY������̂ŏ��������������Ȃ�
					i++;
				}
				if (sticktitle.F[i]) {
					sticktitle.rote[0] = false;
					sticktitle.rote[1] = false;
					for (int j = 0; j < 10; j++) {
						sticktitle.F[j] = false;
					}
				}
			}
		}
	}
	else if (112.5f <= sticktitle.angle && sticktitle.angle < 157.5f) {
		sticktitle.F[3] = true;
		if (sticktitle.F[6]) {
			sticktitle.rote[0] = true;
			sticktitle.rote[1] = false;
			sticktitle.F[6] = false;
		}
		else if (sticktitle.F[2]) {
			sticktitle.rote[1] = true;
			sticktitle.rote[0] = false;
			sticktitle.F[2] = false;
		}
		else {
			for (int i = 1; i < 10; i++) {
				if (i == 3) {
					i++;
				}
				if (sticktitle.F[i]) {
					sticktitle.rote[0] = false;
					sticktitle.rote[1] = false;
					for (int j = 0; j < 10; j++) {
						sticktitle.F[j] = false;
					}
				}
			}
		}
	}
	else if (157.5f <= sticktitle.angle && sticktitle.angle < 202.5f) {
		sticktitle.F[2] = true;
		if (sticktitle.F[3]) {
			sticktitle.rote[0] = true;
			sticktitle.rote[1] = false;
			sticktitle.F[3] = false;
		}
		else if (sticktitle.F[1]) {
			sticktitle.rote[1] = true;
			sticktitle.rote[0] = false;
			sticktitle.F[1] = false;
		}
		else {
			for (int i = 4; i < 10; i++) {
				if (sticktitle.F[i]) {
					sticktitle.rote[0] = false;
					sticktitle.rote[1] = false;
					for (int j = 0; j < 10; j++) {
						sticktitle.F[j] = false;
					}
				}
			}
		}
	}
	else if (202.5f <= sticktitle.angle && sticktitle.angle < 247.5f) {
		sticktitle.F[1] = true;
		if (sticktitle.F[2]) {
			sticktitle.rote[0] = true;
			sticktitle.rote[1] = false;
			sticktitle.F[2] = false;
		}
		else if (sticktitle.F[4]) {
			sticktitle.rote[1] = true;
			sticktitle.rote[0] = false;
			sticktitle.F[4] = false;
		}
		else {
			for (int i = 3; i < 10; i++) {
				if (sticktitle.F[i]) {
					sticktitle.rote[0] = false;
					sticktitle.rote[1] = false;
					for (int j = 0; j < 10; j++) {
						sticktitle.F[j] = false;
					}
				}
			}
		}
	}
	else if (247.5f <= sticktitle.angle && sticktitle.angle < 292.5f) {
		sticktitle.F[4] = true;
		if (sticktitle.F[1]) {
			sticktitle.rote[0] = true;
			sticktitle.rote[1] = false;
			sticktitle.F[1] = false;
		}
		else if (sticktitle.F[7]) {
			sticktitle.rote[1] = true;
			sticktitle.rote[0] = false;
			sticktitle.F[7] = false;
		}
		else {
			for (int i = 1; i < 10; i++) {
				if (i == 4) {
					i++;
				}
				if (sticktitle.F[i]) {
					sticktitle.rote[0] = false;
					sticktitle.rote[1] = false;
					for (int j = 0; j < 10; j++) {
						sticktitle.F[j] = false;
					}
				}
			}
		}
	}
	else if (292.5f <= sticktitle.angle && sticktitle.angle < 337.5f) {
		sticktitle.F[7] = true;
		if (sticktitle.F[4]) {
			sticktitle.rote[0] = true;
			sticktitle.rote[1] = false;
			sticktitle.F[4] = false;
		}
		else if (sticktitle.F[8]) {
			sticktitle.rote[1] = true;
			sticktitle.rote[0] = false;
			sticktitle.F[8] = false;
		}
		else {
			for (int i = 1; i < 10; i++) {
				if (i == 7) {
					i++;
				}
				if (sticktitle.F[i]) {
					sticktitle.rote[0] = false;
					sticktitle.rote[1] = false;
					for (int j = 0; j < 10; j++) {
						sticktitle.F[j] = false;
					}
				}
			}
		}
	}

	for (int i = 0; i < 10; i++) {
		if (sticktitle.F[i]) {//�t���O��true�ɂȂ��Ă���Ƃ�����i�[
			sticktitle.data[cntt] = i;
		}
	}

	if (sticktitle.data[0] == sticktitle.data[1]) {//�O�̃t���[���Ɠ����Ƃ��낪true�Ȃ�
		sticktitle.cntdata++;//�t���O�������������񐔁i�t���[�����j
		if (sticktitle.cntdata >= 13) {//13�t���[������������

			sticktitle.rote[0] = false;//��]
			sticktitle.rote[1] = false;

			for (int i = 0; i < 10; i++) {
				sticktitle.F[i] = false;//�S�Ă�F[]��������
			}
			rote = 0;//�v���C���[�̊p�x��0�ɐݒ�

		}
	}
	else {
		sticktitle.cntdata = 0;//������
	}

	cntt++;//�t���[���J�E���g
	if (cntt == 2) {
		cntt = 0;//������
	}



	////rote+=0.05f;
	if (!g_bEnd) {
		if ((Keyboard_IsTrigger(DIK_SPACE) || GamePad_IsPress(0, BUTTON_C)) && (sticktitle.rote[0] || sticktitle.rote[1])) {
			Fade_Start(true, 30, D3DCOLOR_RGBA(0, 0, 0, 0));
			g_bEnd = true;
			controllerfalse(false);
			PlaySound(SOUND_LABEL_SE_HUE);
		}
	}
	else {
		if (!Fade_IsFade()) {
			Scene_Change(SCENE_INDEX_GAME);
		}
	}
}

void Title_Draw(void)
{
	Sprite_Draw(TEXTURE_INDEX_TITLE, 0.0f, 0.0f);
	Sprite_Draw(TEXTURE_INDEX_PUSH, 800.0f, 500.0f,0,0,710,205,355,102.5,0.3f,0.3f,0);
	Sprite_Draw(TEXTURE_INDEX_TAI, 350.0f, 300.0f, 0, 0, 720, 540, 360, 270, 0.5f, 0.5f, rote);
	Sprite_Draw(TEXTURE_INDEX_SOU, 650.0f, 300.0f, 0, 0, 720, 540, 360, 270, 0.5f, 0.5f, 0);
	Explosion_Draw2();
}