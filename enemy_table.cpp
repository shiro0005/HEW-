
#include "enemy_table.h"

D3DXVECTOR2 animeTable2[2][2][3] =
{
	{
		{//水色　左向き
			D3DXVECTOR2(0.0f, 0.0f),
			D3DXVECTOR2(0.78f, 0.0f),
			D3DXVECTOR2(1.56f, 0.0f),
		},
		{//水色　右向き
			D3DXVECTOR2(0.0f, 1.3f),
			D3DXVECTOR2(0.78f, 1.3f),
			D3DXVECTOR2(1.56f, 1.3f),
		}
	},
	{
		{//赤色　下向き
			D3DXVECTOR2(0.0f, 2.6f),
			D3DXVECTOR2(0.78f, 2.6f),
			D3DXVECTOR2(1.56f, 2.6f),
		},
		{//赤色　左向き
			D3DXVECTOR2(0.0f, 3.9f),
			D3DXVECTOR2(0.78f, 3.9f),
			D3DXVECTOR2(1.56f, 3.9f),
		}
	}
};




D3DXVECTOR2 GetAnimTbl2(int set, int muki, int anim)
{
	return animeTable2[set][muki][anim];
}