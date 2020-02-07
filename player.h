#pragma once

#include "collision.h"
#include "2D.h"

typedef struct
{
	D3DXVECTOR2 pos;			//位置
	int			muki;			//向き
	int			color;			//色
	D3DXVECTOR2 speed;          //プレイヤー移動
	int			animePattern;	//アニメパターン
	int			mode;           //プレイヤー状態
	float         commbo;			//プレイヤーコンボ数

	int         slidemuki;      //slide攻撃
	Capsule2D   collision;		//カプセルの当たり判定
	Capsule2D   first[2];        //足をカプセルで当たり判定を作る　[0]右　[1]左 
	Capsule2D   UpCol;          //上攻撃当たり判定
	Capsule2D   DownCol;        //下攻撃当たり判定
	Capsule2D   SlideCol[2];       //スライディング当たり判定
	Capsule2D   FCol;

	int			hitpoint;		//ヒットポイント
	float       rotate;			//プレイヤーの角度
	int         state;          //プレイヤーの状態
	bool        stop;           //右停止
	bool        leftstop;       //左停止
	bool		attackcol;		//攻撃の当たり判定を一回読み込む
	bool        camerastop;     //これがtrueの時カメラの移動を止めてステージ固定で戦わせる
	int			com;			//連続で同じ攻撃を出させない変数
	bool		karacombo;				//空撃ちで攻撃リセット
	int			allAT;			//○○ATの代わり
	int			comboflame;		//combocommanduketukejikan
	D3DXVECTOR2		size;		//テクスチャの大きさ
	float		kyori;	//playerの進んだ距離
	int			fase;
}PLAYER;

typedef struct
{
	D3DXVECTOR2 vec;//スティックのｘｙ成分
	float angle;//スティックの角度（度）
	bool F[10];//スティック入力フラグ
	bool rote[2];//右回転ならrote[0]がtrue、左回転ならrote[1]がtrue
	int data[2];//Fのフラグのデータを格納する
	int cntdata;//前のフレームと同じ結果の時に加算
}STICK;

void Player_Initialize(void);
void Player_Finalize(void);
void Player_Update(void);
void Player_Draw(void);
//void Player_AddDamage(int damage);
PLAYER GetPlayer();
void PlayerInfoMatch(PLAYER info);
void ChengeCamerastop(bool f);
void FaseCount(int f);
float Getkyori();
int GetFase();
//const Capsule2D* Player_GetCollision();
//const Capsule2D* Player_GetCollisionFR();
//const Capsule2D* Player_GetCollisionFL();
//bool GetfirstAT();
//float Player_Getdir();
//int Player_GetHitPoint();
