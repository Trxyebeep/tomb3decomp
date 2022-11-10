#include "../tomb3/pch.h"
#include "sphere.h"
#include "../3dsystem/3d_gen.h"
#include "draw.h"

void InitInterpolate2(long frac, long rate)
{
	IM_rate = rate;
	IM_frac = frac;
	IMptr = &IMstack[384];
	memcpy(&IMstack[384], phd_mxptr, 48);
}

long GetSpheres(ITEM_INFO* item, SPHERE* ptr, long WorldSpace)
{
	OBJECT_INFO* obj;
	short** meshpp;
	long* bone;
	short* meshp;
	short* frame;
	short* rot;
	short* extra_rot;
	long x, y, z, poppush;

	if (!item)
		return 0;

	if (WorldSpace)
	{
		x = item->pos.x_pos;
		y = item->pos.y_pos;
		z = item->pos.z_pos;
		phd_PushUnitMatrix();
		phd_mxptr[M03] = 0;
		phd_mxptr[M13] = 0;
		phd_mxptr[M23] = 0;
	}
	else
	{
		z = 0;
		y = 0;
		x = 0;
		phd_PushMatrix();
		phd_TranslateAbs(item->pos.x_pos, item->pos.y_pos, item->pos.z_pos);
	}

	phd_RotYXZ(item->pos.y_rot, item->pos.x_rot, item->pos.z_rot);
	frame = GetBestFrame(item);
	phd_TranslateRel(frame[6], frame[7], frame[8]);
	rot = frame + 9;
	gar_RotYXZsuperpack(&rot, 0);
	obj = &objects[item->object_number];
	meshpp = &meshes[obj->mesh_index];
	meshp = *meshpp++;
	bone = &bones[obj->bone_index];

	phd_PushMatrix();
	phd_TranslateRel(meshp[0], meshp[1], meshp[2]);
	ptr->x = x + (phd_mxptr[M03] >> W2V_SHIFT);
	ptr->y = y + (phd_mxptr[M13] >> W2V_SHIFT);
	ptr->z = z + (phd_mxptr[M23] >> W2V_SHIFT);
	ptr->r = meshp[3];
	ptr++;
	phd_PopMatrix();

	extra_rot = (short*)item->data;

	for (int i = 0; i < obj->nmeshes - 1; i++, bone += 3)
	{
		poppush = *bone++;

		if (poppush & 1)
			phd_PopMatrix();

		if (poppush & 2)
			phd_PushMatrix();

		phd_TranslateRel(bone[0], bone[1], bone[2]);
		gar_RotYXZsuperpack(&rot, 0);

		if (poppush & 0x1C && extra_rot)
		{
			if (poppush & 8)
				phd_RotY(*extra_rot++);

			if (poppush & 4)
				phd_RotX(*extra_rot++);

			if (poppush & 0x10)
				phd_RotZ(*extra_rot++);
		}

		meshp = *meshpp++;
		phd_PushMatrix();
		phd_TranslateRel(meshp[0], meshp[1], meshp[2]);
		ptr->x = x + (phd_mxptr[M03] >> W2V_SHIFT);
		ptr->y = y + (phd_mxptr[M13] >> W2V_SHIFT);
		ptr->z = z + (phd_mxptr[M23] >> W2V_SHIFT);
		ptr->r = meshp[3];
		ptr++;
		phd_PopMatrix();
	}

	phd_PopMatrix();
	return obj->nmeshes;
}

void inject_sphere(bool replace)
{
	INJECT(0x00468580, InitInterpolate2, replace);
	INJECT(0x00467F70, GetSpheres, replace);
}
