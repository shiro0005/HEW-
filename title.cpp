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
static STICK stick;
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
	stick.vec.x = GamePad_Stick().lX;
	stick.vec.y = GamePad_Stick().lY;

	//���������ƂɊp�x���擾�i���W�A���j
	rote = -atan2f(stick.vec.x, stick.vec.y) + D3DX_PI;

	//�ϐ�angle�Ɋp�x�i�x�j����
	stick.angle = 180 * rote / D3DX_PI;

	//�p�x�ɂ����F[�Ή����鐔]��true�ɂȂ�A��{�I�ɃA�P�R���Ɠ���
	//�ړ���������Ă���
	if (stick.vec == D3DXVECTOR2(0, 0)) {//�X�e�B�b�N����͂��ĂȂ��Ƃ�
		rote = 0;
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
			stick.data[cntt] = i;
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
			rote = 0;//�v���C���[�̊p�x��0�ɐݒ�

		}
	}
	else {
		stick.cntdata = 0;//������
	}

	cntt++;//�t���[���J�E���g
	if (cntt == 2) {
		cntt = 0;//������
	}



	////rote+=0.05f;
	if (!g_bEnd) {
		if ((Keyboard_IsTrigger(DIK_SPACE) || GamePad_IsPress(0, BUTTON_C)) && (stick.rote[0] || stick.rote[1])) {
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