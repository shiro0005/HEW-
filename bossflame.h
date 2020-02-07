#pragma once

#include "collision.h"
#include <d3dx9.h>
#include "2D.h"

//íeÇÃç≈ëÂï\é¶êî
#define BOSSFLAME_MAX (10)


void BossFlame_Initialize(void);
void BossFlame_Update(void);
void BossFlame_Draw(void);

void BossFlame_Create(int muki, float x, float y, D3DXVECTOR2 dir);

void BossFlame_Destroy(int index);
bool BossFlame_IsEnable(int index);
const Capsule2D* BossFlame_GetCollision(int index);
bool BossFlame_Iscoool(int index);
void BossFlame_IscooolFlase(int index);
void BossFlame_IscooolTrue(int index);
void Flamehit(bool f);
bool Get_flamehit();