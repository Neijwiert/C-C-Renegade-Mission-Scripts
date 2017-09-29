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
#include "M01_Priest_Datadisc_JDG.h"

// When the conversation M01_Church_Priest_JDG ends in M01_Priest_LuckyCharms_Conversation (Action_Complete)
void M01_Priest_Datadisc_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Commands->Set_HUD_Help_Text(7587, Vector3(0.196f, 0.882f, 0.196f)); // Data Link Updated - Satellite Image Enhanced\n

		GameObject *M01DataDiscTextControllerJDGObj = Commands->Find_Object(117188);
		if (M01DataDiscTextControllerJDGObj)
		{
			Commands->Send_Custom_Event(obj, M01DataDiscTextControllerJDGObj, 0, 10, 2.0f);
		}

		Commands->Clear_Map_Cell(5, 1);
		Commands->Clear_Map_Cell(5, 2);
		Commands->Clear_Map_Cell(5, 3);
		Commands->Clear_Map_Cell(5, 4);
		Commands->Clear_Map_Cell(5, 5);
		Commands->Clear_Map_Cell(6, 1);
		Commands->Clear_Map_Cell(6, 2);
		Commands->Clear_Map_Cell(6, 3);
		Commands->Clear_Map_Cell(6, 4);
		Commands->Clear_Map_Cell(6, 5);
		Commands->Clear_Map_Cell(7, 1);
		Commands->Clear_Map_Cell(7, 2);
		Commands->Clear_Map_Cell(7, 3);
		Commands->Clear_Map_Cell(7, 4);
		Commands->Clear_Map_Cell(7, 5);
		Commands->Clear_Map_Cell(8, 1);
		Commands->Clear_Map_Cell(8, 2);
		Commands->Clear_Map_Cell(8, 3);
		Commands->Clear_Map_Cell(8, 4);
		Commands->Clear_Map_Cell(8, 5);
		Commands->Clear_Map_Cell(9, 1);
		Commands->Clear_Map_Cell(9, 2);
		Commands->Clear_Map_Cell(9, 3);
		Commands->Clear_Map_Cell(9, 4);
		Commands->Clear_Map_Cell(9, 5);
	}
}

ScriptRegistrant<M01_Priest_Datadisc_JDG> M01_Priest_Datadisc_JDGRegistrant("M01_Priest_Datadisc_JDG", "");