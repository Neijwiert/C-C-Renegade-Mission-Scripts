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
#include "M01_HON_Dojo_Civ_01_JDG.h"

/*
M01 -> 102050 102049 102048
*/
void M01_HON_Dojo_Civ_01_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->objId, sizeof(this->objId), 1);
	Auto_Save_Variable(&this->dojoCiv01ObjId, sizeof(this->dojoCiv01ObjId), 2);
	Auto_Save_Variable(&this->dojoCiv02ObjId, sizeof(this->dojoCiv02ObjId), 3);
	Auto_Save_Variable(&this->dojoCiv03ObjId, sizeof(this->dojoCiv03ObjId), 4);
	Auto_Save_Variable(&this->field_2C, sizeof(this->field_2C), 5);
	Auto_Save_Variable(&this->dojoThanksConversationId, sizeof(this->dojoThanksConversationId), 6);
}

void M01_HON_Dojo_Civ_01_JDG::Created(GameObject *obj)
{
	this->dojoCiv01ObjId = 102048;
	this->dojoCiv02ObjId = 102049;
	this->dojoCiv03ObjId = 102050;
	this->field_2C = 0;

	Commands->Innate_Disable(obj);

	objId = Commands->Get_ID(obj);
	if (objId == this->dojoCiv01ObjId || objId == this->dojoCiv02ObjId || objId == this->dojoCiv03ObjId)
	{
		this->objId = objId;
	}
}

void M01_HON_Dojo_Civ_01_JDG::Killed(GameObject *obj, GameObject *killer)
{
	Commands->Create_Sound("EVA_Civilian_Killed", Vector3(0.0f, 0.0f, 0.0f), obj);
}

// TODO
void M01_HON_Dojo_Civ_01_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M01_HON_Dojo_Trainer_JDG when conversation ended
	if (param == 16)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 46);
		params.Set_Animation("H_A_J13A", false);

		Commands->Action_Play_Animation(obj, params);
	}

	// Received from ourselves after 0 to 3 seconds when action with id 48 is complete
	else if (param == 27)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 38);
		params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.3f, 2.0f);
		params.WaypathID = 102056;
		params.WaypointStartID = 102058;
		params.WaypointEndID = 102062;

		Commands->Action_Goto(obj, params);
	}

	// Received from M01_HON_Dojo_Trainer_JDG when damaged by star
	else if (param == 189)
	{
		Commands->Action_Reset(obj, 100.0f);

		Vector3 pos = Commands->Get_Position(obj);

		float randXOffset = Commands->Get_Random(0.5f, 1.0f);
		float randYOffset = Commands->Get_Random(0.5f, 2.0f);

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 39);
		params.Set_Movement(Vector3(pos.X + randXOffset, pos.Y + randYOffset, pos.Z), 0.3f, 0.0f, true);
		
		Commands->Action_Goto(obj, params);
	}

	// Received from M01_HON_Dojo_Trainer_JDG when killed
	else if (param == 28)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 40);

		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		params.Set_Look(starObj, 5.0f);

		Commands->Action_Face_Location(obj, params);
		
		float randDelay = Commands->Get_Random(0.0f, 1.0f);
		Commands->Send_Custom_Event(obj, obj, 0, 30, randDelay);
	}

	// Received from ourselves after 0 to 4 seconds when param 30 is received
	else if (param == 29)
	{
		if (obj)
		{
			Commands->Join_Conversation(obj, this->dojoThanksConversationId, false, true, true);

			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			Commands->Join_Conversation(starObj, this->dojoThanksConversationId, false, false, false);
			Commands->Start_Conversation(this->dojoThanksConversationId, this->dojoThanksConversationId);
			Commands->Monitor_Conversation(obj, this->dojoThanksConversationId);
		}
	}

	// Received from ourselves after 0 to 1 seconds when param 28 is received
	else if (param == 30)
	{
		float delay;
		if (this->objId == this->dojoCiv01ObjId)
		{
			this->dojoThanksConversationId = Commands->Create_Conversation("M01_Dojo_Babushka_Thanks_Conversation", 100, 1000.0f, true); // Thank you GDI! I knew they were lying!
			delay = 0.0f;
		}
		else if (this->objId == this->dojoCiv02ObjId)
		{
			this->dojoThanksConversationId = Commands->Create_Conversation("M01_Dojo_Billy_Thanks_Conversation", 100, 1000.0f, true); // Uh.. Is this part of training?
			delay = 2.0f;
		}
		else
		{
			this->dojoThanksConversationId = Commands->Create_Conversation("M01_Dojo_FarmerJohn_Thanks_Conversation", 100, 1000.0f, true); // That looks like a GDI uniform - is this a test?
			delay = 4.0f;
		}

		Commands->Send_Custom_Event(obj, obj, 0, 29, delay);
	}

	// Received from ourselves after 10 seconds after this block or 10 seconds after param 165 or 10 seconds after conversation ended
	else if (param == 67)
	{
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);
		Vector3 starPos = Commands->Get_Position(starObj);

		if (Commands->Get_Distance(pos, starPos) <= 75.0f)
		{
			Commands->Send_Custom_Event(obj, obj, 0, 67, 10.0f);
		}
		else
		{
			if (!Commands->Is_Object_Visible(starObj, obj))
			{
				Commands->Destroy_Object(obj);
			}
		}
	}

	// Received from M01_HON_Dojo_Trainer_JDG when killed
	// Received from M01_mission_Controller_JDG when param 122 is received
	else if (param == 165)
	{
		Commands->Action_Reset(obj, 100.0f);

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 41);
		params.Set_Movement(Vector3(-54.512f, 183.109f, 0.774f), 0.8f, 5.0f);

		Commands->Action_Goto(obj, params);

		Commands->Send_Custom_Event(obj, obj, 0, 67, 10.0f);
	}
}

// TODO
void M01_HON_Dojo_Civ_01_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_CONVERSATION_ENDED)
	{
		if (complete_reason == ACTION_COMPLETE_CONVERSATION_ENDED && action_id == this->dojoThanksConversationId)
		{
			Vector3 pos = Commands->Get_Position(obj);
			Commands->Create_Sound("M01_GDI_Thanks_Twiddler", pos, obj);

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 41);
			params.Set_Movement(Vector3(-54.512f, 183.109f, 0.774f), 0.8f, 5.0f);

			Commands->Action_Goto(obj, params);
			
			Commands->Send_Custom_Event(obj, obj, 0, 67, 10.0f);
		}
	}
	else if(complete_reason == ACTION_COMPLETE_NORMAL)
	{
		if (action_id == 46)
		{
			if (this->field_2C >= 2)
			{
				ActionParamsStruct params;
				params.Set_Basic(this, 100.0f, 47);
				params.Set_Animation("H_A_J13C", false);

				Commands->Action_Play_Animation(obj, params);
			}
			else
			{
				ActionParamsStruct params;
				params.Set_Basic(this, 100.0f, 46);
				params.Set_Animation("H_A_J13B", false);

				Commands->Action_Play_Animation(obj, params);

				this->field_2C++;
			}
		}
		else if (action_id == 47)
		{
			static const char *animations[14] =
			{
				"H_A_A0A0_L01",
				"H_A_A0A0_L02",
				"H_A_A0A0_L03",
				"H_A_A0A0_L04",
				"H_A_A0A0_L05",
				"H_A_A0A0_L13",
				"H_A_X33C",
				"H_A_J33C",
				"H_A_J24C",
				"H_A_J22C",
				"H_A_J17C",
				"H_A_J15C",
				"H_A_J14C",
				"H_A_J11C"
			};
			
			int randAnimationIndex = Commands->Get_Random_Int(0, 14);

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 48);
			params.Set_Animation(animations[randAnimationIndex], false);

			Commands->Action_Play_Animation(obj, params);
		}
		else if (action_id == 48)
		{
			if (this->objId == this->dojoCiv01ObjId)
			{
				float randDelay = Commands->Get_Random(0.0f, 1.0f);
				Commands->Send_Custom_Event(obj, obj, 0, 27, randDelay);
			}
			else if (this->objId == this->dojoCiv02ObjId)
			{
				float randDelay = Commands->Get_Random(1.0f, 2.0f);
				Commands->Send_Custom_Event(obj, obj, 0, 27, randDelay);
			}
			else if (this->objId == this->dojoCiv03ObjId)
			{
				float randDelay = Commands->Get_Random(2.0f, 3.0f);
				Commands->Send_Custom_Event(obj, obj, 0, 27, randDelay);
			}
		}
		else if (action_id == 38)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 49);
			params.Set_Animation("H_A_J12C", false);

			Commands->Action_Play_Animation(obj, params);
		}
		else if (action_id == 49)
		{
			Vector3 pos = Commands->Get_Position(obj);
			GameObject *minigunnerObj = Commands->Create_Object("Nod_MiniGunner_0", pos);
			Commands->Attach_Script(minigunnerObj, "M01_Civ_To_Minigunner_Guy_JDG", "");

			Commands->Destroy_Object(obj);
		}
	}
}

ScriptRegistrant<M01_HON_Dojo_Civ_01_JDG> M01_HON_Dojo_Civ_01_JDGRegistrant("M01_HON_Dojo_Civ_01_JDG", "");