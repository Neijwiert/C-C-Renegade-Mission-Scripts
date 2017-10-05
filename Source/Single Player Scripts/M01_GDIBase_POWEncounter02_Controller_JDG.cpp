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
#include "M01_GDIBase_POWEncounter02_Controller_JDG.h"

/*
M01 -> 103331
*/
void M01_GDIBase_POWEncounter02_Controller_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->field_1D, sizeof(this->field_1D), 2);
	Auto_Save_Variable(&this->field_1E, sizeof(this->field_1E), 3);
	Auto_Save_Variable(&this->field_1F, sizeof(this->field_1F), 4);
	Auto_Save_Variable(&this->waypathObjId, sizeof(this->waypathObjId), 5);
	Auto_Save_Variable(&this->chopperObjId, sizeof(this->chopperObjId), 6);
	Auto_Save_Variable(&this->ropeObjId, sizeof(this->ropeObjId), 7);
}

void M01_GDIBase_POWEncounter02_Controller_JDG::Created(GameObject *obj)
{
	this->field_1D = false;
	this->field_1E = false;
	this->field_1F = false;
	this->field_1C = false;
}

// TODO
void M01_GDIBase_POWEncounter02_Controller_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M01_Base_POW01_JDG or M01_Base_POW02_JDG when killed
	if (param == 22)
	{
		GameObject *gdiPOW1Obj = Commands->Find_Object(116386);
		GameObject *gdiPOW2Obj = Commands->Find_Object(116385);

		if (sender == gdiPOW1Obj)
		{
			this->field_1D = true;
		}
		else if (sender == gdiPOW2Obj)
		{
			this->field_1E = true;
		}

		if (this->field_1D && this->field_1E)
		{
			Commands->Set_Objective_Status(114, OBJECTIVE_STATUS_FAILED);

			this->field_1F = true;
		}
	}

	// Received from M01_GDIBasePOW_Air_Evac_Chopper_JDG on create
	else if (param == 14)
	{
		this->chopperObjId = Commands->Get_ID(sender);
	}

	// Received from M01_GDIBasePOW_Air_Evac_Waypath_JDG on create
	else if (param == 13)
	{
		this->waypathObjId = Commands->Get_ID(sender);
	}

	// Received from M01_GDIBasePOW_Air_Evac_Rope_JDG on create
	else if (param == 12)
	{
		this->ropeObjId = Commands->Get_ID(sender);
	}

	// Received from M01_Base_POW01_JDG or M01_Base_POW02_JDG when action with id 39 is complete
	else if (param == 10)
	{
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *troopBone = Commands->Create_Object("Invisible_Object", pos);
		Commands->Set_Model(troopBone, "XG_EV5_troopBN");
		Commands->Attach_Script(troopBone, "M01_Evac_TroopBone_JDG", "");
		Commands->Innate_Disable(sender);
		Commands->Attach_To_Object_Bone(sender, troopBone, "Troop_L");

		if (troopBone)
		{
			Commands->Set_Animation(troopBone, "XG_EV5_troopBN.XG_EV5_troopBN", false, NULL, 0.0f, -1.0f, false);
		}
		else
		{
			Commands->Debug_Message("troopbone not created yet");
		}

		Commands->Set_Animation(sender, "S_A_Human.XG_EV5_troop", false, NULL, 0.0f, -1.0f, false);
	}

	// Received from M01_GDIBase_POW_Conversation_Controller_JDG when the conversation ended
	else if (param == 27)
	{
		if (!this->field_1C)
		{
			this->field_1C = true;

			Vector3 pos = Commands->Get_Position(obj);
			GameObject *invisObj = Commands->Create_Object("Invisible_Object", pos);
			Commands->Attach_Script(invisObj, "Test_Cinematic", "XG_M01_GDIBasePow_EvacAnim.txt");
		}
	}

	// Received from M01_GDIBasePOW_Air_Evac_Chopper_JDG when custom param 4 is received 
	else if (param == 31)
	{
		GameObject *waypathObj = Commands->Find_Object(this->waypathObjId);
		if (waypathObj)
		{
			Commands->Send_Custom_Event(obj, waypathObj, 0, 27, 0.0f);
		}

		GameObject *chopperObj = Commands->Find_Object(this->chopperObjId);
		if (chopperObj)
		{
			Commands->Send_Custom_Event(obj, chopperObj, 0, 27, 0.0f);
		}

		GameObject *ropeObj = Commands->Find_Object(this->ropeObjId);
		if (chopperObj) // This is not a mistake
		{
			Commands->Send_Custom_Event(obj, ropeObj, 0, 27, 0.0f);
		}

		if (!this->field_1F)
		{
			Commands->Set_Objective_Status(114, OBJECTIVE_STATUS_ACCOMPLISHED);
		}
	}
}

ScriptRegistrant<M01_GDIBase_POWEncounter02_Controller_JDG> M01_GDIBase_POWEncounter02_Controller_JDGRegistrant("M01_GDIBase_POWEncounter02_Controller_JDG", "");