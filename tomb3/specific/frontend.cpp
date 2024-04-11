#include "../tomb3/pch.h"
#include "frontend.h"
#include "specific.h"
#include "../3dsystem/3d_gen.h"
#include "display.h"
#include "input.h"
#include "time.h"
#include "fmv.h"
#include "../game/inventry.h"
#include "output.h"
#include "../3dsystem/hwinsert.h"
#include "../game/invfunc.h"
#include "../tomb3/tomb3.h"
#include "winmain.h"

void S_DrawScreenLine(long x, long y, long z, long w, long h, long c, GOURAUD_FILL* grdptr, ushort f)
{
	InsertLine(x, y, x + w, y + h, phd_znear + 8 * z, (char)c, c);
}

static void MakeFlatGour(char c, GOURAUD_OUTLINE* grdptr)
{
	ulong col;

	col = inv_colours[c];

	for (int i = 0; i < 9; i++)
		grdptr->clr[i] = col;
}

void S_DrawBorder(long x, long y, long z, long w, long h, char c, GOURAUD_OUTLINE* grdptr, ushort flags)
{
	GOURAUD_OUTLINE flat = { 0 };
	long p;
	long bx[3];
	long by[3];
	bool add;

	if (!grdptr)
	{
		grdptr = &flat;
		MakeFlatGour(c, grdptr);
	}

	add = 0;

	if (flags)
		add = 1;

	p = GetRenderScale(1);
	x += p;
	y += p;
	w += p;
	h += p;
	bx[0] = x;
	by[0] = y;
	bx[2] = x + w;
	by[2] = y + h;
	bx[1] = (bx[0] + bx[2]) / 2;
	by[1] = (by[0] + by[2]) / 2;

	InsertGourQuad(bx[0] - p, by[0] - p, bx[1], by[0], phd_znear + 0x50000,
		grdptr->clr[0], grdptr->clr[1],
		grdptr->clr[1], grdptr->clr[0], add);
	InsertGourQuad(bx[1], by[0] - p, bx[2], by[0], phd_znear + 0x50000,
		grdptr->clr[1], grdptr->clr[2],
		grdptr->clr[2], grdptr->clr[1], add);

	InsertGourQuad(bx[2], by[0] - p, bx[2] + p, by[1], phd_znear + 0x50000,
		grdptr->clr[2], grdptr->clr[2],
		grdptr->clr[3], grdptr->clr[3], add);
	InsertGourQuad(bx[2], by[1], bx[2] + p, by[2], phd_znear + 0x50000,
		grdptr->clr[3], grdptr->clr[3],
		grdptr->clr[4], grdptr->clr[4], add);

	InsertGourQuad(bx[1], by[2], bx[2] + p, by[2] + p, phd_znear + 0x50000,
		grdptr->clr[5], grdptr->clr[4],
		grdptr->clr[4], grdptr->clr[5], add);
	InsertGourQuad(bx[0], by[2], bx[1], by[2] + p, phd_znear + 0x50000,
		grdptr->clr[6], grdptr->clr[5],
		grdptr->clr[5], grdptr->clr[6], add);

	InsertGourQuad(bx[0] - p, by[1], bx[0], by[2] + p, phd_znear + 0x50000,
		grdptr->clr[7], grdptr->clr[7],
		grdptr->clr[6], grdptr->clr[6], add);
	InsertGourQuad(bx[0] - p, by[0], bx[0], by[1], phd_znear + 0x50000,
		grdptr->clr[8], grdptr->clr[8],
		grdptr->clr[7], grdptr->clr[7], add);
}

void S_DrawScreenBox(long x, long y, long z, long w, long h, long sprnum, GOURAUD_FILL* grdptr, ushort f)
{
	long sx, sy;

	sx = x + w;
	sy = y + h;
	S_DrawScreenLine(x,			y - 1,	z, w + 1,	0,		15, 0, f);
	S_DrawScreenLine(x + 1,		y,		z, w - 1,	0,		31, 0, f);
	S_DrawScreenLine(sx,		y + 1,	z, 0,		h - 1,	15, 0, f);
	S_DrawScreenLine(sx + 1,	y,		z, 0,		h + 1,	31, 0, f);
	S_DrawScreenLine(x - 1,		y - 1,	z, 0,		h + 1,	15, 0, f);
	S_DrawScreenLine(x,			y,		z, 0,		h - 1,	31, 0, f);
	S_DrawScreenLine(x,			sy,		z, w - 1,	0,		15, 0, f);
	S_DrawScreenLine(x - 1,		sy + 1, z, w + 1,	0,		31, 0, f);
}

void S_DrawScreenFBox(long x, long y, long z, long w, long h, long c, GOURAUD_FILL* grdptr, ushort f)
{
	long adder;
	long bx[3];
	long by[3];
	bool add;

	if (tomb3.psx_boxes)
	{
		adder = GetRenderScale(1);
		x += adder;
		y += adder;
	}
	else
		adder = 2;

	w += adder;
	h += adder;

	if (tomb3.psx_boxes && grdptr)
	{
		add = 0;

		if (f)
			add = 1;

		bx[0] = x;
		by[0] = y;
		bx[2] = x + w;
		by[2] = y + h;
		bx[1] = (bx[0] + bx[2]) / 2;
		by[1] = (by[0] + by[2]) / 2;
		InsertGourQuad(bx[0], by[0], bx[1], by[1], phd_znear + 0x50000,
			grdptr->clr[0][0], grdptr->clr[0][1],
			grdptr->clr[0][2], grdptr->clr[0][3], add);
		InsertGourQuad(bx[1], by[0], bx[2], by[1], phd_znear + 0x50000,
			grdptr->clr[1][0], grdptr->clr[1][1],
			grdptr->clr[1][2], grdptr->clr[1][3], add);
		InsertGourQuad(bx[1], by[1], bx[2], by[2], phd_znear + 0x50000,
			grdptr->clr[2][0], grdptr->clr[2][1],
			grdptr->clr[2][2], grdptr->clr[2][3], add);
		InsertGourQuad(bx[0], by[1], bx[1], by[2], phd_znear + 0x50000,
			grdptr->clr[3][0], grdptr->clr[3][1],
			grdptr->clr[3][2], grdptr->clr[3][3], add);

		return;
	}

	InsertTransQuad(phd_winxmin + x, phd_winymin + y, w, h, phd_znear + 0x50000);
}

void S_FinishInventory()
{
	if (Inventory_Mode != INV_TITLE_MODE)
		TempVideoRemove();
}

void S_Wait(long nFrames, long skip)
{
	while (nFrames > 0)
	{
		if (!skip || !input)
			break;

		S_UpdateInput();

		if (GtWindowClosed)
			return;

		SyncTicks(1);
		nFrames--;
	}

	while (nFrames > 0)
	{
		S_UpdateInput();

		if (GtWindowClosed || (skip && input))
			return;

		SyncTicks(1);
		nFrames--;
	}
}

long S_PlayFMV(LPCTSTR fileName) {
	return PlayFMV(fileName);
}

long S_IntroFMV(LPCTSTR fileName1, LPCTSTR fileName2) {
	return IntroFMV(fileName1, fileName2);
}
