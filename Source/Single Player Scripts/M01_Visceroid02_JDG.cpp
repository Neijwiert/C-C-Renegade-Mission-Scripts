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
#include "M01_Visceroid02_JDG.h"

/*
M01 -> 106909
*/
void M01_Visceroid02_JDG::Created(GameObject *obj)
{
	Commands->Innate_Disable(obj);
}

void M01_Visceroid02_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M01_TibCave_StartZone_JDG when entered
	if (!type && param == 16)
	{
		Commands->Innate_Enable(obj);
		
		Vector3 pos = Commands->Get_Position(obj);
		Commands->Set_Innate_Soldier_Home_Location(obj, pos, 5.0f);

		GameObject *visceroidNodGuy1Obj = Commands->Find_Object(106910);
		if (visceroidNodGuy1Obj)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 16);
			params.Set_Attack(visceroidNodGuy1Obj, 10.0f, 0.0f, true);
			params.AttackCheckBlocked = false;

			Commands->Action_Attack(obj, params);
		}
	}
}

ScriptRegistrant<M01_Visceroid02_JDG> M01_Visceroid02_JDGRegistrant("M01_Visceroid02_JDG", "");