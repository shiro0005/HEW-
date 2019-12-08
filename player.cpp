
#include "player.h"
#include "main.h"
#include "input.h"
#include "sprite.h"
#include "table.h"
#include "bullet.h"
#include "blade.h"
#include "collision.h"
#include "math.h"
#include <time.h>
#include "2D.h"


#define PLAYER_WIDTH	(32)
#define PLAYER_HEIGHT	(32)

#define ANIME_PATTERN_MAX		3
#define ANIME_PATTERN_SKIPFRAME 8



typedef struct
{
	D3DXVECTOR2 vec;//スティックのｘｙ成分
	float angle;//スティックの角度（度）
	bool F[10];//スティック入力フラグ
	bool rote[2];//右回転ならrote[0]がtrue、左回転ならrote[1]がtrue
	int data[2];//Fのフラグのデータを格納する
	int cntdata;//前のフレームと同じ結果の時に加算
}STICK;

//グローバル変数
PLAYER player;
STICK stick;
static int cnt;
static int frame;
static int olddmg;//過去のダメージ
/*
メモ

攻撃に
威力
ベクトル
の数値をつくる

*/




void Player_Initialize(void)
{
	srand((unsigned int)time(NULL));
	//プレイヤーの初期化
	player.pos.x = SCREEN_WIDTH / 2;
	player.pos.y = 430.0f;

	player.color = 0;
	player.muki = 0;
	player.speed = D3DXVECTOR2(0.0f, 0.0f);
	player.collision.s.v.x = 5.0f;
	player.collision.s.v.y = 5.0f;
	player.collision.r = PLAYER_WIDTH * 1.3f;

	//右足
	player.foot[0].r = 5.0f;
	player.foot[0].s.p.x = player.pos.x;
	player.foot[0].s.p.y = player.pos.y;
	player.foot[0].s.v.x = 10;
	player.foot[0].s.v.y = 20;

	//左足
	player.foot[1].r = 10.0f;
	player.foot[1].s.p.x = player.pos.x;
	player.foot[1].s.p.y = player.pos.y;
	player.foot[1].s.v.x = 10;
	player.foot[1].s.v.y = 20;
			
	player.hitpoint = 10;
	olddmg = 10;
	player.firstAT = false;
	cnt = 0;
	frame = 0;
	for (int i = 0; i < 10; i++) {
		stick.F[i] = false;
	}

}

void Player_Finalize(void)
{

}


//プレイヤー移動処理
void Player_Update(void)
{

	player.speed = D3DXVECTOR2(0.0f, 0.0f);

	/*if (Keyboard_IsPress(DIK_UP) || GamePad_IsPress(0, BUTTON_UP)){
		dir.y -= 1.0f;
		player.muki = 3;
	}
	if ((Keyboard_IsPress(DIK_UP) && Keyboard_IsPress(DIK_A)) || GamePad_IsPress(0, BUTTON_UP)&&GamePad_IsPress(0,BUTTON_Y)){
		dir.y -= 10.0f;
		player.muki = 3;
		player.rotate += 0.1f;
	}
	if (Keyboard_IsPress(DIK_DOWN) || GamePad_IsPress(0, BUTTON_DOWN)){
		dir.y += 1.0f;
		player.muki = 0;
	}
	if ((Keyboard_IsPress(DIK_DOWN )&& Keyboard_IsPress(DIK_A)) || GamePad_IsPress(0, BUTTON_DOWN) && GamePad_IsPress(0, BUTTON_Y)) {
		dir.y += 10.0f;
		player.muki = 0;
		player.rotate += 0.1f;
	}
	if (Keyboard_IsPress(DIK_LEFT) || GamePad_IsPress(0, BUTTON_LEFT)){
		dir.x -= 1.0f;
		player.muki = 1;
	}
	if ((Keyboard_IsPress(DIK_LEFT ) && Keyboard_IsPress(DIK_A)) || GamePad_IsPress(0, BUTTON_LEFT) && GamePad_IsPress(0, BUTTON_Y)) {
		dir.x -= 10.0f;
		player.muki = 1;
		player.rotate += 0.1f;
	}
	if (Keyboard_IsPress(DIK_RIGHT) || GamePad_IsPress(0, BUTTON_RIGHT)){
		dir.x += 1.0f;
		player.muki = 2;
	}
	if ((Keyboard_IsPress(DIK_RIGHT) && Keyboard_IsPress(DIK_A)) || GamePad_IsPress(0, BUTTON_RIGHT) && GamePad_IsPress(0, BUTTON_Y)) {
		dir.x += 10.0f;
		player.muki = 2;
		player.rotate += 0.1f;
	}*/

	/*//スペースが押されたら弾を発射
	if (Keyboard_IsTrigger(DIK_Z) || GamePad_IsPress(0, BUTTON_Y) && GamePad_IsTrigger(0, BUTTON_A)) {
		//カーソルキー入力がされていない場合、向きから発射方向を作成する
		if (D3DXVec2Length(&dir) < 0.01f) {
			switch (player.muki)
			{
			case 0://下向き
				dir.y = 1.0f;
				break;
			case 1://左向き
				dir.x = -1.0f;
				break;
			case 2://右向き
				dir.x = 1.0f;
				break;
			case 3://上向き
				dir.y = -1.0f;
				break;
			}
		}
		Bullet_Create(player.pos.x, player.pos.y, dir);
		dir = D3DXVECTOR2(0.0f, 0.0f);
	}

	//ブレイドで攻撃
	if (Keyboard_IsTrigger(DIK_X) || GamePad_IsPress(0, BUTTON_Y) && GamePad_IsTrigger(0, BUTTON_B)) {
		//カーソルキー入力がされていない場合、向きから方向を作成する
		if (D3DXVec2Length(&dir) < 0.01f) {
			switch (player.muki)
			{
			case 0://下向き
				dir.y = 1.0f;
				break;
			case 1://左向き
				dir.x = -1.0f;
				break;
			case 2://右向き
				dir.x = 1.0f;
				break;
			case 3://上向き
				dir.y = -1.0f;
				break;
			}
		}

		Blade_Create(player.pos.x, player.pos.y, dir);
		dir = D3DXVECTOR2(0.0f, 0.0f);
	}*/

	//マップとのヒットチェック
	//Collision_HitCheck_TileMap(player.pos, &player.speed);

	//スティックのX、Y成分を取得
	stick.vec.x = GamePad_Stick().lX;
	stick.vec.y = GamePad_Stick().lY;

	//成分をもとに角度を取得（ラジアン）
	player.rotate = -atan2f(stick.vec.x, stick.vec.y) + D3DX_PI;

	//変数angleに角度（度）を代入
	stick.angle = 180 * player.rotate / D3DX_PI;

	//角度によってF[対応する数]がtrueになる、基本的にアケコンと同じ
	//移動判定もしている
	if (stick.vec == D3DXVECTOR2(0, 0)) {//スティックを入力してないとき
		player.rotate = 0;
		stick.angle = 1000.0f;
		for (int j = 0; j < 10; j++) {
			stick.F[j] = false;
		}
		stick.F[5] = true;
		stick.rote[0] = false;
		stick.rote[1] = false;
	}
	if ((337.5f <= stick.angle && stick.angle <= 360.0f) || (stick.angle >= 0 && stick.angle < 22.5f)) {
		stick.F[8] = true;//入力判定

		//回転判定
		if (stick.F[7]) {
			stick.rote[0] = true;//右回り
			stick.rote[1] = false;
			stick.F[7] = false;//初期化
		}
		else if (stick.F[9]) {
			stick.rote[1] = true;//左回り
			stick.rote[0] = false;
			stick.F[9] = false;
		}
		else {//スティックを回転入力させていない
			for (int i = 1; i < 7; i++) {//F[7]～F[9]まで調べる必要がないため
				if (stick.F[i]) {//回転に関係ないFがtrueなら
					stick.rote[0] = false;//移動させない
					stick.rote[1] = false;
					for (int j = 0; j < 10; j++) {//全てのFを初期化
						stick.F[j] = false;
					}
				}
			}
		}

	}
	else if (22.5f <= stick.angle && stick.angle < 67.5f) {
		stick.F[9] = true;
		if (stick.F[8]) {
			stick.rote[0] = true;
			stick.rote[1] = false;
			stick.F[8] = false;
		}
		else if (stick.F[6]) {
			stick.rote[1] = true;
			stick.rote[0] = false;
			stick.F[6] = false;
		}
		else {
			for (int i = 1; i < 8; i++) {
				if (i == 6) {//効率
					i++;
				}
				if (stick.F[i]) {
					stick.rote[0] = false;
					stick.rote[1] = false;
					for (int j = 0; j < 10; j++) {
						stick.F[j] = false;
					}
				}
			}
		}
	}
	else if (67.5f <= stick.angle && stick.angle < 112.5f) {
		stick.F[6] = true;
		if (stick.F[9]) {
			stick.rote[0] = true;
			stick.rote[1] = false;
			stick.F[9] = false;
		}
		else if (stick.F[3]) {
			stick.rote[1] = true;
			stick.rote[0] = false;
			stick.F[3] = false;
		}
		else {
			for (int i = 1; i < 9; i++) {
				if (i == 6) {//これをしないと毎回ここに該当するので処理がおかしくなる
					i++;
				}
				if (stick.F[i]) {
					stick.rote[0] = false;
					stick.rote[1] = false;
					for (int j = 0; j < 10; j++) {
						stick.F[j] = false;
					}
				}
			}
		}
	}
	else if (112.5f <= stick.angle && stick.angle < 157.5f) {
		stick.F[3] = true;
		if (stick.F[6]) {
			stick.rote[0] = true;
			stick.rote[1] = false;
			stick.F[6] = false;
		}
		else if (stick.F[2]) {
			stick.rote[1] = true;
			stick.rote[0] = false;
			stick.F[2] = false;
		}
		else {
			for (int i = 1; i < 10; i++) {
				if (i == 3) {
					i++;
				}
				if (stick.F[i]) {
					stick.rote[0] = false;
					stick.rote[1] = false;
					for (int j = 0; j < 10; j++) {
						stick.F[j] = false;
					}
				}
			}
		}
	}
	else if (157.5f <= stick.angle && stick.angle < 202.5f) {
		stick.F[2] = true;
		if (stick.F[3]) {
			stick.rote[0] = true;
			stick.rote[1] = false;
			stick.F[3] = false;
		}
		else if (stick.F[1]) {
			stick.rote[1] = true;
			stick.rote[0] = false;
			stick.F[1] = false;
		}
		else {
			for (int i = 4; i < 10; i++) {
				if (stick.F[i]) {
					stick.rote[0] = false;
					stick.rote[1] = false;
					for (int j = 0; j < 10; j++) {
						stick.F[j] = false;
					}
				}
			}
		}
	}
	else if (202.5f <= stick.angle && stick.angle < 247.5f) {
		stick.F[1] = true;
		if (stick.F[2]) {
			stick.rote[0] = true;
			stick.rote[1] = false;
			stick.F[2] = false;
		}
		else if (stick.F[4]) {
			stick.rote[1] = true;
			stick.rote[0] = false;
			stick.F[4] = false;
		}
		else {
			for (int i = 3; i < 10; i++) {
				if (stick.F[i]) {
					stick.rote[0] = false;
					stick.rote[1] = false;
					for (int j = 0; j < 10; j++) {
						stick.F[j] = false;
					}
				}
			}
		}
	}
	else if (247.5f <= stick.angle && stick.angle < 292.5f) {
		stick.F[4] = true;
		if (stick.F[1]) {
			stick.rote[0] = true;
			stick.rote[1] = false;
			stick.F[1] = false;
		}
		else if (stick.F[7]) {
			stick.rote[1] = true;
			stick.rote[0] = false;
			stick.F[7] = false;
		}
		else {
			for (int i = 1; i < 10; i++) {
				if (i == 4) {
					i++;
				}
				if (stick.F[i]) {
					stick.rote[0] = false;
					stick.rote[1] = false;
					for (int j = 0; j < 10; j++) {
						stick.F[j] = false;
					}
				}
			}
		}
	}
	else if (292.5f <= stick.angle && stick.angle < 337.5f) {
		stick.F[7] = true;
		if (stick.F[4]) {
			stick.rote[0] = true;
			stick.rote[1] = false;
			stick.F[4] = false;
		}
		else if (stick.F[8]) {
			stick.rote[1] = true;
			stick.rote[0] = false;
			stick.F[8] = false;
		}
		else {
			for (int i = 1; i < 10; i++) {
				if (i == 7) {
					i++;
				}
				if (stick.F[i]) {
					stick.rote[0] = false;
					stick.rote[1] = false;
					for (int j = 0; j < 10; j++) {
						stick.F[j] = false;
					}
				}
			}
		}
	}

	for (int i = 0; i < 10; i++) {
		if (stick.F[i]) {//フラグがtrueになっているところを格納
			stick.data[cnt] = i;
		}
	}

	if (stick.data[0] == stick.data[1]) {//前のフレームと同じところがtrueなら
		stick.cntdata++;//フラグが同じだった回数（フレーム数）
		if (stick.cntdata >= 13) {//13フレーム同じだった

			stick.rote[0] = false;//回転
			stick.rote[1] = false;

			for (int i = 0; i < 10; i++) {
				stick.F[i] = false;//全てのF[]を初期化
			}
			player.rotate = 0;//プレイヤーの角度を0に設定

		}
	}
	else {
		stick.cntdata = 0;//初期化
	}

	cnt++;//フレームカウント
	if (cnt == 2) {
		cnt = 0;//初期化
	}



	//移動値設定
	if (stick.rote[0]) {
		player.speed.x = 5.0f;
	}
	else if (stick.rote[1]) {
		player.speed.x = -5.0f;
	}
	else {
		player.speed.x = 0;
	}

	//初動攻撃
	if (stick.rote[0] || stick.rote[1]) {//回転中なら
		if (GamePad_IsPress(0, BUTTON_C)) {
			player.firstAT = true;
		}
	}

	if (player.firstAT) {
		frame++;
		if (frame % 5 == 0) {
			player.animePattern++;
		}
		if (player.animePattern == 8) {
			player.animePattern = 0;
			player.firstAT = false;
		}
	}


	if (player.hitpoint!=olddmg) {
		player.mode = 1;
	}
	if (player.mode == 1) {
		frame++;
		if (frame % 5 == 0) {
			player.animePattern++;
		}
		if (player.animePattern == 8) {
			player.animePattern = 0;
			player.mode = 0;
		}
	}


	// 座標の更新処理
	//player.pos += player.speed;

	//当たり判定用座標の更新

	//体全体の当たり判定
	player.collision.r = PLAYER_WIDTH * 1.3f;
	player.collision.s.p.x = (player.pos.x) + 50.0f*cos(D3DX_PI / 2 - player.rotate);
	player.collision.s.p.y = (player.pos.y) - 50.0f*sin(D3DX_PI / 2 - player.rotate);
	player.collision.s.v.x = 100.0f*cos(D3DX_PI / 2 + player.rotate);
	player.collision.s.v.y = 100.0f*sin(D3DX_PI / 2 + player.rotate);

	//右足
	player.foot[0].s.p.x = player.pos.x;
	player.foot[0].s.p.y = player.pos.y;
	player.foot[0].s.v.x = 60.0f*cos(D3DX_PI / 2 + player.rotate-0.7f);
	player.foot[0].s.v.y = 60.0f*sin(D3DX_PI / 2 + player.rotate-0.7f);

	//左足
	player.foot[1].s.p.x = player.pos.x+10.0f;
	player.foot[1].s.p.y = player.pos.y+15.0f;
	player.foot[1].s.v.x = 70.0f*cos(D3DX_PI / 2 + player.rotate + 0.87f);
	player.foot[1].s.v.y = 70.0f*sin(D3DX_PI / 2 + player.rotate + 0.87f);

	//player.rotate = 0;
	olddmg = player.hitpoint;
	////スキップするフレーム値を超えたら
	//if (++player.animeFrame > ANIME_PATTERN_SKIPFRAME)
	//{
	//	//アニメパターンを進める(最大値を超えたらリセット)
	//	if (++player.animePattern >= ANIME_PATTERN_MAX)
	//		player.animePattern = 0;
	//	//フレームは元に戻す
	//	player.animeFrame = 0;
	//}

}

void Player_Draw(void)
{
	Sprite_Draw(TEXTURE_INDEX_YUKIDARUMA,
		player.pos.x,
		player.pos.y,
		GetAnimTbl(player.mode, player.animePattern).x * 256,
		GetAnimTbl(player.mode, player.animePattern).y * 256,
		32,
		32,
		16,
		16,
		4.0f,
		6.0f,
		player.rotate);
}

//const Capsule2D* Player_GetCollision()
//{
//	return &player.collision;
//}
//const Capsule2D* Player_GetCollisionFR()
//{
//	return &player.foot[0];
//}
//const Capsule2D* Player_GetCollisionFL()
//{
//	return &player.foot[1];
//}
//float Player_Getdir() {
//	return player.speed.x;
//}
//
//bool GetfirstAT() {
//	return player.firstAT;
//}
//
//int Player_GetHitPoint()
//{
//	return player.hitpoint;
//}

void Player_AddDamage(int damage)
{
	player.hitpoint -= damage;
	if (player.hitpoint < 0)
		player.hitpoint = 0;
}

PLAYER GetPlayer() {
	return player;
}

