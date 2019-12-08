#pragma once

#include "collision.h"
#include "2D.h"

void Player_Initialize(void);
void Player_Finalize(void);
void Player_Update(void);
void Player_Draw(void);
const Capsule2D* Player_GetCollision();
const Capsule2D* Player_GetCollisionFR();
const Capsule2D* Player_GetCollisionFL();

bool GetfirstAT();
void Player_AddDamage(int damage);
float Player_Getdir();
int Player_GetHitPoint();
