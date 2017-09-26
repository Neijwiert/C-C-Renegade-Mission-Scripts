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
#include "M01_Visceroid_NodGuy02_JDG.h"

/*
M01 -> 106911
*/
void M01_Visceroid_NodGuy02_JDG::Created(GameObject *obj)
{
	Commands->Innate_Disable(obj);
}

// TODO
void M01_Visceroid_NodGuy02_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type && param == 16)
	{
		Commands->Innate_Enable(obj);

		Vector3 pos = Commands->Get_Position(obj);
		Commands->Set_Innate_Soldier_Home_Location(obj, pos, 5.0f);

		Commands->Create_Sound("MX1DSGN_DSGN0018I1DSGN_SND", pos, obj);

		GameObject *visceroid1Obj = Commands->Find_Object(106908);
		if (visceroid1Obj)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 90.0f, 16);
			params.Set_Attack(visceroid1Obj, 25.0f, 0.0f, true);
			params.AttackCheckBlocked = false;

			Commands->Action_Attack(obj, params);
		}
	}
}

ScriptRegistrant<M01_Visceroid_NodGuy02_JDG> M01_Visceroid_NodGuy02_JDGRegistrant("M01_Visceroid_NodGuy02_JDG", "");