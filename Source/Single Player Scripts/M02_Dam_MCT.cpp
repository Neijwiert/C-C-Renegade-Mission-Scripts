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
#include "M02_Dam_MCT.h"

void M02_Dam_MCT::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->destroyed, sizeof(this->destroyed), 1);
}

// On level start
void M02_Dam_MCT::Created(GameObject *obj)
{
	this->destroyed = false;
}

void M02_Dam_MCT::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (Commands->Get_Health(obj) < 1.0f)
	{
		Commands->Set_Health(obj, 0.1f);

		if (!this->destroyed)
		{
			this->destroyed = true;

			GameObject *M02ObjectiveControllerObj = Commands->Find_Object(1111112);
			if (M02ObjectiveControllerObj)
			{
				Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 203, 1, 0.0f);
				
				Vector3 pos = Commands->Get_Position(obj);
				GameObject *starObj = Commands->Get_A_Star(pos);

				Commands->Grant_Key(starObj, 6, true);
			}

			Commands->Set_Animation_Frame(obj, "MCT_NOD.MCT_NOD", 7);
		}
	}
}

ScriptRegistrant<M02_Dam_MCT> M02_Dam_MCTRegistrant("M02_Dam_MCT", "");