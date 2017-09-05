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
#include "M00_BuildingStateSoundController.h"

void M00_BuildingStateSoundController::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->buildingSpeakerId, sizeof(this->buildingSpeakerId), 1);
}

void M00_BuildingStateSoundController::Created(GameObject *obj)
{
	this->buildingSpeakerId = Get_Int_Parameter("BuildingSpeaker_ID");
}

void M00_BuildingStateSoundController::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *buildingSpeakerObj = Commands->Find_Object(this->buildingSpeakerId);
	if (buildingSpeakerObj)
	{
		Commands->Send_Custom_Event(obj, buildingSpeakerObj, 9026, 1, 0.0f);
		Commands->Send_Custom_Event(obj, buildingSpeakerObj, 9027, 1, 0.0f);
	}
}

void M00_BuildingStateSoundController::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// TODO (No custom)
	if (type == 9028)
	{
		GameObject *buildingSpeakerObj = Commands->Find_Object(this->buildingSpeakerId);
		if (buildingSpeakerObj)
		{
			Commands->Send_Custom_Event(obj, buildingSpeakerObj, 9028, 1, 0.0f);
		}
	}

	// TODO (No custom)
	else if (type == 9029)
	{
		GameObject *buildingSpeakerObj = Commands->Find_Object(this->buildingSpeakerId);
		if (buildingSpeakerObj)
		{
			Commands->Send_Custom_Event(obj, buildingSpeakerObj, 9029, 1, 0.0f);
		}
	}
}

ScriptRegistrant<M00_BuildingStateSoundController> M00_BuildingStateSoundControllerRegistrant("M00_BuildingStateSoundController", "BuildingSpeaker_ID:int");