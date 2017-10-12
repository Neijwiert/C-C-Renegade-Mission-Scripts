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
M03 -> 1145881
M06 -> 100157 100330 100423 100445 101003 101004 101005 101006
M07 -> 100788 100780 100722 100721 100796 100798 100779
*/
class M03_Engineer_Target : public ScriptImpClass
{
	public:
		virtual	void Register_Auto_Save_Variables();

	private:
		virtual void Created(GameObject *obj);
		virtual void Destroyed(GameObject *obj);
		virtual void Damaged(GameObject *obj, GameObject *damager, float amount);
		virtual void Custom(GameObject *obj, int type, int param, GameObject *sender);
		virtual void Timer_Expired(GameObject *obj, int number);

		float initialHealth;
		int field_20;
};