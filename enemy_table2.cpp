
#include "enemy_table2.h"

D3DXVECTOR2 animeTable3[2][2][3] =
{
	{
		{//���F�@������
			D3DXVECTOR2(0.0f, 0.05f),
			D3DXVECTOR2(0.95f, 0.05f),
			D3DXVECTOR2(1.9f, 0.05f),
		},
		{//���F�@�E����
			D3DXVECTOR2(0.0f, 1.25f),
			D3DXVECTOR2(0.95f, 1.25f),
			D3DXVECTOR2(1.9f, 1.25f),
		}
	},
	{
		{//�ԐF�@������
			D3DXVECTOR2(0.0f, 2.5f),
			D3DXVECTOR2(0.95f, 2.5f),
			D3DXVECTOR2(1.9f, 2.5f),
		},
		{//�ԐF�@������
			D3DXVECTOR2(0.0f, 4.0f),
			D3DXVECTOR2(0.95f, 4.0f),
			D3DXVECTOR2(1.9f, 4.0f),
		}
	}
};

D3DXVECTOR2 GetAnimTbl3(int set, int muki, int anim)
{
	return animeTable3[set][muki][anim];
}