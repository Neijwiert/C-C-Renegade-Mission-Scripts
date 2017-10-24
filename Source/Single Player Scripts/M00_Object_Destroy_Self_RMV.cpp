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
#include "M00_Object_Destroy_Self_RMV.h"

/*
M03 -> 1100001 1100007 1100014 1100015
*/
void M00_Object_Destroy_Self_RMV::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->startNow, sizeof(this->startNow), 1);
	Auto_Save_Variable(&this->debugMode, sizeof(this->debugMode), 2);
	Auto_Save_Variable(&this->receiveType, sizeof(this->receiveType), 3);
	Auto_Save_Variable(&this->receiveParamOn, sizeof(this->receiveParamOn), 4);
	Auto_Save_Variable(&this->receiveParamOff, sizeof(this->receiveParamOff), 5);
	Auto_Save_Variable(&this->receiveParamActivate, sizeof(this->receiveParamActivate), 6);
}

void M00_Object_Destroy_Self_RMV::Created(GameObject *obj)
{
	this->startNow = (Get_Int_Parameter("Start_Now") == 1);
	this->debugMode = (Get_Int_Parameter("Debug_Mode") == 1);
	this->receiveType = Get_Int_Parameter("Receive_Type");
	this->receiveParamOn = Get_Int_Parameter("Receive_Param_On");
	this->receiveParamOff = Get_Int_Parameter("Receive_Param_Off");
	this->receiveParamActivate = Get_Int_Parameter("Receive_Param_Activate");
}

void M00_Object_Destroy_Self_RMV::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (this->debugMode)
	{
		Commands->Debug_Message("M00_Trigger_Destroy_Self_RMV received custom, type %d and param %d.\n", type, param);
	}

	if (type == this->receiveType)
	{
		if (param == this->receiveParamOn)
		{
			this->startNow = true;

			if (this->debugMode)
			{
				int objId = Commands->Get_ID(obj);
				Commands->Debug_Message("M00_Trigger_Destroy_Self_RMV on object %d made ACTIVE.\n", objId);
			}
		}

		if (param == this->receiveParamOff)
		{
			this->startNow = false;

			if (this->debugMode)
			{
				int objId = Commands->Get_ID(obj);
				Commands->Debug_Message("M00_Trigger_Destroy_Self_RMV on object %d made INACTIVE.\n", objId);
			}
		}

		// Received from RMV_Trigger_Zone after 0 seconds when entered. type = 200, param = 200 (id = 1100015)
		// Received from RMV_Trigger_Zone after 0 seconds when entered. type = 3, param = 3 (id = 1100001)
		// Received from RMV_Trigger_Zone after 0 seconds when entered. type = 200, param = 200 (id = 1100007)
		// Received from RMV_Trigger_Zone after 0 seconds when entered. type = 3, param = 3 (id = 1100014)
		if (param == this->receiveParamActivate && this->startNow)
		{
			if (this->debugMode)
			{
				int objId = Commands->Get_ID(obj);
				Commands->Debug_Message("M00_Trigger_Destroy_Self_RMV activated on object %d, destroying.\n", objId);
			}

			Commands->Destroy_Object(obj);
		}
	}
}

ScriptRegistrant<M00_Object_Destroy_Self_RMV> M00_Object_Destroy_Self_RMVRegistrant("M00_Object_Destroy_Self_RMV", "Start_Now=1:int, Receive_Type=3:int, Receive_Param_On=1:int, Receive_Param_Off=0:int, Receive_Param_Activate:int, Debug_Mode=0:int");