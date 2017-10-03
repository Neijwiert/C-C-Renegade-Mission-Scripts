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
#include "M01_Comm_Base_Commander_JDG.h"

/*
M01 -> 101936
*/
void M01_Comm_Base_Commander_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->awareOfStar, sizeof(this->awareOfStar), 1);
}

void M01_Comm_Base_Commander_JDG::Created(GameObject *obj)
{
	Commands->Innate_Disable(obj);

	this->awareOfStar = false;
}

void M01_Comm_Base_Commander_JDG::Killed(GameObject *obj, GameObject *killer)
{
	Vector3 pos = Commands->Get_Position(obj);
	GameObject *starObj = Commands->Get_A_Star(pos);

	if (killer == starObj)
	{
		GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
		Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 216, 0.0f);

		Vector3 keyCardPos = pos;
		keyCardPos.Z += 0.75f;

		if (!Commands->Has_Key(starObj, 2))
		{
			Commands->Create_Object("Level_02_Keycard", keyCardPos); // It just seriously creates a keycard, instead of granting one...
		}
	}
}

void M01_Comm_Base_Commander_JDG::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	Vector3 pos = Commands->Get_Position(obj);
	GameObject *starObj = Commands->Get_A_Star(pos);

	if (damager == starObj && !this->awareOfStar)
	{
		this->awareOfStar = true;

		Commands->Innate_Enable(obj);
		Commands->Action_Reset(obj, 100.0f);

		Commands->Innate_Force_State_Enemy_Seen(obj, starObj);
		Commands->Enable_Hibernation(obj, true);
	}
}

void M01_Comm_Base_Commander_JDG::Enemy_Seen(GameObject *obj, GameObject *enemy)
{
	Vector3 pos = Commands->Get_Position(obj);
	GameObject *starObj = Commands->Get_A_Star(pos);

	if (enemy == starObj && !this->awareOfStar)
	{
		this->awareOfStar = true;

		Commands->Innate_Enable(obj);
		Commands->Action_Reset(obj, 100.0f);

		Commands->Innate_Force_State_Enemy_Seen(obj, starObj);
		Commands->Enable_Hibernation(obj, true);
	}
}

ScriptRegistrant<M01_Comm_Base_Commander_JDG> M01_Comm_Base_Commander_JDGRegistrant("M01_Comm_Base_Commander_JDG", "");