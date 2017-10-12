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
#include "M03_DataDiscMessage.h"

/*
M03 -> 1206590 1206589
M09 -> 1201187
*/
void M03_DataDiscMessage::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Commands->Set_HUD_Help_Text(7588, Vector3(0.196f, 0.882f, 0.196f)); // Data Link Updated - Nod Database Expanded\n
	}
}

ScriptRegistrant<M03_DataDiscMessage> M03_DataDiscMessageRegistrant("M03_DataDiscMessage", "");