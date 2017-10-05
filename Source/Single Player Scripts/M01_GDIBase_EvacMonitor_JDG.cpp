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
#include "M01_GDIBase_EvacMonitor_JDG.h"

/*
M01 -> 103419
*/
void M01_GDIBase_EvacMonitor_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
}

void M01_GDIBase_EvacMonitor_JDG::Created(GameObject *obj)
{
	this->field_1C = 0;
}

// TODO
void M01_GDIBase_EvacMonitor_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M01_Base_POW01_JDG or M01_Base_POW02_JDG when action with id 39 is complete
	if (param == 4001 && ++this->field_1C == 4)
	{
		int conversationId = Commands->Create_Conversation("M01_AirDrop_Conversation", 100, 1000.0f, false); // Nice work. I'm air-dropping supplies, look for them.
		Commands->Join_Conversation(NULL, conversationId, false, false, true);
		Commands->Start_Conversation(conversationId, conversationId);

		GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(-32.247f, 36.075f, 0.547f));
		Commands->Attach_Script(invisObj, "Test_Cinematic", "X1D_GDIPowerUpDrop.txt");

		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_GDIBase_EvacMonitor_JDG> M01_GDIBase_EvacMonitor_JDGRegistrant("M01_GDIBase_EvacMonitor_JDG", "");