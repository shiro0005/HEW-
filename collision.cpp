
#include "game.h"
#include "collision.h"
#include "player.h"
#include "enemy.h"
#include "boss.h"
#include "enemy_hand.h"
#include "bossbullet.h"
#include "bosslaser.h"
#include "bossflame.h"
#include "bossthunder.h"
#include "explosion.h"
#include "explosion2.h"
#include "scene.h"
#include "tilemap.h"
#include "2D.h"
#include <d3dx9.h>
#include "stage.h"
#include "game.h"
#include "sound.h"

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
void Collision_Bosslaser_vs_Player(void);
void Collision_Bossflame_vs_Player(void);
void Collision_Bossthunder_vs_Player(void);
void Collision_Boss_hand_vs_Player(void);
static bool HitCircle(const CIRCLE* pCircle_a, const CIRCLE* pCircle_b);
void Collision_Player_vs_Enemy(void);
void Collision_Player_vs_Boss(void);




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
	t1 = (DV1V2 * l2.v.dot(P21P11) - DV2V2 * l1.v.dot(P21P11)) / ((DV1V1 * DV2V2 - DV1V2 * DV1V2) + 1);
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
	Collision_Bosslaser_vs_Player();
	Collision_Bossflame_vs_Player();
	Collision_Bossthunder_vs_Player();
	Collision_Boss_hand_vs_Player();
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
		
		if (!enemy.spawn) {
			continue;
		}

		if (!player.com == 0) {
			switch (player.com)
			{
			case 1:
				if (player.muki == 1) {
					if (HitCupsule(Enemy_GetCollision(i), player.first[0]))//初動
					{
						if (enemy.damagecol == true)
						{
							player.karacombo = false;
							player.commbo += 1;
							player.comboflame = 0;
							Game_AddScore(200 + (player.commbo * 20));
							// 当たってる
							enemy.hp -= 1;
							Enemy_AddDamage(i);
							enemy.damagecol = false;
							enemy.rot = 0.0f;

							player.leftstop = false;
							player.stop = false;

							//se
							if (player.pos.x >= enemy.pos.x) {//敵が左
								Explosion_Create(enemy.pos.x + 50.0f, enemy.pos.y, 1.75f);
									PlaySound(SOUND_LABEL_SE_SHORT_PUNCH);
							}
							else
							{
								Explosion_Create(enemy.pos.x - 50.0f, enemy.pos.y, -1.75f);
									PlaySound(SOUND_LABEL_SE_SHORT_PUNCH);
							}
						}
					}
				}
				else if (player.muki == 0) {
					if (HitCupsule(Enemy_GetCollision(i), player.first[1]))//初動
					{
						if (enemy.damagecol == true)
						{

							player.karacombo = false;
							player.commbo += 1;
							player.comboflame = 0;
							Game_AddScore(200 + (player.commbo * 20));
							// 当たってる
							enemy.hp -= 1;
							Enemy_AddDamage(i);
							enemy.damagecol = false;
							enemy.rot = 0.0f;

							player.leftstop = false;
							player.stop = false;

							//se

							if (player.pos.x >= enemy.pos.x) {//敵が左
								Explosion_Create(enemy.pos.x + 50.0f, enemy.pos.y, 1.75f);
									PlaySound(SOUND_LABEL_SE_SHORT_PUNCH);
							}
							else
							{
								Explosion_Create(enemy.pos.x - 50.0f, enemy.pos.y, -1.75f);
									PlaySound(SOUND_LABEL_SE_SHORT_PUNCH);
							}



						}
					}
				}
				break;

			case 2:

				if (HitCupsule(Enemy_GetCollision(i), player.UpCol))//上攻撃
				{
					if (enemy.damagecol == true)
					{
						player.karacombo = false;
						player.commbo += 1;
						player.comboflame = 0;
						Game_AddScore(200 + (player.commbo * 20));
						// 当たってる
						enemy.hp -= 1;
						Enemy_AddDamage(i);
						enemy.damagecol = false;
						enemy.korobu = true;

						player.leftstop = false;
						player.stop = false;

						//se
						enemy.nock = TRUE;
						if (enemy.hp >= 1) {
							enemy.t = 0;
						}

						Explosion_Create2(enemy.pos.x, enemy.pos.y);
							PlaySound(SOUND_LABEL_SE_SHORT_PUNCH);

					}
				}
				break;
			case 3:
				if (HitCupsule(Enemy_GetCollision(i), player.DownCol))//下攻撃
				{
					if (enemy.damagecol == true)
					{
						player.karacombo = false;
						player.commbo += 1;
						player.comboflame = 0;
						Game_AddScore(200 + (player.commbo * 20));
						// 当たってる
						enemy.hp -= 1;
						Enemy_AddDamage(i);
						enemy.damagecol = false;
						enemy.rot = 0.0f;

						player.leftstop = false;
						player.stop = false;

						//se

						enemy.nock = TRUE;
						if (enemy.hp >= 1) {
							enemy.t = 0;
						}

						Explosion_Create(enemy.pos.x + 50.0f, enemy.pos.y, 1.75f);			
							PlaySound(SOUND_LABEL_SE_SHORT_PUNCH);

					}
				}
				break;
			case 4:
				if (player.slidemuki == 0) {
					if (HitCupsule(Enemy_GetCollision(i), player.SlideCol[0]))//スライディング
					{
						if (enemy.damagecol == true)
						{
							player.karacombo = false;
							player.commbo += 1;
							player.comboflame = 0;
							Game_AddScore(200 + (player.commbo * 20));
							// 当たってる
							enemy.hp -= 1;
							Enemy_AddDamage(i);
							enemy.damagecol = false;
							enemy.rot = 0.0f;

							player.leftstop = false;
							player.stop = false;

							//se
							enemy.nock = TRUE;
							if (enemy.hp >= 1) {
								enemy.t = 0;
							}

							Explosion_Create2(enemy.pos.x, enemy.pos.y);
								PlaySound(SOUND_LABEL_SE_SHORT_PUNCH);

						}
					}
				}
				else if (player.slidemuki == 1) {
					if (HitCupsule(Enemy_GetCollision(i), player.SlideCol[1]))//初動
					{
						if (enemy.damagecol == true)
						{
							player.karacombo = false;
							player.commbo += 1;
							player.comboflame = 0;
							Game_AddScore(200 + (player.commbo * 20));
							// 当たってる
							enemy.hp -= 1;
							Enemy_AddDamage(i);
							enemy.damagecol = false;
							enemy.rot = 0.0f;

							player.leftstop = false;
							player.stop = false;

							//se
							enemy.nock = TRUE;
							if (enemy.hp >= 1) {
								enemy.t = 0;
							}

							Explosion_Create2(enemy.pos.x, enemy.pos.y);
								PlaySound(SOUND_LABEL_SE_SHORT_PUNCH);
						}
					}
				}

				break;

			default:
				break;
			}
		}
		else {
			if (HitCupsule(Enemy_GetCollision(i), player.collision))
			{

				if (player.pos.x >= enemy.pos.x) {//敵が左
					player.leftstop = true;
					enemy.enemystop = true;
				}
				else {//敵が右
					player.stop = true;
					enemy.enemyleftstop = true;
				}
			}

		}
		EnemyInfoMatch(enemy, i);
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
				//Player_AddDamage(1);
				player.hitpoint -= 1;
				player.color = 0xffff0000;
				if (player.pos.x < GetHundPos(i)) {
					Explosion_Create5(90.0f, player.pos.y,0.0f);
				}
				if (player.pos.x >= GetHundPos(i)) {
					Explosion_Create5(-90.0f, player.pos.y, 3.0f);
				}
			}

			Bullet_IscoolFlase(i);
		}
	}
}

void Collision_Boss_hand_vs_Player(void)
{
	for (int i = 0; i < BOSSBULLET_MAX; i++)
	{
		// 弾は有効か？
		if (!BossBullet_IsEnable(i))
			continue;

		if (BossBullet_Iscool(i) == true)
		{
			// 弾のコリジョンとエネミーのコリジョン
			if (HitCupsule(player.collision, BossBullet_GetCollision(i)))
			{
				// 当たってる
				//Player_AddDamage(1);
				player.hitpoint -= 1;
				player.color = 0xffff0000;
				//Explosion_Create5(player.pos.x, player.pos.y);
				if (player.pos.x < GetHandPos(i)) {
					Explosion_Create5(90.0f, player.pos.y, 0.0f);
				}
				if (player.pos.x >= GetHandPos(i)) {
					Explosion_Create5(-90.0f, player.pos.y, 3.0f);
				}
			}

			BossBullet_IscoolFlase(i);
		}
	}
}

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

void Collision_Player_vs_Boss(void)
{
	for (int i = 0; i < BOSS_COUNT; i++) {
		boss = GetBoss(i);
		// エネミーは有効か？
		if (!Boss_IsEnable(i)) {
			continue;
		}

		if (!player.com == 0) {
			switch (player.com)
			{
			case 1:
				if (player.muki == 1) {
					if (HitCupsule(Boss_GetCollision(i), player.first[0]))//初動
					{
						if (player.attackcol == true)
						{
							player.karacombo = false;
							player.commbo += 1;
							player.comboflame = 0;
							Game_AddScore(200 + (player.commbo * 20));
							// 当たってる
							boss.hp -= 1;
							Boss_AddDamage(i);
							player.attackcol = false;

							player.leftstop = false;
							player.stop = false;

							//se
							if (player.pos.x >= boss.pos.x) {//敵が左
								Explosion_Create(boss.pos.x + 50.0f, boss.pos.y, 1.75f);
								PlaySound(SOUND_LABEL_SE_SHORT_PUNCH);
							}
							else
							{
								Explosion_Create(boss.pos.x - 50.0f, boss.pos.y, -1.75f);
								PlaySound(SOUND_LABEL_SE_SHORT_PUNCH);
							}
						}
					}
				}
				else if (player.muki == 0) {
					if (HitCupsule(Boss_GetCollision(i), player.first[1]))//初動
					{
						if (player.attackcol == true)
						{

							player.karacombo = false;
							player.commbo += 1;
							player.comboflame = 0;
							Game_AddScore(200 + (player.commbo * 20));
							// 当たってる
							boss.hp -= 1;
							Boss_AddDamage(i);
							player.attackcol = false;

							player.leftstop = false;
							player.stop = false;

							//se

							if (player.pos.x >= enemy.pos.x) {//敵が左
								Explosion_Create(boss.pos.x + 50.0f, boss.pos.y, 1.75f);
								PlaySound(SOUND_LABEL_SE_SHORT_PUNCH);
							}
							else
							{
								Explosion_Create(boss.pos.x - 50.0f, boss.pos.y, -1.75f);
								PlaySound(SOUND_LABEL_SE_SHORT_PUNCH);
							}



						}
					}
				}
				break;

			case 2:

				if (HitCupsule(Boss_GetCollision(i), player.UpCol))//上攻撃
				{
					if (player.attackcol == true)
					{
						player.karacombo = false;
						player.commbo += 1;
						player.comboflame = 0;
						Game_AddScore(200 + (player.commbo * 20));
						// 当たってる
						boss.hp -= 1;
						Boss_AddDamage(i);
						player.attackcol = false;

						player.leftstop = false;
						player.stop = false;

						Explosion_Create2(boss.pos.x, boss.pos.y);
						PlaySound(SOUND_LABEL_SE_SHORT_PUNCH);

					}
				}
				break;
			case 3:
				if (HitCupsule(Boss_GetCollision(i), player.DownCol))//下攻撃
				{
					if (player.attackcol == true)
					{
						player.karacombo = false;
						player.commbo += 1;
						player.comboflame = 0;
						Game_AddScore(200 + (player.commbo * 20));
						// 当たってる
						boss.hp -= 1;
						Boss_AddDamage(i);
						player.attackcol = false;

						player.leftstop = false;
						player.stop = false;

						Explosion_Create(boss.pos.x + 50.0f, boss.pos.y, 1.75f);
						PlaySound(SOUND_LABEL_SE_SHORT_PUNCH);

					}
				}
				break;
			case 4:
				if (player.slidemuki == 0) {
					if (HitCupsule(Boss_GetCollision(i), player.SlideCol[0]))//スライディング
					{
						if (player.attackcol == true)
						{
							player.karacombo = false;
							player.commbo += 1;
							player.comboflame = 0;
							Game_AddScore(200 + (player.commbo * 20));
							// 当たってる
							boss.hp -= 1;
							Boss_AddDamage(i);
							player.attackcol = false;

							player.leftstop = false;
							player.stop = false;

							Explosion_Create2(boss.pos.x, boss.pos.y);
							PlaySound(SOUND_LABEL_SE_SHORT_PUNCH);

						}
					}
				}
				else if (player.slidemuki == 1) {
					if (HitCupsule(Boss_GetCollision(i), player.SlideCol[1]))//初動
					{
						if (player.attackcol == true)
						{
							player.karacombo = false;
							player.commbo += 1;
							player.comboflame = 0;
							Game_AddScore(200 + (player.commbo * 20));
							// 当たってる
							boss.hp -= 1;
							Boss_AddDamage(i);
							player.attackcol = false;

							player.leftstop = false;
							player.stop = false;

							Explosion_Create2(boss.pos.x, boss.pos.y);
							PlaySound(SOUND_LABEL_SE_SHORT_PUNCH);

						}
					}
				}

				break;

			default:
				break;
			}
		}
		else {
			if (HitCupsule(Boss_GetCollision(i), player.collision))
			{

				if (player.pos.x >= boss.pos.x) {//敵が左
					player.leftstop = true;
					boss.bossstop = true;
				}
				else {//敵が右
					player.stop = true;
					boss.bossleftstop = true;
				}
			}

		}
		BossInfoMatch(boss, i);
	}
}

void Collision_Bosslaser_vs_Player(void)
{
	for (int i = 0; i < BOSSLASER_MAX; i++)
	{
		// 弾は有効か？
		if (!BossLaser_IsEnable(i))
			continue;

		if (BossLaser_Iscoool(0) == true)
		{
			// 弾のコリジョンとエネミーのコリジョン
			if (HitCupsule(player.collision, BossLaser_GetCollision(0)) && !Get_laserhit())
			{
				Laserhit(true);
				// 当たってる
				player.hitpoint -= 3;
				player.color = 0xffff0000;
			}


		}
	}
}

void Collision_Bossflame_vs_Player(void)
{
	for (int i = 0; i < BOSSFLAME_MAX; i++)
	{
		// 弾は有効か？
		if (!BossFlame_IsEnable(i))
			continue;

		if (BossFlame_Iscoool(i) == true)
		{
			// 弾のコリジョンとエネミーのコリジョン
			if (HitCupsule(player.collision, BossFlame_GetCollision(0)) && !Get_flamehit())
			{
				Flamehit(true);
				// 当たってる
				player.hitpoint -= 2;

			}
		}
	}
}

void Collision_Bossthunder_vs_Player(void)
{
	for (int i = 0; i < BOSSTHUNDER_MAX; i++)
	{
		// 弾は有効か？
		if (!BossThunder_IsEnable(i))
			continue;

		if (BossThunder_Iscoool(i) == true)
		{
			// 弾のコリジョンとエネミーのコリジョン
			if (HitCupsule(player.collision, BossThunder_GetCollision(0)) && !Get_Thunderhit())
			{
				thunderhit(true);
				// 当たってる
				player.hitpoint -= 1;

			}

		}
	}
}