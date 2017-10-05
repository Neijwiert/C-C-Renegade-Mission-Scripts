/*
* A Command & Conquer: Renegade SSGM Plugin, containing all the single player mission scripts
* Copyright(C) 2017  Neijwiert
*
* This program is free software : you can redistribute it and / or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.If not, see <http://www.gnu.org/licenses/>.
*/

#include "General.h"
#include "M01_GDIBaseCommander_Air_Evac_Chopper_JDG.h"

// After 0 cinematic frames in XG_M01_GDIBaseCommander_EvacAnim.txt
void M01_GDIBaseCommander_Air_Evac_Chopper_JDG::Created(GameObject *obj)
{
	Commands->Set_Animation(obj, "v_GDI_trnspt.XG_EV5_trnsA", false, NULL, 0.0f, -1.0f, false);

	Commands->Debug_Message("**********************script attached to evac chopper\n");

	Commands->Set_Shield_Type(obj, "Blamo");
	Commands->Enable_Hibernation(obj, false);
	Commands->Disable_Physical_Collisions(obj);

	Commands->Send_Custom_Event(obj, obj, 0, 24, 3.0f);

	GameObject *M01GDIBaseCommanderEvacControllerJDGObj = Commands->Find_Object(106694);
	if (M01GDIBaseCommanderEvacControllerJDGObj)
	{
		Commands->Send_Custom_Event(obj, M01GDIBaseCommanderEvacControllerJDGObj, 0, 29, 0.0f);
	}
}

void M01_GDIBaseCommander_Air_Evac_Chopper_JDG::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	float maxHealth = Commands->Get_Max_Health(obj);
	Commands->Set_Health(obj, maxHealth);
}

// TODO
void M01_GDIBaseCommander_Air_Evac_Chopper_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from ourselves after 3 seconds after create
	if (param == 24)
	{
		Commands->Enable_Collisions(obj);
	}

	// Received from M01_GDIBaseCommander_EvacController_JDG when param 28 is received
	else if (param == 27)
	{
		Commands->Debug_Message("*******************************helicopter has received custom to goto exit anim\n");

		Commands->Set_Animation(obj, "v_GDI_trnspt.XG_EV5_trnsz", false, NULL, 0.0f, -1.0f, false);
	}

	// Received from ourselves after 1 second after this block or 2 seconds after animation complete
	else if (param == 4)
	{
		GameObject *captainDuncanObj = Commands->Find_Object(106050);
		GameObject *gdiMinigunnerObj = Commands->Find_Object(116382);
		GameObject *gdiGrenadierObj = Commands->Find_Object(116383);
		if (captainDuncanObj || gdiMinigunnerObj || gdiGrenadierObj)
		{
			Commands->Send_Custom_Event(obj, obj, 0, 4, 1.0f);
		}
		else
		{
			GameObject *M01GDIBaseCommanderEvacControllerJDGObj = Commands->Find_Object(106694);
			if (M01GDIBaseCommanderEvacControllerJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01GDIBaseCommanderEvacControllerJDGObj, 0, 28, 0.0f);
			}
		}
	}
}

// TODO
void M01_GDIBaseCommander_Air_Evac_Chopper_JDG::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, "v_GDI_trnspt.XG_EV5_trnsA"))
	{
		Commands->Debug_Message("**********************chopper has finished entry anim\n");

		Commands->Get_Position(obj); // Random Get_Position here

		Commands->Set_Animation(obj, "v_GDI_trnspt.XG_EV5_trnsL", true, NULL, 0.0f, -1.0f, false);

		GameObject *captainDuncanObj = Commands->Find_Object(106050);
		if (captainDuncanObj)
		{
			Commands->Send_Custom_Event(obj, captainDuncanObj, 0, 4001, 0.0f);
		}

		GameObject *gdiMinigunnerObj = Commands->Find_Object(116382);
		if (gdiMinigunnerObj)
		{
			Commands->Send_Custom_Event(obj, gdiMinigunnerObj, 0, 4001, 3.0f);
		}

		GameObject *gdiGrenadierObj = Commands->Find_Object(116383);
		if (gdiGrenadierObj)
		{
			Commands->Send_Custom_Event(obj, gdiGrenadierObj, 0, 4001, 6.0f);
		}

		Commands->Send_Custom_Event(obj, obj, 0, 4, 2.0f);
	}
	else if (!_stricmp(animation_name, "v_GDI_trnspt.XG_EV5_trnsz"))
	{
		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_GDIBaseCommander_Air_Evac_Chopper_JDG> M01_GDIBaseCommander_Air_Evac_Chopper_JDGRegistrant("M01_GDIBaseCommander_Air_Evac_Chopper_JDG", "");