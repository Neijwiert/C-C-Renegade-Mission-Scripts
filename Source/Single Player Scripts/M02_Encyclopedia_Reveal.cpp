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
#include "M02_Encyclopedia_Reveal.h"

/*
M02 -> 300799 300800
*/
void M02_Encyclopedia_Reveal::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		if (Get_Int_Parameter("Disk_ID") == 1)
		{
			Commands->Reveal_Encyclopedia_Vehicle(5); // Nod_Jet
		}
		else
		{
			Commands->Reveal_Encyclopedia_Building(15); // SP_Refinery_Nod
		}

		Commands->Set_HUD_Help_Text(7588, Vector3(0.196f, 0.882f, 0.196f)); // Data Link Updated - Nod Database Expanded\n
	}
}

ScriptRegistrant<M02_Encyclopedia_Reveal> M02_Encyclopedia_RevealRegistrant("M02_Encyclopedia_Reveal", "Disk_ID:int");