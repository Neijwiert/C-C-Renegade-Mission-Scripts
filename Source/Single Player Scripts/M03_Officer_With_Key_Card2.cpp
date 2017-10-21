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
#include "M03_Officer_With_Key_Card2.h"

/*
M03 -> 2013085
*/
void M03_Officer_With_Key_Card2::Killed(GameObject *obj, GameObject *killer)
{
	if (!Commands->Has_Key(killer, 2))
	{
		Vector3 pos = Commands->Get_Position(obj);
		pos.Z += 0.5f;

		Commands->Create_Object("Level_02_Keycard", pos);
	}
}

ScriptRegistrant<M03_Officer_With_Key_Card2> M03_Officer_With_Key_Card2Registrant("M03_Officer_With_Key_Card2", "");