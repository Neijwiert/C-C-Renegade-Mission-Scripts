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
#include "M00_Generic_Conv_DME.h"

void M00_Generic_Conv_DME::Created(GameObject *obj)
{
	const char *convName = Get_Parameter("ConvName");

	int conversationId = Commands->Create_Conversation(convName, 99, 2000.0f, false);
	Commands->Join_Conversation(obj, conversationId, true, true, true);
	Commands->Start_Conversation(conversationId, 100000);
	Commands->Monitor_Conversation(obj, conversationId);
}

ScriptRegistrant<M00_Generic_Conv_DME> M00_Generic_Conv_DMERegistrant("M00_Generic_Conv_DME", "ConvName:string");