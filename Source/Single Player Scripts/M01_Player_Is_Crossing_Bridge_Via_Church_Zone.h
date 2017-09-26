#pragma once

#include <scripts.h>

/*
M01 -> 100972
*/
class M01_Player_Is_Crossing_Bridge_Via_Church_Zone : public ScriptImpClass
{
	private:
		virtual void Entered(GameObject *obj, GameObject *enterer);
};