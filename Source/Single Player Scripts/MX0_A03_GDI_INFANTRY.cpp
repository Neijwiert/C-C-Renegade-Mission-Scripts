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
#include "MX0_A03_GDI_INFANTRY.h"

void MX0_A03_GDI_INFANTRY::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->troopNum, sizeof(this->troopNum), 1);
	Auto_Save_Variable(&this->targetObjId, sizeof(this->targetObjId), 2);
}

// After 490 cinematic frames in mx0_a03_gdi_troopdrop
// After 579 cinematic frames in mx0_a03_gdi_troopdrop
// After 668 cinematic frames in mx0_a03_gdi_troopdrop
void MX0_A03_GDI_INFANTRY::Created(GameObject *obj)
{
	this->troopNum = Get_Int_Parameter("troop_num");

	Commands->Attach_Script(obj, "M00_Damage_Modifier_DME", "0.15,0,1,1,1");

	if (this->troopNum == 1)
	{
		Commands->Attach_Script(obj, "M00_Send_Object_ID", "1400041,3,0");
		Commands->Action_Reset(obj, 100.0f);

		ActionParamsStruct params;
		params.Set_Basic(this, 91.0f, 0);

		GameObject *dropTroopOneLoc = Commands->Find_Object(1400138);

		params.Set_Movement(dropTroopOneLoc, 0.8f, 0.25f);

		Commands->Action_Goto(obj, params);
	}
	else if (this->troopNum == 2)
	{
		Commands->Attach_Script(obj, "M00_Send_Object_ID", "1400041,5,0");
		Commands->Action_Reset(obj, 100.0f);

		ActionParamsStruct params;
		params.Set_Basic(this, 91.0f, 0);

		GameObject *dropTroopTwoLocObj = Commands->Find_Object(1400139);

		params.Set_Movement(dropTroopTwoLocObj, 0.8f, 0.25f);

		Commands->Action_Goto(obj, params);
	}
	else if (this->troopNum == 3)
	{
		Commands->Attach_Script(obj, "M00_Send_Object_ID", "1400041,7,0");

		this->targetObjId = 1400001; // Nod harvester

		Commands->Action_Reset(obj, 100.0f);

		ActionParamsStruct params;
		params.Set_Basic(this, 91.0f, 1);

		GameObject *dropTroopThreeLocObj = Commands->Find_Object(1400140);;

		params.Set_Movement(dropTroopThreeLocObj, 0.8f, 0.25f);

		Commands->Action_Goto(obj, params);
	}
}

void MX0_A03_GDI_INFANTRY::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received by ourselves. See action_complete action_id 1/2
	if (!type)
	{
		GameObject *targetObj = Commands->Find_Object(this->targetObjId);
		if (targetObj)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 90.0f, 2);
			params.Set_Attack(targetObj, 60.0f, 0.25f, true);

			Commands->Action_Attack(obj, params);
		}
	}
}

void MX0_A03_GDI_INFANTRY::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// Triggered on creation troopNum 1/2
	if (!action_id)
	{
		Commands->Set_Innate_Is_Stationary(obj, true);
	}

	// Triggered on creation troopNum 3
	else if (action_id == 1)
	{
		Commands->Set_Innate_Is_Stationary(obj, true);
		Commands->Send_Custom_Event(obj, obj, 0, 0, 0.0f);
	}

	// Triggered by attacking a targetObj see custom
	else if (action_id == 2)
	{
		this->targetObjId = 1400081; // nod buggy

		Commands->Send_Custom_Event(obj, obj, 0, 0, 0.0f);
	}
}

ScriptRegistrant<MX0_A03_GDI_INFANTRY> MX0_A03_GDI_INFANTRYRegistrant("MX0_A03_GDI_INFANTRY", "troop_num:int");