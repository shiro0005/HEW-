#include "player.h"
#include "sprite.h"
#include "texture.h"
#include "game.h"
#include "Hp_table.h"

static PLAYER player;

#define HP_WIDTH	(298)
#define HP_HEIGHT	(298)

void Hp_Initialize(void)
{
	player = GetPlayer();
}

void Hp_Finalize(void)
{
	
}

void Hp_Update(void)
{
	player = GetPlayer();
}

void Hp_Draw(void)
{
	

	Sprite_Draw(TEXTURE_INDEX_HPSITA,
		60.0f,
		60.0f,
		GetAnimTblHp(9).x * 256,
		GetAnimTblHp(9).y * 256,
		HP_WIDTH,
		HP_HEIGHT,
		HP_WIDTH/2,
		HP_HEIGHT/2,
		0.3f,
		0.3f,
		0.0f);

	Sprite_Draw(TEXTURE_INDEX_HPUE,
		60.0f,
		60.0f + ((((10 - player.hitpoint)*(HP_HEIGHT / 10)) / 2)*0.28f),
		GetAnimTblHp(player.hitpoint - 1).x * 256,
		GetAnimTblHp(player.hitpoint - 1).y * 256,
		HP_WIDTH,
		HP_HEIGHT - (10 - player.hitpoint)*(HP_HEIGHT / 10),
		HP_WIDTH / 2,
		(HP_HEIGHT - (10 - player.hitpoint)*(HP_HEIGHT / 10)) / 2,
		0.3f,
		0.3f,
		0.0f);
}