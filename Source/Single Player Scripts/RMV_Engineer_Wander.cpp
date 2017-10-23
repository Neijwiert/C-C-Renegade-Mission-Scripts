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
#include "RMV_Engineer_Wander.h"

/*
M03 -> 300034 300033
*/
void RMV_Engineer_Wander::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->customType, sizeof(this->customType), 1);
	Auto_Save_Variable(&this->customParam1, sizeof(this->customParam1), 2);
	Auto_Save_Variable(&this->customParam2, sizeof(this->customParam2), 3);
	Auto_Save_Variable(&this->field_28, sizeof(this->field_28), 4);
	Auto_Save_Variable(&this->field_2C, sizeof(this->field_2C), 5);
	Auto_Save_Variable(&this->field_30, sizeof(this->field_30), 6);
	Auto_Save_Variable(&this->field_31, sizeof(this->field_31), 7);
	Auto_Save_Variable(&this->field_32, sizeof(this->field_32), 8);
	Auto_Save_Variable(&this->field_33, sizeof(this->field_33), 9);
}

void RMV_Engineer_Wander::Created(GameObject *obj)
{
	this->field_33 = false;
	this->field_32 = false;
	this->field_31 = false;
	this->field_30 = false;
	this->customType = Get_Int_Parameter("Custom_Type");
	this->customParam1 = Get_Int_Parameter("Custom_Param_1");
	this->customParam2 = Get_Int_Parameter("Custom_Param_2");
}

// TODO
void RMV_Engineer_Wander::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == this->customType)
	{
		this->field_30 = true;
		this->field_28 = Commands->Get_ID(sender);
		this->field_2C = param;

		ActionParamsStruct params;
		params.Set_Basic(this, 70.0f, 0);

		float moveSpeed;
		if (this->field_31)
		{
			moveSpeed = 0.8f;
		}
		else
		{
			moveSpeed = 0.3f;
		}

		params.Set_Movement(sender, moveSpeed, 0.75f);

		Commands->Action_Goto(obj, params);
	}
}

// TODO
void RMV_Engineer_Wander::Sound_Heard(GameObject *obj, const CombatSound & sound)
{
	if (!this->field_33)
	{
		if (sound.sound == 1002)
		{
			if (!this->field_30)
			{
				if (this->field_32)
				{
					this->field_31 = true;

					Commands->Send_Custom_Event(obj, sound.Creator, this->customType, this->customParam1, 0.0f);
				}
				else
				{
					this->field_31 = false;

					Commands->Send_Custom_Event(obj, sound.Creator, this->customType, this->customParam1, 0.0f);
				}
			}
		}
		else if (sound.sound == 1004)
		{
			if (!this->field_30)
			{
				this->field_31 = true;

				Commands->Send_Custom_Event(obj, sound.Creator, this->customType, this->customParam1, 0.0f);
			}
		}
		else if (sound.sound == (Get_Int_Parameter("Building_Number") + 100000))
		{
			Evacuate();
		}
		else if (sound.sound == 1005)
		{
			this->field_32 = true;
		}
		else if (sound.sound == 1006)
		{
			this->field_32 = false;
		}
	}
}

// TODO
void RMV_Engineer_Wander::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (action_id)
	{
		if (action_id == 1 || complete_reason == ACTION_COMPLETE_MOVE_NO_PROGRESS_MADE)
		{
			GameObject *field28Obj = Commands->Find_Object(this->field_28);
			Commands->Send_Custom_Event(obj, field28Obj, this->customType, this->customParam2, 0.0f);

			this->field_31 = false;
			this->field_30 = false;

			if (this->field_32)
			{
				this->field_31 = true;
			}
		}
	}
	else
	{
		if (complete_reason == ACTION_COMPLETE_NORMAL)
		{
			GameObject *field28Obj = Commands->Find_Object(this->field_28);
			if (field28Obj)
			{
				float field28ObjFacing = Commands->Get_Facing(field28Obj);
				field28ObjFacing += 180.0f;

				Commands->Set_Facing(obj, field28ObjFacing);
			}

			ActionParamsStruct params;
			params.Set_Basic(this, 70.0f, 1);
			params.Set_Animation("s_a_human.h_a_con2", false);

			Commands->Action_Play_Animation(obj, params);
		}
	}
}

void RMV_Engineer_Wander::Evacuate()
{
	const char *evacObj = Get_Parameter("Evac_Object");

	Commands->Action_Reset(Owner(), 99.0f);

	this->field_33 = true;

	if (_stricmp(evacObj, "None"))
	{
		GameObject *randomSimpleObj = Commands->Find_Random_Simple_Object(evacObj);
		if (randomSimpleObj)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 99.0f, 0);
			params.Set_Movement(randomSimpleObj, 0.8f, 2.0f);

			Commands->Action_Goto(Owner(), params);
		}
	}
}

ScriptRegistrant<RMV_Engineer_Wander> RMV_Engineer_WanderRegistrant("RMV_Engineer_Wander", "Custom_Type:int, Custom_Param_1:int, Custom_Param_2:int, Building_Number:int, Evac_Object=None:string");