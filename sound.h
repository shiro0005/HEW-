//=============================================================================
//
// �T�E���h���� [sound.h]
//
//=============================================================================
#pragma once


#include <windows.h>
#include "xaudio2.h"						// �T�E���h�����ŕK�v


//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000,		    // BGM0
	SOUND_LABEL_BGM001,		    // BGM1
	SOUND_LABEL_BGM002,		    // BGM2
	SOUND_LABEL_SE_SHOT,		// �e���ˉ�
	SOUND_LABEL_SE_HIT,			// �q�b�g��
	SOUND_LABEL_SE_EXPLOSION2,	// ������
	SOUND_LABEL_SE_SHORT_PUNCH,	// ������
	SOUND_LABEL_SE_EXPLOSION,	// ������
	SOUND_LABEL_SE_UE,			//��
	SOUND_LABEL_SE_SURA,		//�X���C�f�B���O
	SOUND_LABEL_SE_BUTTON,		//�{�^��
	SOUND_LABEL_SE_SITA,		//��
	SOUND_LABEL_SE_DEATH,		//��
	SOUND_LABEL_SE_FIRST,		//����
	SOUND_LABEL_SE_HUE,			//�J
	SOUND_LABEL_TITLE,		    // �^�C�g��
	SOUND_LABEL_GAME,		    // BGM1
	SOUND_LABEL_RESULT,		    // BGM2

	SOUND_LABEL_MAX,

} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);
