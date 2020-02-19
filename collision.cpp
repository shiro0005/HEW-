
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

//�O���[�o���ϐ�
static PLAYER player;
static ENEMY_DATA enemy;
static BOSS_DATA boss;

//�v���g�^�C�v�錾
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
	//��������x�N�g���^�̕ϐ������
	D3DXVECTOR2 dst1(p_circle_a->cx, p_circle_a->cy);
	D3DXVECTOR2 dst2(p_circle_b->cx, p_circle_b->cy);
	//��_�Ԃ̃x�N�g�������
	D3DXVECTOR2 distance = dst2 - dst1;
	//������x�N�g���̒��������߂�
	float length = D3DXVec2Length(&distance);
	//���݂��̔��a�𑫂����l�����߂�
	float size = p_circle_a->r + p_circle_b->r;

	//�x�N�g���̒����Ƃ��݂��̔��a�𑫂����l���r����
	//���x�N�g���̒����̕�����������΃q�b�g���Ă���I
	if (length < size) {
		return true;
	}
	//�q�b�g���Ă��Ȃ�����
	return false;
}

bool HitCupsule(const CIRCLE* p_circle, const CUPSULE* p_cupsule)
{
	//�~�ƃJ�v�Z���̈ʒu
	D3DXVECTOR2 cir_pos = D3DXVECTOR2(p_circle->cx, p_circle->cy);
	D3DXVECTOR2 cup_pos = D3DXVECTOR2(p_cupsule->x, p_cupsule->y);
	D3DXVECTOR2 cup_end = D3DXVECTOR2(p_cupsule->ex, p_cupsule->ey);

	D3DXVECTOR2 distance = cir_pos - cup_pos;

	float t = 0.0f;

	//��_�����߂�
	t = (cup_end.x * distance.x + cup_end.y * distance.y) /
		(cup_end.x * cup_end.x + cup_end.y * cup_end.y);

	if (t < 0.0f)
		t = 0.0f;
	if (t > 1.0f)
		t = 1.0f;

	D3DXVECTOR2 pos_cross;
	pos_cross.x = (cup_end.x * t) + cup_pos.x;
	pos_cross.y = (cup_end.y * t) + cup_pos.y;

	//��_�܂ł̒����Ƃ��݂��̔��a�𑫂������̂̑傫�����ׂ�
	float cross_len = (pos_cross.x - cir_pos.x) * (pos_cross.x - cir_pos.x) +
		(pos_cross.y - cir_pos.y) * (pos_cross.y - cir_pos.y);

	float size = (p_circle->r + p_cupsule->r);

	//���x�N�g���̒����̕�����������΃q�b�g���Ă���I
	if (cross_len < size * size)
	{
		return true;
	}

	//�q�b�g���Ă��Ȃ�����
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

	// 2���������s�H
	if (l1.v.isParallel(l2.v) == true) {

		// �_P11�ƒ���L2�̍ŒZ�����̖��ɋA��
		float len = calcPointLineDist2D(l1.p, l2, p2, t2);
		p1 = l1.p;
		t1 = 0.0f;

		return len;
	}

	// 2�����͂˂���֌W
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

	// �����̒����A�����̑��̍��W�y��t���Z�o
	float len = calcPointLineDist2D(p, seg, h, t);

	if (isSharpAngle(p, seg.p, e) == false) {
		// �n�_���̊O��
		h = seg.p;
		return (seg.p - p).length();
	}
	else if (isSharpAngle(p, e, seg.p) == false) {
		// �I�_���̊O��
		h = e;
		return (e - p).length();
	}

	return len;
}


// 0�`1�̊ԂɃN�����v
void clamp01(float &v) {
	if (v < 0.0f)
		v = 0.0f;
	else if (v > 1.0f)
		v = 1.0f;
}


float calcSegmentSegmentDist2D(const Segment2D &s1, const Segment2D &s2,
	Point2D &p1, Point2D &p2, float &t1, float &t2)
{
	// 2�����̍ŒZ�����i2D�Łj
	// s1 : S1(����1)
	// s2 : S2(����2)
	// p1 : S1���̐����̑��i�߂�l�j
	// p2 : S2���̐����̑��i�߂�l�j
	// t1 : S1���̃x�N�g���W���i�߂�l�j
	// t2 : S2���̃x�N�g���W���i�߂�l�j
	// �߂�l: �ŒZ����


		// S1���k�ނ��Ă���H
	if (s1.v.lengthSq() < _OX_EPSILON_) {
		// S2���k�ށH
		if (s2.v.lengthSq() < _OX_EPSILON_) {
			// �_�Ɠ_�̋����̖��ɋA��
			float len = (s2.p - s1.p).length();
			p1 = s1.p;
			p2 = s2.p;
			t1 = t2 = 0.0f;
			return len;
		}
		else {
			// S1�̎n�_��S2�̍ŒZ���ɋA��
			float len = calcPointSegmentDist2D(s1.p, s2, p2, t2);
			p1 = s1.p;
			t1 = 0.0f;
			clamp01(t2);
			return len;
		}
	}

	// S2���k�ނ��Ă���H
	else if (s2.v.lengthSq() < _OX_EPSILON_) {
		// S2�̎n�_��S1�̍ŒZ���ɋA��
		float len = calcPointSegmentDist2D(s2.p, s1, p1, t1);
		p2 = s2.p;
		clamp01(t1);
		t2 = 0.0f;
		return len;
	}

	/* �������m */

	// 2���������s�������琂���̒[�_�̈��P1�ɉ�����
	if (s1.v.isParallel(s2.v) == true) {
		t1 = 0.0f;
		p1 = s1.p;
		float len = calcPointSegmentDist2D(s1.p, s2, p2, t2);
		if (0.0f <= t2 && t2 <= 1.0f)
			return len;
	}
	else {
		// �����͂˂���̊֌W
		// 2�����Ԃ̍ŒZ���������߂ĉ���t1,t2�����߂�
		float len = calcLineLineDist2D(s1, s2, p1, p2, t1, t2);
		if (
			0.0f <= t1 && t1 <= 1.0f &&
			0.0f <= t2 && t2 <= 1.0f
			) {
			return len;
		}
	}

	// �����̑����O�ɂ��鎖������
	// S1����t1��0�`1�̊ԂɃN�����v���Đ������~�낷
	clamp01(t1);
	p1 = s1.getPoint(t1);
	float len = calcPointSegmentDist2D(p1, s2, p2, t2);
	if (0.0f <= t2 && t2 <= 1.0f)
		return len;

	// S2�����O�������̂�S2�����N�����v�AS1�ɐ������~�낷
	clamp01(t2);
	p2 = s2.getPoint(t2);
	len = calcPointSegmentDist2D(p2, s1, p1, t1);
	if (0.0f <= t1 && t1 <= 1.0f)
		return len;

	// �o���̒[�_���ŒZ�Ɣ���
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
	//player�\���̂̏����擾
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
		// �G�l�~�[�͗L�����H
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
					if (HitCupsule(Enemy_GetCollision(i), player.first[0]))//����
					{
						if (enemy.damagecol == true)
						{
							player.karacombo = false;
							player.commbo += 1;
							player.comboflame = 0;
							Game_AddScore(200 + (player.commbo * 20));
							// �������Ă�
							enemy.hp -= 1;
							Enemy_AddDamage(i);
							enemy.damagecol = false;
							enemy.rot = 0.0f;

							player.leftstop = false;
							player.stop = false;

							//se
							if (player.pos.x >= enemy.pos.x) {//�G����
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
					if (HitCupsule(Enemy_GetCollision(i), player.first[1]))//����
					{
						if (enemy.damagecol == true)
						{

							player.karacombo = false;
							player.commbo += 1;
							player.comboflame = 0;
							Game_AddScore(200 + (player.commbo * 20));
							// �������Ă�
							enemy.hp -= 1;
							Enemy_AddDamage(i);
							enemy.damagecol = false;
							enemy.rot = 0.0f;

							player.leftstop = false;
							player.stop = false;

							//se

							if (player.pos.x >= enemy.pos.x) {//�G����
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

				if (HitCupsule(Enemy_GetCollision(i), player.UpCol))//��U��
				{
					if (enemy.damagecol == true)
					{
						player.karacombo = false;
						player.commbo += 1;
						player.comboflame = 0;
						Game_AddScore(200 + (player.commbo * 20));
						// �������Ă�
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
				if (HitCupsule(Enemy_GetCollision(i), player.DownCol))//���U��
				{
					if (enemy.damagecol == true)
					{
						player.karacombo = false;
						player.commbo += 1;
						player.comboflame = 0;
						Game_AddScore(200 + (player.commbo * 20));
						// �������Ă�
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
					if (HitCupsule(Enemy_GetCollision(i), player.SlideCol[0]))//�X���C�f�B���O
					{
						if (enemy.damagecol == true)
						{
							player.karacombo = false;
							player.commbo += 1;
							player.comboflame = 0;
							Game_AddScore(200 + (player.commbo * 20));
							// �������Ă�
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
					if (HitCupsule(Enemy_GetCollision(i), player.SlideCol[1]))//����
					{
						if (enemy.damagecol == true)
						{
							player.karacombo = false;
							player.commbo += 1;
							player.comboflame = 0;
							Game_AddScore(200 + (player.commbo * 20));
							// �������Ă�
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

				if (player.pos.x >= enemy.pos.x) {//�G����
					player.leftstop = true;
					enemy.enemystop = true;
				}
				else {//�G���E
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
		// �e�͗L�����H
		if (!Bullet_IsEnable(i))
			continue;

		if (Bullet_Iscool(i) == true)
		{
			// �e�̃R���W�����ƃG�l�~�[�̃R���W����
			if (HitCupsule(player.collision, Bullet_GetCollision(i)))
			{
				// �������Ă�
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
		// �e�͗L�����H
		if (!BossBullet_IsEnable(i))
			continue;

		if (BossBullet_Iscool(i) == true)
		{
			// �e�̃R���W�����ƃG�l�~�[�̃R���W����
			if (HitCupsule(player.collision, BossBullet_GetCollision(i)))
			{
				// �������Ă�
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

	//���̈ړ��`�F�b�N
	tile = GetTileInfo(dst.x + pOut->x, dst.y);
	if (tile.isWalk == 0)
	{
		pOut->x = 0.0f;
		hit = true;
	}

	//�c�̈ړ��`�F�b�N
	tile = GetTileInfo(dst.x, dst.y + pOut->y);
	//�����Ȃ��}�b�v��������q�b�g
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
		// �G�l�~�[�͗L�����H
		if (!Boss_IsEnable(i)) {
			continue;
		}

		if (!player.com == 0) {
			switch (player.com)
			{
			case 1:
				if (player.muki == 1) {
					if (HitCupsule(Boss_GetCollision(i), player.first[0]))//����
					{
						if (player.attackcol == true)
						{
							player.karacombo = false;
							player.commbo += 1;
							player.comboflame = 0;
							Game_AddScore(200 + (player.commbo * 20));
							// �������Ă�
							boss.hp -= 1;
							Boss_AddDamage(i);
							player.attackcol = false;

							player.leftstop = false;
							player.stop = false;

							//se
							if (player.pos.x >= boss.pos.x) {//�G����
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
					if (HitCupsule(Boss_GetCollision(i), player.first[1]))//����
					{
						if (player.attackcol == true)
						{

							player.karacombo = false;
							player.commbo += 1;
							player.comboflame = 0;
							Game_AddScore(200 + (player.commbo * 20));
							// �������Ă�
							boss.hp -= 1;
							Boss_AddDamage(i);
							player.attackcol = false;

							player.leftstop = false;
							player.stop = false;

							//se

							if (player.pos.x >= enemy.pos.x) {//�G����
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

				if (HitCupsule(Boss_GetCollision(i), player.UpCol))//��U��
				{
					if (player.attackcol == true)
					{
						player.karacombo = false;
						player.commbo += 1;
						player.comboflame = 0;
						Game_AddScore(200 + (player.commbo * 20));
						// �������Ă�
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
				if (HitCupsule(Boss_GetCollision(i), player.DownCol))//���U��
				{
					if (player.attackcol == true)
					{
						player.karacombo = false;
						player.commbo += 1;
						player.comboflame = 0;
						Game_AddScore(200 + (player.commbo * 20));
						// �������Ă�
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
					if (HitCupsule(Boss_GetCollision(i), player.SlideCol[0]))//�X���C�f�B���O
					{
						if (player.attackcol == true)
						{
							player.karacombo = false;
							player.commbo += 1;
							player.comboflame = 0;
							Game_AddScore(200 + (player.commbo * 20));
							// �������Ă�
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
					if (HitCupsule(Boss_GetCollision(i), player.SlideCol[1]))//����
					{
						if (player.attackcol == true)
						{
							player.karacombo = false;
							player.commbo += 1;
							player.comboflame = 0;
							Game_AddScore(200 + (player.commbo * 20));
							// �������Ă�
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

				if (player.pos.x >= boss.pos.x) {//�G����
					player.leftstop = true;
					boss.bossstop = true;
				}
				else {//�G���E
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
		// �e�͗L�����H
		if (!BossLaser_IsEnable(i))
			continue;

		if (BossLaser_Iscoool(0) == true)
		{
			// �e�̃R���W�����ƃG�l�~�[�̃R���W����
			if (HitCupsule(player.collision, BossLaser_GetCollision(0)) && !Get_laserhit())
			{
				Laserhit(true);
				// �������Ă�
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
		// �e�͗L�����H
		if (!BossFlame_IsEnable(i))
			continue;

		if (BossFlame_Iscoool(i) == true)
		{
			// �e�̃R���W�����ƃG�l�~�[�̃R���W����
			if (HitCupsule(player.collision, BossFlame_GetCollision(0)) && !Get_flamehit())
			{
				Flamehit(true);
				// �������Ă�
				player.hitpoint -= 2;

			}
		}
	}
}

void Collision_Bossthunder_vs_Player(void)
{
	for (int i = 0; i < BOSSTHUNDER_MAX; i++)
	{
		// �e�͗L�����H
		if (!BossThunder_IsEnable(i))
			continue;

		if (BossThunder_Iscoool(i) == true)
		{
			// �e�̃R���W�����ƃG�l�~�[�̃R���W����
			if (HitCupsule(player.collision, BossThunder_GetCollision(0)) && !Get_Thunderhit())
			{
				thunderhit(true);
				// �������Ă�
				player.hitpoint -= 1;

			}

		}
	}
}