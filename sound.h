//=============================================================================
//
// サウンド処理 [sound.h]
//
//=============================================================================
#pragma once


#include <windows.h>
#include "xaudio2.h"						// サウンド処理で必要


//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000,		    // BGM0
	SOUND_LABEL_BGM001,		    // BGM1
	SOUND_LABEL_BGM002,		    // BGM2
	SOUND_LABEL_SE_SHOT,		// 弾発射音
	SOUND_LABEL_SE_HIT,			// ヒット音
	SOUND_LABEL_SE_EXPLOSION2,	// 爆発音
	SOUND_LABEL_SE_SHORT_PUNCH,	// 爆発音
	SOUND_LABEL_SE_EXPLOSION,	// 爆発音
	SOUND_LABEL_SE_UE,			//↑
	SOUND_LABEL_SE_SURA,		//スライディング
	SOUND_LABEL_SE_BUTTON,		//ボタン
	SOUND_LABEL_SE_SITA,		//下
	SOUND_LABEL_SE_DEATH,		//死
	SOUND_LABEL_SE_FIRST,		//初動
	SOUND_LABEL_SE_HUE,			//笛
	SOUND_LABEL_TITLE,		    // タイトル
	SOUND_LABEL_GAME,		    // BGM1
	SOUND_LABEL_RESULT,		    // BGM2

	SOUND_LABEL_MAX,

} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);
