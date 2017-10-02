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
M01 -> 107554
*/
class M01_PaintballRoom_ChatterController_JDG : public ScriptImpClass
{
	public:
		virtual	void Register_Auto_Save_Variables();

	private:
		virtual void Created(GameObject *obj);
		virtual void Custom(GameObject *obj, int type, int param, GameObject *sender);

		int honPaintballTeam02OfficerObjId;
		int honPaintballTeam01OfficerObjId;
		int honPaintballTeam01Guy02ObjId;
		int honPaintballTeam01Guy01ObjId;
		int honPaintballTeam02Guy01ObjId;
		int honPaintballTeam02Guy02ObjId;
		bool field_34;
		bool field_35;
		bool field_36;
		bool field_37;
		bool field_38;
		bool field_39;
		bool field_3A;
};