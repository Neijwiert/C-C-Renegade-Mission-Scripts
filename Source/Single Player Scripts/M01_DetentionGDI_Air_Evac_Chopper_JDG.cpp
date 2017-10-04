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
#include "M01_DetentionGDI_Air_Evac_Chopper_JDG.h"

// This script is never used
void M01_DetentionGDI_Air_Evac_Chopper_JDG::Created(GameObject *obj)
{
	Commands->Set_Shield_Type(obj, "Blamo");
	Commands->Enable_Hibernation(obj, false);
	Commands->Disable_Physical_Collisions(obj);

	Commands->Send_Custom_Event(obj, obj, 0, 24, 3.0f);
}

void M01_DetentionGDI_Air_Evac_Chopper_JDG::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	float maxHealth = Commands->Get_Max_Health(obj);
	Commands->Set_Health(obj, maxHealth);
}

void M01_DetentionGDI_Air_Evac_Chopper_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received after 3 seconds from ourselves after creation
	if (param == 24)
	{
		Commands->Enable_Collisions(obj);
	}

	// Received after 0 seconds when param 4 is received
	else if (param == 27)
	{
		Commands->Set_Animation(obj, "v_GDI_trnspt.XG_EV1_trnsz", false, NULL, 0.0f, -1.0f, false);
	}

	// Received after 2 seconds after this block or 2 seconds after animation complete
	else if (param == 4)
	{
		GameObject *detentionSoldier1Obj = Commands->Find_Object(101926);
		GameObject *detentionSoldier2Obj = Commands->Find_Object(101927);
		GameObject *detentionSoldier3Obj = Commands->Find_Object(101928);
		if (detentionSoldier1Obj || detentionSoldier2Obj || detentionSoldier3Obj)
		{
			Commands->Send_Custom_Event(obj, obj, 0, 4, 2.0f);
		}
		else
		{
			GameObject *nonExistingObj = Commands->Find_Object(103381);
			if (nonExistingObj)
			{
				Commands->Send_Custom_Event(obj, nonExistingObj, 0, 27, 0.0f);
			}
		}
	}
}

void M01_DetentionGDI_Air_Evac_Chopper_JDG::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, "v_GDI_trnspt.XG_EV1_trnsA"))
	{
		Commands->Set_Animation(obj, "v_GDI_trnspt.XG_EV1_trnsloop", true, NULL, 0.0f, -1.0f, false);

		Commands->Send_Custom_Event(obj, obj, 0, 4, 2.0f);

		GameObject *detentionSoldier1Obj = Commands->Find_Object(101926);
		GameObject *detentionSoldier2Obj = Commands->Find_Object(101927);
		GameObject *detentionSoldier3Obj = Commands->Find_Object(101928);

		if (detentionSoldier1Obj)
		{
			Commands->Send_Custom_Event(obj, detentionSoldier1Obj, 0, 4002, 0.0f);
		}

		if (detentionSoldier2Obj)
		{
			Commands->Send_Custom_Event(obj, detentionSoldier2Obj, 0, 4002, 0.0f);
		}

		if (detentionSoldier3Obj)
		{
			Commands->Send_Custom_Event(obj, detentionSoldier3Obj, 0, 4002, 0.0f);
		}
	}
	else if (!_stricmp(animation_name, "v_GDI_trnspt.XG_EV1_trnsz"))
	{
		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_DetentionGDI_Air_Evac_Chopper_JDG> M01_DetentionGDI_Air_Evac_Chopper_JDGRegistrant("M01_DetentionGDI_Air_Evac_Chopper_JDG", "");