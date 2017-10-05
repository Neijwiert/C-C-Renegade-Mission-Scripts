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
#include "M01_ChurchArea_Air_Evac_Chopper_JDG.h"

// After 0 cinematic frames in XG_M01_ChurchArea_EvacAnim.txt
void M01_ChurchArea_Air_Evac_Chopper_JDG::Created(GameObject *obj)
{
	Commands->Set_Animation(obj, "v_GDI_trnspt.XG_EV5_trnsA", false, NULL, 0.0f, -1.0f, false);

	Commands->Debug_Message("**********************script attached to evac chopper\n");

	Commands->Set_Shield_Type(obj, "Blamo");
	Commands->Enable_Hibernation(obj, false);
	Commands->Disable_Physical_Collisions(obj);

	Commands->Send_Custom_Event(obj, obj, 0, 24, 3.0f);

	GameObject *M01ChurchEvacControllerJDGObj = Commands->Find_Object(103394);
	if (M01ChurchEvacControllerJDGObj)
	{
		Commands->Send_Custom_Event(obj, M01ChurchEvacControllerJDGObj, 0, 14, 0.0f);
	}
}

void M01_ChurchArea_Air_Evac_Chopper_JDG::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	float maxHealth = Commands->Get_Max_Health(obj);
	Commands->Set_Health(obj, maxHealth);
}

void M01_ChurchArea_Air_Evac_Chopper_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from ourselves after 3 seconds when created
	if (param == 24)
	{
		Commands->Enable_Collisions(obj);
	}

	// Received from M01_Church_EvacController_JDG when param 15 is received
	else if (param == 27)
	{
		Commands->Debug_Message("*******************************helicopter has received custom to goto exit anim\n");

		Commands->Set_Animation(obj, "v_GDI_trnspt.XG_EV5_trnsz", false, NULL, 0.0f, -1.0f, false);
	}

	// Received from ourselves after 2 seconds after this block or when animation complete
	else if (param == 4)
	{
		GameObject *loveShackNunObj = Commands->Find_Object(101310);
		GameObject *churchInteriorNunObj = Commands->Find_Object(101314);
		GameObject *churchInteriorPriestObj = Commands->Find_Object(101315);

		if (loveShackNunObj || churchInteriorNunObj || churchInteriorPriestObj)
		{
			Commands->Send_Custom_Event(obj, obj, 0, 4, 2.0f);
		}
		else
		{
			Commands->Debug_Message("*******************************all barn civs on board--about to send cutom to controller\n"); // Yes, they re-used the barn debug message

			GameObject *M01ChurchEvacControllerJDGObj = Commands->Find_Object(103394);
			if (M01ChurchEvacControllerJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01ChurchEvacControllerJDGObj, 0, 15, 0.0f);

				Commands->Debug_Message("*******************************all barn civs on board--sent cutom to controller\n"); // Yes, they re-used the barn debug message
			}
		}
	}
}

void M01_ChurchArea_Air_Evac_Chopper_JDG::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, "v_GDI_trnspt.XG_EV5_trnsA"))
	{
		Commands->Debug_Message("**********************chopper has finished entry anim\n");

		Commands->Get_Position(obj); // Random Get_Position here

		Commands->Set_Animation(obj, "v_GDI_trnspt.XG_EV5_trnsL", true, NULL, 0.0f, -1.0f, false);

		GameObject *loveShackNunObj = Commands->Find_Object(101310);
		GameObject *churchInteriorNunObj = Commands->Find_Object(101314);
		GameObject *churchInteriorPriestObj = Commands->Find_Object(101315);

		if (loveShackNunObj)
		{
			Commands->Send_Custom_Event(obj, loveShackNunObj, 0, 4001, 0.0f);
		}

		if (churchInteriorNunObj)
		{
			Commands->Send_Custom_Event(obj, churchInteriorNunObj, 0, 4001, 0.0f);
		}

		if (churchInteriorPriestObj)
		{
			Commands->Send_Custom_Event(obj, churchInteriorPriestObj, 0, 4001, 2.0f);
		}

		Commands->Send_Custom_Event(obj, obj, 0, 4, 2.0f);
	}
	else if (!_stricmp(animation_name, "v_GDI_trnspt.XG_EV5_trnsz"))
	{
		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_ChurchArea_Air_Evac_Chopper_JDG> M01_ChurchArea_Air_Evac_Chopper_JDGRegistrant("M01_ChurchArea_Air_Evac_Chopper_JDG", "");