
#include "enemy_table.h"

D3DXVECTOR2 animeTablep[4][2][3] =
{
	{
		{//�v���C���[�@����
			D3DXVECTOR2(0.0f, 0.0f),
		},
	},
	{
		{//�v���C���[�@��
			D3DXVECTOR2(0.0f, 1.75f),
		},
		{//�v���C���[�@�E
			D3DXVECTOR2(1.0f, 1.75f),
		}
	},
	{
		{//�v���C���[�@��U��
			D3DXVECTOR2(0.0f, 3.75f),
			D3DXVECTOR2(1.75f, 3.25f),
			D3DXVECTOR2(2.625f, 3.75f),
		},
	},
	{
		{//�v���C���[�@���U��
			D3DXVECTOR2(0.0f, 5.125f),
			D3DXVECTOR2(1.75f, 5.125f),
			D3DXVECTOR2(2.625f, 5.125f),
		},
	},


};

D3DXVECTOR2 GetAnimTblp(int set, int muki, int anim)
{
	return animeTablep[set][muki][anim];
}