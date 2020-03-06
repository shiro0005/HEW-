#include "input.h"
#include "scene.h"
#include "sprite.h"
#include "texture.h"
#include "fade.h"
#include "game.h"
#include "score_draw.h"
#include "sound.h"

int g_Score;
static bool g_bEnd = false;

void Result_Initialize(void)
{
	Fade_Start(false, 90, D3DCOLOR_RGBA(0, 0, 0, 0));
	PlaySound(SOUND_LABEL_RESULT);
	g_bEnd = false;
	g_Score = 0;
}

void Result_Finalize(void)
{
	StopSound(SOUND_LABEL_RESULT);
}

void Result_Update(void)
{
	g_Score = GetScore();

	if (!g_bEnd) {
		if (Keyboard_IsTrigger(DIK_SPACE) || GamePad_IsPress(0, BUTTON_C)) {
			Fade_Start(true, 30, D3DCOLOR_RGBA(0, 0, 0, 0));
			g_bEnd = true;
			PlaySound(SOUND_LABEL_SE_BUTTON);
		}
	}
	else {
		if (!Fade_IsFade()) {
			Scene_Change(SCENE_INDEX_TITLE);
		}
	}
}

void Result_Draw(void)
{
	if (GetGamend())
	{
		Sprite_Draw(TEXTURE_INDEX_RESULT, 0.0f, 0.0f);
		Score_Draw(430, 180, 1.0f, 1.0f, g_Score, 7, true);
	}
	else
	{
		Sprite_Draw(TEXTURE_INDEX_GAMEOVER, 0.0f, 0.0f);
		Score_Draw(430, 180, 1.0f, 1.0f, g_Score, 7, true);
	}
}
