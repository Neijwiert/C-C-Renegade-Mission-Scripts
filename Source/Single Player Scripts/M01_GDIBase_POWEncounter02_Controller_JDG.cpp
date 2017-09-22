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
	Auto_Save_Variable(&this->field_20, sizeof(this->field_20), 5);
	Auto_Save_Variable(&this->field_24, sizeof(this->field_24), 6);
	Auto_Save_Variable(&this->field_28, sizeof(this->field_28), 7);
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
	else if (param == 14)
	{
		this->field_24 = Commands->Get_ID(sender);
	}
	else if (param == 13)
	{
		this->field_20 = Commands->Get_ID(sender);
	}
	else if (param == 12)
	{
		this->field_28 = Commands->Get_ID(sender);
	}
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
	else if (param == 31)
	{
		GameObject *field20Obj = Commands->Find_Object(this->field_20);
		if (field20Obj)
		{
			Commands->Send_Custom_Event(obj, field20Obj, 0, 27, 0.0f);
		}

		GameObject *field24Obj = Commands->Find_Object(this->field_24);
		if (field24Obj)
		{
			Commands->Send_Custom_Event(obj, field24Obj, 0, 27, 0.0f);
		}

		GameObject *field28Obj = Commands->Find_Object(this->field_28);
		if (field24Obj) // This is not a mistake
		{
			Commands->Send_Custom_Event(obj, field28Obj, 0, 27, 0.0f);
		}

		if (!this->field_1F)
		{
			Commands->Set_Objective_Status(114, OBJECTIVE_STATUS_ACCOMPLISHED);
		}
	}
}

ScriptRegistrant<M01_GDIBase_POWEncounter02_Controller_JDG> M01_GDIBase_POWEncounter02_Controller_JDGRegistrant("M01_GDIBase_POWEncounter02_Controller_JDG", "");