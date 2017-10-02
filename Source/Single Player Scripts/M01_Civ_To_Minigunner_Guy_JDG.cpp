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
#include "M01_Civ_To_Minigunner_Guy_JDG.h"

// When M01_HON_Dojo_Civ_01_JDG completes action with id 49
void M01_Civ_To_Minigunner_Guy_JDG::Created(GameObject *obj)
{
	GameObject *nonExistingObj = Commands->Find_Object(101938);
	if (nonExistingObj)
	{
		Vector3 nonExistingObjPos = Commands->Get_Position(nonExistingObj);

		ActionParamsStruct params;
		params.Set_Basic(this, 50.0f, 38);
		params.Set_Movement(nonExistingObjPos, 0.8f, 20.0f);

		Commands->Action_Goto(obj, params);
	}
}

ScriptRegistrant<M01_Civ_To_Minigunner_Guy_JDG> M01_Civ_To_Minigunner_Guy_JDGRegistrant("M01_Civ_To_Minigunner_Guy_JDG", "");