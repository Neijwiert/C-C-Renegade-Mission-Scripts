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
#include "M00_ArmorMedal_TextMessage_JDG.h"

/*
M02 -> 406723
M08 -> 1600226
M09 -> 1202190
M10 -> 1211505
*/
void M00_ArmorMedal_TextMessage_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Commands->Set_HUD_Help_Text(7670, Vector3(0.196f, 0.882f, 0.196f)); // Maximum Armor Increased\n
	}
}

ScriptRegistrant<M00_ArmorMedal_TextMessage_JDG> M00_ArmorMedal_TextMessage_JDGRegistrant("M00_ArmorMedal_TextMessage_JDG", "");