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
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->field_20, sizeof(this->field_20), 2);
}

void M01_Duncan_InHere_ConvController_JDG::Created(GameObject *obj)
{
	this->field_1C = 0;
	this->field_20 = false;
}

// TODO
void M01_Duncan_InHere_ConvController_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type)
	{
		if (param == 22)
		{
			if (++this->field_1C > 3)
			{
				Commands->Send_Custom_Event(obj, obj, 0, 16, 2.0f);
			}
		}
		else if (param == 27)
		{
			this->field_20 = true;
		}
		else if(param == 16)
		{
			if (!this->field_20)
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