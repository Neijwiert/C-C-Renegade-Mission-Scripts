#pragma once

#include <scripts.h>

/*
M01 -> 101305
*/
class M01_Church_LoveShack_MiniGunner_JDG : public ScriptImpClass
{
	private:
		virtual void Created(GameObject *obj);
		virtual void Killed(GameObject *obj, GameObject *killer);
		virtual void Damaged(GameObject *obj, GameObject *damager, float amount);
		virtual void Custom(GameObject *obj, int type, int param, GameObject *sender);
};