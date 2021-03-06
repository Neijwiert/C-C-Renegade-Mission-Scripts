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
M02 -> 403133 403132 403131 467781 1100085 1100094 1100120 1100130 1157687 1157688 1157690
*/
class M02_Destroy_Objective : public ScriptImpClass
{
	private:
		virtual void Killed(GameObject *obj, GameObject *killer);
		virtual void Damaged(GameObject *obj, GameObject *damager, float amount);
};