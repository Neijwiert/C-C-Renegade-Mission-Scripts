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
#include "M01_ChurchArea_EvacMonitor_JDG.h"

/*
M01 -> 103421
*/
void M01_ChurchArea_EvacMonitor_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->rescuedPrisonerCount, sizeof(this->rescuedPrisonerCount), 1);
}

void M01_ChurchArea_EvacMonitor_JDG::Created(GameObject *obj)
{
	this->rescuedPrisonerCount = 0;
}

void M01_ChurchArea_EvacMonitor_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M01_Church_Interior_Nun_JDG or M01_Church_LoveShack_Nun_JDG or M01_Church_Priest_JDG when action with id 4001 is complete
	if (param == 4001 && ++this->rescuedPrisonerCount == 3)
	{
		int conversationId = Commands->Create_Conversation("M01_AirDrop_Conversation", 100, 1000.0f, false); // Nice work. I'm air-dropping supplies, look for them.
		Commands->Join_Conversation(NULL, conversationId, false, false, true);
		Commands->Start_Conversation(conversationId, conversationId);

		GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(-158.807f, 351.532f, 10.353f));
		Commands->Attach_Script(invisObj, "Test_Cinematic", "X1D_GDIPowerUpDrop.txt");

		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_ChurchArea_EvacMonitor_JDG> M01_ChurchArea_EvacMonitor_JDGRegistrant("M01_ChurchArea_EvacMonitor_JDG", "");