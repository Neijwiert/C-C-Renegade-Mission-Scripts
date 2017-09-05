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
#include "MX0_NOD_INFANTRY.h"

void MX0_NOD_INFANTRY::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->troopNum, sizeof(this->troopNum), 1);
}

void MX0_NOD_INFANTRY::Created(GameObject *obj)
{
	this->troopNum = Get_Int_Parameter("troop_num");

	GameObject *MX0A03ControllerObj = Commands->Find_Object(1400041);
	if (MX0A03ControllerObj)
	{
		Commands->Send_Custom_Event(obj, MX0A03ControllerObj, 406, 0, 0.0f);
	}

	if (this->troopNum == 1)
	{
		Commands->Attach_Script(obj, "M00_Send_Object_ID", "1400041,15,0");
	}
	else
	{
		Commands->Attach_Script(obj, "M00_Send_Object_ID", "1400041,17,0");
	}
}

void MX0_NOD_INFANTRY::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *MX0A03ControllerObj = Commands->Find_Object(1400041);
	if (MX0A03ControllerObj)
	{
		Commands->Send_Custom_Event(obj, MX0A03ControllerObj, 407, this->troopNum, 2.0f);
	}
}

void MX0_NOD_INFANTRY::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type)
	{
		if (sender)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 90.0f, 1);
			params.Set_Attack(sender, 100.0f, 0.75f, true);

			Commands->Action_Attack(obj, params);
		}
	}
	else if (type == 1)
	{
		Commands->Action_Reset(obj, 100.0f);

		GameObject *ledgeMoveLocationObj;
		if (this->troopNum == 1)
		{
			ledgeMoveLocationObj = Commands->Find_Object(1400153);
		}
		else
		{
			ledgeMoveLocationObj = Commands->Find_Object(1400155);
		}

		ActionParamsStruct params;
		params.Set_Basic(this, 90.0f, 0);
		params.Set_Movement(ledgeMoveLocationObj, 0.8f, 0.25f);

		Commands->Action_Goto(obj, params);
	}
}

void MX0_NOD_INFANTRY::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// Triggered when we moved to the edge of the cliff
	if (!action_id)
	{
		Commands->Set_Innate_Is_Stationary(obj, true);
	}
}

void MX0_NOD_INFANTRY::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, "S_A_Human.H_A_TroopDrop"))  // Should be == 0, but it doesn't appear to be compiled like that
	{
		Commands->Send_Custom_Event(obj, obj, 1, 0, 0.33000001f);
	}
}

ScriptRegistrant<MX0_NOD_INFANTRY> MX0_NOD_INFANTRYRegistrant("MX0_NOD_INFANTRY", "troop_num:int");