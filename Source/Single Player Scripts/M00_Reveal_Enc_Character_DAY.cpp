#include "General.h"
#include "M00_Reveal_Enc_Character_DAY.h"

/*
M03 -> 1206590 1206589
*/
void M00_Reveal_Enc_Character_DAY::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		int characterEncyclopediaId = Get_Int_Parameter("CharacterEncyclopediaID");
		if (Commands->Reveal_Encyclopedia_Character(characterEncyclopediaId))
		{
			Commands->Display_Encyclopedia_Event_UI();
		}
	}
}

ScriptRegistrant<M00_Reveal_Enc_Character_DAY> M00_Reveal_Enc_Character_DAYRegistrant("M00_Reveal_Enc_Character_DAY", "CharacterEncyclopediaID:int");