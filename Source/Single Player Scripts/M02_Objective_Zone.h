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

class M02_Objective_Zone : public ScriptImpClass
{
	public:
		virtual	void Register_Auto_Save_Variables();

	private:
		virtual void Created(GameObject *obj);
		virtual void Custom(GameObject *obj, int type, int param, GameObject *sender);
		virtual void Timer_Expired(GameObject *obj, int number);
		virtual void Entered(GameObject *obj, GameObject *enterer);

		void Send_and_Destroy(GameObject *obj, int type);
		void Call_GDI_Soldiers(int a2);
		void Create_Apache(int a2);
		void Create_Sakura(int a2);
		void Create_Buggy(int a2);

		bool canSendCustomAndDestroy;
		bool isZoneTriggered;
		bool starWarnedForLeavingPrematurely;
		int mendozaObjId;
};