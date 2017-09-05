#pragma once

#include <scripts.h>

class MX0_GDI_Soldier_DLS : public ScriptImpClass
{
	public:
		virtual	void Register_Auto_Save_Variables();

	private:
		virtual void Created(GameObject *obj);
		virtual void Custom(GameObject *obj, int type, int param, GameObject *sender);
		virtual void Enemy_Seen(GameObject *obj, GameObject *enemy);
		virtual void Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason);
		virtual void Timer_Expired(GameObject *obj, int number);

		int attackLocations[4];
		int attackLocationIndex;
		float speed;
		int moveAttemptCount;
};