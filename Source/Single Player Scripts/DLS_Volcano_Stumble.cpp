#include "General.h"
#include "DLS_Volcano_Stumble.h"

/*
M03 -> 300052 1147534 1147533 1147531 1147530 1147529 1146701 1146700 300009 300028 2013085 1144724 1144723 1144722 1144721 1144720 1144719 1144718 1144717 1144716 1144715 1144714 1144713 1144712 1144711 1144653 300032 1141162 300043 300034 300033 1144452 1144451 1144450 1144672 1144673 1144682 1144500 300010
*/
void DLS_Volcano_Stumble::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
}

void DLS_Volcano_Stumble::Created(GameObject *obj)
{
	this->field_1C = false;

	Commands->Set_Innate_Take_Cover_Probability(obj, 100.0f);
	Commands->Set_Innate_Aggressiveness(obj, 100.0f);
}

// TODO
void DLS_Volcano_Stumble::Sound_Heard(GameObject *obj, const CombatSound & sound)
{
	if (sound.sound == 1001 && !this->field_1C)
	{
		this->field_1C = true;

		Commands->Action_Reset(obj, 97.0f);

		int randInt = Commands->Get_Random_Int(0, 2);
		if (!randInt)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 98.0f, 0);
			params.Set_Animation("S_A_HUMAN.H_A_622A", false);

			Commands->Action_Play_Animation(obj, params);
		}
		else if (randInt == 1)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 98.0f, 5);
			params.Set_Animation("S_A_HUMAN.H_A_611A", false);

			Commands->Action_Play_Animation(obj, params);
		}
		else if (randInt == 2)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 98.0f, 1);
			params.Set_Animation("S_A_HUMAN.H_A_632A", false);

			Commands->Action_Play_Animation(obj, params);
		}
	}
}

// TODO
void DLS_Volcano_Stumble::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (action_id == 0)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 99.0f, 2);
		params.Set_Animation("S_A_HUMAN.H_A_H11C", false);

		Commands->Action_Play_Animation(obj, params);
	}
	else if (action_id == 1)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 99.0f, 3);
		params.Set_Animation("S_A_HUMAN.H_A_H13C", false);

		Commands->Action_Play_Animation(obj, params);
	}
	else if (action_id == 3)
	{
		this->field_1C = false;

		ActionParamsStruct params;
		params.Set_Basic(this, 70.0f, 4);
		params.Set_Movement(Vector3(-288.02f, 78.11f, 9.51f), 1.0f, 15.0f);

		Commands->Action_Goto(obj, params);
	}
	else if (action_id == 2 || action_id == 5)
	{
		this->field_1C = false;

		ActionParamsStruct params;
		params.Set_Basic(this, 70.0f, 4);
		params.Set_Movement(Vector3(-288.02f, 78.11f, 9.51f), 1.0f, 15.0f);

		Commands->Action_Goto(obj, params);
	}
}

ScriptRegistrant<DLS_Volcano_Stumble> DLS_Volcano_StumbleRegistrant("DLS_Volcano_Stumble", "Debug_Mode=0:int");