#pragma once

#include "collision.h"
#include <d3dx9.h>
#include "2D.h"

//íeÇÃç≈ëÂï\é¶êî
#define BOSSLASER_MAX (1)


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