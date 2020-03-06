#include "texture.h"
#include "player.h"
#include "enemy.h"
#include "enemy_hand.h"
#include "fade.h"
#include "scene.h"
#include "collision.h"
#include "boss.h"
#include "explosion.h"
#include "explosion2.h"
#include "score_draw.h"
#include "stage.h"
#include "sound.h"
#include "player_hp.h"
#include "bossbullet.h"
#include "bosslaser.h"
#include "bossthunder.h"
#include "bossflame.h"
#include "sprite.h"

typedef enum PHASE_INDEX
{
	PHASE_INDEX_FADE,
	PHASE_INDEX_PLAYER_IN,
	PHASE_INDEX_PLAYER_MUTEKI,
	PHASE_INDEX_PLAYER_NORMAL,
	PHASE_INDEX_BOSS,
	PHASE_INDEX_STAGE_CLEAR,
	PHASE_INDEX_END,

	PHASE_INDEX_MAX
};

static PHASE_INDEX g_Phase;
static int g_GameFrameCount;
static PLAYER player;
static int g_Score = 0;// 点数
static int g_KillCount = 0;
static int g_Bosskill = 0;
static bool gamend;

static bool Game_Bossappear(void);

static bool Game_EndCheck(void);

void FaseCheck();

void Game_Initialize(void)
{

	Player_Initialize();
	Hp_Initialize();
	Field1_Initialize();
	Enemy_Initialize();
	Boss_Initialize();
	BossBullet_Initialize();
	BossLaser_Initialize();
	BossThunder_Initialize();
	BossFlame_Initialize();
	Bullet_Initialize();
	Explosion_Initialize();
	Explosion_Initialize2();

	PlaySound(SOUND_LABEL_GAME);


	g_Phase = PHASE_INDEX_FADE;
	g_GameFrameCount = 0;
	g_Score = 0;
	g_KillCount = 0;
	g_Bosskill = 0;
	Fade_Start(false, 30, D3DCOLOR_RGBA(0,0,0,0));
}

void Game_Finalize(void)
{

	Player_Finalize();
	Enemy_Finalize();
	Boss_Finalize();
	
	StopSound(SOUND_LABEL_GAME);
}

void Game_Update(void)
{
	//player構造体の情報を取得
	player = GetPlayer();

	switch(g_Phase)
	{
	case PHASE_INDEX_FADE:
		if( !Fade_IsFade() ) {
			g_Phase = PHASE_INDEX_PLAYER_NORMAL;
			// PlaySound(SOUND_LABEL_BGM000);
		}
		break;
	case PHASE_INDEX_PLAYER_IN:
	case PHASE_INDEX_PLAYER_MUTEKI:
	case PHASE_INDEX_PLAYER_NORMAL:
		Player_Update();
		Hp_Update();
		Field1_Update();
		Enemy_Update();
		Bullet_Update();
		BossBullet_Update();
		Explosion_Update();
		Explosion_Update2();

		//当たり判定は必ずすべてのアップデート処理が終わった後に行う
		Collision_Update();

		FaseCheck();

		//ボス出現
		if (Game_Bossappear())
		{
			Boss_StateInit(0);
			g_Phase = PHASE_INDEX_BOSS;
		}
		if (Game_EndCheck())
		{
			Fade_Start(true, 90, D3DCOLOR_RGBA(0, 0, 0, 0));
			g_Phase = PHASE_INDEX_STAGE_CLEAR;
		}
		break;
	case PHASE_INDEX_BOSS:
 		Player_Update();
		Hp_Update();
		Enemy_Update();
		Bullet_Update();
		Field1_Update();
		BossLaser_Update();
		BossThunder_Update();
		BossFlame_Update();
		Boss_Update();
		BossBullet_Update();
		/*BossBullet_Draw();
		BossLaser_Update();*/
		Explosion_Update();
		Explosion_Update2();

		//当たり判定は必ずすべてのアップデート処理が終わった後に行う
		Collision_Update();

		//ゲームの終了チェック
		if (Game_EndCheck())
		{
			Fade_Start(true, 90, D3DCOLOR_RGBA(0, 0, 0, 0));
			g_Phase = PHASE_INDEX_STAGE_CLEAR;
		}
		break;
	case PHASE_INDEX_STAGE_CLEAR:
		if (!Fade_IsFade()) {
			Scene_Change(SCENE_INDEX_RESULT);
			g_Phase = PHASE_INDEX_END;
		}
		break;
	case PHASE_INDEX_END:
		break;
	}

}

void Game_Draw(void)
{
	//TileMap_Draw();
	Field1_Draw();
	Player_Draw();
	Hp_Draw();
	Enemy_Draw();
	Boss_Draw();
	BossLaser_Draw();
	BossThunder_Draw();
	BossFlame_Draw();
	Bullet_Draw();
	Explosion_Draw();
	Explosion_Draw2();
	//BossBullet_Draw();
	//ライフスコア表示
	if (!player.commbo == 0)
	{
		if (player.commbo < 10)
		{
			Score_Draw(864, 150, 1.3f*(player.commbo / 20.0f + 1.0f), 1.3f*(player.commbo / 20.0f + 1.0f), player.commbo, 1, true);
		}
		if (player.commbo >= 10)
		{
			Score_Draw(832.8, 150, 1.3f*1.5f, 1.2f*1.5f,player.commbo, 2, true);
		}
		Sprite_Draw(TEXTURE_INDEX_HIT,
			920.0f,
			160.0f,
			0,
			0,
			608,
			253,
			608/2,
			253/2,
			0.1f,
			0.1f,
			0.0f);
	}
	Score_Draw(800, 32, 1.0f, 1.0f, g_Score, 7, true);
}

bool Game_Bossappear(void)
{
	if(player.fase==3)
	{

		if (player.kyori > 5000.0f) {
			StopSound(SOUND_LABEL_GAME);
		}

		if (player.kyori > 9000.0f)
		{
			ChengeCamerastop(true);
			StopSound(SOUND_LABEL_GAME);
			PlaySound(SOUND_LABEL_BOSS);

			return true;
		}

	}

	/*if (player.hitpoint <= 0)
		return true;*/

	return false;

}

bool Game_EndCheck(void)
{
	//ボス倒したら終了
	if (g_Bosskill >= 1)
	{
		gamend = true;
		
		

		return true;
	}

	//if(g_KillCount>=3)
	//	return true;

	if (player.dead)
	{
		gamend = false;
		StopSound(SOUND_LABEL_GAME);
		StopSound(SOUND_LABEL_BOSS);
		return true;
	}

	return false;
}

void Game_AddScore(int score)
{
	g_Score += score;
}

void Game_AddKillCount()
{
	g_KillCount += 1;
	
}

void Game_AddKillBossCount()
{
	g_Bosskill += 1;
}

int GetScore()
{
	return g_Score;
}

void FaseCheck()
{
	if (player.fase == 0)
	{
		if (g_KillCount >= 3)
		{
			FaseCount(1);
			ChengeCamerastop(false);
		}
	}

	if (player.fase == 1)
	{
		if (player.kyori >= 1200.0f)
		{
			ChengeCamerastop(true);
			if (g_KillCount >= 7)
			{
				FaseCount(2);
				ChengeCamerastop(false);
			}
		}
	}

	if (player.fase == 2)
	{
		if (player.kyori >= 2000.0f)
		{
			ChengeCamerastop(true);
			if (g_KillCount >= 31)
			{
				FaseCount(3);
				ChengeCamerastop(false);
			}
		}
	}
}

bool GetGamend()
{
	return gamend;
}