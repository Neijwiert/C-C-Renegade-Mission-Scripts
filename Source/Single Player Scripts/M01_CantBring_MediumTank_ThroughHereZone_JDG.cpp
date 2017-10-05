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
#include "M01_CantBring_MediumTank_ThroughHereZone_JDG.h"

/*
M01 -> 125661
*/
void M01_CantBring_MediumTank_ThroughHereZone_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->field_20, sizeof(this->field_20), 2);
}

void M01_CantBring_MediumTank_ThroughHereZone_JDG::Created(GameObject *obj)
{
	this->field_1C = false;
}

// TODO
void M01_CantBring_MediumTank_ThroughHereZone_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type)
	{
		// Received from M01_Medium_Tank_JDG when entered
		if (param == 27)
		{
			this->field_1C = true;
		}

		// Received from M01_Medium_Tank_JDG when exited
		else if (param == 28)
		{
			this->field_1C = false;
		}

		// Received from M01_Medium_Tank_JDG when created
		else if (param == 29)
		{
			this->field_20 = Commands->Get_ID(sender);
		}
	}
}

void M01_CantBring_MediumTank_ThroughHereZone_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	if (enterer == Commands->Find_Object(this->field_20) && this->field_1C)
	{
		int conversationId = Commands->Create_Conversation("M01_Must_Leave_Tank_Conversation", 100, 100.0f, true); // You can't drive over those barricades. It looks like you'll need to proceed on foot from here, Havoc.
		Commands->Join_Conversation(NULL, conversationId, true, true, true);
		Commands->Start_Conversation(conversationId, conversationId);

		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_CantBring_MediumTank_ThroughHereZone_JDG> M01_CantBring_MediumTank_ThroughHereZone_JDGRegistrant("M01_CantBring_MediumTank_ThroughHereZone_JDG", "");