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
#include "M01_TibField_Guard01_New_JDG.h"

/*
M01 -> 103270
*/
void M01_TibField_Guard01_New_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M01_mission_Controller_JDG when custom param 148 is received
	// Received from M01_mission_Controller_JDG after 5 seconds when param 153 or 154 is received
	if (param == 16)
	{
		GameObject *tibFieldGuard1Obj = Commands->Find_Object(103270);
		GameObject *tibFieldGuard2Obj = Commands->Find_Object(103271);
		GameObject *tibFieldGuard3Obj = Commands->Find_Object(103272);
		if (tibFieldGuard1Obj && tibFieldGuard2Obj && tibFieldGuard3Obj)
		{
			// This isn't going to last, brother.
			// If we keep GDI away from the master control terminals and SAM Sites, we will survive.
			// Why is the Black Hand taking taking over this base? Who put them in charge?
			// I'd like to toss them in the Tiberium field and watch them mutate.
			// Watch your mouth.  The Black Hand are Kane's right hand - they have no tolerance for insubordination.
			int conversationId = Commands->Create_Conversation("M01_TiberiumField_Conversation", 80, 25.0f, true);
			Commands->Join_Conversation(tibFieldGuard1Obj, conversationId, true, true, true);
			Commands->Join_Conversation(tibFieldGuard2Obj, conversationId, true, true, true);
			Commands->Join_Conversation(tibFieldGuard3Obj, conversationId, true, true, true);
			Commands->Start_Conversation(conversationId, conversationId);
		}
	}
}

ScriptRegistrant<M01_TibField_Guard01_New_JDG> M01_TibField_Guard01_New_JDGRegistrant("M01_TibField_Guard01_New_JDG", "");