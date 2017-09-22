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

#include  "General.h"
#include "M01_Nod_Chinook_Reinforcement_Guy_JDG.h"

// After 401/489 cinematic frames in X1I_EasyNodChinook.txt
// After 401/489 cinematic frames in X1I_EasyNodChinook01.txt
void M01_Nod_Chinook_Reinforcement_Guy_JDG::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, "s_a_human.h_a_troopdrop"))
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 10);
		
		float randXOffset = Commands->Get_Random(1.0f, 3.0f);
		float randYOffset = Commands->Get_Random(1.0f, 3.0f);

		Vector3 pos = Commands->Get_Position(obj);

		params.Set_Movement(Vector3(pos.X + randXOffset, pos.Y + randYOffset, 1.0f), 0.25f, 0.0f);

		Commands->Action_Goto(obj, params);

		Commands->Send_Custom_Event(obj, obj, 0, 65, 1.0f);
	}
}

ScriptRegistrant<M01_Nod_Chinook_Reinforcement_Guy_JDG> M01_Nod_Chinook_Reinforcement_Guy_JDGRegistrant("M01_Nod_Chinook_Reinforcement_Guy_JDG", "");