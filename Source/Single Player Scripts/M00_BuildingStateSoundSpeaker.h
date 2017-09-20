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
M02 -> 102416 102415 101535 101534 101533 101532 101531 101529 101528 101526 101522 101521 101520 101519 101010 101012 101013 101016 101018 101020 101022 101024 101026 101027 101029 101030 101031 101033 101036 101039 101041 101043 101045 101047 101049 101051 101054 101058 101060 101062 101063 101064 101065 101066 101067 101068 101069 101070 101071 101072 101073 101074 101075 101083 101084 101085 101086 101089 101090 101091 101092 101093 101094 101095 101096 101097 101098 101099 101100 101101 101105 101107 101109 101110 101111 101112 101113 101115 101116 101118 101119
M13 -> 1700013 1700011 1700009 1700007
*/
class M00_BuildingStateSoundSpeaker : public ScriptImpClass
{
	public:
		virtual	void Register_Auto_Save_Variables();

	private:
		virtual void Created(GameObject *obj);
		virtual void Custom(GameObject *obj, int type, int param, GameObject *sender);
		virtual void Timer_Expired(GameObject *obj, int number);

		int soundId;
		bool isDestroyed;
		bool doExplosions;
};