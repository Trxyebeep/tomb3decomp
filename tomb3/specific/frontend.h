#pragma once
#include "../global/types.h"

ushort S_COLOUR(long r, long g, long b);
void S_DrawScreenLine(long x, long y, long z, long w, long h, long c, GOURAUD_FILL* grdptr, ushort f);
void S_DrawBorder(long x, long y, long z, long w, long h, char c, GOURAUD_OUTLINE* gour, ushort flags);
void S_DrawScreenBox(long x, long y, long z, long w, long h, long sprnum, GOURAUD_FILL* grdptr, ushort f);
void S_DrawScreenFBox(long x, long y, long z, long w, long h, long c, GOURAUD_FILL* grdptr, ushort f);
void S_FinishInventory();
void S_Wait(long nFrames, long skip);
long S_PlayFMV(LPCTSTR fileName);
long S_IntroFMV(LPCTSTR fileName1, LPCTSTR fileName2);
