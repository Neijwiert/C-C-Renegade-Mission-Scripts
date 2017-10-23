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
#include "M03_Base_Harvester.h"

/*
M03 -> 1144518
M10 -> 2004537
*/
void M03_Base_Harvester::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->field_24, sizeof(this->field_24), 2);
	Auto_Save_Variable(&this->field_25, sizeof(this->field_25), 3);
	Auto_Save_Variable(&this->field_20, sizeof(this->field_20), 4);
	Auto_Save_Variable(&this->field_26, sizeof(this->field_26), 5);
}

void M03_Base_Harvester::Created(GameObject *obj)
{
	this->field_1C = 0;
	this->field_24 = false;
	this->field_25 = false;
	this->field_20 = 0;
	this->field_26 = false;

	ActionParamsStruct params;
	params.Set_Basic(this, 99.0f, 0);

	Vector3 tiberiumLoc = Get_Vector3_Parameter("Tiberium_Loc");

	params.Set_Movement(tiberiumLoc, 1.0f, 1.0f);

	Commands->Action_Goto(obj, params);
}

// TODO
void M03_Base_Harvester::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 622)
	{
		if (param == 622)
		{
			Commands->Set_Animation(obj, NULL, false, NULL, 0.0f, -1.0f, false);

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 3);
			params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 1.0f, 1.0f);
			params.DockLocation = Get_Vector3_Parameter("Dock_Location");
			params.DockEntrance = Get_Vector3_Parameter("Dock_Entrance");

			Commands->Action_Dock(obj, params);
		}
	}
	else if (type == 722)
	{
		if (param == 722 && !this->field_24)
		{
			Commands->Set_Animation(obj, "V_NOD_HRVSTR.V_NOD_HRVSTR", true, NULL, 0.0f, -1.0f, false);

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 4);

			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			float moveSpeed = Commands->Get_Difficulty_Level() / 10.0f + 0.80000001f;

			params.Set_Movement(starObj, moveSpeed, 0.0f);

			Commands->Action_Goto(obj, params);
		}
	}
	else if (type == 7800)
	{
		if (param == 7800)
		{
			this->field_26 = true;
		}
	}
}

// TODO
void M03_Base_Harvester::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_NORMAL)
	{
		if (!action_id)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 99.0f, 1);

			Vector3 pos = Commands->Get_Position(obj);

			float randXOffset = Commands->Get_Random(-3.0f, 3.0f);
			float randYOffset = Commands->Get_Random(-3.0f, 3.0f);

			params.Set_Movement(Vector3(pos.X + randXOffset, pos.Y + randYOffset, pos.Z), 1.0f, 1.0f);

			Commands->Action_Goto(obj, params);
		}
		else if (action_id == 2)
		{
			float randInterval;
			if (this->field_26)
			{
				randInterval = 55.0f;
			}
			else
			{
				randInterval = 35.0f;
			}

			Commands->Start_Timer(obj, this, randInterval, 5);
		}
		else if (action_id == 1)
		{
			Commands->Set_Animation(obj, "V_NOD_HRVSTR.V_NOD_HRVSTR", false, NULL, 0.0f, -1.0f, false);

			this->field_25 = true;
		}
		else if (action_id == 3)
		{
			this->field_24 = true;
		}
	}
}

// TODO
void M03_Base_Harvester::Timer_Expired(GameObject *obj, int number)
{
	ActionParamsStruct params;
	params.Set_Basic(this, 99.0f, 0);

	Vector3 tiberiumLoc = Get_Vector3_Parameter("Tiberium_Loc");

	params.Set_Movement(tiberiumLoc, 1.0f, 1.0f);

	Commands->Action_Goto(obj, params);
}

void M03_Base_Harvester::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (this->field_25)
	{
		if (++this->field_20 <= 2)
		{
			Commands->Set_Animation(obj, "V_NOD_HRVSTR.V_NOD_HRVSTR", false, NULL, 0.0f, -1.0f, false);

			return;
		}

		this->field_25 = false;
		this->field_20 = 0;
	}

	if (this->field_1C <= 3)
	{
		this->field_1C++;

		ActionParamsStruct params;
		params.Set_Basic(this, 99.0f, 1);

		Vector3 pos = Commands->Get_Position(obj);

		float randXOffset = Commands->Get_Random(-3.0f, 3.0f);
		float randYOffset = Commands->Get_Random(-3.0f, 3.0f);

		params.Set_Movement(Vector3(pos.X + randXOffset, pos.Y + randYOffset, pos.Z), 1.0f, 1.0f);

		Commands->Action_Goto(obj, params);
	}
	else
	{
		this->field_1C = 0;

		ActionParamsStruct params;
		params.Set_Basic(this, 99.0f, 2);
		params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 1.0f, 1.0f);
		params.DockLocation = Get_Vector3_Parameter("Dock_Location");
		params.DockEntrance = Get_Vector3_Parameter("Dock_Entrance");

		Commands->Action_Dock(obj, params);
	}
}

ScriptRegistrant<M03_Base_Harvester> M03_Base_HarvesterRegistrant("M03_Base_Harvester", "Tiberium_Loc:vector3, Dock_Location:vector3, Dock_Entrance:vector3, Sakura_Dest:vector3");