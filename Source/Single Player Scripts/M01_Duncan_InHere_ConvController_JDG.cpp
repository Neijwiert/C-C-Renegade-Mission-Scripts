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
#include "M01_Duncan_InHere_ConvController_JDG.h"

/*
M01 -> 125857
*/
void M01_Duncan_InHere_ConvController_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->assailantDeathCount, sizeof(this->assailantDeathCount), 1);
	Auto_Save_Variable(&this->starInDuncanShack, sizeof(this->starInDuncanShack), 2);
}

void M01_Duncan_InHere_ConvController_JDG::Created(GameObject *obj)
{
	this->assailantDeathCount = 0;
	this->starInDuncanShack = false;
}

void M01_Duncan_InHere_ConvController_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type)
	{
		// Received from M01_Duncan_Assailer_JDG when killed
		if (param == 22)
		{
			if (++this->assailantDeathCount > 3)
			{
				Commands->Send_Custom_Event(obj, obj, 0, 16, 2.0f);
			}
		}

		// Received from M01_Commander_Shack_Zone_JDG when entered 
		else if (param == 27)
		{
			this->starInDuncanShack = true;
		}

		// Received from ourselves after 2 seconds when param 22 is received or after 10 seconds after this block
		else if(param == 16)
		{
			if (!this->starInDuncanShack)
			{
				GameObject *captainDuncanObj = Commands->Find_Object(106050);
				if (captainDuncanObj)
				{
					int conversationId = Commands->Create_Conversation("M01_DuncanInHere_Conversation", 100, 20.0f, false); // In here!
					Commands->Join_Conversation(captainDuncanObj, conversationId, false, true, true);
					
					Vector3 pos = Commands->Get_Position(obj);
					GameObject *starObj = Commands->Get_A_Star(pos);

					Commands->Join_Conversation(starObj, conversationId, false, false, false);
					Commands->Start_Conversation(conversationId, conversationId);

					Commands->Send_Custom_Event(obj, obj, 0, 16, 10.0f);
				}
			}
		}
	}
}

ScriptRegistrant<M01_Duncan_InHere_ConvController_JDG> M01_Duncan_InHere_ConvController_JDGRegistrant("M01_Duncan_InHere_ConvController_JDG", "");