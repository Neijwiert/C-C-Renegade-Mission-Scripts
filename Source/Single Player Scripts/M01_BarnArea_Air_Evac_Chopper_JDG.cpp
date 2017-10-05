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
#include "M01_BarnArea_Air_Evac_Chopper_JDG.h"

// After 0 cinematic frames in XG_M01_BarnArea_EvacAnim.txt
void M01_BarnArea_Air_Evac_Chopper_JDG::Created(GameObject *obj)
{
	Commands->Set_Animation(obj, "v_GDI_trnspt.XG_EV5_trnsA", false, NULL, 0.0f, -1.0f, false);

	Commands->Debug_Message("**********************script attached to evac chopper\n");

	Commands->Set_Shield_Type(obj, "Blamo");
	Commands->Enable_Hibernation(obj, false);
	Commands->Disable_Physical_Collisions(obj);

	Commands->Send_Custom_Event(obj, obj, 0, 24, 3.0f);

	GameObject *M01BarnAreaEvacMonitorJDGObj = Commands->Find_Object(103420);
	if (M01BarnAreaEvacMonitorJDGObj)
	{
		Commands->Send_Custom_Event(obj, M01BarnAreaEvacMonitorJDGObj, 0, 14, 0.0f);
	}

	GameObject *barnFemalePrisonerObj = Commands->Find_Object(101442);
	GameObject *billyObj = Commands->Find_Object(101444);
	GameObject *barnMalePrisonerPierreObj = Commands->Find_Object(101443);

	if (barnFemalePrisonerObj)
	{
		Commands->Send_Custom_Event(obj, barnFemalePrisonerObj, 0, 75, 0.0f);
	}

	if (billyObj)
	{
		Commands->Send_Custom_Event(obj, billyObj, 0, 75, 0.0f);
	}

	if (barnMalePrisonerPierreObj)
	{
		Commands->Send_Custom_Event(obj, barnMalePrisonerPierreObj, 0, 75, 0.0f);
	}
}

void M01_BarnArea_Air_Evac_Chopper_JDG::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	float maxHealth = Commands->Get_Max_Health(obj);
	Commands->Set_Health(obj, maxHealth);
}

void M01_BarnArea_Air_Evac_Chopper_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from ourselves after 3 seconds on create
	if (param == 24)
	{
		Commands->Enable_Collisions(obj);
	}

	// Received from M01_BarnArea_EvacMonitor_JDG when custom param 15 is received
	else if (param == 27)
	{
		Commands->Debug_Message("*******************************helicopter has received custom to goto exit anim\n");

		Commands->Set_Animation(obj, "v_GDI_trnspt.XG_EV5_trnsz", false, NULL, 0.0f, -1.0f, false);
	}

	// Received from ourselves after 1 second in this block or after 2 seconds when animation is complete
	else if (param == 4)
	{
		GameObject *barnFemalePrisonerObj = Commands->Find_Object(101442);
		GameObject *billyObj = Commands->Find_Object(101444);
		GameObject *barnMalePrisonerPierreObj = Commands->Find_Object(101443);
		if (barnFemalePrisonerObj || billyObj || barnMalePrisonerPierreObj)
		{
			Commands->Send_Custom_Event(obj, obj, 0, 4, 1.0f);
		}
		else
		{
			Commands->Debug_Message("*******************************all barn civs on board--about to send cutom to controller\n");

			GameObject *M01BarnAreaEvacMonitorJDGObj = Commands->Find_Object(103420);
			if (M01BarnAreaEvacMonitorJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01BarnAreaEvacMonitorJDGObj, 0, 15, 0.0f);

				Commands->Debug_Message("*******************************all barn civs on board--sent cutom to controller\n");
			}

			GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
			if (M01MissionControllerJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 115, 0.0f);
			}
		}
	}
}

void M01_BarnArea_Air_Evac_Chopper_JDG::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, "v_GDI_trnspt.XG_EV5_trnsA"))
	{
		Commands->Debug_Message("**********************chopper has finished entry anim\n");

		Commands->Get_Position(obj); // Random Get_Position here

		Commands->Set_Animation(obj, "v_GDI_trnspt.XG_EV5_trnsL", true, NULL, 0.0f, -1.0f, false);

		GameObject *barnFemalePrisonerObj = Commands->Find_Object(101442);
		GameObject *billyObj = Commands->Find_Object(101444);
		GameObject *barnMalePrisonerPierreObj = Commands->Find_Object(101443);

		if (barnFemalePrisonerObj)
		{
			Commands->Send_Custom_Event(obj, barnFemalePrisonerObj, 0, 4002, 0.0f);
		}

		if (billyObj)
		{
			Commands->Send_Custom_Event(obj, billyObj, 0, 4002, 0.0f);
		}

		if (barnMalePrisonerPierreObj)
		{
			Commands->Send_Custom_Event(obj, barnMalePrisonerPierreObj, 0, 4002, 2.0f);
		}

		Commands->Send_Custom_Event(obj, obj, 0, 4, 2.0f);
	}
	else if (!_stricmp(animation_name, "v_GDI_trnspt.XG_EV5_trnsz"))
	{
		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_BarnArea_Air_Evac_Chopper_JDG> M01_BarnArea_Air_Evac_Chopper_JDGRegistrant("M01_BarnArea_Air_Evac_Chopper_JDG", "");