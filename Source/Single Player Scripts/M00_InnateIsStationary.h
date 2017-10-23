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
M03 -> 300052
M05 -> 100648 100170 100158 100109 100100 100099 100647 100264 100157 100108 100169
M07 -> 123257 100672 100671 100732 100733 101053
M09 -> 2005480
M10 -> 1100134 1100135
x5d_c130troopdrop4.txt at frame(s): 145
x5d_c130troopdrop3.txt at frame(s): 145
*/
class M00_InnateIsStationary : public ScriptImpClass
{
	private:
		virtual void Created(GameObject *obj);
};