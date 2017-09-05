#include "General.h"
#include "MX0_GDI_Soldier_DLS.h"

void MX0_GDI_Soldier_DLS::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(this->attackLocations, sizeof(this->attackLocations), 1);
	Auto_Save_Variable(&this->attackLocationIndex, sizeof(this->attackLocationIndex), 2);
	Auto_Save_Variable(&this->speed, sizeof(this->speed), 3);
	Auto_Save_Variable(&this->moveAttemptCount, sizeof(this->moveAttemptCount), 4);
}

void MX0_GDI_Soldier_DLS::Created(GameObject *obj)
{
	Commands->Enable_Enemy_Seen(obj, true);
	Commands->Enable_Engine(obj, true);

	this->attackLocations[0] = Get_Int_Parameter("Attack_Loc0");
	this->attackLocations[1] = Get_Int_Parameter("Attack_Loc1");
	this->attackLocations[2] = Get_Int_Parameter("Attack_Loc2");
	this->attackLocations[3] = Get_Int_Parameter("Attack_Loc3");
	this->attackLocationIndex = 0;
	this->speed = Get_Float_Parameter("Speed");

	ActionParamsStruct params;
	params.Set_Basic(this, 95.0f, 10);

	GameObject *attackLocationObj = Commands->Find_Object(this->attackLocations[this->attackLocationIndex]);
	Vector3 attackLocationObjPos = Commands->Get_Position(attackLocationObj);

	params.Set_Movement(attackLocationObjPos, this->speed, 5.0f);

	GameObject *muzzleForwardTargetObj = Commands->Find_Object(1500024);

	params.Set_Attack(muzzleForwardTargetObj, 0.0f, 0.0f, true);

	Commands->Action_Attack(obj, params);
}

void MX0_GDI_Soldier_DLS::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 445006)
	{
		this->attackLocationIndex = param;

		ActionParamsStruct params;
		params.Set_Basic(this, 95.0f, 445006);

		GameObject *attackLocationObj = Commands->Find_Object(this->attackLocations[this->attackLocationIndex]);
		Vector3 attackLocationObjPos = Commands->Get_Position(attackLocationObj);

		params.Set_Movement(attackLocationObjPos, this->speed, 5.0f);

		GameObject *muzzleForwardTargetObj = Commands->Find_Object(1500024);

		params.Set_Attack(muzzleForwardTargetObj, 0.0f, 0.0f, true);
		params.MoveCrouched = (Commands->Get_Random_Int(0, 1) == 0);

		Commands->Action_Attack(obj, params);

		Commands->Debug_Message("Attack_Loc [%d] = %d \n", this->attackLocationIndex, this->attackLocations[this->attackLocationIndex]);
	}
	else if (type == 445002)
	{
		if (param == 445012)                      
		{
			int conversationId = Commands->Create_Conversation("MX0_A04_CON003", 100, 200.0f, false); // They have an Obelisk!
			Commands->Join_Conversation(obj, conversationId, false, true, true);
			Commands->Start_Conversation(conversationId, 445012);

			Commands->Fade_Background_Music("Level 0 Nod base.mp3", 2, 2);
		}
		else if (param == 445013)
		{
			int conversationId = Commands->Create_Conversation("MX0_A04_CON004", 100, 200.0f, false); // The Obelisk is hot!  Look out!
			Commands->Join_Conversation(obj, conversationId, false, true, true);
			Commands->Start_Conversation(conversationId, 445013);
		}
		else if (param == 445014)
		{
			int conversationId = Commands->Create_Conversation("MX0_A04_CON006", 100, 200.0f, false); // It’s down! The Obelisk is down!
			Commands->Join_Conversation(obj, conversationId, false, true, true);
			Commands->Start_Conversation(conversationId, 445014);
		}
		else if (param == 445015)
		{
			int conversationId = Commands->Create_Conversation("MX0_A04_CON007", 100, 200.0f, false); // Go! Go!  Knock out those SAMs!
			Commands->Join_Conversation(obj, conversationId, false, true, true);
			Commands->Start_Conversation(conversationId, 445015);
		}
		else if (param == 445016)
		{
			int conversationId = Commands->Create_Conversation("MX0_A04_CON008", 100, 200.0f, false); // The SAMs are history!
			Commands->Join_Conversation(obj, conversationId, false, true, true);
			Commands->Start_Conversation(conversationId, 445016);
		}
		else if (param == 445017)
		{
			int conversationId = Commands->Create_Conversation("MX0_A04_CON009", 100, 200.0f, false); // There, on the wall!
			Commands->Join_Conversation(obj, conversationId, false, true, true);
			Commands->Start_Conversation(conversationId, 445017);
		}
		else if (param == 445021)
		{
			int conversationId = Commands->Create_Conversation("MX0_A04_CON014", 100, 200.0f, false); // We need your help - over here, Sir!
			Commands->Join_Conversation(obj, conversationId, false, true, true);
			Commands->Start_Conversation(conversationId, 445021);
		}
	}
}

void MX0_GDI_Soldier_DLS::Enemy_Seen(GameObject *obj, GameObject *enemy)
{
	Commands->Debug_Message("ID %d sees Enemy ID %d \n", Commands->Get_ID(obj), Commands->Get_ID(enemy));

	ActionParamsStruct params;
	params.Set_Basic(this, 95.0f, 10);

	GameObject *attackLocationObj = Commands->Find_Object(this->attackLocations[this->attackLocationIndex]);
	Vector3 attackLocationObjPos = Commands->Get_Position(attackLocationObj);

	params.Set_Movement(attackLocationObjPos, this->speed, 5.0f);

	GameObject *muzzleForwardTargetObj = Commands->Find_Object(1500024);

	params.Set_Attack(muzzleForwardTargetObj, 0.0f, 0.0f, true);

	Commands->Action_Attack(obj, params);
}

void MX0_GDI_Soldier_DLS::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (action_id == 445006)
	{
		if (complete_reason && this->moveAttemptCount < 4)
		{
			this->moveAttemptCount++;

			Commands->Start_Timer(obj,this, 4.0f, 445006);

			Commands->Debug_Message("Attempt %d for MX0_SOLDIER_MOVE on loc %d", this->moveAttemptCount, this->attackLocationIndex);
		}
	}
	else if (action_id == 10)
	{
		Commands->Debug_Message("Action_Complete");
	}
}

void MX0_GDI_Soldier_DLS::Timer_Expired(GameObject *obj, int number)
{
	if (number == 445006)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 95.0f, 445006);

		GameObject *attackLocationObj = Commands->Find_Object(this->attackLocations[this->attackLocationIndex]);
		Vector3 attackLocationObjPos = Commands->Get_Position(attackLocationObj);

		params.Set_Movement(attackLocationObjPos, this->speed, 5.0f);

		GameObject *muzzleForwardTargetObj = Commands->Find_Object(1500024);

		params.Set_Attack(muzzleForwardTargetObj, 0.0f, 0.0f, true);

		Commands->Action_Attack(obj, params);
	}
}

ScriptRegistrant<MX0_GDI_Soldier_DLS> MX0_GDI_Soldier_DLSRegistrant("MX0_GDI_Soldier_DLS", "Attack_Loc0=0:int, Attack_Loc1=0:int, Attack_Loc2=0:int, Attack_Loc3=0:int, Speed=1.0:float");