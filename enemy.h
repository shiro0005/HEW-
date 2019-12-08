#include "collision.h"
#include "2D.h"

typedef struct
{
	int			enable;	//�����t���O

	D3DXVECTOR2 pos;	//�ʒu
	float		rot;	//��]
	int			color;	//�F
	int			muki;	//�����i�ړ������j
	int			animePattern;	//�A�j���p�^�[��
	int			animeFrame;		//�p�^�[���؂�ւ��E�G�C�g

	
	Capsule2D   colcol;

	int			frame;
	int			frame_attack;  //�U�����[�V�����t���[��
	bool		animeAttack;
	bool		stay;		//�U�����̈ړ�����
	bool		ready_attack;
	bool		attack;
	bool		move;		//������΂��ꂽ�ウ�˂݁[�̒ǔ��������~�߂�
	int			frame_alpha;
	int			state;
	D3DXVECTOR2 dir_destroy;		//������΂�������
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