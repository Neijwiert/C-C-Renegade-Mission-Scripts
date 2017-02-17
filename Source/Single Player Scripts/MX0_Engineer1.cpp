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
#include "MX0_Engineer1.h"

void MX0_Engineer1::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(float), 1);
	Auto_Save_Variable(&this->lastHealthAmount, sizeof(float), 2);
	Auto_Save_Variable(&this->field_24, sizeof(int), 3);
	Auto_Save_Variable(&this->field_28, sizeof(int), 4);
	Auto_Save_Variable(&this->field_2C, sizeof(bool), 5);
}

void MX0_Engineer1::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (action_id == 113)
	{
		// Initialize ActionParamsStruct
		ActionParamsStruct actionParamsStruct;
		actionParamsStruct.Set_Basic(this, 96.0f, 101);

		// Get the position of the dave's arrow where the engineer waits for Havoc to come
		GameObject *davesArrowWaitingForHavocObj = Commands->Find_Object(1200029);
		Vector3 davesArrowWaitingForHavocObjPos = Commands->Get_Position(davesArrowWaitingForHavocObj);

		// Set movement to that location
		actionParamsStruct.Set_Movement(davesArrowWaitingForHavocObjPos, 0.8f, 0.8f);

		// Execute movement
		Commands->Action_Goto(obj, actionParamsStruct);
	}
	else if (action_id == 135)
	{
		this->field_2C = 0;
	}
	else if (action_id == 101)
	{

	}
	/*
	this_1 = this;
	switch (action_id)
	{
	case 101:
		if (Commands->Get_Action_ID(obj) != 101
			&& !Commands->Is_Performing_Pathfind_Action(obj)
			&& complete_reason == ACTION_COMPLETE_LOW_PRIORITY)
		{
			Commands->Start_Timer(obj, &this_1->ScriptImpClassBase.ScriptClassBase, 5.0, 116);
		}
		if (this_1->field_24 == 3 && complete_reason == ACTION_COMPLETE_NORMAL)
		{
			Send_Custom_Event = &Commands->Send_Custom_Event;
			MX0MissionStartDMEObj = Commands->Find_Object(1200001);
			(*Send_Custom_Event)(obj, MX0MissionStartDMEObj, 100004, 0, 0.0);
		}
		break;
	}
	*/
}