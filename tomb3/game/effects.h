#pragma once
#include "../global/vars.h"

void inject_effects(bool replace);

void LaraBreath(ITEM_INFO* item);

#define effect_routines (*(void(__cdecl*(*)[60])(ITEM_INFO*)) 0x004C5478)

#define WadeSplash	( (void(__cdecl*)(ITEM_INFO*, long, long)) 0x0042E9F0 )
#define Splash	( (void(__cdecl*)(ITEM_INFO*)) 0x0042E8C0 )
#define DoBloodSplat	( (short(__cdecl*)(long, long, long, short, short, short)) 0x0042E2C0 )
#define DoLotsOfBlood	( (void(__cdecl*)(long, long, long, short, short, short, long)) 0x0042E3B0 )
#define DoLotsOfBloodD	( (void(__cdecl*)(long, long, long, short, short, short, long)) 0x0042E460 )
#define SoundEffects	( (void(__cdecl*)()) 0x0042E200 )
#define ItemNearLara	( (long(__cdecl*)(PHD_3DPOS*, long)) 0x0042E170 )
#define Richochet	( (void(__cdecl*)(GAME_VECTOR*)) 0x0042E270 )