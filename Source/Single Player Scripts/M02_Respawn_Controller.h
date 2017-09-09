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

class M02_Respawn_Controller : public ScriptImpClass
{
	public:
		virtual	void Register_Auto_Save_Variables();

	private:
		virtual void Created(GameObject *obj);
		virtual void Custom(GameObject *obj, int type, int param, GameObject *sender);
		virtual void Timer_Expired(GameObject *obj, int number);

		void Check_Respawns(int a2);
		void Replacement_Vehicle(GameObject *obj, int a3);
		void Use_Heli_Drop_01(int a2);
		void Use_Heli_Drop_02(int a2);
		void Use_Parachute_Drop(int a2);
		bool Use_Spawners(int a2);

		bool field_1C[26];
		bool field_36[26];
		bool field_50[26];
		bool field_6A[26];
		BYTE field_84[26];
		BYTE field_9E[26];
		int field_B8[26];
		DWORD field_120;
		int field_124;
		DWORD field_128;
		DWORD field_12C;
};