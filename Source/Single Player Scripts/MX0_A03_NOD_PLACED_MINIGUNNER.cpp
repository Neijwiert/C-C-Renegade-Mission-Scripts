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
#include "MX0_A03_NOD_PLACED_MINIGUNNER.h"

// On level start
void MX0_A03_NOD_PLACED_MINIGUNNER::Created(GameObject *obj)
{
	Vector3 pos = Commands->Get_Position(obj);

	Commands->Set_Innate_Soldier_Home_Location(obj, pos, 5.0f);
	Commands->Innate_Disable(obj);
	Commands->Set_Innate_Is_Stationary(obj, true);
}

void MX0_A03_NOD_PLACED_MINIGUNNER::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from MX0_A03_CONTROLLER_DAK when custom type 401 is received
	if (type == 417)
	{
		Commands->Innate_Enable(obj);
		Commands->Set_Innate_Is_Stationary(obj, false);
	}
}

ScriptRegistrant<MX0_A03_NOD_PLACED_MINIGUNNER> MX0_A03_NOD_PLACED_MINIGUNNERRegistrant("MX0_A03_NOD_PLACED_MINIGUNNER", "");