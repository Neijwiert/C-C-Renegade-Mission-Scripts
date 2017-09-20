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
M02 -> 304006 303203 301601 401982 401187 401186 401114 401113 401102 401101 401066 401001 400502 400501 400316 400274 400273 400272 400271 400270 400269 400268 400267 400198 400197 400196 400195 400194 400193 400189 400188 400187 400186 400185 400184 400192 400201 400551 400990 400991 400992 401029 401054 401070 401079 401080 401123 401130 401131 401196 405116 405117 405118 405119 405120 405121 405122
*/
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
		void Call_GDI_Soldiers(int areaId);
		void Create_Apache(int areaId);
		void Create_Sakura(int areaId);
		void Create_Buggy(int areaId);

		bool canSendCustomAndDestroy;
		bool isZoneTriggered;
		bool starWarnedForLeavingPrematurely;
		int mendozaObjId;
};