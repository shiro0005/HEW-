#include "collision.h"
#include "2D.h"

typedef struct
{
	int			enable;				//生存フラグ
	int			hp;

	D3DXVECTOR2 pos;				//位置
	float		rot;				//回転
	int			color;				//色
	int			muki;				//向き（移動方向）
	int			animePattern;		//アニメパターン
	int			animeFrame;			//パターン切り替えウエイト
	float       t;
	
	Capsule2D   colcol;
	int			frame;
	int			frame_attack;		//攻撃モーションフレーム
	bool        stop[2];			//enemyのstopフラグ
	bool		animeAttack;
	bool		stay;				//攻撃中の移動判定
	bool		ready_attack;
	bool		attack;
	bool		move;				//吹っ飛ばされた後えねみーの追尾処理を止める
	int			frame_alpha;
	int			state;
	D3DXVECTOR2 dir_destroy;		//吹っ飛ばされる方向
	D3DXVECTOR2 pos_return;

}ENEMY_DATA;


#define ENEMY_COUNT 3

void Enemy_Initialize(void);
void Enemy_Finalize(void);
void Enemy_Update(void);
void Enemy_Draw(void);
void Enemy_Destroy(int index);
bool Enemy_IsEnable(int index);
const Capsule2D* Enemy_GetCollision(int index);
void Enemy_Attack(int index);
int Enemy_AddDamage(int damage, int index);
int Enemy_GetHitPoint(int index);
void Enemy_NockBack(int index);
ENEMY_DATA GetEnemy(int i);