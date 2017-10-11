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
#include "M03_AggAndCover.h"

/*
M03 -> 1144448 1144731 1144732
*/
void M03_AggAndCover::Created(GameObject *obj)
{
	Commands->Set_Innate_Aggressiveness(obj, 50.0f);
	Commands->Set_Innate_Take_Cover_Probability(obj, 100.0f);
}

ScriptRegistrant<M03_AggAndCover> M03_AggAndCoverRegistrant("M03_AggAndCover", "");