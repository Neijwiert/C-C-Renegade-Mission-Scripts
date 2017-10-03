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
#include "M01_MovieProjector_JDG.h"

/*
M01 -> 103942
*/
void M01_MovieProjector_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->damagedByStar, sizeof(this->damagedByStar), 1);
}

void M01_MovieProjector_JDG::Created(GameObject *obj)
{
	this->damagedByStar = false;
}

void M01_MovieProjector_JDG::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (damager == Commands->Get_A_Star(pos) && !damagedByStar)
	{
		this->damagedByStar = true;

		Commands->Set_Animation(obj, "DSP_PROJECTOR.DSP_PROJECTOR", false, NULL, 0.0f, 9.0f, false);

		GameObject *M01PropagandaSoundsControllerJDGObj = Commands->Find_Object(103099);
		if (M01PropagandaSoundsControllerJDGObj)
		{
			Commands->Send_Custom_Event(obj, M01PropagandaSoundsControllerJDGObj, 0, 22, 0.0f);
		}

		Commands->Create_Explosion("Explosion_Barrel_Fuel", pos, NULL);
	}

	Commands->Set_Health(obj, 0.25f);
}

ScriptRegistrant<M01_MovieProjector_JDG> M01_MovieProjector_JDGRegistrant("M01_MovieProjector_JDG", "");