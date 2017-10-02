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
#include "M01_HON_Escorts_Warroom_MCT_Protector02_JDG.h"

/*
M01 -> 103375
*/
void M01_HON_Escorts_Warroom_MCT_Protector02_JDG::Created(GameObject *obj)
{
	Commands->Set_Innate_Is_Stationary(obj, true);
}

ScriptRegistrant<M01_HON_Escorts_Warroom_MCT_Protector02_JDG> M01_HON_Escorts_Warroom_MCT_Protector02_JDGRegistrant("M01_HON_Escorts_Warroom_MCT_Protector02_JDG", "");