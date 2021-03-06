#include "explosion2.h"
#include "texture.h"
#include "sprite.h"
#include "player.h"
#include "boss.h"
//impact
#define EXPLOSION_MAX    (256)
#define EXPLOSION_WIDTH  (64)
#define EXPLOSION_HEIGHT (64)
#define EXPLOSION_PATTERN_FRAME (1)
#define EXPLOSION_PATTERN_MAX   (16)
#define EXPLOSION_PATTERN_H_MAX (4)

//場外
#define EXPLOSION2_MAX    (256)
#define EXPLOSION2_WIDTH  (64)
#define EXPLOSION2_HEIGHT (320)
#define EXPLOSION2_PATTERN_FRAME (1)
#define EXPLOSION2_PATTERN_MAX   (17)
#define EXPLOSION2_PATTERN_H_MAX (4)

//攻撃スピード感
#define EXPLOSION3_MAX    (256)
#define EXPLOSION3_WIDTH  (64)
#define EXPLOSION3_HEIGHT (64)
#define EXPLOSION3_PATTERN_FRAME (1)
#define EXPLOSION3_PATTERN_MAX   (32)
#define EXPLOSION3_PATTERN_H_MAX (4)

//血しぶき
#define EXPLOSION4_MAX    (256)
#define EXPLOSION4_WIDTH  (256)
#define EXPLOSION4_HEIGHT (128)
#define EXPLOSION4_PATTERN_FRAME (1)
#define EXPLOSION4_PATTERN_MAX   (20)
#define EXPLOSION4_PATTERN_H_MAX (4)

//controller
#define CONTROLLER_MAX    (256)
#define CONTROLLER_WIDTH  (600)
#define CONTROLLER_HEIGHT (350)
#define CONTROLLER_PATTERN_FRAME (1)
#define CONTROLLER_PATTERN_MAX   (9)
#define CONTROLLER_PATTERN_H_MAX (9)

//bossdisappear
#define DISAPPEAR_MAX    (256)
#define DISAPPEAR_WIDTH  (64)
#define DISAPPEAR_HEIGHT (64)
#define DISAPPEAR_PATTERN_FRAME (1)
#define DISAPPEAR_PATTERN_MAX   (48)
#define DISAPPEAR_PATTERN_H_MAX (4)

//bossappear
#define APPEAR_MAX    (256)
#define APPEAR_WIDTH  (64)
#define APPEAR_HEIGHT (64)
#define APPEAR_PATTERN_FRAME (1)
#define APPEAR_PATTERN_MAX   (52)
#define APPEAR_PATTERN_H_MAX (4)


//explosion
#define EXPLOSION0_MAX    (256)
#define EXPLOSION0_WIDTH  (32)
#define EXPLOSION0_HEIGHT (32)
#define EXPLOSION0_PATTERN_FRAME (1)
#define EXPLOSION0_PATTERN_MAX   (16)
#define EXPLOSION0_PATTERN_H_MAX (4)

//下攻撃HIT　effect
#define EXPLOSIONDOWN_MAX    (256)
#define EXPLOSIONDOWN_WIDTH  (256)
#define EXPLOSIONDOWN_HEIGHT (256)
#define EXPLOSIONDOWN_PATTERN_FRAME (1)
#define EXPLOSIONDOWN_PATTERN_MAX   (16)
#define EXPLOSIONDOWN_PATTERN_H_MAX (4)

typedef struct Explosion_tag//impact
{
	float x, y;			//座標
	bool enable;		//
	int create_frame;
	int pattern;
} Explosion;

typedef struct Explosion_tag2//場外
{
	float x, y;			//座標
	bool enable;		//
	int create_frame;
	int pattern;
	float rot;
} Explosion2;

typedef struct Explosion_tag3//場外
{
	float x, y;			//座標
	bool enable;		//
	int create_frame;
	int pattern;
	float rot;
} Explosion3;

typedef struct Explosion4_tag//impact
{
	float x, y;			//座標
	bool enable;		//
	int create_frame;
	int pattern;
	float rot;
} Explosion4;

typedef struct Cont_tag1//場外
{
	float x, y;			//座標
	bool enable;		//
	int create_frame;
	int pattern;
	float rot;
} Cont;

typedef struct Dap_tag1//場外
{
	float x, y;			//座標
	bool enable;		//
	int create_frame;
	int pattern;
	float rot;
} Dap;

typedef struct App_tag1//場外
{
	float x, y;			//座標
	bool enable;		//
	int create_frame;
	int pattern;
	float rot;
} App;


typedef struct Explosion_tag0//impact
{
	float x, y;			//座標
	bool enable;		//
	int create_frame;
	int pattern;
} Explosion0;

typedef struct ExplosionDown_tag//impact
{
	float x, y;			//座標
	bool enable;		//
	int create_frame;
	int pattern;
} ExplosionDown;

static Cont       g_controller[EXPLOSION_MAX];
static Explosion g_Explosions[EXPLOSION_MAX];
static Explosion2 g_Explosions2[EXPLOSION2_MAX];
static Explosion3 g_Explosions3[EXPLOSION3_MAX];
static Explosion4 g_Explosions4[EXPLOSION4_MAX];
static Explosion0 g_Explosions0[EXPLOSION0_MAX];
static ExplosionDown g_ExplosionsDown[EXPLOSIONDOWN_MAX];

static int g_ExplosionFrameCount = 0;
static PLAYER player;
static BOSS_DATA boss;
static Dap g_Explosions5[DISAPPEAR_MAX];
static App g_Explosions6[APPEAR_MAX];

void Explosion_Initialize2(void)
{
	for (int i = 0; i < EXPLOSION_MAX; i++) {
		g_Explosions[i].enable = false;
	}

	for (int i = 0; i < EXPLOSION2_MAX; i++) {
		g_Explosions2[i].enable = false;
	}

	for (int i = 0; i < EXPLOSION3_MAX; i++) {
		g_Explosions3[i].enable = false;
	}

	for (int i = 0; i < EXPLOSION4_MAX; i++) {
		g_Explosions4[i].enable = false;
	}

	for (int i = 0; i < DISAPPEAR_MAX; i++) {
		g_Explosions5[i].enable = false;
	}

	for (int i = 0; i < APPEAR_MAX; i++) {
		g_Explosions6[i].enable = false;
	}

	for (int i = 0; i < EXPLOSION0_MAX; i++) {
		g_Explosions0[i].enable = false;
	}

	for (int i = 0; i < EXPLOSIONDOWN_MAX; i++) {
		g_ExplosionsDown[i].enable = false;
	}
}

void Explosion_Update2(void)
{
	player = GetPlayer();

	for (int i = 0; i < EXPLOSION_MAX; i++) {

		if (g_Explosions[i].enable) {

			int age = g_ExplosionFrameCount - g_Explosions[i].create_frame;

			g_Explosions[i].pattern = age / EXPLOSION_PATTERN_FRAME;

			//最後のパターンが表示されたら終了する処理
			if (g_Explosions[i].pattern >= EXPLOSION_PATTERN_MAX) {
				g_Explosions[i].enable = false;
			}
		}

		if (g_controller[i].enable) {

			int age = g_ExplosionFrameCount - g_controller[i].create_frame;

			g_controller[i].pattern = (age / EXPLOSION3_PATTERN_FRAME) / 3;

			//最後のパターンが表示されたら終了する処理
			if (g_controller[i].pattern >= 9) {
				g_controller[i].enable = false;
				Explosion_Initialize2();
				Controller_Create(500.0f, 480.0f, 0.0f);

			}
		}
	}

	for (int i = 0; i < EXPLOSION2_MAX; i++) {
		if (g_Explosions2[i].enable) {

			int age = g_ExplosionFrameCount - g_Explosions2[i].create_frame;

			g_Explosions2[i].pattern = age / EXPLOSION2_PATTERN_FRAME;

			//最後のパターンが表示されたら終了する処理
			if (g_Explosions2[i].pattern >= EXPLOSION2_PATTERN_MAX) {
				g_Explosions2[i].enable = false;
			}
		}
	}

	for (int i = 0; i < EXPLOSION3_MAX; i++) {
		if (g_Explosions3[i].enable) {
			
			int age = g_ExplosionFrameCount - g_Explosions3[i].create_frame;

			g_Explosions3[i].pattern = age / EXPLOSION3_PATTERN_FRAME;

			//最後のパターンが表示されたら終了する処理
			if (g_Explosions3[i].pattern >= EXPLOSION3_PATTERN_MAX) {
				g_Explosions3[i].enable = false;
			}
		}
	}

	for (int i = 0; i < EXPLOSION4_MAX; i++) {
		if (g_Explosions4[i].enable) {

			int age = g_ExplosionFrameCount - g_Explosions4[i].create_frame;

			g_Explosions4[i].pattern = age / EXPLOSION4_PATTERN_FRAME;

			//最後のパターンが表示されたら終了する処理
			if (g_Explosions4[i].pattern >= EXPLOSION4_PATTERN_MAX) {
				g_Explosions4[i].enable = false;
			}
		}
	}

	for (int i = 0; i < DISAPPEAR_MAX; i++) {
		if (g_Explosions5[i].enable) {

			int age = g_ExplosionFrameCount - g_Explosions5[i].create_frame;

			g_Explosions5[i].pattern = age / DISAPPEAR_PATTERN_FRAME;

			//最後のパターンが表示されたら終了する処理
			if (g_Explosions5[i].pattern >= DISAPPEAR_PATTERN_MAX) {
				g_Explosions5[i].enable = false;
			}
		}
	}

	for (int i = 0; i < APPEAR_MAX; i++) {
		if (g_Explosions6[i].enable) {

			int age = g_ExplosionFrameCount - g_Explosions6[i].create_frame;

			g_Explosions6[i].pattern = age / APPEAR_PATTERN_FRAME;

			//最後のパターンが表示されたら終了する処理
			if (g_Explosions6[i].pattern >= APPEAR_PATTERN_MAX) {
				g_Explosions6[i].enable = false;
			}
		}
	}

	for (int i = 0; i < EXPLOSION0_MAX; i++) {

		if (g_Explosions0[i].enable) {

			int age = g_ExplosionFrameCount - g_Explosions0[i].create_frame;

			g_Explosions0[i].pattern = age / EXPLOSION0_PATTERN_FRAME;

			//最後のパターンが表示されたら終了する処理
			if (g_Explosions0[i].pattern >= EXPLOSION0_PATTERN_MAX) {
				g_Explosions0[i].enable = false;
			}
		}

	}

	for (int i = 0; i < EXPLOSIONDOWN_MAX; i++) {

		if (g_ExplosionsDown[i].enable) {

			int age = g_ExplosionFrameCount - g_ExplosionsDown[i].create_frame;

			g_ExplosionsDown[i].pattern = age / EXPLOSIONDOWN_PATTERN_FRAME;

			//最後のパターンが表示されたら終了する処理
			if (g_ExplosionsDown[i].pattern >= EXPLOSIONDOWN_PATTERN_MAX) {
				g_ExplosionsDown[i].enable = false;
			}
		}

	}
	g_ExplosionFrameCount++;
}

void Explosion_Draw2(void)
{
	for (int i = 0; i < EXPLOSION_MAX; i++) {

		if (!g_Explosions[i].enable) {
			continue;
		}

		// 現在表示するべきパターン番号から切り取り座標を算出する
		int tx = EXPLOSION_WIDTH * (g_Explosions[i].pattern % EXPLOSION_PATTERN_H_MAX);
		int ty = EXPLOSION_HEIGHT * (g_Explosions[i].pattern / EXPLOSION_PATTERN_H_MAX);

		Sprite_Draw(TEXTURE_INDEX_EXPLOSION3,
			g_Explosions[i].x,
			g_Explosions[i].y,
			tx,
			ty,
			EXPLOSION_WIDTH,
			EXPLOSION_HEIGHT,
			EXPLOSION_WIDTH / 2,
			EXPLOSION_HEIGHT / 2,
			3.0f,
			3.0f,
			0.0f);
	}

	for (int i = 0; i < EXPLOSION2_MAX; i++) {

		if (!g_Explosions2[i].enable) {
			continue;
		}

		// 現在表示するべきパターン番号から切り取り座標を算出する
		int tx = EXPLOSION2_WIDTH * (g_Explosions2[i].pattern % EXPLOSION2_PATTERN_H_MAX);
		int ty = EXPLOSION2_HEIGHT * (g_Explosions2[i].pattern / EXPLOSION2_PATTERN_H_MAX);

		Sprite_Draw(TEXTURE_INDEX_JOUGAI,
			g_Explosions2[i].x,
			g_Explosions2[i].y,
			tx,
			ty,
			EXPLOSION2_WIDTH,
			EXPLOSION2_HEIGHT,
			EXPLOSION2_WIDTH / 2,
			EXPLOSION2_HEIGHT / 10 * 9,
			2.0f,
			2.0f,
			g_Explosions2[i].rot);
	}

	for (int i = 0; i < EXPLOSION3_MAX; i++) {

		if (!g_Explosions3[i].enable) {
			continue;
		}

		// 現在表示するべきパターン番号から切り取り座標を算出する
		int tx = EXPLOSION3_WIDTH * (g_Explosions3[i].pattern % EXPLOSION3_PATTERN_H_MAX);
		int ty = EXPLOSION3_HEIGHT * (g_Explosions3[i].pattern / EXPLOSION3_PATTERN_H_MAX);

		Sprite_Draw(TEXTURE_INDEX_ATTACK_SPEED,
			player.pos.x,
			player.pos.y,
			tx,
			ty,
			EXPLOSION3_WIDTH,
			EXPLOSION3_HEIGHT,
			EXPLOSION3_WIDTH / 2,
			EXPLOSION3_HEIGHT / 2,
			4.0f,
			4.0f,
			g_Explosions3[i].rot);
	}

	for (int i = 0; i < EXPLOSION4_MAX; i++) {

		if (!g_Explosions4[i].enable) {
			continue;
		}

		// 現在表示するべきパターン番号から切り取り座標を算出する
		int tx = EXPLOSION4_WIDTH * (g_Explosions4[i].pattern % EXPLOSION4_PATTERN_H_MAX);
		int ty = EXPLOSION4_HEIGHT * (g_Explosions4[i].pattern / EXPLOSION4_PATTERN_H_MAX);

		Sprite_Draw(TEXTURE_INDEX_TISIBUKI,
			player.pos.x + g_Explosions4[i].x,
			player.pos.y,
			tx,
			ty,
			EXPLOSION4_WIDTH,
			EXPLOSION4_HEIGHT,
			EXPLOSION4_WIDTH / 2,
			EXPLOSION4_HEIGHT / 2,
			1.0f,
			1.0f,
			g_Explosions4[i].rot);
	}
	for (int i = 0; i < CONTROLLER_MAX; i++) {

		if (!g_controller[i].enable) {
			continue;
		}

		// 現在表示するべきパターン番号から切り取り座標を算出する
		int tx = CONTROLLER_WIDTH * (g_controller[i].pattern % CONTROLLER_PATTERN_H_MAX);
		int ty = CONTROLLER_HEIGHT * (g_controller[i].pattern / CONTROLLER_PATTERN_H_MAX);

		Sprite_Draw(TEXTURE_INDEX_CONTROLLER,
			g_controller[i].x,
			g_controller[i].y,
			tx,
			ty,
			CONTROLLER_WIDTH,
			CONTROLLER_HEIGHT,
			CONTROLLER_WIDTH / 2,
			CONTROLLER_HEIGHT / 2,
			0.5f,
			0.5f,
			g_controller[i].rot);
	}
	for (int i = 0; i < DISAPPEAR_MAX; i++) {

		if (!g_Explosions5[i].enable) {
			continue;
		}

		// 現在表示するべきパターン番号から切り取り座標を算出する
		int tx = DISAPPEAR_WIDTH * (g_Explosions5[i].pattern % DISAPPEAR_PATTERN_H_MAX);
		int ty = DISAPPEAR_HEIGHT * (g_Explosions5[i].pattern / DISAPPEAR_PATTERN_H_MAX);

		Sprite_Draw(TEXTURE_INDEX_DELETE,
			g_Explosions5[i].x,
			g_Explosions5[i].y,
			tx,
			ty,
			DISAPPEAR_WIDTH,
			DISAPPEAR_HEIGHT,
			DISAPPEAR_WIDTH / 2,
			DISAPPEAR_HEIGHT / 2,
			3.0f,
			3.0f,
			g_Explosions5[i].rot);
	}
	for (int i = 0; i < APPEAR_MAX; i++) {

		if (!g_Explosions6[i].enable) {
			continue;
		}

		// 現在表示するべきパターン番号から切り取り座標を算出する
		int tx = APPEAR_WIDTH * (g_Explosions6[i].pattern % APPEAR_PATTERN_H_MAX);
		int ty = APPEAR_HEIGHT * (g_Explosions6[i].pattern / APPEAR_PATTERN_H_MAX);

		Sprite_Draw(TEXTURE_INDEX_CREATE,
			g_Explosions6[i].x,
			g_Explosions6[i].y,
			tx,
			ty,
			APPEAR_WIDTH,
			APPEAR_HEIGHT,
			APPEAR_WIDTH / 2,
			APPEAR_HEIGHT / 2,
			3.0f,
			3.0f,
			g_Explosions6[i].rot);
	}


	for (int i = 0; i < EXPLOSION0_MAX; i++) {

		if (!g_Explosions0[i].enable) {
			continue;
		}

		// 現在表示するべきパターン番号から切り取り座標を算出する
		int tx = EXPLOSION0_WIDTH * (g_Explosions0[i].pattern % EXPLOSION0_PATTERN_H_MAX);
		int ty = EXPLOSION0_HEIGHT * (g_Explosions0[i].pattern / EXPLOSION0_PATTERN_H_MAX);

		Sprite_Draw(TEXTURE_INDEX_EXPLOSION,
			g_Explosions0[i].x,
			g_Explosions0[i].y,
			tx,
			ty,
			EXPLOSION0_WIDTH,
			EXPLOSION0_HEIGHT,
			EXPLOSION0_WIDTH / 2,
			EXPLOSION0_HEIGHT / 2,
			3.0f,
			3.0f,
			0);
	}

	for (int i = 0; i < EXPLOSIONDOWN_MAX; i++) {

		if (!g_ExplosionsDown[i].enable) {
			continue;
		}

		// 現在表示するべきパターン番号から切り取り座標を算出する
		int tx = EXPLOSIONDOWN_WIDTH * (g_ExplosionsDown[i].pattern % EXPLOSIONDOWN_PATTERN_H_MAX);
		int ty = EXPLOSIONDOWN_HEIGHT * (g_ExplosionsDown[i].pattern / EXPLOSIONDOWN_PATTERN_H_MAX);

		Sprite_Draw(TEXTURE_INDEX_DOWNHIT,
			g_ExplosionsDown[i].x,
			g_ExplosionsDown[i].y,
			tx,
			ty,
			EXPLOSIONDOWN_WIDTH,
			EXPLOSIONDOWN_HEIGHT,
			EXPLOSIONDOWN_WIDTH / 2,
			EXPLOSIONDOWN_HEIGHT / 2,
			1.0f,
			1.0f,
			0);
	}

}

void Explosion_Create2(float x, float y)
{
	for (int i = 0; i < EXPLOSION_MAX; i++) {

		if (g_Explosions[i].enable) {
			continue;
		}

		g_Explosions[i].x = x;
		g_Explosions[i].y = y;
		g_Explosions[i].create_frame = g_ExplosionFrameCount;
		g_Explosions[i].pattern = 0;
		g_Explosions[i].enable = true;

		break;
	}
}

void Explosion_Create3(float x, float y,float rot)
{
	for (int i = 0; i < EXPLOSION2_MAX; i++) {

		if (g_Explosions2[i].enable) {
			continue;
		}

		g_Explosions2[i].x = x;
		g_Explosions2[i].y = y;
		g_Explosions2[i].rot = rot;
		g_Explosions2[i].create_frame = g_ExplosionFrameCount;
		g_Explosions2[i].pattern = 0;
		g_Explosions2[i].enable = true;

		break;
	}
}

void Explosion_Create4(float x, float y)
{
	for (int i = 0; i < EXPLOSION3_MAX; i++) {

		if (g_Explosions3[i].enable) {
			continue;
		}

		g_Explosions3[i].x = x;
		g_Explosions3[i].y = y;
		g_Explosions3[i].rot = -1.5f;
		g_Explosions3[i].create_frame = g_ExplosionFrameCount;
		g_Explosions3[i].pattern = 0;
		g_Explosions3[i].enable = true;

		break;
	}
}

void Explosion_Create5(float x, float y,float rot)
{
	for (int i = 0; i < EXPLOSION4_MAX; i++) {

		if (g_Explosions4[i].enable) {
			continue;
		}

		g_Explosions4[i].x = x;
		g_Explosions4[i].y = y;
		g_Explosions4[i].create_frame = g_ExplosionFrameCount;
		g_Explosions4[i].rot = rot;
		g_Explosions4[i].pattern = 0;
		g_Explosions4[i].enable = true;

		break;
	}
}

void Controller_Create(float x, float y, float rot)
{
	for (int i = 0; i < EXPLOSION3_MAX; i++) {

		if (g_controller[i].enable) {
			continue;
		}

		g_controller[i].x = x;
		g_controller[i].y = y;
		g_controller[i].rot = rot;
		g_controller[i].create_frame = g_ExplosionFrameCount;
		g_controller[i].pattern = 0;
		g_controller[i].enable = true;

		break;
	}
}

void controllerfalse(bool f) {
	for (int i = 0; i < EXPLOSION3_MAX; i++) {
		g_controller[i].enable = f;
	}
}

void Explosion_Create6(float x, float y, float rot)
{
	for (int i = 0; i < DISAPPEAR_MAX; i++) {

		if (g_Explosions5[i].enable) {
			continue;
		}

		g_Explosions5[i].x = x;
		g_Explosions5[i].y = y;
		g_Explosions5[i].create_frame = g_ExplosionFrameCount;
		g_Explosions5[i].rot = rot;
		g_Explosions5[i].pattern = 0;
		g_Explosions5[i].enable = true;

		break;
	}
}

void Explosion_Create7(float x, float y, float rot)
{
	for (int i = 0; i < APPEAR_MAX; i++) {

		if (g_Explosions6[i].enable) {
			continue;
		}

		g_Explosions6[i].x = x;
		g_Explosions6[i].y = y;
		g_Explosions6[i].create_frame = g_ExplosionFrameCount;
		g_Explosions6[i].rot = rot;
		g_Explosions6[i].pattern = 0;
		g_Explosions[i].enable = true;

		break;
	}
}

void Explosion_Create0(float x, float y)
{
	for (int i = 0; i < EXPLOSION0_MAX; i++) {

		if (g_Explosions0[i].enable) {
			continue;
		}

		g_Explosions0[i].x = x;
		g_Explosions0[i].y = y;
		g_Explosions0[i].create_frame = g_ExplosionFrameCount;
		g_Explosions0[i].pattern = 0;
		g_Explosions0[i].enable = true;

		break;
	}
}


void Explosion_CreateDown(float x, float y)
{
	for (int i = 0; i < EXPLOSIONDOWN_MAX; i++) {

		if (g_ExplosionsDown[i].enable) {
			continue;
		}

		g_ExplosionsDown[i].x = x;
		g_ExplosionsDown[i].y = y;
		g_ExplosionsDown[i].create_frame = g_ExplosionFrameCount;
		g_ExplosionsDown[i].pattern = 0;
		g_ExplosionsDown[i].enable = true;

		break;
	}
}
