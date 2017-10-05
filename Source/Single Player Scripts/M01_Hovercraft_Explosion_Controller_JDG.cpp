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
#include "M01_Hovercraft_Explosion_Controller_JDG.h"

/*
M01 -> 121839
*/
void M01_Hovercraft_Explosion_Controller_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->beachMediumTankObjId, sizeof(this->beachMediumTankObjId), 1);
}

void M01_Hovercraft_Explosion_Controller_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type)
	{
		// Received from M01_Medium_Tank01_JDG when created
		if (param == 228)
		{
			this->beachMediumTankObjId = Commands->Get_ID(sender);
		}

		// Received from M01_FodderHovercraft_Script_JDG when killed
		else if (param == 22)
		{
			GameObject *beachMediumTankObj = Commands->Find_Object(this->beachMediumTankObjId);
			if (beachMediumTankObj)
			{
				Commands->Apply_Damage(beachMediumTankObj, 100000.0f, "Steel", NULL);
			}
		}
	}
}

ScriptRegistrant<M01_Hovercraft_Explosion_Controller_JDG> M01_Hovercraft_Explosion_Controller_JDGRegistrant("M01_Hovercraft_Explosion_Controller_JDG", "");