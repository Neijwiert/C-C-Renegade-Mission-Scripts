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
#include "M01_Interior_Nun_Conversation_Zone_JDG.h"

/*
M01 -> 101370
*/
void M01_Interior_Nun_Conversation_Zone_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
}

void M01_Interior_Nun_Conversation_Zone_JDG::Created(GameObject *obj)
{
	this->field_1C = false;
}

// TODO
void M01_Interior_Nun_Conversation_Zone_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M01_Church_Interior_Nun_JDG when action with id 39 is complete
	if (param == 16)
	{
		this->field_1C = true;
	}
}

void M01_Interior_Nun_Conversation_Zone_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (enterer == Commands->Get_A_Star(pos) && this->field_1C)
	{
		GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
		Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 211, 0.0f);

		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_Interior_Nun_Conversation_Zone_JDG> M01_Interior_Nun_Conversation_Zone_JDGRegistrant("M01_Interior_Nun_Conversation_Zone_JDG", "");