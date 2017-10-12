#pragma once

#include <scripts.h>

/*
M03 -> 1206590 1206589
*/
class M00_Reveal_Enc_Character_DAY : public ScriptImpClass
{
	private:
		virtual void Custom(GameObject *obj, int type, int param, GameObject *sender);
};