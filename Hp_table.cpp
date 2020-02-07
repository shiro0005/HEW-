#include "Hp_table.h"

D3DXVECTOR2 animeTableHp[10] =
{
	{//1
		D3DXVECTOR2(0.0f, 0.99f)
	},
	{//2
		D3DXVECTOR2(0.0f, 0.88f)
	},
	{//3
		D3DXVECTOR2(0.0f, 0.77f)
	},
	{//4
		D3DXVECTOR2(0.0f, 0.66f)
	},
	{//5
		D3DXVECTOR2(0.0f, 0.55f)
	},
	{//6
		D3DXVECTOR2(0.0f, 0.44f)
	},
	{//7
		D3DXVECTOR2(0.0f, 0.33f)
	},
	{//8
		D3DXVECTOR2(0.0f, 0.22f)
	},
	{//9
		D3DXVECTOR2(0.0f, 0.11f)
	},
	{//10
		D3DXVECTOR2(0.0f, 0.0f)
	},
};

D3DXVECTOR2 GetAnimTblHp(int set)
{
	return animeTableHp[set];
}