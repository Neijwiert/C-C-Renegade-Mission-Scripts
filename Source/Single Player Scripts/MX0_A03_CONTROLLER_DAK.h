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

class MX0_A03_CONTROLLER_DAK : public ScriptImpClass
{
	public:
		virtual	void Register_Auto_Save_Variables();

	private:
		virtual void Created(GameObject *obj);
		virtual void Custom(GameObject *obj, int type, int param, GameObject *sender);

		int field_1C;
		int field_20;
		int field_24;
		int field_28;
		int field_2C;
		int field_30;
		int field_34;
		DWORD field_38;
		DWORD field_3C;
		int field_40;
		int field_44;
		DWORD field_48; // Not initialized and not used
		bool field_4C;
		bool field_4D;
		bool field_4E;
		DWORD field_50;
};