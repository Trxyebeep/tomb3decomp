#include "../tomb3/pch.h"
#include "lot.h"
#include "../specific/init.h"
#include "objects.h"

void InitialiseLOTarray()
{
	CREATURE_INFO* creature;

	baddie_slots = (CREATURE_INFO*)game_malloc(5 * sizeof(CREATURE_INFO), 33);

	for (int i = 0; i < 5; i++)
	{
		creature = &baddie_slots[i];
		creature->item_num = NO_ITEM;
		creature->LOT.node = (BOX_NODE*)game_malloc(sizeof(BOX_NODE) * number_boxes, 34);
	}

	slots_used = 0;

	non_lot_slots = (CREATURE_INFO*)game_malloc(12 * sizeof(CREATURE_INFO), 33);

	for (int i = 0; i < 12; i++)
	{
		creature = &non_lot_slots[i];
		creature->item_num = NO_ITEM;
	}

	nonlot_slots_used = 0;
}

void InitialiseNonLotAI(short item_number, long slot)
{
	ITEM_INFO* item;
	CREATURE_INFO* creature;

	item = &items[item_number];
	creature = &non_lot_slots[slot];

	if (item_number == lara.item_number)
		lara.creature = &non_lot_slots[slot];
	else
		item->data = creature;

	creature->item_num = item_number;
	creature->mood = BORED_MOOD;
	creature->joint_rotation[0] = 0;
	creature->joint_rotation[1] = 0;
	creature->joint_rotation[2] = 0;
	creature->joint_rotation[3] = 0;
	creature->alerted = 0;
	creature->head_left = 0;
	creature->head_right = 0;
	creature->reached_goal = 0;
	creature->hurt_by_lara = 0;
	creature->patrol2 = 0;
	creature->maximum_turn = 182;
	creature->flags = 0;
	creature->enemy = 0;
	creature->LOT.step = 256;
	creature->LOT.drop = -512;
	creature->LOT.block_mask = 0x4000;
	creature->LOT.fly = 0;

	switch (item->object_number)
	{
	case LARA:
		creature->LOT.step = 20480;
		creature->LOT.drop = -20480;
		creature->LOT.fly = 256;
		break;

	case WHALE:
	case DIVER:
	case CROW:
	case VULTURE:
		creature->LOT.step = 20480;
		creature->LOT.drop = -20480;
		creature->LOT.fly = 16;

		if (item->object_number == WHALE)
			creature->LOT.block_mask = 0x8000;

		break;

	case LIZARD_MAN:
	case MP1:
		creature->LOT.step = 1024;
		creature->LOT.drop = -1024;
		break;
	}

	nonlot_slots_used++;
}

long EnableNonLotAI(short item_number, long Always)
{
	ITEM_INFO* item;
	CREATURE_INFO* creature;
	long x, y, z, slot, worstslot, dist, worstdist;

	item = &items[item_number];

	if (nonlot_slots_used < 12)
	{
		for (int i = 0; i < 12; i++)
		{
			creature = &non_lot_slots[i];

			if (creature->item_num == NO_ITEM)
			{
				InitialiseNonLotAI(item_number, i);
				return 1;
			}
		}
	}

	if (Always)
		worstdist = 0;
	else
	{
		x = (item->pos.x_pos - camera.pos.x) >> 8;
		y = (item->pos.y_pos - camera.pos.y) >> 8;
		z = (item->pos.z_pos - camera.pos.z) >> 8;
		worstdist = SQUARE(x) + SQUARE(y) + SQUARE(z);
	}

	worstslot = -1;

	for (slot = 0; slot < 12; slot++)
	{
		creature = &non_lot_slots[slot];
		item = &items[creature->item_num];
		x = (item->pos.x_pos - camera.pos.x) >> 8;
		y = (item->pos.y_pos - camera.pos.y) >> 8;
		z = (item->pos.z_pos - camera.pos.z) >> 8;
		dist = SQUARE(x) + SQUARE(y) + SQUARE(z);

		if (dist > worstdist)
		{
			worstslot = slot;
			worstdist = dist;
		}
	}

	if (worstslot >= 0)
	{
		items[non_lot_slots[worstslot].item_num].status = ITEM_INVISIBLE;
		DisableBaddieAI(non_lot_slots[worstslot].item_num);
		InitialiseNonLotAI(item_number, worstslot);
		return 1;
	}

	return 0;
}

void DisableBaddieAI(short item_number)
{
	ITEM_INFO* item;
	CREATURE_INFO* creature;

	item = &items[item_number];

	if (item_number == lara.item_number)
	{
		creature = lara.creature;
		lara.creature = 0;
	}
	else
	{
		creature = (CREATURE_INFO*)item->data;
		item->data = 0;
	}

	if (creature)
	{
		creature->item_num = NO_ITEM;

		if (objects[item->object_number].non_lot)
			nonlot_slots_used--;
		else
			slots_used--;
	}
}

void inject_lot(bool replace)
{
	INJECT(0x00452F10, InitialiseLOTarray, replace);
	INJECT(0x00453740, InitialiseNonLotAI, replace);
	INJECT(0x004535B0, EnableNonLotAI, replace);
	INJECT(0x00452F90, DisableBaddieAI, replace);
}
