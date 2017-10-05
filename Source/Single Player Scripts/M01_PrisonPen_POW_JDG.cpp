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
#include "M01_PrisonPen_POW_JDG.h"

/*
M01 -> 101928 101927 101926
*/
void M01_PrisonPen_POW_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->field_1D, sizeof(this->field_1D), 2);
	Auto_Save_Variable(&this->field_1E, sizeof(this->field_1E), 3);
	Auto_Save_Variable(&this->field_1F, sizeof(this->field_1F), 4);
	Auto_Save_Variable(&this->detentionSoldier2ObjId, sizeof(this->detentionSoldier2ObjId), 5);
}

void M01_PrisonPen_POW_JDG::Created(GameObject *obj)
{
	Commands->Set_Obj_Radar_Blip_Color(obj, RADAR_BLIP_COLOR_GDI);

	this->detentionSoldier2ObjId = 101927;
	this->field_1C = false;
	this->field_1D = false;
	this->field_1E = false;
	this->field_1F = false;

	Commands->Set_Innate_Is_Stationary(obj, true);
	Commands->Innate_Soldier_Enable_Enemy_Seen(obj, false);
	Commands->Innate_Soldier_Enable_Gunshot_Heard(obj, false);
	Commands->Innate_Soldier_Enable_Bullet_Heard(obj, false);
}

void M01_PrisonPen_POW_JDG::Killed(GameObject *obj, GameObject *killer)
{

}

// TODO
void M01_PrisonPen_POW_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M01_Detention_GuardTower_Enter_Zone_JDG when exited
	if (param == 24)
	{
		Commands->Innate_Soldier_Enable_Gunshot_Heard(obj, false);
		Commands->Innate_Soldier_Enable_Bullet_Heard(obj, false);

		this->field_1C = false;
	}
	else if (param == 46)
	{
		static const char *animations[4] =
		{
			"H_A_J01C",
			"H_A_J18C",
			"H_A_J24C",
			"H_A_J26C"
		};

		int randAnimationIndex = Commands->Get_Random_Int(0, 4);

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 46);
		params.Set_Animation(animations[randAnimationIndex], false);

		Commands->Action_Play_Animation(obj, params);
	}
	else if (param == 63)
	{
		Commands->Innate_Disable(obj);

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 63);
		
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		params.Set_Attack(starObj, 0.0f, 0.0f, true);

		Commands->Action_Attack(obj, params);

		float randDelay = Commands->Get_Random(0.25f, 2.5f);
		Commands->Send_Custom_Event(obj, obj, 0, 46, randDelay);
	}

	// Received from M01_Detention_GuardTower_Enter_Zone_JDG when entered
	else if (param == 16)
	{
		Commands->Innate_Soldier_Enable_Gunshot_Heard(obj, true);
		Commands->Innate_Soldier_Enable_Bullet_Heard(obj, true);

		this->field_1C = true;
	}
}

// TODO
void M01_PrisonPen_POW_JDG::Sound_Heard(GameObject *obj, const CombatSound & sound)
{
	if (sound.sound == 400004)
	{
		if (!this->field_1E)
		{
			this->field_1E = true;

			Commands->Set_Innate_Is_Stationary(obj, false);

			Commands->Action_Reset(obj, 100.0f);

			if (Commands->Find_Object(100034))
			{
				Vector3 moveLocation;

				int objId = Commands->Get_ID(obj);
				switch (objId)
				{
					case 101926:
						moveLocation = Vector3(-312.413f, 577.03f, 27.567f);

						break;
					case 101927:
						moveLocation = Vector3(-311.138f, 578.398f, 27.687f);

						break;
					default:
						moveLocation = Vector3(-309.667f, 576.016f, 27.753f);

						break;
				}
				
				ActionParamsStruct params;
				params.Set_Basic(this, 100.0f, 40);
				params.Set_Movement(moveLocation, 0.8f, 0.5f);

				Commands->Action_Goto(obj, params);
			}
		}
	}
	else if (sound.sound == 400005)
	{
		this->field_1F = true;

		Commands->Set_Innate_Is_Stationary(obj, false);

		Commands->Action_Reset(obj, 100.0f);
	
		Vector3 moveLocation;

		int objId = Commands->Get_ID(obj);
		switch (objId)
		{
		case 101926:
			moveLocation = Vector3(-312.413f, 577.03f, 27.567f);

			break;
		case 101927:
			moveLocation = Vector3(-311.138f, 578.398f, 27.687f);

			break;
		default:
			moveLocation = Vector3(-309.667f, 576.016f, 27.753f);

			break;
		}

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 40);
		params.Set_Movement(moveLocation, 0.8f, 0.5f);

		Commands->Action_Goto(obj, params);
	}
	else if (sound.sound == SOUND_TYPE_GUNSHOT || sound.sound == SOUND_TYPE_BULLET_HIT)
	{
		Vector3 pos = Commands->Get_Position(obj);
		if (sound.Creator == Commands->Get_A_Star(pos) && this->field_1C && !this->field_1D)
		{
			this->field_1D = true;

			Commands->Send_Custom_Event(obj, obj, 0, 63, 0.0f);
		}
	}
}

// TODO
void M01_PrisonPen_POW_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	static const char *animations[4] =
	{
		"H_A_J01C",
		"H_A_J18C",
		"H_A_J24C",
		"H_A_J26C"
	};

	if (action_id == 39)
	{
		if (obj == Commands->Find_Object(this->detentionSoldier2ObjId))
		{
			Commands->Set_Innate_Is_Stationary(obj, true);

			int conversationId = Commands->Create_Conversation("M01_Detention_GDI_Hello_Conversation01", 100, 1000.0f, false); // Good to see you Captain - get us out of here!
			Commands->Join_Conversation(obj, conversationId, false, true, true);
			Commands->Start_Conversation(conversationId, conversationId);
		}
	}
	else if (action_id == 46)
	{
		if (obj == Commands->Find_Object(this->detentionSoldier2ObjId))
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 39);
			params.Set_Movement(Vector3(-297.801f, 572.799f, 27.458f), 0.8f, 1.0f);

			Commands->Action_Goto(obj, params);
		}
	}
	else if (action_id == 40)
	{
		int randAnimationIndex = Commands->Get_Random_Int(0, 4);

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 48);
		params.Set_Animation(animations[randAnimationIndex], false);

		Commands->Action_Play_Animation(obj, params);
	}
	else if (action_id == 41)
	{
		int randAnimationIndex = Commands->Get_Random_Int(0, 4);

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 49);
		params.Set_Animation(animations[randAnimationIndex], false);

		Commands->Action_Play_Animation(obj, params);

		if (obj == Commands->Find_Object(this->detentionSoldier2ObjId))
		{
			int conversationId = Commands->Create_Conversation("M01_Detention_GDI_Evac_Conversation01", 100, 1000.0f, false); // Thank you sir!
			Commands->Join_Conversation(obj, conversationId, true, true, true);
			Commands->Start_Conversation(conversationId, conversationId);
		}
	}
	else if (action_id == 48)
	{
		Commands->Set_Innate_Is_Stationary(obj, true);
	}
	else if (action_id == 49)
	{
		GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
		Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 4, 5.0f);

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 42);
		params.Set_Movement(Vector3(-328.136f, 548.518f, 30.044f), 0.8f, 3.0f);

		Commands->Action_Goto(obj, params);
	}
	else if (action_id == 42)
	{
		Commands->Set_Innate_Is_Stationary(obj, true);

		GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
		if (M01MissionControllerJDGObj)
		{
			Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 4, 5.0f);
		}
	}
}

void M01_PrisonPen_POW_JDG::Poked(GameObject *obj, GameObject *poker)
{
	if (this->field_1E && !this->field_1F)
	{
		int conversationId = Commands->Create_Conversation("M01_Detention_GDI_Poke_Conversation01", 100, 1000.0f, false); // How are we gonna evacuate without a chopper?
		Commands->Join_Conversation(obj, conversationId, false, true, true);
		Commands->Start_Conversation(conversationId, conversationId);
	}
}

ScriptRegistrant<M01_PrisonPen_POW_JDG> M01_PrisonPen_POW_JDGRegistrant("M01_PrisonPen_POW_JDG", "");