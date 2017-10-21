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
#include "M03_Mct_Poke.h"

/*
M03 -> 1100009
*/
void M03_Mct_Poke::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->poked, sizeof(this->poked), 1);
}

void M03_Mct_Poke::Created(GameObject *obj)
{
	this->poked = false;
}

void M03_Mct_Poke::Poked(GameObject *obj, GameObject *poker)
{
	if (!this->poked)
	{
		this->poked = true;

		GameObject *M03CommCenterArrowObj = Commands->Find_Object(2009818);
		Commands->Send_Custom_Event(obj, M03CommCenterArrowObj, 40002, 0, 0.0f);
	}
}

ScriptRegistrant<M03_Mct_Poke> M03_Mct_PokeRegistrant("M03_Mct_Poke", "");