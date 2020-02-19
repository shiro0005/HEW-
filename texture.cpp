#include <d3dx9.h>
//#include "debugPrintf.h"
#include "mydirect3d.h"
#include "texture.h"

#define TEXTURE_FILENAME_MAX (64)


typedef struct TextureFile_tag
{
	char filename[TEXTURE_FILENAME_MAX];
	int width;
	int height;
} TextureFile;

static const TextureFile g_TextureFiles[] = {
    { "asset/texture/player.tga", 256, 256 },
    { "asset/texture/zonbimovi.png", 600, 1400 },
	{ "asset/texture/bullet00.png", 32, 32 },
	{ "asset/texture/laser.png", 256, 320 },
	{ "asset/texture/exp.png", 128, 128 },
	{ "asset/texture/number3.png", 320, 32 },
	{ "asset/texture/start.png", 1024, 576 },
	{ "asset/texture/enemy.tga", 256, 256 },

	{ "asset/texture/start.png", 1024, 576 },
	{ "asset/texture/start.png", 1024, 576 },
	{ "asset/texture/end.png", 1024, 576 },
	{ "asset/texture/start.png", 1024, 576 },
	{ "asset/texture/end.png", 1024, 576 },

	{ "asset/texture/bgloopbigguiro.png", 1024, 576},
	{ "asset/texture/bgloopiro.png", 1024, 576 },

	{ "asset/texture/end.png", 1024, 576 },
	{ "asset/texture/kokosozai.png", 512, 512 },
	{ "asset/texture/CIRCLE.png", 720, 720 },
	{ "asset/texture/impact1.png", 256, 256 },
	{ "asset/texture/impact.png", 256, 256 },
	{ "asset/texture/jougai.png", 256, 2560 },
	{ "asset/texture/attack_speed.png", 256, 448 },
	{ "asset/texture/taisou.png", 1100, 1700 },
    {"asset/texture/number3.png",320,34},
    {"asset/texture/titlemozi.png",710,205},
    {"asset/texture/タイトル　体.png",720,540},
    {"asset/texture/タイトル　葬.png",720,540},
	{"asset/texture/ライフ選手赤透過.png",298,298},
	{"asset/texture/ライフ選手緑透過.png",298,298},
	{"asset/texture/playerdamage.png",1024,512},
	{"asset/texture/hit.png",608,253},
	{"asset/texture/controller.png",5400,350},
	{"asset/texture/bosscreate.png",256,830},
	{"asset/texture/bossdelete.png",256,765},
	{"asset/texture/bosslaserhint.png",512,3328},
};

static const int TEXTURE_FILE_COUNT = sizeof(g_TextureFiles) / sizeof(g_TextureFiles[0]);

static_assert(TEXTURE_INDEX_MAX == TEXTURE_FILE_COUNT, "TEXTURE_INDEX_MAX != TEXTURE_FILE_COUNT");

static LPDIRECT3DTEXTURE9 g_pTextures[TEXTURE_FILE_COUNT] = {};

int Texture_Load(void)
{   
    LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	if( !pDevice ) {
		return TEXTURE_FILE_COUNT;
	}

	int failed_count = 0;

	for( int i = 0; i < TEXTURE_FILE_COUNT; i++ ) {
		
		if( FAILED(D3DXCreateTextureFromFile(pDevice, g_TextureFiles[i].filename, &g_pTextures[i])) ) {
            // DebugPrintf("テクスチャの読み込みに失敗 ... %s\n", g_TextureFiles[i].filename);
			failed_count++;
		}
	}

	return failed_count;
}

void Texture_Release(void)
{
	for( int i = 0; i < TEXTURE_FILE_COUNT; i++ ) {
		
		if( g_pTextures[i] ) {
			g_pTextures[i]->Release();
			g_pTextures[i] = NULL;
		}
	}
}

LPDIRECT3DTEXTURE9 Texture_GetTexture(TextureIndex index)
{
    if( index < 0 || index >= TEXTURE_INDEX_MAX ) {
        return NULL;
    }

	return g_pTextures[index];
}

int Texture_GetWidth(TextureIndex index)
{
    if( index < 0 || index >= TEXTURE_INDEX_MAX ) {
        return NULL;
    }

	return g_TextureFiles[index].width;
}

int Texture_GetHeight(TextureIndex index)
{
    if( index < 0 || index >= TEXTURE_INDEX_MAX ) {
        return NULL;
    }

	return g_TextureFiles[index].height;
}
