#pragma once
#include "../global/types.h"

bool __cdecl FMV_Init();
void __cdecl FMV_Cleanup();
bool __cdecl FMV_Play(LPCTSTR fileName);
bool __cdecl FMV_PlayIntro(LPCTSTR fileName1, LPCTSTR fileName2);
void __cdecl FFPlayFMV(LPCTSTR fileName, bool isPlayback);

extern long fmv_playing;
