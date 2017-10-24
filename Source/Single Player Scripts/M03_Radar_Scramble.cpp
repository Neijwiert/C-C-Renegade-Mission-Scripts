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

#include "General.h"
#include "M03_Radar_Scramble.h"

/*
M03 -> 1147513 1144628
*/
void M03_Radar_Scramble::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->radarScrambledBefore, sizeof(this->radarScrambledBefore), 1);
}

void M03_Radar_Scramble::Created(GameObject *obj)
{
	this->radarScrambledBefore = false;
}

void M03_Radar_Scramble::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from ourselves after 0 seconds when entered
	if (type == 40004)
	{
		this->radarScrambledBefore = true;
	}
}

void M03_Radar_Scramble::Entered(GameObject *obj, GameObject *enterer)
{
	GameObject *commCenterBuildingObj = Commands->Find_Object(1150002);
	if (Commands->Get_Health(commCenterBuildingObj) > 0.0f)
	{
		if (!this->radarScrambledBefore)
		{
			GameObject *M03RadarScrambleScriptZone1Obj = Commands->Find_Object(1147513);
			Commands->Send_Custom_Event(obj, M03RadarScrambleScriptZone1Obj, 40004, 0, 0.0f);

			GameObject *M03RadarScrambleScriptZone2Obj = Commands->Find_Object(1144628);
			Commands->Send_Custom_Event(obj, M03RadarScrambleScriptZone2Obj, 40004, 0, 0.0f);

			this->radarScrambledBefore = true;

			int conversationId = Commands->Create_Conversation("M03CON068", 99, 2000.0f, false); // Warning - localized radar jamming detected.
			Commands->Join_Conversation(NULL, conversationId, true, true, true);
			Commands->Start_Conversation(conversationId, 100068);
			Commands->Monitor_Conversation(obj, conversationId);
		}

		Commands->Enable_Radar(false);
	}
}

ScriptRegistrant<M03_Radar_Scramble> M03_Radar_ScrambleRegistrant("M03_Radar_Scramble", "");