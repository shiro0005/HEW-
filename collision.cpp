#include "game.h"
#include "collision.h"
#include "player.h"
#include "enemy.h"
#include "boss.h"
#include "bullet.h"
#include "enemy_hand.h"
#include "enemybullet.h"
#include "enemylaser.h"
#include "bossbullet.h"
#include "bosslaser.h"
#include "explosion.h"
#include "scene.h"
#include "tilemap.h"
#include "2D.h"
#include <d3dx9.h>
#include "stage.h"
#include "game.h"

//グローバル変数
static PLAYER player;
static ENEMY_DATA enemy;
static BOSS_DATA boss;

//プロトタイプ宣言
bool HitCupsule(const CIRCLE* p_circle, const CUPSULE* p_cupsule);
bool HitCupsule(const Capsule2D* c1, const Capsule2D* c2);
bool HitCupsule(const Capsule2D c1, const Capsule2D* c2);
bool HitCupsule(const Capsule2D* c1, const Capsule2D c2);
void Collision_Player_vs_Enemy(void);
void Collision_Enemy_hand_vs_Player(void);
static bool HitCircle(const CIRCLE* pCircle_a, const CIRCLE* pCircle_b);
void Collision_Player_vs_Enemy(void);
void Collision_Player_vs_Boss(void);
//void Collision_Bullet_vs_Enemy(void);
//void Collision_Player_vs_EnemyBullet(void);
//void Collision_Player_vs_EnemyLaser(void);
//void Collision_Player_vs_Boss(void);
//void Collision_Bullet_vs_Boss(void);
//void Collision_Player_vs_BossBullet(void);
//void Collision_Player_vs_BossLaser(void);
//



bool HitCircle(const CIRCLE* p_circle_a, const CIRCLE* p_circle_b)
{
	//引数からベクトル型の変数を作る
	D3DXVECTOR2 dst1(p_circle_a->cx, p_circle_a->cy);
	D3DXVECTOR2 dst2(p_circle_b->cx, p_circle_b->cy);
	//二点間のベクトルを作る
	D3DXVECTOR2 distance = dst2 - dst1;
	//作ったベクトルの長さを求める
	float length = D3DXVec2Length(&distance);
	//お互いの半径を足した値を求める
	float size = p_circle_a->r + p_circle_b->r;

	//ベクトルの長さとお互いの半径を足した値を比較する
	//→ベクトルの長さの方が小さければヒットしている！
	if (length < size) {
		return true;
	}
	//ヒットしていなかった
	return false;
}

/*
bool HitCircle(const CIRCLE* p_circle_a, const CIRCLE* p_circle_b)
{
//引数からベクトル型の変数を作る
D3DXVECTOR2 dst1(p_circle_a->cx, p_circle_a->cy);
D3DXVECTOR2 dst2(p_circle_b->cx, p_circle_b->cy);
//二点間のベクトルを作る
D3DXVECTOR2 distance = dst2 - dst1;
//作ったベクトルの長さを求める
float length = D3DXVec2LengthSq(&distance);
//お互いの半径を足した値を求める
float size = (p_circle_a->r + p_circle_b->r) * (p_circle_a->r + p_circle_b->r);

//ベクトルの長さとお互いの半径を足した値を比較する
//→ベクトルの長さの方が小さければヒットしている！
if (length < (p_circle_a->r + p_circle_b->r)){
return true;
}
//ヒットしていなかった
return false;
}
*/

bool HitCupsule(const CIRCLE* p_circle, const CUPSULE* p_cupsule)
{
	//円とカプセルの位置
	D3DXVECTOR2 cir_pos = D3DXVECTOR2(p_circle->cx, p_circle->cy);
	D3DXVECTOR2 cup_pos = D3DXVECTOR2(p_cupsule->x, p_cupsule->y);
	D3DXVECTOR2 cup_end = D3DXVECTOR2(p_cupsule->ex, p_cupsule->ey);

	D3DXVECTOR2 distance = cir_pos - cup_pos;

	float t = 0.0f;

	//交点を求める
	t = (cup_end.x * distance.x + cup_end.y * distance.y) /
		(cup_end.x * cup_end.x + cup_end.y * cup_end.y);

	if (t < 0.0f)
		t = 0.0f;
	if (t > 1.0f)
		t = 1.0f;

	D3DXVECTOR2 pos_cross;
	pos_cross.x = (cup_end.x * t) + cup_pos.x;
	pos_cross.y = (cup_end.y * t) + cup_pos.y;

	//交点までの長さとお互いの半径を足したものの大きさを比べる
	float cross_len = (pos_cross.x - cir_pos.x) * (pos_cross.x - cir_pos.x) +
		(pos_cross.y - cir_pos.y) * (pos_cross.y - cir_pos.y);

	float size = (p_circle->r + p_cupsule->r);

	//→ベクトルの長さの方が小さければヒットしている！
	if (cross_len < size * size)
	{
		return true;
	}

	//ヒットしていなかった
	return false;
}


bool isSharpAngle(const Point2D &p1, const Point2D &p2, const Point2D &p3) {
	return (p1 - p2).dot(p3 - p2) >= 0.0f;
}

float calcPointLineDist2D(const Point2D &p, const Line2D &l, Point2D &h, float &t) {
	float lenSqV = l.v.lengthSq();
	t = 0.0f;
	if (lenSqV > 0.0f)
		t = l.v.dot(p - l.p) / lenSqV;
	h = l.p + t * l.v;
	return (h - p).length();
}

float calcLineLineDist2D(const Line2D &l1, const Line2D &l2, Point2D &p1, Point2D &p2, float &t1, float &t2) {

	// 2直線が平行？
	if (l1.v.isParallel(l2.v) == true) {

		// 点P11と直線L2の最短距離の問題に帰着
		float len = calcPointLineDist2D(l1.p, l2, p2, t2);
		p1 = l1.p;
		t1 = 0.0f;

		return len;
	}

	// 2直線はねじれ関係
	float DV1V2 = l1.v.dot(l2.v);
	float DV1V1 = l1.v.lengthSq();
	float DV2V2 = l2.v.lengthSq();
	Vec2 P21P11 = l1.p - l2.p;
	t1 = (DV1V2 * l2.v.dot(P21P11) - DV2V2 * l1.v.dot(P21P11)) / (DV1V1 * DV2V2 - DV1V2 * DV1V2);
	p1 = l1.getPoint(t1);
	t2 = l2.v.dot(p1 - l2.p) / DV2V2;
	p2 = l2.getPoint(t2);

	return (p2 - p1).length();
}

float calcPointSegmentDist2D(const Point2D &p, const Segment2D &seg, Point2D &h, float &t) {

	const Point2D e = seg.getEndPoint();

	// 垂線の長さ、垂線の足の座標及びtを算出
	float len = calcPointLineDist2D(p, seg, h, t);

	if (isSharpAngle(p, seg.p, e) == false) {
		// 始点側の外側
		h = seg.p;
		return (seg.p - p).length();
	}
	else if (isSharpAngle(p, e, seg.p) == false) {
		// 終点側の外側
		h = e;
		return (e - p).length();
	}

	return len;
}


// 0～1の間にクランプ
void clamp01(float &v) {
	if (v < 0.0f)
		v = 0.0f;
	else if (v > 1.0f)
		v = 1.0f;
}


float calcSegmentSegmentDist2D(const Segment2D &s1, const Segment2D &s2,
	Point2D &p1, Point2D &p2, float &t1, float &t2)
{
	// 2線分の最短距離（2D版）
	// s1 : S1(線分1)
	// s2 : S2(線分2)
	// p1 : S1側の垂線の足（戻り値）
	// p2 : S2側の垂線の足（戻り値）
	// t1 : S1側のベクトル係数（戻り値）
	// t2 : S2側のベクトル係数（戻り値）
	// 戻り値: 最短距離


		// S1が縮退している？
	if (s1.v.lengthSq() < _OX_EPSILON_) {
		// S2も縮退？
		if (s2.v.lengthSq() < _OX_EPSILON_) {
			// 点と点の距離の問題に帰着
			float len = (s2.p - s1.p).length();
			p1 = s1.p;
			p2 = s2.p;
			t1 = t2 = 0.0f;
			return len;
		}
		else {
			// S1の始点とS2の最短問題に帰着
			float len = calcPointSegmentDist2D(s1.p, s2, p2, t2);
			p1 = s1.p;
			t1 = 0.0f;
			clamp01(t2);
			return len;
		}
	}

	// S2が縮退している？
	else if (s2.v.lengthSq() < _OX_EPSILON_) {
		// S2の始点とS1の最短問題に帰着
		float len = calcPointSegmentDist2D(s2.p, s1, p1, t1);
		p2 = s2.p;
		clamp01(t1);
		t2 = 0.0f;
		return len;
	}

	/* 線分同士 */

	// 2線分が平行だったら垂線の端点の一つをP1に仮決定
	if (s1.v.isParallel(s2.v) == true) {
		t1 = 0.0f;
		p1 = s1.p;
		float len = calcPointSegmentDist2D(s1.p, s2, p2, t2);
		if (0.0f <= t2 && t2 <= 1.0f)
			return len;
	}
	else {
		// 線分はねじれの関係
		// 2直線間の最短距離を求めて仮のt1,t2を求める
		float len = calcLineLineDist2D(s1, s2, p1, p2, t1, t2);
		if (
			0.0f <= t1 && t1 <= 1.0f &&
			0.0f <= t2 && t2 <= 1.0f
			) {
			return len;
		}
	}

	// 垂線の足が外にある事が判明
	// S1側のt1を0～1の間にクランプして垂線を降ろす
	clamp01(t1);
	p1 = s1.getPoint(t1);
	float len = calcPointSegmentDist2D(p1, s2, p2, t2);
	if (0.0f <= t2 && t2 <= 1.0f)
		return len;

	// S2側が外だったのでS2側をクランプ、S1に垂線を降ろす
	clamp01(t2);
	p2 = s2.getPoint(t2);
	len = calcPointSegmentDist2D(p2, s1, p1, t1);
	if (0.0f <= t1 && t1 <= 1.0f)
		return len;

	// 双方の端点が最短と判明
	clamp01(t1);
	p1 = s1.getPoint(t1);
	return (p2 - p1).length();


	////カプセルとカプセルの位置
	//D3DXVECTOR2 cup1_pos = D3DXVECTOR2(p_cupsule1->x, p_cupsule1->y);
	//D3DXVECTOR2 cup1_end = D3DXVECTOR2(p_cupsule1->ex, p_cupsule1->ey);
	//D3DXVECTOR2 cup2_pos = D3DXVECTOR2(p_cupsule2->x, p_cupsule2->y);
	//D3DXVECTOR2 cup2_end = D3DXVECTOR2(p_cupsule2->ex, p_cupsule2->ey);
	//D3DXVECTOR2 distance = cup1_pos - cup2_pos;
	//float t = 0.0f;
	////交点を求める
	//t = (cup2_end.x * distance.x + cup2_end.y * distance.y) /
	//	(cup2_end.x * cup2_end.x  + cup2_end.y * cup2_end.y);
	//
	//if (t < 0.0f)
	//	t = 0.0f;
	//if (t > 1.0f)
	//	t = 1.0f;
	//D3DXVECTOR2 pos_cross1;
	//D3DXVECTOR2 pos_cross2;
	//pos_cross1.x = (cup1_end.x * t) + cup1_pos.x;
	//pos_cross1.y = (cup1_end.y * t) + cup1_pos.y;
	//pos_cross2.x = (cup2_end.x * t) + cup2_pos.x;
	//pos_cross2.y = (cup2_end.y * t) + cup2_pos.y;
	////交点までの長さとお互いの半径を足したものの大きさを比べる
	//float cross_len = (pos_cross2.x - pos_cross1.x) * (pos_cross2.x - pos_cross1.x) +
 //               	  (pos_cross2.y - pos_cross1.y) * (pos_cross2.y - pos_cross1.y);
	//float size = (p_cupsule1->r + p_cupsule2->r);
	////→ベクトルの長さの方が小さければヒットしている！
	//if (cross_len < size * size)
	//{
	//	return true;
	//}
	////ヒットしていなかった
	//return false;
}

bool HitCupsule(const Capsule2D* c1, const Capsule2D* c2) {
	Point2D p1, p2;
	float t1, t2;
	float d = calcSegmentSegmentDist2D(c1->s, c2->s, p1, p2, t1, t2);
	return (d <= c1->r + c2->r);
}

bool HitCupsule(const Capsule2D c1, const Capsule2D* c2) {
	Point2D p1, p2;
	float t1, t2;
	float d = calcSegmentSegmentDist2D(c1.s, c2->s, p1, p2, t1, t2);
	return (d <= c1.r + c2->r);
}

bool HitCupsule(const Capsule2D* c1, const Capsule2D c2) {
	Point2D p1, p2;
	float t1, t2;
	float d = calcSegmentSegmentDist2D(c1->s, c2.s, p1, p2, t1, t2);
	return (d <= c1->r + c2.r);
}


void Collision_Initialize(void)
{
}

void Collision_Update(void)
{
	//player構造体の情報を取得
	player = GetPlayer();

	
	Collision_Player_vs_Enemy();
	Collision_Enemy_hand_vs_Player();
	Collision_Player_vs_Boss();
	//Collision_Player_vs_Boss();
	//Collision_Player_vs_BossBullet();
	/*Collision_Bullet_vs_Enemy();
	Collision_Player_vs_EnemyBullet();
	Collision_Player_vs_EnemyLaser();
	Collision_Bullet_vs_Boss();
	Collision_Player_vs_BossBullet();
	Collision_Player_vs_BossLaser();*/

	PlayerInfoMatch(player);
}

void Collision_Player_vs_Enemy(void)
{
	for (int i = 0; i < ENEMY_COUNT; i++) {
		enemy = GetEnemy(i);

		// エネミーは有効か？
		if (!Enemy_IsEnable(i)) {
			continue;
		}

		if (player.com != 0) {
			// プレイヤーのコリジョンとエネミーのコリジョン
			if (HitCupsule(Enemy_GetCollision(i), player.foot[0]))
			{
				if (player.attackcol) {
					player.commbo = 1;
					Game_AddScore(200);
					// 当たってる
					Enemy_NockBack(i);
					Enemy_AddDamage(1, i);
					//当たり判定を消す
					player.attackcol = false;
				}
			}
			if (HitCupsule(Enemy_GetCollision(i), player.foot[1]))
			{
				if (player.attackcol) {
					player.commbo = 1;
					Game_AddScore(200);
					// 当たってる
					Enemy_NockBack(i);
					Enemy_AddDamage(1, i);
					//当たり判定を消す
					player.attackcol = false;
				}
			}
		}
		else {

			if (HitCupsule(Enemy_GetCollision(i), player.collision))
			{

				if (player.pos.x >= enemy.pos.x) {//敵が左
					player.stop[1] = true;
					enemy.stop[0] = true;
				}
				else {//敵が右
					player.stop[0] = true;
					enemy.stop[1] = true;
				}
			}

		}
		
	}
}

void Collision_Player_vs_Boss(void)
{

	for (int i = 0; i < BOSS_COUNT; i++) {

		// ボスは有効か？
		if (!Boss_IsEnable(i)) {
			continue;
		}
		if (player.allAT!=0) {
			// プレイヤーのコリジョンとエネミーのコリジョン
			if (HitCupsule(Boss_GetCollision(i), player.foot[0]))
			{
				if (player.attackcol == true)
				{
					player.commbo = 1;
					Game_AddScore(200);
					// 当たってる
					Boss_AddDamage(i);
					Boss_NockBack(i);
					player.attackcol = false;
				}
			}
			if (HitCupsule(Boss_GetCollision(i), player.foot[1]))
			{
				if (player.attackcol == true)
				{
					player.commbo = 1;
					Game_AddScore(200);
					// 当たってる
					Boss_AddDamage(i);
					Boss_NockBack(i);
					player.attackcol = false;
				}
			}
		}
		else {
			if (HitCupsule(Boss_GetCollision(i), player.collision))
			{

				if (player.pos.x >= boss.pos.x) {//敵が左
					player.stop[1] = true;
					boss.bossstop = true;
				}
				else {//敵が右
					player.stop[0] = true;
					boss.bossleftstop = true;
				}
			}


		}
		// プレイヤーのコリジョンとボスのコリジョン
		//if (HitCupsule(Player_GetCollision(), Boss_GetCollision(i)))
		//{
		//	// 当たってる
		//	Boss_Destroy(i);
		//}
	}
}
void Collision_Enemy_hand_vs_Player(void)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		// 弾は有効か？
		if (!Bullet_IsEnable(i))
			continue;

		if (Bullet_Iscool(i) == true)
		{
			// 弾のコリジョンとエネミーのコリジョン
			if (HitCupsule(player.collision, Bullet_GetCollision(i)))
			{
				// 当たってる
				Player_AddDamage(1);
			}

			Bullet_IscoolFlase(i);
		}
	}
}
/*void Collision_Player_vs_EnemyBullet(void)
{
	for (int i = 0; i < ENEMYBULLET_MAX; i++) {

		// エネミーは有効か？
		if (!EnemyBullet_IsEnable(i)) {
			continue;
		}

		// プレイヤーのコリジョンとエネミーのコリジョン
		if (HitC(Player_GetCollision(), EnemyBullet_GetCollision(i)))
		{
			// 当たってる
			Player_AddDamage(30);

			// 爆発エフェクトの作成
			Explosion_Create(EnemyBullet_GetCollision(i)->cx, EnemyBullet_GetCollision(i)->cy);

			// 弾の消滅処理
			EnemyBullet_Destroy(i);
		}
	}
}

void Collision_Player_vs_EnemyLaser(void)
{
	for (int i = 0; i < ENEMYLASER_MAX; i++) {

		// エネミーは有効か？
		if (!EnemyLaser_IsEnable(i)) {
			continue;
		}

		// プレイヤーのコリジョンとレーザーのコリジョン
		if (HitCupsule(Player_GetCollision(), EnemyLaser_GetCollision(i)))
		{
			// 当たってる
			Player_AddDamage(10);

			// 爆発エフェクトの作成
			Explosion_Create(Player_GetCollision()->cx, Player_GetCollision()->cy);
		}
	}
}

void Collision_Bullet_vs_Enemy(void)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		// 弾は有効か？
		if (!Bullet_IsEnable(i)) {
			continue;
		}

		for (int j = 0; j < ENEMY_COUNT; j++)
		{
			// エネミーは有効か？
			if (!Enemy_IsEnable(j)) {
				continue;
			}

			// 弾のコリジョンとエネミーのコリジョン
			if (HitCircle(Bullet_GetCollision(i), Enemy_GetCollision(j)))
			{
				// 当たってる

				// 得点の追加・敵の消滅カウントの追加


				Game_AddScore(100);
				Game_AddKillCount();

				// 爆発エフェクトの作成
				Explosion_Create(Enemy_GetCollision(j)->cx, Enemy_GetCollision(j)->cy);

				// 敵の消滅処理
				Enemy_Destroy(j);

				// 弾の消滅処理
				Bullet_Destroy(i);

				// この弾の処理は終了
				break;
			}
		}
	}
}*/


bool Collision_HitCheck_TileMap(D3DXVECTOR2 dst, D3DXVECTOR2* pOut)
{
	TILE_DATA tile;
	bool hit = false;

	//横の移動チェック
	tile = GetTileInfo(dst.x + pOut->x, dst.y);
	if (tile.isWalk == 0)
	{
		pOut->x = 0.0f;
		hit = true;
	}

	//縦の移動チェック
	tile = GetTileInfo(dst.x, dst.y + pOut->y);
	//歩けないマップだったらヒット
	if (tile.isWalk == 0)
	{
		pOut->y = 0.0f;
		hit = true;
	}

	return hit;
}

/*void Collision_Player_vs_BossBullet(void)
{
	for (int i = 0; i < BOSSBULLET_MAX; i++) {

		// ボスは有効か？
		if (!BossBullet_IsEnable(i)) {
			continue;
		}

		// プレイヤーのコリジョンとボスのコリジョン
		if (HitCircle(Player_GetCollision(), BossBullet_GetCollision(i)))
		{
			// 当たってる
			Player_AddDamage(30);

			// 爆発エフェクトの作成
			Explosion_Create(BossBullet_GetCollision(i)->cx, BossBullet_GetCollision(i)->cy);

			// 弾の消滅処理
			BossBullet_Destroy(i);
		}
	}
}

void Collision_Player_vs_BossLaser(void)
{
	for (int i = 0; i < BOSSLASER_MAX; i++) {

		// ボスは有効か？
		if (!BossLaser_IsEnable(i)) {
			continue;
		}

		// プレイヤーのコリジョンとレーザーのコリジョン
		if (HitCupsule(Player_GetCollision(), BossLaser_GetCollision(i)))
		{
			// 当たってる
			Player_AddDamage(10);

			// 爆発エフェクトの作成
			Explosion_Create(Player_GetCollision()->cx, Player_GetCollision()->cy);
		}
	}
}

void Collision_Bullet_vs_Boss(void)
{

	int bosslife = 0;
	for (int i = 0; i < BULLET_MAX; i++)
	{

		// 弾は有効か？
		if (!Bullet_IsEnable(i)) {
			continue;
		}

		for (int j = 0; j < BOSS_COUNT; j++)
		{
			// ボスは有効か？
			if (!Boss_IsEnable(j)) {
				continue;
			}

			// 弾のコリジョンとボスのコリジョン
			if (HitCircle(Bullet_GetCollision(i), Boss_GetCollision(j)))
			{
				// 当たってる


				Game_AddScore(100);


				// 得点の追加・敵の消滅カウントの追加

				bosslife = Boss_AddDamage(500);

				if (bosslife <= 0) {
					Game_AddKillBossCount();


					// 爆発エフェクトの作成
					Explosion_Create(Enemy_GetCollision(j)->cx, Boss_GetCollision(j)->cy);

					// 敵の消滅処理
					Boss_Destroy(j);


				}

				// 弾の消滅処理
				Bullet_Destroy(i);


				// この弾の処理は終了
				break;


			}
		}
	}
}*/

//void Collision_Player_vs_BossBullet(void)
//{
//	for (int i = 0; i < BOSSBULLET_MAX; i++)
//	{
//		// 弾は有効か？
//		if (!BossBullet_IsEnable(i))
//			continue;
//
//		if (BossBullet_Iscool(i) == true)
//		{
//			// 弾のコリジョンとエネミーのコリジョン
//			if (HitCupsule(player.collision, BossBullet_GetCollision(i)))
//			{
//				// 当たってる
//				Player_AddDamage(1);
//			}
//
//			BossBullet_IscoolFlase(i);
//		}
//	}
//}