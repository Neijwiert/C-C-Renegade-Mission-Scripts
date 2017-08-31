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
#include "M00_Send_Object_ID.h"

void M00_Send_Object_ID::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->field_20, sizeof(this->field_20), 2);
}

void M00_Send_Object_ID::Created(GameObject *obj)
{
	int receiverId = Get_Int_Parameter("Receiver_ID");
	int param = Get_Int_Parameter("Param");
	float delay = Get_Float_Parameter("Delay");

	if (Commands->Find_Object(receiverId))
	{
		GameObject *receiverObj = Commands->Find_Object(receiverId);
		Commands->Send_Custom_Event(obj, receiverObj, 9035, param, delay);
	}
	else
	{
		int objId = Commands->Get_ID(obj);
		Commands->Debug_Message("M00_Send_Object_ID on object_id %d Warning! Receiver_ID %d does not exist! /n", objId, receiverId); // Yes, forward slash
	}
}

ScriptRegistrant<M00_Send_Object_ID> M00_Send_Object_IDRegistrant("M00_Send_Object_ID", "Receiver_ID:int, Param=0:int, Delay=1.0f:int"); // Yes, 'Delay' is registered as an integer