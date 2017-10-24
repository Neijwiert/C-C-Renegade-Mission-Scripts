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
#include "M00_Trigger_When_Killed_RMV.h"

#include "M00_Controller_Trigger_RMV.h"

/*
M03 -> 300059 300058 300056 1150001 1100020
*/
void M00_Trigger_When_Killed_RMV::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->startNow, sizeof(this->startNow), 1);
	Auto_Save_Variable(&this->debugMode, sizeof(this->debugMode), 2);
}

void M00_Trigger_When_Killed_RMV::Created(GameObject *obj)
{
	this->debugMode = (Get_Int_Parameter("Debug_Mode") == 1);
	this->startNow = (Get_Int_Parameter("Start_Now") != 0);
}

void M00_Trigger_When_Killed_RMV::Killed(GameObject *obj, GameObject *killer)
{
	int targetId = Get_Int_Parameter("Target_ID");
	float minDelay = Get_Float_Parameter("Min_Delay");
	float maxDelay = Get_Float_Parameter("Max_Delay");
	int sendParam = Get_Int_Parameter("Send_Param");
	int sendType = Get_Int_Parameter("Send_Type");

	GameObject *targetObj = Commands->Get_A_Star(Vector3(0.0f, 0.0f, 0.0f));
	if (this->startNow)
	{
		if (targetId > 0)
		{
			targetObj = Commands->Find_Object(targetId);
		}

		if (targetObj)
		{
			M00_Controller_Trigger_RMV(obj, targetObj, minDelay, maxDelay, sendType, sendParam);
		}
	}
}

void M00_Trigger_When_Killed_RMV::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 9000 || type == 9001 || type == Get_Int_Parameter("Receive_Type"))
	{
		if (param == Get_Int_Parameter("Receive_Param_On"))
		{
			this->startNow = true;
		}

		if (param == Get_Int_Parameter("Receive_Param_Off"))
		{
			this->startNow = false;
		}
	}
}

ScriptRegistrant<M00_Trigger_When_Killed_RMV> M00_Trigger_When_Killed_RMVRegistrant("M00_Trigger_When_Killed_RMV", "Start_Now=1:int, Receive_Type=1:int, Receive_Param_On=1:int, Receive_Param_Off=0:int, Target_ID:int, Send_Type:int, Send_Param:int, Min_Delay=0.0:float, Max_Delay=0.0:float, Debug_Mode=0:int");