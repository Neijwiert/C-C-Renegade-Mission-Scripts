#include "General.h"
#include "M00_Controller_Trigger_RMV.h"

void M00_Controller_Trigger_RMV(GameObject *from, GameObject *to, float minDelay, float maxDelay, int type, int param)
{
	if (to)
	{
		if (minDelay == 0.0f && maxDelay == 0.0f)
		{
			Commands->Send_Custom_Event(from, to, type, param, 0.0f);
		}
		else if (minDelay != 0.0f && maxDelay == 0.0f)
		{
			Commands->Send_Custom_Event(from, to, type, param, minDelay);
		}
		else if (maxDelay != 0.0f)
		{
			float randDelay = Commands->Get_Random(minDelay, maxDelay);
			Commands->Send_Custom_Event(from, to, type, param, randDelay);
		}
	}
}