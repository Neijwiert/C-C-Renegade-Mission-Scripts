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
M03 -> 1144606
*/
class M03_Announce_Refinery_Controller_JDG : public ScriptImpClass
{
	public:
		virtual	void Register_Auto_Save_Variables();

	private:
		virtual void Created(GameObject *obj);
		virtual void Killed(GameObject *obj, GameObject *killer);
		virtual void Custom(GameObject *obj, int type, int param, GameObject *sender);

		int soundIndexerMaxRandom;
		float minSound2Delay;
		float maxSound2Delay;
		int startupParam;
		int field_2C;
		int soundIndexerParam;
		int sound2Param;
		int sound1Param;
		int sound2Index;
		int sound1Index;
		Vector3 soundLocation1;
		Vector3 soundLocation2;
		Vector3 soundLocation3;
		Vector3 soundLocation4;
		Vector3 soundLocation5;
		Vector3 soundLocation6;
		Vector3 soundLocation7;
		Vector3 soundLocation8;
		Vector3 soundLocation9;
		Vector3 soundLocation10;
		Vector3 soundLocation11;
		Vector3 soundLocation12;
		Vector3 soundLocation13;
		Vector3 soundLocation14;
		Vector3 soundLocation15;
		Vector3 soundLocation16;
		Vector3 soundLocation17;
};