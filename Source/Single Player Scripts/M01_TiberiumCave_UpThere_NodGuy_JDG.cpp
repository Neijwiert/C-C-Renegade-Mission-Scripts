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
#include "M01_TiberiumCave_UpThere_NodGuy_JDG.h"

/*
M01 -> 103274
*/
void M01_TiberiumCave_UpThere_NodGuy_JDG::Created(GameObject *obj)
{
	Vector3 pos = Commands->Get_Position(obj);
	Commands->Set_Innate_Soldier_Home_Location(obj, pos, 5.0f);
}

// TODO
void M01_TiberiumCave_UpThere_NodGuy_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type)
	{
		if (param == 16)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 16);
			
			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			params.Set_Attack(starObj, 0.0f, 0.0f, true);

			Commands->Action_Attack(obj, params);

			Commands->Send_Custom_Event(obj, obj, 0, 27, 3.0f);
		}
		else if (param == 27)
		{
			Vector3 pos = Commands->Get_Position(obj);
			Commands->Create_Sound("MX1DSGN_DSGN0019I1DSGN_SND", pos, obj);

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 16);

			GameObject *starObj = Commands->Get_A_Star(pos);

			params.Set_Attack(starObj, 50.0f, 0.0f, true);

			Commands->Action_Attack(obj, params);
		}
	}
}

ScriptRegistrant<M01_TiberiumCave_UpThere_NodGuy_JDG> M01_TiberiumCave_UpThere_NodGuy_JDGRegistrant("M01_TiberiumCave_UpThere_NodGuy_JDG", "");