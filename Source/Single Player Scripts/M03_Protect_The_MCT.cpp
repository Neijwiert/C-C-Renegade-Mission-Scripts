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
#include "M03_Protect_The_MCT.h"

/*
M03 -> 300047, 300048, 300049, 300045
*/
void M03_Protect_The_MCT::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->starSeen, sizeof(this->starSeen), 1);
}

void M03_Protect_The_MCT::Created(GameObject *obj)
{
	this->starSeen = false;
}

void M03_Protect_The_MCT::Enemy_Seen(GameObject *obj, GameObject *enemy)
{
	if (!this->starSeen && Commands->Is_A_Star(enemy))
	{
		this->starSeen = true;

		int building = Get_Int_Parameter("Building");
		if (building == 1) // Comms?
		{
			Commands->Set_Innate_Soldier_Home_Location(obj, Vector3(-124.763f, 41.308f, -1.948f), 10.0f);
		}
		else if (building == 2) // PP
		{
			Commands->Set_Innate_Soldier_Home_Location(obj, Vector3(-84.583f, 11.975f, -7.074f), 10.0f);
		}
		else if(!building) // Ref
		{
			Commands->Set_Innate_Soldier_Home_Location(obj, Vector3(-154.921f, 15.227f, 2.382f), 10.0f);
		}
	}
}

ScriptRegistrant<M03_Protect_The_MCT> M03_Protect_The_MCTRegistrant("M03_Protect_The_MCT", "Building:int");