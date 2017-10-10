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

#pragma once

#include <scripts.h>

/*
M03 -> 1147534 1147533 1147532 1147531 1147530 1147529 1147527 1146701 1146700 300009 300028 300032 1141162 1144451 1144448 1144731 1144732 300010
*/
class M03_Killed_Sound : public ScriptImpClass
{
	private:
		virtual void Killed(GameObject *obj, GameObject *killer);
};