#pragma once

void Game_Initialize(void);
void Game_Finalize(void);
void Game_Update(void);
void Game_Draw(void);

void Game_AddScore(int score);
void Game_AddKillCount(void);
void Game_AddKillBossCount(void);
int GetScore();
bool GetGamend();
//int Game_GetScore();