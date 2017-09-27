#include "General.h"
#include "M01_Church_LoveShack_MiniGunner_JDG.h"

/*
M01 -> 101305
*/
void M01_Church_LoveShack_MiniGunner_JDG::Created(GameObject *obj)
{
	Commands->Innate_Disable(obj);
}

void M01_Church_LoveShack_MiniGunner_JDG::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *M01ChurchLoveShackInterrogationConvZoneJDGObj = Commands->Find_Object(103392);
	if (M01ChurchLoveShackInterrogationConvZoneJDGObj)
	{
		Commands->Destroy_Object(M01ChurchLoveShackInterrogationConvZoneJDGObj);
	}

	GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
	Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 174, 0.0f);
	Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 178, 0.0f);
}

void M01_Church_LoveShack_MiniGunner_JDG::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	Vector3 pos = Commands->Get_Position(obj);
	GameObject *starObj = Commands->Get_A_Star(pos);

	if (damager == starObj)
	{
		if (obj)
		{
			Commands->Innate_Enable(obj);

			ActionParamsStruct params;
			params.Set_Basic(this, 45.0f, 38);
			params.Set_Movement(starObj, 0.8f, 5.0f);
			params.Set_Attack(starObj, 15.0f, 0.0f, true);

			Commands->Action_Attack(obj, params);
		}
	}
}

// TODO
void M01_Church_LoveShack_MiniGunner_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (param == 16)
	{
		Commands->Innate_Enable(obj);

		ActionParamsStruct params;
		params.Set_Basic(this, 85.0f, 38);
		params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.3f, 2.0f);
		params.WaypathID = 100760;
		params.WaypointStartID = 100761;
		params.WaypointEndID = 100769;

		Commands->Action_Goto(obj, params);
	}
}

ScriptRegistrant<M01_Church_LoveShack_MiniGunner_JDG> M01_Church_LoveShack_MiniGunner_JDGRegistrant("M01_Church_LoveShack_MiniGunner_JDG", "");