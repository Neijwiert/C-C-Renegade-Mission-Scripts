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
M03 -> 1146701 1146700 300009 300028 1144724 1144723 1144722 1144721 1144720 1144719 1144718 1144717 1144716 1144715 1144714 1144713 1144712 1144711 1144448 1144731 1144732 300010
M10 -> 2009686 2009685 1111329
*/
class RMV_Home_Point : public ScriptImpClass
{
	private:
		virtual void Created(GameObject *obj);
};