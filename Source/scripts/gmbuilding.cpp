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
#include "gmbuilding.h"
#include "engine.h"
#include "BuildingGameObj.h"
#include "gmlog.h"
#include "gmgame.h"
void SSGM_Building::Created(GameObject *obj)
{
	triggerrepairing = true;
	triggerrepaired = true;
	Vector3	pos	= Commands->Get_Position(obj);
	SSGMGameLog::Log_Gamelog("CREATED;BUILDING;%d;%s;%d;%d;%d;%d;%f;%f;%d", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj),	int(pos.Y),int(pos.X),int(pos.Z), int(Commands->Get_Facing(obj)), Commands->Get_Max_Health(obj),Commands->Get_Max_Shield_Strength(obj),Get_Object_Type(obj));
}

void SSGM_Building::Destroyed(GameObject *obj)
{
	Vector3	pos	= Commands->Get_Position(obj);
	SSGMGameLog::Log_Gamelog("DESTROYED;BUILDING;%d;%s;%d;%d;%d", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), int(pos.Y), int(pos.X), int(pos.Z));
}

void SSGM_Building::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if (amount != 0) {
		Vector3	victimpos =	Commands->Get_Position(obj);
		Vector3	damagerpos = Commands->Get_Position(damager);
		SSGMGameLog::Log_Gamelog("DAMAGED;BUILDING;%d;%s;%d;%d;%d;%d;%d;%s;%d;%d;%d;%d;%f;%d;%d;%d", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), int(victimpos.Y), int(victimpos.X), int(victimpos.Z), int(Commands->Get_Facing(obj)), Commands->Get_ID(damager), Commands->Get_Preset_Name(damager), int(damagerpos.Y), int(damagerpos.X),	int(damagerpos.Z), int(Commands->Get_Facing(damager)), amount, int(Commands->Get_Health(obj)), int(Commands->Get_Shield_Strength(obj)),	int(Commands->Get_Points(obj)));
	}
	if (ShowExtraMessages)
	{
		if (triggerrepairing)
		{
			if (Commands->Is_A_Star(damager))
			{
				if (amount < 0.0f)
				{
					char repairmsg[250];
					const char *bld = Get_Translated_Preset_Name_Ini(obj);
					const char *str = Get_Player_Name(damager);
					sprintf(repairmsg,"%s is repairing the %s",str,bld);
					delete[] str;
					Send_Message_Team(Commands->Get_Player_Type(damager),104,234,40,repairmsg);
					delete[] bld;
					Commands->Start_Timer(obj,this,30.0f,22222);
					triggerrepairing = false;
				}
			}
		}
		if (triggerrepaired)
		{
			if (Commands->Is_A_Star(damager))
			{
				if (amount < 0.0f)
				{
					if (Commands->Get_Health(obj) == Commands->Get_Max_Health(obj))
					{
						triggerrepaired = false;
						int team = Commands->Get_Player_Type(damager);
						if (strcmp(Commands->Get_Preset_Name(obj),"mp_GDI_Advanced_Guard_Tower") == 0)
						{
							char repairmsg[250];
							const char *bld = Get_Translated_Preset_Name_Ex(obj);
							const char *str = Get_Player_Name(damager);
							sprintf(repairmsg,"%s repaired the %s",str,bld);
							delete[] str;
							Send_Message_Team(team,104,234,40,repairmsg);
							delete[] bld;
							Create_2D_WAV_Sound_Team("m00bgat_dsgn0007i1evag_snd.wav",team);
							Commands->Start_Timer(obj,this,30.0f,22223);
						}
						else if (strcmp(Commands->Get_Preset_Name(obj),"mp_Nod_Obelisk") == 0)
						{
							char repairmsg[250];
							const char *bld = Get_Translated_Preset_Name_Ex(obj);
							const char *str = Get_Player_Name(damager);
							sprintf(repairmsg,"%s repaired the %s",str,bld);
							delete[] str;
							Send_Message_Team(team,104,234,40,repairmsg);
							delete[] bld;
							Create_2D_WAV_Sound_Team("m00bnol_dsgn0008i1evan_snd.wav",team);
							Commands->Start_Timer(obj,this,30.0f,22223);
						}
						else
						{
							BuildingGameObj *b = obj->As_BuildingGameObj();
							if (b)
							{
								if (b->As_WarFactoryGameObj())
								{
									char repairmsg[250];
									const char *bld = Get_Translated_Preset_Name_Ex(obj);
									const char *str = Get_Player_Name(damager);
									sprintf(repairmsg,"%s repaired the %s",str,bld);
									delete[] str;
									Send_Message_Team(team,104,234,40,repairmsg);
									delete[] bld;
									if (team == 0)
									{
										Create_2D_WAV_Sound_Team("m00bnaf_dsgn0010i1evan_snd.wav",team);
									}
									else if (team == 1)
									{
										Create_2D_WAV_Sound_Team("m00bgwf_dsgn0009i1evag_snd.wav",team);
									}
									Commands->Start_Timer(obj,this,30.0f,22223);
								}
								else if (b->As_SoldierFactoryGameObj())
								{
									char repairmsg[250];
									const char *bld = Get_Translated_Preset_Name_Ex(obj);
									const char *str = Get_Player_Name(damager);
									sprintf(repairmsg,"%s repaired the %s",str,bld);
									delete[] str;
									Send_Message_Team(team,104,234,40,repairmsg);
									delete[] bld;
									if (team == 0)
									{
										Create_2D_WAV_Sound_Team("m00bnhn_dsgn0015i1evag_snd.wav",team);
									}
									else if (team == 1)
									{
										Create_2D_WAV_Sound_Team("m00bgib_dsgn0008i1evag_snd.wav",team);
									}
									Commands->Start_Timer(obj,this,30.0f,22223);
								}
								else if (b->As_RefineryGameObj())
								{
									char repairmsg[250];
									const char *bld = Get_Translated_Preset_Name_Ex(obj);
									const char *str = Get_Player_Name(damager);
									sprintf(repairmsg,"%s repaired the %s",str,bld);
									delete[] str;
									Send_Message_Team(team,104,234,40,repairmsg);
									delete[] bld;
									if (team == 0)
									{
										Create_2D_WAV_Sound_Team("m00bntr_dsgn0010i1evan_snd.wav",team);
									}
									else if (team == 1)
									{
										Create_2D_WAV_Sound_Team("m00bgtr_dsgn0009i1evag_snd.wav",team);
									}
									Commands->Start_Timer(obj,this,30.0f,22223);
								}
								else if (b->As_PowerPlantGameObj())
								{
									char repairmsg[250];
									const char *bld = Get_Translated_Preset_Name_Ex(obj);
									const char *str = Get_Player_Name(damager);
									sprintf(repairmsg,"%s repaired the %s",str,bld);
									delete[] str;
									Send_Message_Team(team,104,234,40,repairmsg);
									delete[] bld;
									if (team == 0)
									{
										Create_2D_WAV_Sound_Team("m00bnpp_dsgn0010i1evan_snd.wav",team);
									}
									else if (team == 1)
									{
										Create_2D_WAV_Sound_Team("m00bgpp_dsgn0009i1evag_snd.wav",team);
									}
									Commands->Start_Timer(obj,this,30.0f,22223);
								}
							}
						}
					}
				}
			}
		}
	}
}

void SSGM_Building::Timer_Expired(GameObject *obj, int number)
{
	if (number == 22222)
	{
		triggerrepairing = true;
	}
	if (number == 22223)
	{
		triggerrepaired = true;
	}
}

void SSGM_Building::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&triggerrepairing,sizeof(triggerrepairing),1);
}

void SSGM_Building::Killed(GameObject *obj,GameObject *killer)
{
	if (BuildingDeathPages)
	{
		StringClass str;
		const char *str3 = Get_Translated_Preset_Name_Ini(obj);
		const char *str2 = Get_Team_Name(obj->As_BuildingGameObj()->Get_Player_Type());
		str.Format("ppage %d The %s %s has been destroyed thanks to you",Get_Player_ID(killer),str2,str3);
		delete[] str3;
		delete[] str2;
		Console_Input(str);
	}
	GameObject *preset = GetExplosionObj();
	if (!preset)
	{
		preset = killer;
	}
	if (preset)
	{
		StringClass str;
		const char *str4 = Get_Translated_Preset_Name_Ini(obj);
		const char *str2 = Get_Translated_Preset_Name_Ex(preset);
		const char *str3 = Get_Player_Name(killer);
		str.Format("%s destroyed thanks to %s (%s)",str4,str3,str2);
		delete[] str3;
		delete[] str4;
		delete[] str2;
		SSGMGameLog::Log_Message(str,"_BUILDING");
	}
	Vector3	victimpos =	Commands->Get_Position(obj);
	Vector3	damagerpos = Commands->Get_Position(killer);
	const char *str = Get_Translated_Preset_Name_Ex(obj);
	const char *str2 = Get_Translated_Preset_Name_Ex(killer);
	SSGMGameLog::Log_Gamelog("KILLED;BUILDING;%d;%s;%d;%d;%d;%d;%d;%s;%d;%d;%d;%d;%s;%s;%s",Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), int(victimpos.Y), int(victimpos.X), int(victimpos.Z), int(Commands->Get_Facing(obj)), Commands->Get_ID(killer),	Commands->Get_Preset_Name(killer),	int(damagerpos.Y), int(damagerpos.X), int(damagerpos.Z), int(Commands->Get_Facing(killer)),Get_Current_Weapon(killer),str,str2);
	delete[] str;
	delete[] str2;

	if ( ((obj->As_BuildingGameObj() && obj->As_BuildingGameObj()->As_ConstructionYardGameObj())) || (Is_Script_Attached(obj,"TDA_ConYard_Controller")) )
	{
		if (Get_Object_Type(obj) == 1)
		{
			Create_2D_WAV_Sound_Team("m00bgcy_kill0002i1evan_snd.wav",0);
			Create_2D_WAV_Sound_Team("m00bgcy_kill0001i1evag_snd.wav",1);
		}
		else if (Get_Object_Type(obj) == 0)
		{
			Create_2D_WAV_Sound_Team("m00bncy_kill0001i1evan_snd.wav",0);
			Create_2D_WAV_Sound_Team("m00bncy_kill0002i1evag_snd.wav",1);
		}
	}
	else if (obj->As_BuildingGameObj() && obj->As_BuildingGameObj()->As_ComCenterGameObj())
	{
		if (Get_Object_Type(obj) == 1)
		{
			Create_2D_WAV_Sound_Team("m00bgcc_kill0002i1evan_snd.wav",0);
			Create_2D_WAV_Sound_Team("m00bgcc_kill0001i1evag_snd.wav",1);
		}
		else if (Get_Object_Type(obj) == 0)
		{
			Create_2D_WAV_Sound_Team("m00bncc_kill0001i1evan_snd.wav",0);
			Create_2D_WAV_Sound_Team("m00bncc_kill0002i1evag_snd.wav",1);
		}
	}
	else if (obj->As_BuildingGameObj() && obj->As_BuildingGameObj()->As_RepairBayGameObj())
	{
		if (Get_Object_Type(obj) == 1)
		{
			Create_2D_WAV_Sound_Team("m00bgrf_kill0002i1evan_snd.wav",0);
			Create_2D_WAV_Sound_Team("m00bgrf_kill0001i1evag_snd.wav",1);
		}
		else if (Get_Object_Type(obj) == 0)
		{
			Create_2D_WAV_Sound_Team("m00bnrf_kill0001i1evan_snd.wav",0);
			Create_2D_WAV_Sound_Team("m00bnrf_kill0002i1evag_snd.wav",1);
		}
	}
}

ScriptRegistrant<SSGM_Building> SSGM_Building_Registrant("SSGM_Building","");
