#pragma once
#include "math_tbls.h"

/*typedefs*/
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;

#define SQUARE(x) ((x)*(x))
#define	TRIGMULT2(a, b)	(((a) * (b)) >> W2V_SHIFT)
#define	TRIGMULT3(a, b, c)	(TRIGMULT2((TRIGMULT2(a, b)), c))
#define key_pressed(x) (keymap[x] & 0x80)
#define RGBA(r, g, b, a)	((a << 24) | (r << 16) | (g << 8) | (b))

//S_DrawSprite flags
#define SPR_RGB(r, g, b)	((r) | ((g) << 8) | ((b) << 16))
#define SPR_ABS				0x1000000
#define SPR_SEMITRANS		0x2000000
#define SPR_SCALE			0x4000000
#define SPR_SHADE			0x8000000
#define SPR_TINT			0x10000000
#define SPR_BLEND_ADD		0x20000000
#define SPR_BLEND_SUB		0x40000000

/********************DX defs********************/
#if (DIRECT3D_VERSION >= 0x900)
#define LPDIRECT3DX				LPDIRECT3D9
#define LPDIRECT3DDEVICEX		LPDIRECT3DDEVICE9
#define LPDIRECTDRAWSURFACEX	LPDIRECT3DSURFACE9
#define TEXHANDLE				LPDIRECT3DTEXTURE9
#define DDSURFACEDESCX			D3DLOCKED_RECT
#define LPDIRECT3DTEXTUREX		LPDIRECT3DTEXTURE9
#define VTXBUF_LEN				512
#define D3DFVF_TLVERTEX			(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1)
#define RGBA_SETALPHA(rgba, x)	(((x) << 24) | ((rgba) & 0x00ffffff))
#define RGBA_GETALPHA(rgb)		((rgb) >> 24)
#define RGBA_GETRED(rgb)		(((rgb) >> 16) & 0xff)
#define RGBA_GETGREEN(rgb)		(((rgb) >> 8) & 0xff)
#define RGBA_GETBLUE(rgb)		((rgb) & 0xff)
#define RGBA_MAKE(r, g, b, a)	((D3DCOLOR) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b)))
#define RGB_GETRED(rgb)			(((rgb) >> 16) & 0xff)
#define RGB_GETGREEN(rgb)		(((rgb) >> 8) & 0xff)
#define RGB_GETBLUE(rgb)		((rgb) & 0xff)
#define RGB_MAKE(r, g, b)		((D3DCOLOR) (((r) << 16) | ((g) << 8) | (b)))
#else
#define LPDIRECT3DX				LPDIRECT3D2
#define LPDIRECT3DDEVICEX		LPDIRECT3DDEVICE2
#define LPDIRECTDRAWSURFACEX	LPDIRECTDRAWSURFACE3
#define TEXHANDLE				D3DTEXTUREHANDLE
#define DDSURFACEDESCX			DDSURFACEDESC
#define LPDIRECT3DTEXTUREX		LPDIRECT3DTEXTURE2
#define LPDIRECTDRAWX			LPDIRECTDRAW2
#define LPDIRECT3DVIEWPORTX		LPDIRECT3DVIEWPORT2
#define D3DMATERIALX			D3DMATERIAL
#define LPDIRECT3DMATERIALX		LPDIRECT3DMATERIAL2
#endif

#define LPDDSURFACEDESCX		DDSURFACEDESCX*
#define DDSCAPSX				DDSCAPS
#define LPDDSCAPSX				DDSCAPSX*
#define LPDIRECTINPUTX			LPDIRECTINPUT8
#define LPDIRECTINPUTDEVICEX	LPDIRECTINPUTDEVICE8

#if (DIRECT3D_VERSION < 0x900)
#define DDSGUID					IID_IDirectDrawSurface3
#define DDGUID					IID_IDirectDraw2
#define D3DGUID					IID_IDirect3D2
#define D3DTEXGUID				IID_IDirect3DTexture2
#endif
#define DIGUID					IID_IDirectInput8
#define DSNGUID					IID_IDirectSoundNotify
/***********************************************/

//constants
#define NO_HEIGHT		-32512
#define DONT_TARGET		-16384
#define NO_ITEM			-1
#define W2V_SHIFT		14
#define NO_ROOM			255
#define WALL_SHIFT		10
#define WALL_SIZE		(1 << WALL_SHIFT)
#define WALL_MASK		(WALL_SIZE - 1)
#define MAX_LOT			20		//was 5
#define MAX_NONLOT		20		//was 12

#define MAX_BUCKETS		6
#define BUCKET_EXTRA	32
#define BUCKET_VERTS	(256 + BUCKET_EXTRA)

#define MAX_TLVERTICES	0x12000	//*8
#define MAX_SORTLISTS	0x17700	//*8
#define MAX_VBUF		12000	//*8
#define MAX_VINFO		320		//*8
#define MAX_TPAGES		128		//*4
#define MAX_TINFOS		0x4000	//*4

#define MAX_STATICS		256		//was 50
#define MAX_ITEMS		1024	//was 256
#define NLAYOUTKEYS		15
#define MAX_WEATHER		256
#define MAX_WEATHER_ALIVE	16	//was 8

#define MALLOC_SIZE	15000000	//15 MB (was around 3.6 MB)

#define FRAMES_PER_SECOND	30
#define TICKS_PER_FRAME		2
#define TICKS_PER_SECOND	(FRAMES_PER_SECOND * TICKS_PER_FRAME)

/*enums*/
enum texture_flags
{
	TF_EMPTY = 0,
	TF_USED = 1 << 0,
	TF_UNUSED = 1 << 1,
	TF_UNUSED2 = 1 << 2,
	TF_LEVELTEX = 1 << 3,
	TF_PICTEX = 1 << 4
};

enum death_tiles
{
	DEATH_LAVA,
	DEATH_RAPIDS,
	DEATH_ELECTRIC
};

enum bite_offsets
{
	OILRED_BITE,
	WHITE_SOLDIER_BITE,
	SWAT_GUN_BITE,
	SWAT_GUN_LASER_BITE,
	AUTOGUN_LEFT_BITE,
	AUTOGUN_RIGHT_BITE,
	ARMYSMG_GUN_BITE
};

enum target_type
{
	NO_TARGET,
	PRIME_TARGET,
	SECONDARY_TARGET
};

enum ai_bits
{
	GUARD = 1 << 0,
	AMBUSH = 1 << 1,
	PATROL1 = 1 << 2,
	MODIFY = 1 << 3,
	FOLLOW = 1 << 4
};

enum spark_flags
{
	SF_NONE =			0x0,
	SF_UNUSED1 =		0x1,
	SF_SCALE =			0x2,	//scale using sptr->Scalar
	SF_BLOOD =			0x4,	//set for blood only, to avoid killing it in GetFreeSpark if no free slots are found.
	SF_DEF =			0x8,	//use sptr->Def for the drawn sprite (otherwise do flat quad)
	SF_ROTATE =			0x10,	//rotate the drawn sprite (only supported for sparks with SF_DEF)
	SF_UNUSED2 =		0x20,
	SF_FX =				0x40,	//spark is attached to an effect
	SF_ITEM =			0x80,	//spark is attached to an item
	SF_OUTSIDE =		0x100,	//spark is affected by wind
	SF_ALTDEF =			0x200,	//alters sptr->Def based on spark colors (for smoke)
	SF_ATTACHEDPOS =	0x400,	//spark uses the position of the FX/ITEM it is attached to
	SF_UNWATER =		0x800,	//for underwater explosions to create bubbles etc.
	SF_ATTACHEDNODE =	0x1000,	//spark is attached to an item node, uses NodeOffsets
	SF_GREEN =			0x2000,	//turns the spark into a green-ish blue (for explosions only)
};

enum status_codes
{
	RNG_OPENING,
	RNG_OPEN,
	RNG_CLOSING,
	RNG_MAIN2OPTION,
	RNG_MAIN2KEYS,
	RNG_KEYS2MAIN,
	RNG_OPTION2MAIN,
	RNG_SELECTING,
	RNG_SELECTED,
	RNG_DESELECTING,
	RNG_DESELECT,
	RNG_CLOSING_ITEM,
	RNG_EXITING_INVENTORY,
	RNG_DONE
};

enum poly_types
{
	//original ones TBD

	POLYTYPE_HEALTHBAR = 96,
	POLYTYPE_AIRBAR = 97,
	POLYTYPE_DASHBAR = 98,
	POLYTYPE_COLDBAR = 99
};

enum inv_modes
{
	INV_GAME_MODE,
	INV_TITLE_MODE,
	INV_KEYS_MODE,
	INV_SAVE_MODE,
	INV_LOAD_MODE,
	INV_DEATH_MODE,
	INV_FMV_MODE,
	INV_LEVELSELECT_MODE
};

enum fire_types
{
	BIG_FIRE,
	SMALL_FIRE,
	JET_FIRE,
	SIDE_FIRE
};

enum draw_types
{
	DT_POLY_GT = 9,				//Gouraud + Textured
	DT_POLY_WGT = 10,			//Gouraud + Textured + Color key
	DT_POLY_G = 11,				//Gouraud
	DT_LINE_SOLID = 12,			//Solid Line
	DT_POLY_GA = 13,			//Gouraud + Alpha
	DT_POLY_WGTA = 14,			//Gouraud + Textured + Color key + Alpha
	DT_POLY_COLSUB = 15,		//colsub
	DT_POLY_GTA = 16,			//Gouraud + Textured + Alpha
	DT_LINE_ALPHA = 17,			//line + color key + alpha
};

enum T_flags
{
	T_TOPALIGN,
	T_LEFTALIGN = 0,
	T_ACTIVE = 1 << 0,
	T_FLASH = 1 << 1,
	T_ROTATE_H = 1 << 2,
	T_ROTATE_V = 1 << 3,
	T_CENTRE_H = 1 << 4,
	T_CENTRE_V = 1 << 5,
	T_RIGHTALIGN = 1 << 7,
	T_BOTTOMALIGN = 1 << 8,
	T_ADDBACKGROUND = 1 << 9,
	T_ADDOUTLINE = 1 << 10,
	T_RIGHTJUSTIFY = 1 << 11,
};

enum R_flags	//requester flags
{
	R_CENTRE = 0,
	R_USE = 1 << 0,
	R_LEFTALIGN = 1 << 1,
	R_RIGHTALIGN = 1 << 2,
	R_HEADING = 1 << 3,
	R_BEST_TIME = 1 << 4,
	R_NORMAL_TIME = 1 << 5,
	R_NO_TIME = 1 << 6
};

enum mood_type
{
	BORED_MOOD,
	ATTACK_MOOD,
	ESCAPE_MOOD,
	STALK_MOOD,
};

enum title_options
{
	STARTGAME = 0,
	STARTSAVEDGAME = 0x100,
	STARTCINE = 0x200,
	STARTFMV = 0x300,
	STARTDEMO = 0x400,
	EXIT_TO_TITLE = 0x500,
	LEVELCOMPLETE = 0x600,
	EXITGAME = 0x700,
	EXIT_TO_OPTION = 0x800,
	TITLE_DESELECT = 0x900,
	DOSTORYSOFAR = 0xA00,
	DOLEVELSELECT = 0xB00
};

enum game_mode
{
	GAMEMODE_NOT_IN_GAME,
	GAMEMODE_IN_GAME,
	GAMEMODE_IN_DEMO,
	GAMEMODE_IN_CUTSCENE
};

enum command_types
{
	COMMAND_NULL,
	COMMAND_MOVE_ORIGIN,
	COMMAND_JUMP_VELOCITY,
	COMMAND_ATTACK_READY,
	COMMAND_DEACTIVATE,
	COMMAND_SOUND_FX,
	COMMAND_EFFECT
};

enum input_buttons
{
	IN_NONE = 0x0,
	IN_FORWARD = 0x1,
	IN_BACK = 0x2,
	IN_LEFT = 0x4,
	IN_RIGHT = 0x8,
	IN_JUMP = 0x10,
	IN_DRAW = 0x20,
	IN_ACTION = 0x40,
	IN_WALK = 0x80,
	IN_OPTION = 0x100,
	IN_LOOK = 0x200,
	IN_LSTEP = 0x400,
	IN_RSTEP = 0x800,
	IN_ROLL = 0x1000,
	IN_PAUSE = 0x2000,
	IN_A = 0x4000,
	IN_B = 0x8000,
	IN_C = 0x10000,
	IN_D = 0x20000,
	IN_E = 0x40000,
	IN_FLARE = 0x80000,
	IN_SELECT = 0x100000,
	IN_DESELECT = 0x200000,
	IN_SAVE = 0x400000,
	IN_LOAD = 0x800000,
	IN_STEPSHIFT = 0x1000000,
	IN_LOOKLEFT = 0x2000000,
	IN_LOOKRIGHT = 0x4000000,
	IN_LOOKFORWARD = 0x8000000,
	IN_LOOKBACK = 0x10000000,
	IN_DUCK = 0x20000000,
	IN_SPRINT = 0x40000000,
	IN_ALL = 0xFFFFFFFF
};

enum lara_gun_status
{
	LG_ARMLESS,
	LG_HANDSBUSY,
	LG_DRAW,
	LG_UNDRAW,
	LG_READY,
	LG_SPECIAL,
};

enum collision_types
{
	CT_NONE = 0x0,
	CT_FRONT = 0x1,
	CT_LEFT = 0x2,
	CT_RIGHT = 0x4,
	CT_TOP = 0x8,
	CT_TOP_FRONT = 0x10,
	CT_CLAMP = 0x20
};

enum room_flags
{
	ROOM_UNDERWATER = 0x1,
	ROOM_OUTSIDE = 0x8,
	ROOM_DYNAMIC_LIT = 0x10,
	ROOM_NOT_INSIDE = 0x20,
	ROOM_INSIDE = 0x40,
	ROOM_SWAMP = 0x80
};

enum lara_water_states
{
	LARA_ABOVEWATER,
	LARA_UNDERWATER,
	LARA_SURFACE,
	LARA_CHEAT,
	LARA_WADE
};

enum camera_type
{
	CHASE_CAMERA,
	FIXED_CAMERA,
	LOOK_CAMERA,
	COMBAT_CAMERA,
	CINEMATIC_CAMERA,
	HEAVY_CAMERA
};

enum height_types
{
	WALL,
	SMALL_SLOPE,
	BIG_SLOPE,
	DIAGONAL,
	SPLIT_TRI
};

enum quadrant_names
{
	NORTH,
	EAST,
	SOUTH,
	WEST
};

enum lara_gun_types
{
	LG_UNARMED,
	LG_PISTOLS,
	LG_MAGNUMS,
	LG_UZIS,
	LG_SHOTGUN,
	LG_M16,
	LG_ROCKET,
	LG_GRENADE,
	LG_HARPOON,
	LG_FLARE,
	LG_SKIDOO,
	NUM_WEAPONS
};

enum floor_types
{
	FLOOR_TYPE,
	DOOR_TYPE,
	TILT_TYPE,
	ROOF_TYPE,
	TRIGGER_TYPE,
	LAVA_TYPE,
	CLIMB_TYPE,
	SPLIT1,
	SPLIT2,
	SPLIT3,
	SPLIT4,
	NOCOLF1T,
	NOCOLF1B,
	NOCOLF2T,
	NOCOLF2B,
	NOCOLC1T,
	NOCOLC1B,
	NOCOLC2T,
	NOCOLC2B,
	MONKEY_TYPE,
	MINEL_TYPE,
	MINER_TYPE
};

enum trigger_types
{
	TRIGGER,
	PAD,
	SWITCH,
	KEY,
	PICKUP,
	HEAVY,
	ANTIPAD,
	COMBAT,
	DUMMY,
	ANTITRIGGER
};

enum trigobj_types
{
	TO_OBJECT,
	TO_CAMERA,
	TO_SINK,
	TO_FLIPMAP,
	TO_FLIPON,
	TO_FLIPOFF,
	TO_TARGET,
	TO_FINISH,
	TO_CD,
	TO_FLIPEFFECT,
	TO_SECRET,
	TO_BODYBAG
};

enum LARA_MESHES
{
	HIPS,
	THIGH_L,
	CALF_L,
	FOOT_L,
	THIGH_R,
	CALF_R,
	FOOT_R,
	TORSO,
	UARM_R,
	LARM_R,
	HAND_R,
	UARM_L,
	LARM_L,
	HAND_L,
	HEAD,
	NUM_LARA_MESHES
};

enum sort_type
{
	MID_SORT,
	FAR_SORT,
	BACK_SORT
};

enum matrix_indices
{
	M00, M01, M02, M03,
	M10, M11, M12, M13,
	M20, M21, M22, M23,

	indices_count
};

enum item_status
{
	ITEM_INACTIVE,
	ITEM_ACTIVE,
	ITEM_DEACTIVATED,
	ITEM_INVISIBLE
};

enum item_flags
{
	IFL_TRIGGERED = 0x20,
	IFL_SWITCH_ONESHOT = 0x40,	//oneshot for switch items
	IFL_ANTITRIGGER_ONESHOT = 0x80,	//oneshot for antitriggers
	IFL_INVISIBLE = 0x100,	//also used as oneshot for everything else
	IFL_CODEBITS = 0x3E00,
	IFL_REVERSE = 0x4000,
	IFL_CLEARBODY = 0x8000
};

enum sfx_options
{
	SFX_DEFAULT = 0,
	SFX_WATER = 1,
	SFX_ALWAYS = 2,
	SFX_SETPITCH = 4
};

enum sfx_types
{
	SFX_LANDANDWATER,
	SFX_LANDONLY = 0x4000,
	SFX_WATERONLY = 0x8000
};

/*structs*/
#if (DIRECT3D_VERSION >= 0x900)
struct D3DTLVERTEX
{
	D3DVALUE sx;
	D3DVALUE sy;
	D3DVALUE sz;
	D3DVALUE rhw;
	D3DCOLOR color;
	D3DCOLOR specular;
	D3DVALUE tu;
	D3DVALUE tv;
};
#endif

#pragma pack(push, 1)
struct GOURAUD_FILL	//not og
{
	ulong clr[4][4];
};

struct GOURAUD_OUTLINE
{
	ulong clr[9];
};

struct PHD_VECTOR
{
	long x;
	long y;
	long z;
};

struct PHD_3DPOS
{
	long x_pos;
	long y_pos;
	long z_pos;
	short x_rot;
	short y_rot;
	short z_rot;
};

struct GAME_VECTOR
{
	long x;
	long y;
	long z;
	short room_number;
	short box_number;
};

struct SPHERE
{
	long x;
	long y;
	long z;
	long r;
};

struct OBJECT_VECTOR
{
	long x;
	long y;
	long z;
	short data;
	short flags;
};

struct FX_INFO
{
	PHD_3DPOS pos;
	short room_number;
	short object_number;
	short next_fx;
	short next_active;
	short speed;
	short fallspeed;
	short frame_number;
	short counter;
	short shade;
	short flag1;
	short flag2;
};

struct LARA_ARM
{
	short* frame_base;
	short frame_number;
	short anim_number;
	short lock;
	short y_rot;
	short x_rot;
	short z_rot;
	short flash_gun;
};

struct ITEM_LIGHT
{
	PHD_VECTOR sun;
	PHD_VECTOR bulb;
	PHD_VECTOR dynamic;
	uchar sunr;
	uchar sung;
	uchar sunb;
	char init;
	uchar bulbr;
	uchar bulbg;
	uchar bulbb;
	uchar ambient;
	uchar dynamicr;
	uchar dynamicg;
	uchar dynamicb;
	uchar pad2;
};

struct ITEM_INFO
{
	long floor;
	ulong touch_bits;
	ulong mesh_bits;
	short object_number;
	short current_anim_state;
	short goal_anim_state;
	short required_anim_state;
	short anim_number;
	short frame_number;
	short room_number;
	short next_item;
	short next_active;
	short speed;
	short fallspeed;
	short hit_points;
	ushort box_number;
	short timer;
	short flags;
	short shade;
	short shadeB;
	short carried_item;
	short after_death;
	ushort fired_weapon;
	short item_flags[4];
	void* data;
	PHD_3DPOS pos;
	ITEM_LIGHT il;
	ushort active : 1;
	ushort status : 2;
	ushort gravity_status : 1;
	ushort hit_status : 1;
	ushort collidable : 1;
	ushort looked_at : 1;
	ushort dynamic_light : 1;
	ushort clear_body : 1;
	ushort ai_bits : 5;
	ushort really_active : 1;
};

struct BOX_NODE
{
	short exit_box;
	ushort search_number;
	short next_expansion;
	short box_number;
};

struct LOT_INFO
{
	BOX_NODE* node;
	short head;
	short tail;
	ushort search_number;
	ushort block_mask;
	short step;
	short drop;
	short fly;
	short zone_count;
	short target_box;
	short required_box;
	PHD_VECTOR target;
};

struct CREATURE_INFO
{
	short joint_rotation[4];
	short maximum_turn;
	short flags;
	ushort alerted : 1;
	ushort head_left : 1;
	ushort head_right : 1;
	ushort reached_goal : 1;
	ushort hurt_by_lara : 1;
	ushort patrol2 : 1;
	mood_type mood;
	short item_num;
	PHD_VECTOR target;
	ITEM_INFO* enemy;
	LOT_INFO LOT;
};

struct AMMO_INFO
{
	long ammo;
};

struct LARA_INFO
{
	short item_number;
	short gun_status;
	short gun_type;
	short request_gun_type;
	short last_gun_type;
	short calc_fallspeed;
	short water_status;
	short climb_status;
	short pose_count;
	short hit_frame;
	short hit_direction;
	short air;
	short dive_count;
	short death_count;
	short current_active;
	short current_xvel;
	short current_yvel;
	short current_zvel;
	short spaz_effect_count;
	short flare_age;
	short skidoo;
	short weapon_item;
	short back_gun;
	short flare_frame;
	short poisoned;
	short electric;
	ushort flare_control_left : 1;
	ushort flare_control_right : 1;
	ushort extra_anim : 1;
	ushort look : 1;
	ushort burn : 1;
	ushort keep_ducked : 1;
	ushort CanMonkeySwing : 1;
	ushort MineL : 1;
	ushort MineR : 1;
	ushort BurnGreen : 1;
	ushort IsDucked : 1;
	ushort has_fired : 1;
	long water_surface_dist;
	PHD_VECTOR last_pos;
	FX_INFO* spaz_effect;
	long mesh_effects;
	short* mesh_ptrs[15];
	ITEM_INFO* target;
	short target_angles[2];
	short turn_rate;
	short move_angle;
	short head_y_rot;
	short head_x_rot;
	short head_z_rot;
	short torso_y_rot;
	short torso_x_rot;
	short torso_z_rot;
	LARA_ARM left_arm;
	LARA_ARM right_arm;
	AMMO_INFO pistols;
	AMMO_INFO magnums;
	AMMO_INFO uzis;
	AMMO_INFO shotgun;
	AMMO_INFO harpoon;
	AMMO_INFO rocket;
	AMMO_INFO grenade;
	AMMO_INFO m16;
	CREATURE_INFO* creature;
};

struct COLL_INFO
{
	long mid_floor;
	long mid_ceiling;
	long mid_type;
	long front_floor;
	long front_ceiling;
	long front_type;
	long left_floor;
	long left_ceiling;
	long left_type;
	long right_floor;
	long right_ceiling;
	long right_type;
	long left_floor2;
	long left_ceiling2;
	long left_type2;
	long right_floor2;
	long right_ceiling2;
	long right_type2;
	long radius;
	long bad_pos;
	long bad_neg;
	long bad_ceiling;
	PHD_VECTOR shift;
	PHD_VECTOR old;
	short old_anim_state;
	short old_anim_number;
	short old_frame_number;
	short facing;
	short quadrant;
	short coll_type;
	short* trigger;
	char tilt_x;
	char tilt_z;
	char hit_by_baddie;
	char hit_static;
	ushort slopes_are_walls : 2;
	ushort slopes_are_pits : 1;
	ushort lava_is_pit : 1;
	ushort enable_baddie_push : 1;
	ushort enable_spaz : 1;
	ushort hit_ceiling : 1;
};

struct FLOOR_INFO
{
	ushort index;
	ushort fx : 4;
	ushort box : 11;
	ushort stopper : 1;
	uchar pit_room;
	char floor;
	uchar sky_room;
	char ceiling;
};

struct ANIM_STRUCT
{
	short* frame_ptr;
	short interpolation;
	short current_anim_state;
	long velocity;
	long acceleration;
	short frame_base;
	short frame_end;
	short jump_anim_num;
	short jump_frame_num;
	short number_changes;
	short change_index;
	short number_commands;
	short command_index;
};

struct WAKE_PTS
{
	long x[2];
	long y;
	long z[2];
	short xvel[2];
	short zvel[2];
	uchar life;
	uchar pad[3];
};

struct SUB_WAKE_PTS
{
	long x[2];
	long y[2];
	long z[2];
	uchar life;
	uchar pad[3];
};

struct SUNLIGHT
{
	short nx;
	short ny;
	short nz;
	short pad;
};

struct SPOTLIGHT
{
	long intensity;
	long falloff;
};

struct LIGHT_INFO
{
	long x;
	long y;
	long z;
	uchar r;
	uchar g;
	uchar b;
	uchar type;

	union
	{
		SUNLIGHT sun;
		SPOTLIGHT spot;
	};
};

struct MESH_INFO
{
	long x;
	long y;
	long z;
	short y_rot;
	short shade;
	short shadeB;
	short static_number;
};

struct ROOM_INFO
{
	short* data;
	short* door;
	FLOOR_INFO* floor;
	LIGHT_INFO* light;
	MESH_INFO* mesh;
	long x;
	long y;
	long z;
	long minfloor;
	long maxceiling;
	short x_size;
	short y_size;
	short ambient;
	short lighting;
	short num_lights;
	short num_meshes;
	short ReverbType;
	char MeshEffect;
	char bound_active;
	short left;
	short right;
	short top;
	short bottom;
	short test_left;
	short test_right;
	short test_top;
	short test_bottom;
	short item_number;
	short fx_number;
	short flipped_room;
	ushort flags;
};

struct CAMERA_INFO
{
	GAME_VECTOR pos;
	GAME_VECTOR target;
	camera_type type;
	camera_type old_type;
	long shift;
	long flags;
	long fixed_camera;
	long number_frames;
	long bounce;
	long underwater;
	long target_distance;
	long target_square;
	short target_angle;
	short actual_angle;
	short target_elevation;
	short box;
	short number;
	short last;
	short timer;
	short speed;
	long min_ypos;
	ITEM_INFO* item;
	ITEM_INFO* last_item;
	OBJECT_VECTOR* fixed;
	long mike_at_lara;
	PHD_VECTOR mike_pos;
};

struct GAMEFLOW_INFO
{
	long firstOption;
	long title_replace;
	long ondeath_demo_mode;
	long ondeath_ingame;
	long noinput_time;
	long on_demo_interrupt;
	long on_demo_end;
	long unused1[9];
	short num_levels;
	short num_picfiles;
	short num_titlefiles;
	short num_fmvfiles;
	short num_cutfiles;
	short num_demos;
	short title_track;
	short singlelevel;
	short unused2[16];
	ushort demoversion : 1;
	ushort title_disabled : 1;
	ushort cheatmodecheck_disabled : 1;
	ushort noinput_timeout : 1;
	ushort loadsave_disabled : 1;
	ushort screensizing_disabled : 1;
	ushort lockout_optionring : 1;
	ushort dozy_cheat_enabled : 1;
	ushort cyphered_strings : 1;
	ushort gym_enabled : 1;
	ushort play_any_level : 1;
	ushort cheat_enable : 1;
	ushort securitytag : 1;
#if 0
	ushort unused3[3];
#else
	//NEW STUFF
	ushort unused3[2];
	ushort map_enabled : 1;
	ushort globe_enabled : 1;
	ushort force_water_color : 1;
#endif
	uchar cypher_code;
	uchar language;
	uchar secret_track;
	uchar stats_track;
	char pads[4];
};

struct START_INFO
{
	ushort pistol_ammo;
	ushort magnum_ammo;
	ushort uzi_ammo;
	ushort shotgun_ammo;
	ushort m16_ammo;
	ushort rocket_ammo;
	ushort harpoon_ammo;
	ushort grenade_ammo;
	uchar num_medis;
	uchar num_big_medis;
	uchar num_scions;
	uchar num_flares;
	uchar num_sgcrystals;
	char gun_status;
	char gun_type;
	ushort available : 1;
	ushort got_pistols : 1;
	ushort got_magnums : 1;
	ushort got_uzis : 1;
	ushort got_shotgun : 1;
	ushort got_m16 : 1;
	ushort got_rocket : 1;
	ushort got_grenade : 1;
	ushort got_harpoon : 1;
	ushort secrets;
	uchar num_icon1;
	uchar num_icon2;
	uchar num_icon3;
	uchar num_icon4;
	ulong timer;
	ulong ammo_used;
	ulong ammo_hit;
	ulong distance_travelled;
	ushort kills;
	uchar secrets_found;
	uchar health_used;
};

struct SAVEGAME_INFO
{
	ulong best_assault_times[10];
	ulong best_quadbike_times[10];
	ulong QuadbikeKeyFlag;
	START_INFO start[25];
	ulong timer;
	ulong ammo_used;
	ulong ammo_hit;
	ulong distance_travelled;
	ushort kills;
	uchar secrets;
	uchar health_used;
	ulong cp_timer;
	ulong cp_ammo_used;
	ulong cp_ammo_hit;
	ulong cp_distance_travelled;
	ushort cp_kills;
	uchar cp_secrets;
	uchar cp_health_used;
	short current_level;
	uchar bonus_flag;
	uchar num_pickup1;
	uchar num_pickup2;
	uchar num_puzzle1;
	uchar num_puzzle2;
	uchar num_puzzle3;
	uchar num_puzzle4;
	uchar num_key1;
	uchar num_key2;
	uchar num_key3;
	uchar num_key4;
	char checksum;
	char GameComplete;
	ulong mid_level_save : 1;
	ulong AfterAdventureSave : 1;
	ulong WorldRequired : 3;
	ulong IndiaComplete : 1;
	ulong SPacificComplete : 1;
	ulong LondonComplete : 1;
	ulong NevadaComplete : 1;
	ulong AntarcticaComplete : 1;
	ulong PeruComplete : 1;
	ulong AfterIndia : 3;
	ulong AfterSPacific : 3;
	ulong AfterLondon : 3;
	ulong AfterNevada : 3;
	char buffer[12416];
};

struct OBJECT_INFO
{
	short nmeshes;
	short mesh_index;
	long bone_index;
	short* frame_base;
	void (*initialise)(short item_number);
	void (*control)(short item_number);
	void (*floor)(ITEM_INFO* item, long x, long y, long z, long* height);
	void (*ceiling)(ITEM_INFO* item, long x, long y, long z, long* height);
	void (*draw_routine)(ITEM_INFO* item);
	void (*collision)(short item_num, ITEM_INFO* laraitem, COLL_INFO* coll);
	short anim_index;
	short hit_points;
	short pivot_length;
	short radius;
	short shadow_size;
	ushort bite_offset;	//bite_offsets enum
	ushort loaded : 1;
	ushort intelligent : 1;
	ushort non_lot : 1;
	ushort save_position : 1;
	ushort save_hitpoints : 1;
	ushort save_flags : 1;
	ushort save_anim : 1;
	ushort semi_transparent : 1;
	ushort water_creature : 1;
};

struct TEXTSTRING
{
	long flags;
	ushort textflags;
	ushort bgndflags;
	ushort outlflags;
	short xpos;
	short ypos;
	short zpos;
	short letterSpacing;
	short wordSpacing;
	short flashRate;
	short flashCount;
	short bgndColour;
	GOURAUD_FILL* bgndGour;
	short outlColour;
	GOURAUD_OUTLINE* outlGour;
	short bgndSizeX;
	short bgndSizeY;
	short bgndOffX;
	short bgndOffY;
	short bgndOffZ;
	long scaleH;
	long scaleV;
	short Colour;
	char* string;
};

struct DISPLAYPU
{
	short duration;
	short sprnum;
};

struct OLD_CAMERA
{
	short current_anim_state;
	short goal_anim_state;
	PHD_3DPOS pos;
	PHD_3DPOS pos2;
};

struct DYNAMIC
{
	long x;
	long y;
	long z;
	ushort falloff;
	uchar on;
	uchar r;
	uchar g;
	uchar b;
};

struct PISTOL_DEF
{
	short ObjectNum;
	char Draw1Anim2;
	char Draw1Anim;
	char Draw2Anim;
	char RecoilAnim;
};

struct WEAPON_INFO
{
	short lock_angles[4];
	short left_angles[4];
	short right_angles[4];
	short aim_speed;
	short shot_accuracy;
	short gun_height;
	short target_dist;
	char damage;
	char recoil_frame;
	char flash_time;
	char draw_frame;
	short sample_num;
};

struct PHD_VBUF
{
	float xv;
	float yv;
	float zv;
	float ooz;
	float xs;
	float ys;
	long z;
	char clip;
	ulong color;
	ushort u;
	ushort v;
};

struct VERTEX_INFO
{
	float x;
	float y;
	float ooz;
	float u;
	float v;
	float g;
	long vr;
	long vg;
	long vb;
};

struct DISPLAYMODE
{
	long w;
	long h;
	ulong bpp;
#if (DIRECT3D_VERSION < 0x900)
	bool bPalette;
	uchar rbpp;
	uchar gbpp;
	uchar bbpp;
	uchar abpp;
	DDSURFACEDESCX ddsd;
	uchar rshift;
	uchar gshift;
	uchar bshift;
	uchar ashift;
#endif
};

struct DXDIRECTSOUNDINFO
{
	char Name[256];
	char About[256];
	LPGUID lpGuid;
	GUID Guid;
};

#if (DIRECT3D_VERSION >= 0x900)
struct DIRECT3DINFO
{
	char Name[256];
	char About[256];
	LPGUID lpGuid;
	GUID Guid;
	D3DCAPS9 caps;
	long nDisplayMode;
	DISPLAYMODE* DisplayMode;
	ulong index;
};

struct DEVICEINFO
{
	ulong nD3DInfo;
	DIRECT3DINFO* D3DInfo;
	long nDSInfo;
	DXDIRECTSOUNDINFO* DSInfo;
	//Joystick enumeration stuff was here
};
#else
struct D3DTEXTUREINFO
{
	ulong bpp;
	bool bPalette;
	bool bAlpha;
	uchar rbpp;
	uchar gbpp;
	uchar bbpp;
	uchar abpp;
	DDSURFACEDESCX ddsd;
	DDPIXELFORMAT ddpf;
	uchar rshift;
	uchar gshift;
	uchar bshift;
	uchar ashift;
};

struct DIRECT3DINFO
{
	char Name[256];
	char About[256];
	LPGUID lpGuid;
	GUID Guid;
	D3DDEVICEDESC DeviceDesc;
	bool bAlpha;
	long nDisplayMode;
	DISPLAYMODE* DisplayMode;
	long nTexture;
	D3DTEXTUREINFO* Texture;
};

struct DIRECTDRAWINFO
{
	char Name[256];
	char About[256];
	LPGUID lpGuid;
	GUID Guid;
	DDCAPS DDCaps;
	long nDisplayMode;
	DISPLAYMODE* DisplayMode;
	long nD3DInfo;
	DIRECT3DINFO* D3DInfo;
};

struct DEVICEINFO
{
	long nDDInfo;
	DIRECTDRAWINFO* DDInfo;
	long nDSInfo;
	DXDIRECTSOUNDINFO* DSInfo;
	//Joystick enumeration stuff was here
};
#endif

struct DXCONFIG
{
#if (DIRECT3D_VERSION < 0x900)
	long nDD;
#endif
	long nD3D;
	long nDS;
	long nVMode;
#if (DIRECT3D_VERSION < 0x900)
	long D3DTF;
#endif
	long bZBuffer;
	long Dither;
	long Filter;
	long sound;
};

struct WINAPP
{
	WNDCLASS WindowClass;
	HWND WindowHandle;
	HINSTANCE hInstance;
	DEVICEINFO DeviceInfo;
	DXCONFIG DXConfig;
	DEVICEINFO* lpDeviceInfo;
	DXCONFIG* lpDXConfig;
#if (DIRECT3D_VERSION >= 0x900)
	LPDIRECT3DX D3D;
	LPDIRECT3DDEVICEX D3DDev;
	LPDIRECT3DVERTEXBUFFER9 DestVB;
	LPDIRECTDRAWSURFACEX CaptureBuffer;
	LPDIRECTDRAWSURFACEX PictureBuffer;
#else
	LPDIRECTDRAWX DDraw;
	LPDIRECT3DX D3D;
	LPDIRECT3DDEVICEX D3DDev;
	LPDIRECTDRAWSURFACEX FrontBuffer;
	LPDIRECTDRAWSURFACEX BackBuffer;
	LPDIRECTDRAWSURFACEX ZBuffer;
	LPDIRECTDRAWSURFACEX PictureBuffer;
	LPDIRECT3DVIEWPORTX D3DView;
	LPDIRECT3DMATERIALX D3DMaterial;
#endif
	RECT rScreen;
	RECT rViewport;
	ulong WindowStyle;
	bool Windowed;
	bool FFPlayLoaded;
	volatile bool bFocus;
	long nUVAdd;
#if (DIRECT3D_VERSION < 0x900)
	ulong nFrames;
#endif
	float fps;
};

struct LEADER_INFO
{
	short angle;
	uchar speed;
	uchar on;
	short angle_time;
	short speed_time;
	short Xrange;
	short Zrange;
	short Yrange;
};

struct FISH_INFO
{
	short x;
	short y;
	short z;
	ushort angle;
	short desty;
	char angadd;
	uchar speed;
	uchar acc;
	uchar swim;
};

struct PHDTEXTURESTRUCT
{
	ushort drawtype;
	ushort tpage;
	ushort u1;
	ushort v1;
	ushort u2;
	ushort v2;
	ushort u3;
	ushort v3;
	ushort u4;
	ushort v4;
};

struct SPARKS
{
	long x;
	long y;
	long z;
	short Xvel;
	short Yvel;
	short Zvel;
	short Gravity;
	short RotAng;
	short Flags;
	uchar sWidth;
	uchar dWidth;
	uchar sHeight;
	uchar dHeight;
	uchar Friction;
	uchar Width;
	uchar Height;
	uchar Scalar;
	uchar Def;
	char RotAdd;
	char MaxYvel;
	uchar On;
	uchar sR;
	uchar sG;
	uchar sB;
	uchar dR;
	uchar dG;
	uchar dB;
	uchar R;
	uchar G;
	uchar B;
	uchar ColFadeSpeed;
	uchar FadeToBlack;
	uchar sLife;
	uchar Life;
	uchar TransType;
	uchar extras;
	char Dynamic;
	uchar FxObj;
	uchar RoomNumber;
	uchar NodeNumber;
	uchar pad;
};

struct SP_DYNAMIC
{
	uchar On;
	uchar Falloff;
	uchar R;
	uchar G;
	uchar B;
	uchar Flags;
	uchar Pad[2];
};

#if (DIRECT3D_VERSION >= 0x900)
struct DXTEXTURE
{
	TEXHANDLE tex;
	long nWidth;
	long nHeight;
	ulong dwFlags;
};
#else
struct TEXTURE
{
	LPVOID DXTex;	//DXTEXTURE*
	LPDIRECTDRAWSURFACEX pSurf;
	LPDIRECTDRAWPALETTE pPalette;
	LPDIRECT3DTEXTUREX pTexture;
	TEXHANDLE handle;
	long num;
	ulong nFrames;
	ulong bpp;
};

struct DXTEXTURE
{
	LPDIRECTDRAWSURFACEX pSystemSurface;
	LPDIRECTDRAWPALETTE pPalette;
	long nWidth;
	long nHeight;
	ulong dwFlags;
	ulong* pData;
	TEXTURE* tex;
	ulong bpp;
};
#endif

struct TEXTUREBUCKET
{
	DXTEXTURE* TPage;
	long nVtx;
	D3DTLVERTEX vtx[BUCKET_VERTS];
};

struct POINT_INFO
{
	float xv;
	float yv;
	float zv;
	float ooz;
	float xs;
	float ys;
	float u;
	float v;
	float g;
	long vr;
	long vg;
	long vb;
};

struct PHDSPRITESTRUCT
{
	ushort tpage;
	ushort offset;
	ushort width;
	ushort height;
	short x1;
	short y1;
	short x2;
	short y2;
};

struct FOOTPRINT
{
	long x;
	long y;
	long z;
	short YRot;
	short Active;
};

struct STATIC_INFO
{
	short mesh_number;
	short flags;
	short x_minp;
	short x_maxp;
	short y_minp;
	short y_maxp;
	short z_minp;
	short z_maxp;
	short x_minc;
	short x_maxc;
	short y_minc;
	short y_maxc;
	short z_minc;
	short z_maxc;
};

struct HWCONFIG
{
	bool bDither;
	long nFilter;
	long nShadeMode;
	long nFillMode;
};

struct BITE_INFO
{
	long x;
	long y;
	long z;
	long mesh_num;
};

struct WATERTAB
{
	char shimmer;
	char choppy;
	uchar random;
	uchar abs;
};

struct EXPLOSION_VERTS
{
	short x;
	short z;
	long rgb;
};

struct EXPLOSION_RING
{
	short on;
	short life;
	short speed;
	short radius;
	short xrot;
	short zrot;
	long x;
	long y;
	long z;
	EXPLOSION_VERTS verts[16];
};

struct BIGGUNINFO
{
	short RotX;
	short RotY;
	short StartRotY;
	char Flags;
	char FireCount;
};

struct CHANGE_STRUCT
{
	short goal_anim_state;
	short number_ranges;
	short range_index;
};

struct RANGE_STRUCT
{
	short start_frame;
	short end_frame;
	short link_anim_num;
	short link_frame_num;
};

struct KAYAKINFO
{
	long Vel;
	long Rot;
	long FallSpeedF;
	long FallSpeedL;
	long FallSpeedR;
	long Water;
	PHD_3DPOS OldPos;
	char Turn;
	char Forward;
	char TrueWater;
	char Flags;
};

struct RIPPLE_STRUCT
{
	long x;
	long y;
	long z;
	char flags;
	uchar life;
	uchar size;
	uchar init;
};

struct SPLASH_VERTS
{
	short wx;
	short wy;
	short wz;
	short xv;
	long yv;
	short zv;
	short oxv;
	short ozv;
	uchar friction;
	uchar gravity;
};

struct SPLASH_STRUCT
{
	long x;
	long y;
	long z;
	char flags;
	uchar life;
	char pad[2];
	SPLASH_VERTS sv[48];
};

struct SPLASH_SETUP
{
	int x;
	int y;
	int z;
	short InnerXZoff;
	short InnerXZsize;
	short InnerYsize;
	short InnerXZvel;
	short InnerYvel;
	short InnerGravity;
	short InnerFriction;
	short MiddleXZoff;
	short MiddleXZsize;
	short MiddleYsize;
	short MiddleXZvel;
	short MiddleYvel;
	short MiddleGravity;
	short MiddleFriction;
	short OuterXZoff;
	short OuterXZsize;
	short OuterXZvel;
	short OuterFriction;
};

struct SUBINFO
{
	long Vel;
	long Rot;
	long RotX;
	short FanRot;
	char Flags;
	char WeaponTimer;
};

struct SHIELD_POINTS
{
	short x;
	short y;
	short z;
	uchar rsub;
	uchar gsub;
	uchar bsub;
	uchar pad[3];
	long rgb;
};

struct AI_INFO
{
	short zone_number;
	short enemy_zone;
	long distance;
	long ahead;
	long bite;
	short angle;
	short x_angle;
	short enemy_facing;
};

struct BOX_INFO
{
	uchar left;
	uchar right;
	uchar top;
	uchar bottom;
	short height;
	short overlap_index;
};

struct SAMPLE_INFO
{
	short number;
	uchar volume;
	char radius;
	char randomness;
	char pitch;
	short flags;
};

struct REQUEST_INFO
{
	ushort noselector : 1;
	ushort item;
	ushort selected;
	ushort vis_lines;
	ushort line_offset;
	ushort line_oldoffset;
	ushort pixwidth;
	ushort line_height;
	short xpos;
	short ypos;
	short zpos;
	short itemtextlen;
	char* itemtexts1;
	char* itemtexts2;
	ulong* itemtexts1_flags;
	ulong* itemtexts2_flags;
	ulong heading1_flags;
	ulong heading2_flags;
	ulong background_flags;
	ulong moreup_flags;
	ulong moredown_flags;
	ulong texts1_flags[24];
	ulong texts2_flags[24];
	TEXTSTRING* heading1text;
	TEXTSTRING* heading2text;
	TEXTSTRING* backgroundtext;
	TEXTSTRING* moreuptext;
	TEXTSTRING* moredowntext;
	TEXTSTRING* texts1[24];
	TEXTSTRING* texts2[24];
	char heading1_str[64];
	char heading2_str[64];
	long original_render_width;
	long original_render_height;
};

struct IMOTION_INFO
{
	short count;
	short status;
	short status_target;
	short radius_target;
	short radius_rate;
	short camera_ytarget;
	short camera_yrate;
	short camera_pitch_target;
	short camera_pitch_rate;
	short rotate_target;
	short rotate_rate;
	short item_ptxrot_target;
	short item_ptxrot_rate;
	short item_xrot_target;
	short item_xrot_rate;
	long item_ytrans_target;
	long item_ytrans_rate;
	long item_ztrans_target;
	long item_ztrans_rate;
	long misc;
};

struct INVENTORY_SPRITE
{
	short shape;
	short x;
	short y;
	short z;
	long param1;
	long param2;
	GOURAUD_FILL* grdptr;
	short sprnum;
};

struct INVENTORY_ITEM
{
	char* itemText;
	short object_number;
	short frames_total;
	short current_frame;
	short goal_frame;
	short open_frame;
	short anim_direction;
	short anim_speed;
	short anim_count;
	short pt_xrot_sel;
	short pt_xrot;
	short x_rot_sel;
	short x_rot_nosel;
	short x_rot;
	short y_rot_sel;
	short y_rot;
	long ytrans_sel;
	long ytrans;
	long ztrans_sel;
	long ztrans;
	ulong which_meshes;
	ulong drawn_meshes;
	short inv_pos;
	INVENTORY_SPRITE** sprlist;
	long misc_data[4];
};

struct RING_INFO
{
	INVENTORY_ITEM** list;
	short type;
	short radius;
	short camera_pitch;
	short rotating;
	short rot_count;
	short current_object;
	short target_object;
	short number_of_objects;
	short angle_adder;
	short rot_adder;
	short rot_adderL;
	short rot_adderR;
	PHD_3DPOS ringpos;
	PHD_3DPOS camerapos;
	PHD_VECTOR light;
	IMOTION_INFO* imo;
};

struct BOSS_STRUCT
{
	short attack_count;
	short death_count;
	uchar attack_flag;
	uchar attack_type;
	uchar attack_head_count;
	uchar ring_count;
	short explode_count;
	short LizmanItem;
	short LizmanRoom;
	short hp_counter;
	short dropped_icon;
	uchar charged;
	uchar dead;
	PHD_VECTOR BeamTarget;
};

struct RAINDROP
{
	long x;
	long y;
	long z;
	char xv;
	uchar yv;
	char zv;
	uchar life;
};

struct SNOWFLAKE
{
	long x;
	long old_roomflags : 1;
	long stopped : 1;
	long y : 30;
	long z;
	char xv;
	uchar yv;
	char zv;
	uchar life;
};

struct RES_TXT
{
	char res[20];
};

struct GLOBE_LEVEL
{
	short xrot;
	short yrot;
	short zrot;
	short txt;
};

struct BAT_STRUCT
{
	long x;
	long y;
	long z;
	short angle;
	short speed;
	uchar WingYoff;
	uchar flags;
	uchar life;
	uchar pad;
};

struct TRACK_INFO
{
	char name[260];
	ulong size;
	ulong offset;
};

struct SOUND_SLOT
{
	long nVolume;
	long nPan;
	long nSampleInfo;
	long nPitch;
};

struct BOAT_INFO
{
	long boat_turn;
	long left_fallspeed;
	long right_fallspeed;
	short tilt_angle;
	short extra_rotation;
	long water;
	long pitch;
	short prop_rot;
};

struct QUADINFO
{
	long Velocity;
	short FrontRot;
	short RearRot;
	long Revs;
	long EngineRevs;
	short track_mesh;
	long skidoo_turn;
	long left_fallspeed;
	long right_fallspeed;
	short momentum_angle;
	short extra_rotation;
	long pitch;
	uchar Flags;
};

struct CARTINFO
{
	long Speed;
	long MidPos;
	long FrontPos;
	long TurnX;
	long TurnZ;
	short TurnLen;
	short TurnRot;
	short YVel;
	short Gradient;
	char Flags;
	char StopDelay;
};

struct LIFT_INFO
{
	long start_height;
	long wait_time;
};

struct DOORPOS_DATA
{
	FLOOR_INFO* floor;
	FLOOR_INFO data;
	short block;
};

struct DOOR_DATA
{
	DOORPOS_DATA d1;
	DOORPOS_DATA d1flip;
	DOORPOS_DATA d2;
	DOORPOS_DATA d2flip;
};

struct SVECTOR
{
	short x;
	short y;
	short z;
	short pad;
};

// new stuff
struct FVECTOR
{
	float x;
	float y;
	float z;
};

enum t3_shadow_mode
{
	SHADOW_ORIGINAL,
	SHADOW_PSX,
	NSHADOW_MODES
};

enum t3_bar_mode
{
	BAR_ORIGINAL,
	BAR_PSX,
	NBAR_MODES
};

enum t3_summon_ring_style
{
	SRINGS_PC,
	SRINGS_PSX,
	SRINGS_IMPROVED_PC,
	NSRING_STYLES
};

enum t3_bar_pos
{
	BPOS_ORIGINAL,
	BPOS_IMPROVED,
	BPOS_PSX,
	NBAR_POS
};

enum t3_ammo_counter
{
	ACTR_PC,
	ACTR_PSX,
	NACTR_MODES
};

struct TOMB3_SAVE
{
	char RoomsVisited[255];
	short dash_timer;
	short exposure_meter;
	LEADER_INFO fish_leaders[8];
	FISH_INFO fishies[200];
	EXPLOSION_RING exp_rings[6];
	EXPLOSION_RING kb_rings[6];
	ulong lara_meshbits;
};

struct TOMB3_OPTIONS
{
	bool footprints;
	bool pickup_display;
	bool improved_rain;
	bool improved_lasers;
	bool uwdust;
	bool flexible_crawl;
	bool duck_roll;
	bool flexible_sprint;
	bool slide_to_run;
	bool kayak_mist;
	bool dozy;
	bool disable_gamma;
	bool disable_ckey;
	bool crawl_tilt;
	bool improved_poison_bar;
	bool custom_water_color;
	bool psx_text_colors;
	bool upv_wake;
	bool psx_fov;
	bool psx_boxes;
	bool psx_mono;
	bool psx_saving;
	bool psx_crystal_sfx;
	bool blue_crystal_light;
	bool improved_electricity;
	bool psx_contrast;
	bool gold;
	long shadow_mode;	//t3_shadow_mode enum
	long bar_mode;		//t3_bar_mode enum
	long sophia_rings;	//t3_summon_ring_style
	long bar_pos;		//t3_bar_pos enum
	long ammo_counter;	//t3_ammo_counter
	float GUI_Scale;
	float INV_Scale;
	float unwater_music_mute;
	float inv_music_mute;
};
#pragma pack(pop)