#pragma once

#include "collision.h"
#include <d3dx9.h>
#include "2D.h"

#define BOSSLASER_IMAGEMAX (100)
#define BOSSLASER_SPEED	(32)
#define BOSSLASER_VISIBLE_FRAME 100

#define BOSSLASER_MAX    (3)
#define BOSSLASER_WIDTH  (64)
#define BOSSLASER_HEIGHT (64)
#define BOSSLASER_PATTERN_FRAME (1)
#define BOSSLASER_PATTERN_MAX   (22)
#define BOSSLASER_PATTERN_H_MAX (4)

#define LASERHINT_MAX (3)
#define LASERHINT_WIDTH (128)
#define LASERHINT_HEIGHT (256)
#define LASERHINT_PATTERN_FRAME (1)
#define LASERHINT_PATTERN_MAX   (52)
#define LASERHINT_PATTERN_H_MAX (4)


typedef struct
{
	float		x, y;			//ï\é¶à íu
	float		move_x, move_y;	//
	float		rotation;
	Capsule2D		coco;
	bool		bEnable;
	bool coool;
	bool hit;  //playerÇ…ÉåÅ[ÉUÅ[Ç™ÇgÇhÇsÇµÇΩÇ©Ç«Ç§Ç©ÇÃflg

	int frame;
	int pattern;

}BossLaser;

void BossLaser_Initialize(void);
void BossLaser_Update(void);
void BossLaser_Draw(void);

void BossLaser_Create(int muki, float x, float y, D3DXVECTOR2 dir);

void BossLaser_Destroy(int index);
bool BossLaser_IsEnable(int index);
const Capsule2D* BossLaser_GetCollision(int index);
bool BossLaser_Iscoool(int index);
void BossLaser_IscooolFlase(int index);
void BossLaser_IscooolTrue(int index);
void Laserhit(bool f);
bool Get_laserhit();