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
#include "M00_Put_Script_On_Commando.h"

/*
M03 -> 1100004
*/
void M00_Put_Script_On_Commando::Created(GameObject *obj)
{
	Commands->Start_Timer(obj, this, 0.2f, 0);
}

// TODO
void M00_Put_Script_On_Commando::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 12176 && param == 12176)
	{
		Commands->Start_Timer(obj, this, 2.0f, 0);
	}
}

// TODO
void M00_Put_Script_On_Commando::Timer_Expired(GameObject *obj, int number)
{
	if (!number)
	{
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);
		if (starObj)
		{
			Commands->Attach_Script(starObj, "M00_Damaged_Warning", "");
		}
		else
		{
			Commands->Start_Timer(obj, this, 1.0f, 0);
		}
	}
}

ScriptRegistrant<M00_Put_Script_On_Commando> M00_Put_Script_On_CommandoRegistrant("M00_Put_Script_On_Commando", "");