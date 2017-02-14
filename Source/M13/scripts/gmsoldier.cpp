/*	Renegade Scripts.dll
	Copyright 2013 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/
#include "General.h"
#include "scripts.h"
#include "engine.h"
#include "gmsoldier.h"
#include "HashTemplateClass.h"
#include "gmlog.h"
#include "gmgame.h"
#include "gmvehicle.h"
extern HashTemplateClass<StringClass,DynamicVectorClass<StringClass> *> WeaponGrants;
extern HashTemplateClass<StringClass,DynamicVectorClass<StringClass> *> WeaponDrops;

void SSGM_Log_Key::Created(GameObject *obj)
{
	InstallHook(Get_Parameter("Key"),obj);
}

void SSGM_Log_Key::KeyHook()
{
	char message[100];
	sprintf(message,"%ls: %s",Get_Wide_Player_Name(Owner()),Get_Parameter("Write"));
	SSGMGameLog::Log_RenLog(message);
}

void SSGM_C4_Key::Created(GameObject *obj)
{
	InstallHook("C4Count",obj);
}

void SSGM_C4_Key::KeyHook()
{
	Print_C4(Owner());
}

void SSGM_Bind_Key::Created(GameObject *obj)
{
	InstallHook("VehBind",obj);
}

void SSGM_Bind_Key::KeyHook()
{
	Bind_Vehicle(Owner());
}

void SSGM_BL_Key::Created(GameObject *obj)
{
	InstallHook("VehBL",obj);
}

void SSGM_BL_Key::KeyHook()
{
	Bind_Vehicle(Owner());
	Lock_Vehicle(Owner());
}

void SSGM_Soldier::Created(GameObject *obj)
{
	kills = 0;
	RefillTime = The_Game()->Get_Game_Duration_S();
	if (Commands->Is_A_Star(obj))
	{
		Commands->Attach_Script(obj,"SSGM_Log_Key","VoteYes,!vote yes");
		Commands->Attach_Script(obj,"SSGM_Log_Key","VoteNo,!vote no");
		Commands->Attach_Script(obj,"SSGM_C4_Key","");
		if (VehicleOwnership)
		{
			Commands->Attach_Script(obj,"SSGM_Bind_Key","");
			Commands->Attach_Script(obj,"SSGM_BL_Key","");
		}
		if (MapForceTeam != -1)
		{
			if (Commands->Get_Player_Type(obj) != MapForceTeam)
			{
				Change_Team(obj,MapForceTeam);
				return;
			}
		}
		if (strcmp(Commands->Get_Preset_Name(obj),"GDI_Engineer_2SF") == 0)//uberwire crate grants this hotwire character
		{
			Commands->Give_PowerUp(obj,"CnC_POW_RepairGun_Player",false);//grant adv repairgun
			Commands->Select_Weapon(obj,"CnC_Weapon_RepairGun_Player_Special");//select adv repairgun
			Commands->Give_PowerUp(obj,"POW_Chaingun_Player",false);
			Commands->Give_PowerUp(obj,"CnC_MineProximity_05",false);
			Commands->Give_PowerUp(obj,"POW_Pistol_Player",false);
			Commands->Give_PowerUp(obj,"CnC_POW_MineRemote_02",false);
			Commands->Give_PowerUp(obj,"CnC_POW_MineTimed_Player_01",false);
			Commands->Give_PowerUp(obj,"CnC_POW_MineTimed_Player_02",false);
			Commands->Give_PowerUp(obj,"CnC_Ammo_Crate",false);
			Remove_Weapon(obj,"Weapon_RepairGun_Ai");//remove the ai repairgun
		}
		if (CharactersDropDNA)
		{
			Attach_Script_Once(obj,"SSGM_Drop_DNA_Powerup","");
		}
	}
	DynamicVectorClass<StringClass> *weapons = WeaponGrants.Get(Commands->Get_Preset_Name(obj),0);
	if (weapons)
	{
		for (int i = 0;i < weapons->Count();i++)
		{
			Commands->Give_PowerUp(obj,(*weapons)[i],false);
		}
	}
	if (Commands->Is_A_Star(obj))
	{
		StringClass str;
		const char *str2 = Get_Translated_Preset_Name_Ini(obj);
		const char *str3 = Get_Player_Name(obj);
		str.Format("%s changed character to a %s",str3,str2);
		delete[] str2;
		delete[] str3;
		SSGMGameLog::Log_Message(str,"_CHANGECHARACTER");
	}
	Vector3	pos	= Commands->Get_Position(obj);
	const char *str = Get_Player_Name(obj);
	SSGMGameLog::Log_Gamelog("CREATED;SOLDIER;%d;%s;%d;%d;%d;%d;%d;%d;%d;%s", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj),	int(pos.Y),int(pos.X),int(pos.Z), int(Commands->Get_Facing(obj)), int(Commands->Get_Max_Health(obj)),int(Commands->Get_Max_Shield_Strength(obj)),Commands->Get_Player_Type(obj),str);
	delete[] str;
}

void SSGM_Soldier::Detach(GameObject *obj)
{
	ScriptImpClass::Detach(obj);
	//Unbind_Vehicle(obj, true);
}

void SSGM_Soldier::Destroyed(GameObject *obj)
{
	Vector3	pos	= Commands->Get_Position(obj);
	SSGMGameLog::Log_Gamelog("DESTROYED;SOLDIER;%d;%s;%d;%d;%d;%d", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj),	int(pos.Y),	int(pos.X),	int(pos.Z),	int(Commands->Get_Points(obj)));
	Unbind_Vehicle(obj, true);
}

void SSGM_Soldier::Damaged(GameObject *obj, GameObject *damager,float amount)
{
	if (amount != 0)
	{
		Vector3	victimpos =	Commands->Get_Position(obj);
		Vector3	damagerpos = Commands->Get_Position(damager);
		SSGMGameLog::Log_Gamelog("DAMAGED;SOLDIER;%d;%s;%d;%d;%d;%d;%d;%s;%d;%d;%d;%d;%f;%d;%d;%d", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj),	int(victimpos.Y), int(victimpos.X),	int(victimpos.Z), int(Commands->Get_Facing(obj)), Commands->Get_ID(damager), Commands->Get_Preset_Name(damager), int(damagerpos.Y),	int(damagerpos.X), int(damagerpos.Z), int(Commands->Get_Facing(damager)), amount, int(Commands->Get_Health(obj)), int(Commands->Get_Shield_Strength(obj)), int(Commands->Get_Points(damager)));
	}
}

void SSGM_Soldier::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000001)
	{
		if (param == 1)
		{
			if (PlayQuakeSounds)
			{
				kills++;

				//killingspree
				if (kills == 3)
				{
					Create_2D_WAV_Sound_Player(obj,"multikill.wav");
					Send_Message_Player(obj,104,234,40,"Multikill!");
				}
				else if (kills == 5)
				{
					//killspree
					Create_2D_WAV_Sound_Player(obj,"killingspree.wav");
					Send_Message_Player(obj,104,234,40,"Killingspree!");
				}
				else if (kills == 7)
				{
					//rampage
					Create_2D_WAV_Sound_Player(obj,"rampage.wav");
					Send_Message_Player(obj,104,234,40,"Rampage!");
				}
				else if (kills == 9)
				{
					//ultra
					Create_2D_WAV_Sound_Player(obj,"ultrakill.wav");
					Send_Message_Player(obj,104,234,40,"UltraKill!");
				}
				else if (kills == 11)
				{
					//holyshit
					char msg[250];
					const char *str = Get_Player_Name(obj);
					sprintf(msg,"%s achieved Holy Shit!!! (11 kills)",str);
					delete[] str;
					Commands->Create_2D_WAV_Sound("holyshit.wav");
					Send_Message(255,255,255,msg);
				}
				else if (kills == 13)
				{
					//monster
					char msg2[250];
					const char *str = Get_Player_Name(obj);
					sprintf(msg2,"%s achieved M-M-M-M-Monster Kill!!! (13 kills)",str);
					delete[] str;
					Commands->Create_2D_WAV_Sound("monsterkill.wav");
					Send_Message(255,255,255,msg2);
				}
				else if (kills == 15)
				{
					//godlike
					char msg3[250];
					const char *str = Get_Player_Name(obj);
					sprintf(msg3,"%s achieved GODLIKE!!! (15 kills)",str);
					delete[] str;
					Commands->Create_2D_WAV_Sound("godlike.wav");
					Send_Message(255,255,255,msg3);
					kills = 0;//reset killcount so sounds start from beginning again
				}
			}
		}
	}
	else if (message == 100100)//crate pickup for ammo regen
	{
		if (param == 1)
		{
			Commands->Give_PowerUp(obj,"CnC_Ammo_Crate",false);
			Commands->Start_Timer(obj,this,60.0f,100100);
		}
	}
	else if (message == 100101)//crate pickup for human silo
	{
		if (param == 1)
		{
			Commands->Start_Timer(obj,this,1.0f,100101);
		}
	}
}

void SSGM_Soldier::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100100)//ammoregen crate timer
	{
		Commands->Give_PowerUp(obj,"CnC_Ammo_Crate",false);
		Commands->Start_Timer(obj,this,60.0f,100100);
	}
	else if (number == 100101)//human silo crate timer
	{
		Commands->Give_Money(obj,1.0f,true);
		Commands->Start_Timer(obj,this,1.0f,100101);
	}
}

void SSGM_Soldier::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&kills,sizeof(kills),1);
}


void SSGM_Soldier::Killed(GameObject *obj,GameObject *killer)
{
	if (!Commands->Is_A_Star(obj))
	{
		return;
	}
	GameObject *preset = GetExplosionObj();
	if (!preset) {
		preset = killer;
	}
	if (!preset)
	{
		StringClass str;
		const char *str2 = Get_Translated_Preset_Name_Ex(obj);
		const char *str3 = Get_Player_Name(obj);
		str.Format("%s was killed (%s)",str3,str2);
		delete[] str2;
		delete[] str3;
		SSGMGameLog::Log_Message(str,"_PLAYERKILL");
	}
	else if (obj == preset)
	{
		StringClass str;
		const char *str2 = Get_Translated_Preset_Name_Ex(obj);
		const char *str3 = Get_Player_Name(obj);
		str.Format("%s killed their self (%s)",str3,str2);
		delete[] str2;
		delete[] str3;
		SSGMGameLog::Log_Message(str,"_PLAYERKILL");
	}
	else if (!Commands->Is_A_Star(preset))
	{
		StringClass str;
		const char *str2 = Get_Translated_Preset_Name_Ini(preset);
		const char *str3 = Get_Translated_Preset_Name_Ex(obj);
		const char *str4 = Get_Player_Name(obj);
		str.Format("The %s killed %s (%s)",str2,str4,str3);
		delete[] str2;
		delete[] str3;
		delete[] str4;
		SSGMGameLog::Log_Message(str,"_PLAYERKILL");
	}
	else
	{
		StringClass str;
		const char *str2 = Get_Translated_Preset_Name_Ex(preset);
		const char *str3 = Get_Translated_Preset_Name_Ex(obj);
		const char *str4 = Get_Player_Name(killer);
		const char *str5 = Get_Player_Name(obj);
		str.Format("%s killed %s (%s VS. %s)",str4,str5,str2,str3);
		delete[] str2;
		delete[] str3;
		delete[] str4;
		delete[] str5;
		SSGMGameLog::Log_Message(str,"_PLAYERKILL");
	}
	Vector3	victimpos =	Commands->Get_Position(obj);
	Vector3	damagerpos = Commands->Get_Position(killer);
	const char *str = Get_Translated_Preset_Name_Ex(obj);
	const char *str2 = Get_Translated_Preset_Name_Ex(killer);
	SSGMGameLog::Log_Gamelog("KILLED;SOLDIER;%d;%s;%d;%d;%d;%d;%d;%s;%d;%d;%d;%d;%s;%s;%s",Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), int(victimpos.Y), int(victimpos.X), int(victimpos.Z), int(Commands->Get_Facing(obj)), Commands->Get_ID(killer),	Commands->Get_Preset_Name(killer),	int(damagerpos.Y), int(damagerpos.X), int(damagerpos.Z), int(Commands->Get_Facing(killer)),Get_Current_Weapon(killer),str,str2);
	delete[] str;
	delete[] str2;
	Unbind_Vehicle(obj, true);
	if (ExtraKillMessages)
	{
		if (Commands->Get_ID(killer) == 0)
		{
			StringClass str6;
			str6.Format("%ls was killed",Get_Wide_Player_Name(obj));
			Send_Message_With_Team_Color(Get_Object_Type(obj),str6);
		}
		else if (Commands->Get_ID(obj) == Commands->Get_ID(killer))
		{
			StringClass str5;
			str5.Format("%ls killed their self",Get_Wide_Player_Name(obj));
			Send_Message_With_Team_Color(Get_Object_Type(obj),str5);
		}
		else if (!Commands->Is_A_Star(killer))
		{
			StringClass str4;
			const char *str3 = Get_Translated_Preset_Name_Ini(killer);
			str4.Format("The %s killed %ls",str3,Get_Wide_Player_Name(obj));
			delete[] str3;
			Send_Message_With_Team_Color(Get_Object_Type(killer),str4);
		}
	}
	if (Commands->Get_ID(killer) != Commands->Get_ID(obj))
	{
		Commands->Send_Custom_Event(obj,killer,1000001,1,0);//increment the killcounter
	}
	if (Commands->Get_ID(killer) == Commands->Get_ID(obj))//dumbass :P
	{
		if (PlayQuakeSounds)
		{
			Commands->Create_2D_WAV_Sound("humiliation.wav");
		}
	}

  // Weapon drops
	DynamicVectorClass<StringClass> *weapons = WeaponDrops.Get(Commands->Get_Preset_Name(obj),0);
	if (weapons)
	{
		GameObject *pup = Commands->Create_Object((*weapons)[Commands->Get_Random_Int(0,weapons->Count())],Commands->Get_Position(obj));
		Attach_Script_Once(pup,"SSGM_Powerup_Expire","");
	}
}

ScriptRegistrant<SSGM_Soldier> SSGM_Soldier_Registrant("SSGM_Soldier","");
ScriptRegistrant<SSGM_Log_Key> SSGM_Log_Key_Registrant("SSGM_Log_Key","Key:string,Write:string");
ScriptRegistrant<SSGM_C4_Key> SSGM_C4_Key_Registrant("SSGM_C4_Key","");
ScriptRegistrant<SSGM_Bind_Key> SSGM_Bind_Key_Registrant("SSGM_Bind_Key","");
ScriptRegistrant<SSGM_BL_Key> SSGM_BL_Key_Registrant("SSGM_BL_Key","");
