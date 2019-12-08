//=============================================================================
//
// �w�i���� [bg.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "tileMap.h"
#include "input.h"
#include "player.h"
#include "texture.h"
#include "sprite.h"





#define TILETABLE_TABLE_NUM			5		//�}�b�v�e�[�u���T�C�Y

#define TILETABLE_TABLE_SIZE_X		32		//�}�b�v�T�C�Y��
#define TILETABLE_TABLE_SIZE_Y		18		//�}�b�v�T�C�Y�c


TILE_DATA TileTable[TILETABLE_TABLE_NUM] =
{
	{ 0.375f,  0.3125f, 1 },	//0:�n��
	{ 0.0f,    0.3125f, 0 },	//1:�ǃu���b�N
	{ 0.0625f, 0.3125f, 1 },	//2:�͂���
	{ 0.125f,  0.3125f, 0 },	//3:��
	{ 0.3125f, 0.3125f, 0 },	//4:�󔠁i�J�j
};


int MAP_DATA[TILETABLE_TABLE_SIZE_Y][TILETABLE_TABLE_SIZE_X] =
{
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
};


void TileMap_Initialize(void)
{
}

void TileMap_Finalize(void)
{

}

void TileMap_Update(void)
{
}

void TileMap_Draw(void)
{
	/*float w = TILEMAP_SIZE_W / TILEMAP_DIVIDE_X;
	float h = TILEMAP_SIZE_H / TILEMAP_DIVIDE_Y;

	for (int y = 0; y < TILETABLE_TABLE_SIZE_Y; y++)
	{
		for (int x = 0; x < TILETABLE_TABLE_SIZE_X; x++)
		{
			Sprite_Draw(TEXTURE_INDEX_TILEMAP,
				x * w,
				y * h,
				TileTable[MAP_DATA[y][x]].u * TILEMAP_SIZE_W,
				TileTable[MAP_DATA[y][x]].v * TILEMAP_SIZE_H,
				w,
				h);
		}
	}*/

	Sprite_Draw(TEXTURE_INDEX_BGMAIN, 0.0f, 0.0f);
}

TILE_DATA GetTileInfo(float x, float y)
{

	int tile_x = x / (TILEMAP_SIZE_W / TILEMAP_DIVIDE_X);
	int tile_y = y / (TILEMAP_SIZE_H / TILEMAP_DIVIDE_Y);

	
	return TileTable[MAP_DATA[tile_y][tile_x]];
}

void SetTileType(int x, int y, int type)
{
	//�͈̓`�F�b�N
	if (type >= TILETABLE_TABLE_NUM)
		return;

	MAP_DATA[y][x] = type;
}
