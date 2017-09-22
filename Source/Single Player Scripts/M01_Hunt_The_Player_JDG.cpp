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
#include "M01_Hunt_The_Player_JDG.h"

// After 401/489 cinematic frames in X1I_EasyNodChinook.txt
// After 401/489 cinematic frames in X1I_EasyNodChinook01.txt
void M01_Hunt_The_Player_JDG::Created(GameObject *obj)
{
	Commands->Enable_Hibernation(obj, false);

	Commands->Start_Timer(obj, this, 60.0f, 24);
	Commands->Start_Timer(obj, this, 5.0f, 65);

	ActionParamsStruct params;
	params.Set_Basic(this, 45.0f, 63);

	Vector3 pos = Commands->Get_Position(obj);
	GameObject *starObj = Commands->Get_A_Star(pos);

	params.Set_Movement(starObj, 0.8f, 1.0f);

	Commands->Action_Goto(obj, params);
}

void M01_Hunt_The_Player_JDG::Timer_Expired(GameObject *obj, int number)
{
	// Triggered after 60 seconds on create or 20 seconds after this block
	if (number == 24)
	{
		Commands->Enable_Hibernation(obj, true);
		
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);
		Vector3 starPos = Commands->Get_Position(starObj);

		float distance = Commands->Get_Distance(starPos, pos);
		if (distance < 80.0f || Commands->Is_Object_Visible(obj, starObj))
		{
			Commands->Start_Timer(obj, this, 20.0f, 24);
		}
		else
		{
			Commands->Apply_Damage(obj, 100000.0f, "BlamoKiller", NULL);
		}
	}

	// Triggered 5 seconds after create or 5 seconds after this block
	else if (number == 65)
	{
		if (obj)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 45.0f, 63);

			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			params.Set_Movement(starObj, 0.8f, 1.0f);

			Commands->Action_Goto(obj, params);

			Commands->Start_Timer(obj, this, 5.0f, 65);
		}
	}
}

ScriptRegistrant<M01_Hunt_The_Player_JDG> M01_Hunt_The_Player_JDGRegistrant("M01_Hunt_The_Player_JDG", "");