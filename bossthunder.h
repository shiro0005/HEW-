#pragma once

#include "collision.h"
#include <d3dx9.h>
#include "2D.h"

//íeÇÃç≈ëÂï\é¶êî
#define BOSSTHUNDER_MAX (10)


void BossThunder_Initialize(void);
void BossThunder_Update(void);
void BossThunder_Draw(void);

void BossThunder_Create(int muki, float x, float y, D3DXVECTOR2 dir);

void BossThunder_Destroy(int index);
bool BossThunder_IsEnable(int index);
const Capsule2D* BossThunder_GetCollision(int index);
bool BossThunder_Iscoool(int index);
void BossThunder_IscooolFlase(int index);
void BossThunder_IscooolTrue(int index);
void thunderhit(bool f);
bool Get_Thunderhit();