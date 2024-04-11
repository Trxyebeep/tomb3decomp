#include "../tomb3/pch.h"
#include "fmv.h"
#include "specific.h"
#include "winmain.h"
#include "file.h"
#include "input.h"
#include "audio.h"
#include "../tomb3/tomb3.h"

long fmv_playing;

#define GET_DLL_PROC(dll, proc) { \
	*(FARPROC *)&(proc) = GetProcAddress((dll), #proc); \
	if( proc == NULL ) throw #proc; \
}

#define FFPLAY_DLL_NAME "ffplay.dll"
static HMODULE hFFplay = NULL;

// Imports from ffplay.dll
static int(__stdcall* ffplay_init)(HWND, int, const char*);
static int(__stdcall* ffplay_play_video)(const char*, int, int, int, int);
static int(__stdcall* ffplay_cleanup)(void);

static const char videoExts[][4] = {
	"AVI",
	"MP4",
	"BIK",
	"RPL",
};

static bool FFplayInit() {
	if (hFFplay != NULL) {
		return true;
	}

	hFFplay = LoadLibrary(FFPLAY_DLL_NAME);
	if (hFFplay == NULL) {
		// failed to load DLL
		return false;
	}

	try {
		GET_DLL_PROC(hFFplay, ffplay_init);
		GET_DLL_PROC(hFFplay, ffplay_play_video);
		GET_DLL_PROC(hFFplay, ffplay_cleanup);
	}
	catch (LPCTSTR procName) {
		// failed to load one of the procs
		FreeLibrary(hFFplay);
		hFFplay = NULL;
		return false;
	}

	if (0 != ffplay_init(App.WindowHandle, 2, "winmm")) {
		// failed to init FFplay
		FreeLibrary(hFFplay);
		hFFplay = NULL;
		return false;
	}

	return true;
}

bool __cdecl FMV_Init() {
	if (FFplayInit()) {
		return true;
	}
	else {
		return false;
	}
}

void __cdecl FMV_Cleanup() {
	if (hFFplay != NULL) {
		ffplay_cleanup();
		FreeLibrary(hFFplay);
		hFFplay = NULL;
	}
}

bool __cdecl PlayFMV(LPCTSTR fileName) {
	LPCTSTR fullPath;

	if (!App.FFPlayLoaded)
		return 0;

	fmv_playing = 1;
	S_CDStop();
	ShowCursor(FALSE);
	WinFreeDX(0);

	fullPath = GetFullPath(fileName);
	WinPlayFMV(fullPath, true);

	fmv_playing = 0;
	if (!GtWindowClosed)
		WinDXInit(&App.DeviceInfo, &App.DXConfig, 0);

	ShowCursor(TRUE);
	return GtWindowClosed;
}

bool __cdecl IntroFMV(LPCTSTR fileName1, LPCTSTR fileName2) {
	LPCTSTR fullPath;

	if (!App.FFPlayLoaded)
		return 0;

	fmv_playing = 1;
	ShowCursor(FALSE);
	WinFreeDX(0);

	fullPath = GetFullPath(fileName1);
	WinPlayFMV(fullPath, true);

	fullPath = GetFullPath(fileName2);
	WinPlayFMV(fullPath, true);

	fmv_playing = 0;
	if (!GtWindowClosed)
		WinDXInit(&App.DeviceInfo, &App.DXConfig, 0);

	ShowCursor(TRUE);
	return GtWindowClosed;
}

void __cdecl WinPlayFMV(LPCTSTR fileName, bool isPlayback) {
	if (hFFplay != NULL) {
		char extFileName[256] = { 0 };
		char* extension;

		strncpy_s(extFileName, fileName, sizeof(extFileName) - 1);
		extension = PathFindExtension(extFileName);
		if (extension == NULL) {
			extension = strchr(extFileName, 0);
			*extension = '.';
		}
		for (unsigned int i = 0; i < sizeof(videoExts) / 4; ++i) {
			memcpy(extension + 1, videoExts[i], 4);
			if (INVALID_FILE_ATTRIBUTES != GetFileAttributes(extFileName)) {
				ffplay_play_video(extFileName, 0, 0, 0, 100);
				return;
			}
		}
		ffplay_play_video(fileName, 0, 0, 0, 100);
		return;
	}
}