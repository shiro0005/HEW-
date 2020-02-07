#pragma once

#include "collision.h"
#include <d3dx9.h>
#include "2D.h"

//íeÇÃç≈ëÂï\é¶êî
#define BULLET_MAX (128)


void Bullet_Initialize(void);
void Bullet_Update(void);
void Bullet_Draw(void);

void Bullet_Create(int muki,float x, float y, D3DXVECTOR2 dir);

void Bullet_Destroy(int index);
bool Bullet_IsEnable(int index);
const Capsule2D* Bullet_GetCollision(int index);
bool Bullet_Iscool(int index);
void Bullet_IscoolFlase(int index);
void Bullet_IscoolTrue(int index);
float GetHundPos(int i);