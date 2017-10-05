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
#include "M01_Billys_Conversation_Zone_JDG.h"

/*
M01 -> 101661
*/
void M01_Billys_Conversation_Zone_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->starInBillysShack, sizeof(this->starInBillysShack), 2);
}

void M01_Billys_Conversation_Zone_JDG::Created(GameObject *obj)
{
	this->field_1C = false;
	this->starInBillysShack = false;
}

// TODO
void M01_Billys_Conversation_Zone_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M01_Barn_Prisoner_02_JDG when action with id 38 is complete
	if (param == 16)
	{
		this->field_1C = true;

		Commands->Send_Custom_Event(obj, obj, 0, 225, 0.0f);
	}

	// Received from ourselves after param 16 or when entered
	else if (param == 225 && this->starInBillysShack && this->field_1C)
	{
		GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
		Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 225, 0.0f);

		GameObject *M01BarnBabushkasConversationZoneJDGObj = Commands->Find_Object(101662);
		GameObject *M01BillysConversationZoneJDGObj = Commands->Find_Object(101661);

		if (M01BarnBabushkasConversationZoneJDGObj)
		{
			Commands->Destroy_Object(M01BarnBabushkasConversationZoneJDGObj);
		}

		if (M01BillysConversationZoneJDGObj)
		{
			Commands->Destroy_Object(M01BillysConversationZoneJDGObj);
		}
	}
}

void M01_Billys_Conversation_Zone_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (enterer == Commands->Get_A_Star(pos))
	{
		this->starInBillysShack = true;

		Commands->Send_Custom_Event(obj, obj, 0, 225, 0.0f);
	}
}

void M01_Billys_Conversation_Zone_JDG::Exited(GameObject *obj, GameObject *exiter)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (exiter == Commands->Get_A_Star(pos))
	{
		this->starInBillysShack = false;
	}
}

ScriptRegistrant<M01_Billys_Conversation_Zone_JDG> M01_Billys_Conversation_Zone_JDGRegistrant("M01_Billys_Conversation_Zone_JDG", "");