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
#include "M10_Pokeable_Item_OnePoke.h"

/*
M03 -> 1145036 1100009
M10 -> 2017706 1100170 1100169 1100166
*/
void M10_Pokeable_Item_OnePoke::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->poked, sizeof(this->poked), 1);
}

void M10_Pokeable_Item_OnePoke::Created(GameObject *obj)
{
	this->poked = false;

	Commands->Enable_HUD_Pokable_Indicator(obj, true);
}

void M10_Pokeable_Item_OnePoke::Poked(GameObject *obj, GameObject *poker)
{
	if (!this->poked)
	{
		this->poked = true;

		Commands->Enable_HUD_Pokable_Indicator(obj, false);
	}
}

ScriptRegistrant<M10_Pokeable_Item_OnePoke> M10_Pokeable_Item_OnePokeRegistrant("M10_Pokeable_Item_OnePoke", "");