#pragma once

// �}�N����`
#define TILEMAP_TEXTURE		_T("data/TEXTURE/kokosozai.png")	// �T���v���p�摜
#define TILEMAP_SIZE_W		(512)	// �e�N�X�`���T�C�Y
#define TILEMAP_SIZE_H		(512)	// ����
#define TILEMAP_POS_X		(0)		// �|���S���̏����ʒuX(����)
#define TILEMAP_POS_Y		(0)		// ����

#define TILEMAP_DIVIDE_X	(16)	// �c�̕�����
#define TILEMAP_DIVIDE_Y	(16)	// ���̕�����


typedef struct
{
	float u;
	float v;
	int	  isWalk;	//���s�\�t���O

}TILE_DATA;


void TileMap_Initialize(void);
void TileMap_Finalize(void);
void TileMap_Update(void);
void TileMap_Draw(void);

TILE_DATA GetTileInfo(float x, float y);
void SetTileType(int x, int y, int type);
