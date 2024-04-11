#include "../tomb3/pch.h"
#include "init.h"
#include "texture.h"
#include "audio.h"
#include "dxshell.h"
#include "di.h"
#include "ds.h"
#include "drawprimitive.h"
#include "winmain.h"
#include "../3dsystem/phd_math.h"
#include "game.h"
#include "transform.h"
#include "../3dsystem/hwinsert.h"
#include "smain.h"
#include "display.h"
#include "fmv.h"
#include "../newstuff/discord.h"

char* malloc_ptr;
char* malloc_buffer;
static long malloc_free;
static long malloc_size;
static long malloc_used;

D3DTLVERTEX* CurrentTLVertex;
D3DTLVERTEX* VertexBuffer;
D3DTLVERTEX* UnRollBuffer;

static D3DTLVERTEX* TLVertexBuffer;
static D3DTLVERTEX* TLUnRollBuffer;

WATERTAB WaterTable[22][64];
float wibble_table[32];

void ShutdownGame()
{
	RPC_close();	
	GlobalFree(TLVertexBuffer);
	GlobalFree(TLUnRollBuffer);
	ACMClose();
#if (DIRECT3D_VERSION < 0x900)
	DXFreeTPages();
	DXSetCooperativeLevel(App.DDraw, App.WindowHandle, DDSCL_NORMAL);
#endif

	if (malloc_buffer)
		GlobalFree(malloc_buffer);

	DXClearAllTextures(Textures);
#if (DIRECT3D_VERSION < 0x900)
	DXReleasePalette();
#endif
	DI_Finish();
	DS_Finish();
	WinFreeDX(1);
	DXFreeDeviceInfo(&App.DeviceInfo);
	FMV_Cleanup();

#ifdef DO_LOG
	if (logF)
		fclose(logF);
#endif
}

void CalculateWibbleTable()
{
	long s;

	for (int i = 0; i < 32; i++)
	{
		s = phd_sin(i * 0x10000 / 32);
		wibble_table[i] = float((2 * s) >> W2V_SHIFT);
	}
}

ushort GetRandom(WATERTAB* wt, long lp)
{
	long loop;
	ushort ret;

	do
	{
		ret = rand() & 0xFC;

		for (loop = 0; loop < lp; loop++)
		{
			if (wt[loop].random == ret)
				break;
		}

	} while (loop != lp);

	return ret;
}

void init_water_table()
{
	short s;
	static short water_shimmer[4] = { 31, 63, 95, 127 };
	static short water_choppy[4] = { 16, 53, 90, 127 };
	static uchar water_abs[4] = { 4, 8, 12, 16 };

	srand(121197);

	for (int i = 0; i < 64; i++)
	{
		s = rcossin_tbl[i << 7];
		WaterTable[0][i].shimmer = (63 * s) >> 15;
		WaterTable[0][i].choppy = (16 * s) >> 12;
		WaterTable[0][i].random = (uchar)GetRandom(&WaterTable[0][0], i);
		WaterTable[0][i].abs = 0;

		WaterTable[1][i].shimmer = (32 * s) >> 15;
		WaterTable[1][i].choppy = 0;
		WaterTable[1][i].random = (uchar)GetRandom(&WaterTable[1][0], i);
		WaterTable[1][i].abs = -3;

		WaterTable[2][i].shimmer = (64 * s) >> 15;
		WaterTable[2][i].choppy = 0;
		WaterTable[2][i].random = (uchar)GetRandom(&WaterTable[2][0], i);
		WaterTable[2][i].abs = 0;

		WaterTable[3][i].shimmer = (96 * s) >> 15;
		WaterTable[3][i].choppy = 0;
		WaterTable[3][i].random = (uchar)GetRandom(&WaterTable[3][0], i);
		WaterTable[3][i].abs = 4;

		WaterTable[4][i].shimmer = (127 * s) >> 15;
		WaterTable[4][i].choppy = 0;
		WaterTable[4][i].random = (uchar)GetRandom(&WaterTable[4][0], i);
		WaterTable[4][i].abs = 8;

		for (int j = 0, k = 5; j < 4; j++, k += 4)
		{
			for (int m = 0; m < 4; m++)
			{
				WaterTable[k + m][i].shimmer = -((s * water_shimmer[m]) >> 15);
				WaterTable[k + m][i].choppy = s * water_choppy[j] >> 12;
				WaterTable[k + m][i].random = (uchar)GetRandom(&WaterTable[k + m][0], i);
				WaterTable[k + m][i].abs = water_abs[m];
			}
		}
	}
}

void init_game_malloc()
{
	malloc_ptr = malloc_buffer;
	malloc_free = malloc_size;
	malloc_used = 0;
}

void* game_malloc(long size)
{
	void* ptr;

	size = (size + 3) & ~3;

	if (size > malloc_free)
	{
		wsprintf(exit_message, "game_malloc(): OUT OF MEMORY. Needed: %d, Free: %d", size, malloc_free);
		S_ExitSystem(exit_message);
	}

	ptr = malloc_ptr;
	malloc_free -= size;
	malloc_used += size;
	malloc_ptr += size;
	return ptr;
}

void game_free(long size)
{
	size = (size + 3) & ~3;
	malloc_ptr -= size;
	malloc_free += size;
	malloc_used -= size;
}

long S_InitialiseSystem()
{
	DISPLAYMODE* dm;

#if (DIRECT3D_VERSION >= 0x900)
	dm = &App.lpDeviceInfo->D3DInfo[App.lpDXConfig->nD3D].DisplayMode[App.lpDXConfig->nVMode];
#else
	dm = &App.lpDeviceInfo->DDInfo[App.lpDXConfig->nDD].D3DInfo[App.lpDXConfig->nD3D].DisplayMode[App.lpDXConfig->nVMode];
#endif
	DumpX = 0;
	DumpY = 0;
	DumpWidth = (short)dm->w;
	DumpHeight = (short)dm->h;
	InitZTable();
	InitUVTable();

	TLVertexBuffer = (D3DTLVERTEX*)GlobalAlloc(GMEM_FIXED, MAX_TLVERTICES * sizeof(D3DTLVERTEX));
	VertexBuffer = (D3DTLVERTEX*)(((long)TLVertexBuffer + 32) & 0xFFFFFFE0);

	TLUnRollBuffer = (D3DTLVERTEX*)GlobalAlloc(GMEM_FIXED, MAX_TLVERTICES * sizeof(D3DTLVERTEX));
	UnRollBuffer = (D3DTLVERTEX*)(((long)TLUnRollBuffer + 32) & 0xFFFFFFE0);

	malloc_size = MALLOC_SIZE;
	return 1;
}
