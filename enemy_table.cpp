
#include "enemy_table.h"

D3DXVECTOR2 animeTable2[3][4][3] =
{
	{
		{//���F�@������
			D3DXVECTOR2(0.0f, 0.0f),
			D3DXVECTOR2(0.125f, 0.0f),
			D3DXVECTOR2(0.25f, 0.0f),
		},
		{//���F�@������
			D3DXVECTOR2(0.0f, 0.125f),
			D3DXVECTOR2(0.125f, 0.125f),
			D3DXVECTOR2(0.25f, 0.125f),
		},
		{//���F�@�E����
			D3DXVECTOR2(0.0f, 0.25f),
			D3DXVECTOR2(0.125f, 0.25f),
			D3DXVECTOR2(0.25f, 0.25f),
		},
		{//���F�@�����
			D3DXVECTOR2(0.0f, 0.375f),
			D3DXVECTOR2(0.125f, 0.375f),
			D3DXVECTOR2(0.25f, 0.375f),
		},
	},
	{
		{//�ԐF�@������
			D3DXVECTOR2(0.375f, 0.0f),
			D3DXVECTOR2(0.5f,   0.0f),
			D3DXVECTOR2(0.625f, 0.0f),
		},
		{//�ԐF�@������
			D3DXVECTOR2(0.375f, 0.125f),
			D3DXVECTOR2(0.5f,   0.125f),
			D3DXVECTOR2(0.625f, 0.125f),
		},
		{//�ԐF�@�E����
			D3DXVECTOR2(0.375f, 0.25f),
			D3DXVECTOR2(0.5f,   0.25f),
			D3DXVECTOR2(0.625f, 0.25f),
		},
		{//�ԐF�@�����
			D3DXVECTOR2(0.375f, 0.375f),
			D3DXVECTOR2(0.5f,   0.375f),
			D3DXVECTOR2(0.625f, 0.375f),
		},
	},
	{
		{//�F�@������
			D3DXVECTOR2(0.0f,   0.5f),
			D3DXVECTOR2(0.125f, 0.5f),
			D3DXVECTOR2(0.25f,  0.5f),
		},
		{//�F�@������
			D3DXVECTOR2(0.0f,   0.625f),
			D3DXVECTOR2(0.125f, 0.625f),
			D3DXVECTOR2(0.25f,  0.625f),
		},
		{//�F�@�E����
			D3DXVECTOR2(0.0f,   0.75f),
			D3DXVECTOR2(0.125f, 0.75f),
			D3DXVECTOR2(0.25f,  0.75f),
		},
		{//�F�@�����
			D3DXVECTOR2(0.0f,   0.875f),
			D3DXVECTOR2(0.125f, 0.875f),
			D3DXVECTOR2(0.25f,  0.875f),
		},
	},
};

D3DXVECTOR2 GetAnimTbl2(int set, int muki, int anim)
{
	return animeTable2[set][muki][anim];
}