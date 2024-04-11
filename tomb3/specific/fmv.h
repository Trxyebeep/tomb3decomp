#pragma once
#include "../global/types.h"

bool __cdecl FMV_Init();
void __cdecl FMV_Cleanup();
bool __cdecl PlayFMV(LPCTSTR fileName);
bool __cdecl IntroFMV(LPCTSTR fileName1, LPCTSTR fileName2);
void __cdecl WinPlayFMV(LPCTSTR fileName, bool isPlayback);

extern long fmv_playing;
