
#include "table.h"

//D3DXVECTOR2 animeTable[3][4][3] =
//{
//	{
//		{//水色　下向き
//			D3DXVECTOR2(0.0f, 0.0f),
//			D3DXVECTOR2(0.125f, 0.0f),
//			D3DXVECTOR2(0.25f, 0.0f),
//		},
//		{//水色　左向き
//			D3DXVECTOR2(0.0f, 0.125f),
//			D3DXVECTOR2(0.125f, 0.125f),
//			D3DXVECTOR2(0.25f, 0.125f),
//		},
//		{//水色　右向き
//			D3DXVECTOR2(0.0f, 0.25f),
//			D3DXVECTOR2(0.125f, 0.25f),
//			D3DXVECTOR2(0.25f, 0.25f),
//		},
//		{//水色　上向き
//			D3DXVECTOR2(0.0f, 0.375f),
//			D3DXVECTOR2(0.125f, 0.375f),
//			D3DXVECTOR2(0.25f, 0.375f),
//		},
//	},
//	{
//		{//赤色　下向き
//			D3DXVECTOR2(0.375f, 0.0f),
//			D3DXVECTOR2(0.5f,   0.0f),
//			D3DXVECTOR2(0.625f, 0.0f),
//		},
//		{//赤色　左向き
//			D3DXVECTOR2(0.375f, 0.125f),
//			D3DXVECTOR2(0.5f,   0.125f),
//			D3DXVECTOR2(0.625f, 0.125f),
//		},
//		{//赤色　右向き
//			D3DXVECTOR2(0.375f, 0.25f),
//			D3DXVECTOR2(0.5f,   0.25f),
//			D3DXVECTOR2(0.625f, 0.25f),
//		},
//		{//赤色　上向き
//			D3DXVECTOR2(0.375f, 0.375f),
//			D3DXVECTOR2(0.5f,   0.375f),
//			D3DXVECTOR2(0.625f, 0.375f),
//		},
//	},
//	{
//		{//青色　下向き
//			D3DXVECTOR2(0.0f,   0.5f),
//			D3DXVECTOR2(0.125f, 0.5f),
//			D3DXVECTOR2(0.25f,  0.5f),
//		},
//		{//青色　左向き
//			D3DXVECTOR2(0.0f,   0.625f),
//			D3DXVECTOR2(0.125f, 0.625f),
//			D3DXVECTOR2(0.25f,  0.625f),
//		},
//		{//青色　右向き
//			D3DXVECTOR2(0.0f,   0.75f),
//			D3DXVECTOR2(0.125f, 0.75f),
//			D3DXVECTOR2(0.25f,  0.75f),
//		},
//		{//青色　上向き
//			D3DXVECTOR2(0.0f,   0.875f),
//			D3DXVECTOR2(0.125f, 0.875f),
//			D3DXVECTOR2(0.25f,  0.875f),
//		},
//	},
//};

D3DXVECTOR2 animeTable[3][8] =
{
	{
	  {
		D3DXVECTOR2(0.0,0.0)
	  },
	  {
		D3DXVECTOR2(0.129,0.0)
	  },
	  {
		D3DXVECTOR2(0.254,0.0)
	  },
	  {
		D3DXVECTOR2(0.379,0.0)
	  },
	  {
		D3DXVECTOR2(0.504,0.0)
	  },
	  {
		D3DXVECTOR2(0.633,0.0)
	  },
	  {
		D3DXVECTOR2(0.758,0.0)
	  },
	  {
		D3DXVECTOR2(0.879,0.0)
	  },
	},
	{
		  {
			D3DXVECTOR2(0.0,0.0)
		  },
		  {
			D3DXVECTOR2(0,0.121)
		  },
		  {
			D3DXVECTOR2(0,0.891)
		  },
		  {
			D3DXVECTOR2(0,0.773)
		  },
		  {
			D3DXVECTOR2(0,0.641)
		  },
		  {
			D3DXVECTOR2(0,0.512)
		  },
		  {
			D3DXVECTOR2(0,0.391)
		  },
		  {
			D3DXVECTOR2(0,0.25)
		  },
	},
	{
	  {
		D3DXVECTOR2(0.0,0.0)
	  },
	  {
		D3DXVECTOR2(0.129,0.121)
	  },
	  {
		D3DXVECTOR2(0.254,0.121)
	  },
	  {
		D3DXVECTOR2(0.379,0.121)
	  },
	  {
		D3DXVECTOR2(0.504,0.121)
	  },
	  {
		D3DXVECTOR2(0.633,0.121)
	  },
	  {
		D3DXVECTOR2(0.758,0.121)
	  },
	  {
		D3DXVECTOR2(0.879,0.121)
	  },
	},
};


D3DXVECTOR2 GetAnimTbl(int mode, int anim)
{
	return animeTable[mode][anim];
}
