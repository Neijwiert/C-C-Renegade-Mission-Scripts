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
#include "gmvehicle.h"
#include "engine.h"
#include "VehicleGameObj.h"
#include "GameObjManager.h"
#include "HashTemplateClass.h"
#include "gmlog.h"
#include "VehicleGameObj.h"
#include "gmgame.h"
SCRIPTS_API VehicleGameObj *Find_My_Vehicle(GameObject *obj)
{
	SLNode<VehicleGameObj> *x = GameObjManager::VehicleGameObjList.Head();
	while (x)
	{
		VehicleGameObj *o = x->Data();
		if (o)
		{
			SSGM_Vehicle *scr = (SSGM_Vehicle *)Find_Script_On_Object(o,"SSGM_Vehicle");
			if (scr)
			{
				if (!Commands->Find_Object(scr->OwnerID))
				{
					scr->OwnerID = 0;
					if (scr->IconModelID)
					{
						Commands->Destroy_Object(Commands->Find_Object(scr->IconModelID));
					}
				}
				if (Commands->Find_Object(scr->OwnerID) == obj)
				{
					return o;
				}
			}
		}
		x = x->Next();
	}
	return NULL;
}

SCRIPTS_API void Bind_Vehicle(GameObject *obj)
{
	if (Find_My_Vehicle(obj) == Get_Vehicle(obj) && Get_Vehicle(obj) != 0)
	{
		StringClass str;
		str.Format("ppage %d This vehicle is already bound to you.",Get_Player_ID(obj));
		Console_Input(str);
		return;
	}
	GameObject *veh = Get_Vehicle(obj);
	if (veh)
	{
		Unbind_Vehicle(obj, true);
		SSGM_Vehicle *scr = (SSGM_Vehicle *)Find_Script_On_Object(veh,"SSGM_Vehicle");
		if ((!scr->OwnerID) || (!Commands->Find_Object(scr->OwnerID)))
		{
			if (Get_Vehicle_Driver(veh) == obj)
			{
				scr->OwnerID = Commands->Get_ID(obj);
				StringClass str;
				str.Format("ppage %d Your vehicle has been bound to you.",Get_Player_ID(obj));
				Console_Input(str);
				if (VehicleLockIcons)
				{
					GameObject *icon = Commands->Create_Object_At_Bone(veh,"Invisible_Object","muzzlea0");
					if (!veh->As_VehicleGameObj()->Is_Stealth_Enabled() && !Find_Script_On_Object(veh,"SH_UndergroundVehicle"))
					{
						Commands->Set_Model(icon,"o_em_apc");
					}
					Commands->Attach_To_Object_Bone ( icon, veh, "muzzlea0" );
					scr->IconModelID = Commands->Get_ID(icon);
				}
				else
				{
					scr->IconModelID = 0;
				}
			}
			else
			{
				StringClass str;
				str.Format("ppage %d You are not the driver of the vehicle.",Get_Player_ID(obj));
				Console_Input(str);
			}
		}
		else
		{
			StringClass str;
			str.Format("ppage %d The vehicle you are in is already bound.",Get_Player_ID(obj));
			Console_Input(str);
		}
	}
	else
	{
		StringClass str;
		str.Format("ppage %d You have to be in a vehicle to bind.",Get_Player_ID(obj));
		Console_Input(str);
	}
}

SCRIPTS_API void Lock_Vehicle(GameObject *obj)
{
	VehicleGameObj *boundveh = Find_My_Vehicle(obj);
	if (boundveh)
	{
		if (!boundveh->Get_Owner())
		{
			boundveh->Set_Owner(obj->As_SoldierGameObj());
			SSGM_Vehicle *scr = (SSGM_Vehicle *)Find_Script_On_Object(boundveh,"SSGM_Vehicle");
			if (!boundveh->As_VehicleGameObj()->Is_Stealth_Enabled() && !Find_Script_On_Object(boundveh,"SH_UndergroundVehicle"))
			{
				if (scr->IconModelID)
				{
					Commands->Set_Model(Commands->Find_Object(scr->IconModelID),"p_keycrd_red");
				}
			}
			StringClass str;
			str.Format("ppage %d Your vehicle has been locked.",Get_Player_ID(obj));
			Console_Input(str);
			if (Get_Vehicle_Driver(boundveh) != obj)
			{
				Force_Occupant_Exit(boundveh, 0);
			}
		}
		else
		{
			StringClass str;
			str.Format("ppage %d Your vehicle is already locked.",Get_Player_ID(obj));
			Console_Input(str);
		}
	}
	else
	{
		StringClass str;
		str.Format("ppage %d You have no vehicle bound to you.",Get_Player_ID(obj));
		Console_Input(str);
	}
}

SCRIPTS_API void Unlock_Vehicle(GameObject *obj, bool silent)
{
	VehicleGameObj *boundveh = Find_My_Vehicle(obj);
	if (boundveh)
	{
		if (boundveh->Get_Owner())
		{
			boundveh->Set_Owner(0);
			SSGM_Vehicle *scr = (SSGM_Vehicle *)Find_Script_On_Object(boundveh,"SSGM_Vehicle");
			if (!boundveh->As_VehicleGameObj()->Is_Stealth_Enabled() && !Find_Script_On_Object(boundveh,"SH_UndergroundVehicle"))
			{
				if (scr->IconModelID)
				{
					Commands->Set_Model(Commands->Find_Object(scr->IconModelID),"o_em_apc");
				}
			}
			
			if ( !silent )
			{
				StringClass str;
				str.Format("ppage %d Your vehicle has been unlocked.",Get_Player_ID(obj));
				Console_Input(str);
			}
		}
		else
		{
			if ( !silent )
			{
				StringClass str;
				str.Format("ppage %d Your vehicle is not locked.",Get_Player_ID(obj));
				Console_Input(str);
			}
		}
	}
	else if ( !silent )
	{
		StringClass str;
		str.Format("ppage %d You have no vehicle bound to you.",Get_Player_ID(obj));
		Console_Input(str);
	}
}

SCRIPTS_API void Unbind_Vehicle(GameObject *obj, bool silent)
{
	VehicleGameObj *boundveh = Find_My_Vehicle(obj);
	if (boundveh)
	{
		if (boundveh->Get_Owner())
		{
			boundveh->Set_Owner(0);
		}
		SSGM_Vehicle *scr = (SSGM_Vehicle *)Find_Script_On_Object(boundveh,"SSGM_Vehicle");
		scr->OwnerID = 0;
		if (scr->IconModelID)
		{
			Commands->Destroy_Object(Commands->Find_Object(scr->IconModelID));
			scr->IconModelID = 0;
		}

		if ( !silent )
		{
			StringClass str;
			str.Format("ppage %d Your vehicle has been unbound.",Get_Player_ID(obj));
			Console_Input(str);
		}
	}
	else if ( !silent )
	{
		StringClass str;
		str.Format("ppage %d You have no vehicle bound to you.",Get_Player_ID(obj));
		Console_Input(str);
	}
}

SCRIPTS_API void Kick_Vehicle_Driver(GameObject *obj)
{
	VehicleGameObj *boundveh = Find_My_Vehicle(obj);
	if (boundveh)
	{
		if (Get_Vehicle_Driver(boundveh) != obj)
		{
			Force_Occupant_Exit(boundveh,0);
			StringClass str;
			str.Format("ppage %d The driver has been kicked out of your vehicle.",Get_Player_ID(obj));
			Console_Input(str);
		}
		else
		{
			StringClass str;
			str.Format("ppage %d You cant kick yourself out of your vehicle.",Get_Player_ID(obj));
			Console_Input(str);
		}
	}
	else
	{
		StringClass str;
		str.Format("ppage %d You have no vehicle bound to you.",Get_Player_ID(obj));
		Console_Input(str);
	}
}

void SSGM_Vehicle::Created(GameObject *obj)
{
	triggerrepairing = true;
	triggerrepaired = true;
	OwnerID = 0;
	IconModelID = 0;
	if (Get_Vehicle_Mode(obj) == VEHICLE_TYPE_TURRET)
	{
		Vector3	pos	= Commands->Get_Position(obj);
		SSGMGameLog::Log_Gamelog("CREATED;BUILDING;%d;%s;%d;%d;%d;%d;%f;%f;%d", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj),	int(pos.Y),int(pos.X),int(pos.Z), int(Commands->Get_Facing(obj)), Commands->Get_Max_Health(obj),Commands->Get_Max_Shield_Strength(obj),Get_Object_Type(obj));
	}
	else
	{
		Vector3	pos	= Commands->Get_Position(obj);
		SSGMGameLog::Log_Gamelog("CREATED;VEHICLE;%d;%s;%d;%d;%d;%d;%d;%d;%d",	Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), int(pos.Y),int(pos.X),int(pos.Z), int(Commands->Get_Facing(obj)), int(Commands->Get_Max_Health(obj)),int(Commands->Get_Max_Shield_Strength(obj)), Commands->Get_Player_Type(obj));
	}
}

void SSGM_Vehicle::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&triggerrepairing,sizeof(triggerrepairing),1);
	Auto_Save_Variable(&triggerrepaired,sizeof(triggerrepaired),2);
}

extern HashTemplateClass<StringClass,StringClass> VehicleWreckages;
void SSGM_Vehicle::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (VehicleOwnership)
		{
			if (sender == obj->As_VehicleGameObj()->Get_Lock_Owner())
			{
				Bind_Vehicle(sender);
			}
			if (OwnerID)
			{
				if (Commands->Find_Object(OwnerID))
				{
					if (Commands->Find_Object(OwnerID)->As_DamageableGameObj()->Get_Player_Type() != sender->As_DamageableGameObj()->Get_Player_Type())
					{
						StringClass str;
						str.Format("ppage %d The enemy has stolen your vehicle.",Get_Player_ID(Commands->Find_Object(OwnerID)));
						Console_Input(str);
						Unbind_Vehicle(Commands->Find_Object(OwnerID),true);
					}
					if (Commands->Find_Object(OwnerID) != sender)
					{
						if (Get_Vehicle_Occupant(obj,0) == sender)
						{
							StringClass str;
							const char *str2 = Get_Player_Name(sender);
							str.Format("ppage %d Player %s has entered your vehicle.",Get_Player_ID(Commands->Find_Object(OwnerID)),str2);
							delete[] str2;
							Console_Input(str);
						}
					}
				}
				else
				{
					OwnerID = 0;
					if (IconModelID)
					{
						Commands->Destroy_Object(Commands->Find_Object(IconModelID));
						IconModelID = 0;
					}
				}
			}
		}
		Vector3	pos	= Commands->Get_Position(obj);
		Vector3	senderpos =	Commands->Get_Position(sender);
		SSGMGameLog::Log_Gamelog("ENTER;%d;%s;%d;%d;%d;%d;%s;%d;%d;%d", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj),	int(pos.Y),	int(pos.X),	int(pos.Z),	Commands->Get_ID(sender), Commands->Get_Preset_Name(sender), int(senderpos.Y), int(senderpos.X), int(senderpos.Z));
	}
	else if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		Vector3	pos	= Commands->Get_Position(obj);
		Vector3	senderpos =	Commands->Get_Position(sender);
		SSGMGameLog::Log_Gamelog("EXIT;%d;%s;%d;%d;%d;%d;%s;%d;%d;%d",	Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), int(pos.Y), int(pos.X), int(pos.Z), Commands->Get_ID(sender), Commands->Get_Preset_Name(sender),	int(senderpos.Y), int(senderpos.X),	int(senderpos.Z));
	}
}

void SSGM_Vehicle::Killed(GameObject *obj,GameObject *killer)
{
	if (Commands->Find_Object(OwnerID) && VehicleOwnership)
	{
		StringClass str;
		str.Format("ppage %d Your vehicle was destroyed.",Get_Player_ID(Commands->Find_Object(OwnerID)));
		Console_Input(str);
	}
	StringClass *strx = VehicleWreckages.Get(Commands->Get_Preset_Name(obj));
	if (strx)
	{
		Vector3 pos = Commands->Get_Position(obj);
		float facing = Commands->Get_Facing(obj);
		GameObject *destroyed = Commands->Create_Object(strx->Peek_Buffer(),pos);
		Commands->Set_Facing(destroyed,facing);
		Commands->Attach_Script(destroyed,"SSGM_Vehicle_Wreckage",Commands->Get_Preset_Name(obj));
	}
	if (IconModelID)
	{
		if (Commands->Find_Object(IconModelID))
		{
			Commands->Destroy_Object(Commands->Find_Object(IconModelID));
			IconModelID = 0;
		}
	}
	GameObject *preset = GetExplosionObj();
	if (!preset) {
		preset = killer;
	}
	if (!Commands->Is_A_Star(obj))
	{
		if (obj->As_VehicleGameObj()->Get_Type() == VEHICLE_TYPE_TURRET)
		{
			StringClass str;
			const char *str2 = Get_Translated_Preset_Name_Ini(obj);
			const char *str3 = Get_Translated_Preset_Name_Ex(preset);
			const char *str4 = Get_Player_Name(killer);
			str.Format("%s destroyed thanks to %s (%s)",str2,str4,str3);
			delete[] str2;
			delete[] str3;
			delete[] str4;
			SSGMGameLog::Log_Message(str,"_BUILDING");
			return;
		}
	}
	if (!preset)
	{
		StringClass str;
		const char *str2 = Get_Translated_Preset_Name_Ini(obj);
		str.Format("A %s was destroyed",str2);
		delete[] str2;
		SSGMGameLog::Log_Message(str,"_VEHKILL");
	}
	else if (!Commands->Is_A_Star(preset))
	{
		StringClass str;
		const char *str2 = Get_Translated_Preset_Name_Ini(preset);
		const char *str5 = Get_Translated_Preset_Name_Ini(obj);
		const char *str3 = Get_Player_Name(Get_Vehicle_Occupant(obj,0));
		const char *str4 = Get_Player_Name(Commands->Find_Object(OwnerID));
		str.Format("The %s destroyed a %s (Driver: %s - Owner: %s)",str2,str5,str3,str4);
		delete[] str2;
		delete[] str3;
		delete[] str4;
		delete[] str5;
		SSGMGameLog::Log_Message(str,"_VEHKILL");
	}
	else
	{
		StringClass str;
		const char *str2 = Get_Translated_Preset_Name_Ex(preset);
		const char *str3 = Get_Player_Name(killer);
		const char *str4 = Get_Player_Name(Get_Vehicle_Occupant(obj,0));
		const char *str5 = Get_Player_Name(Commands->Find_Object(OwnerID));
		const char *str6 = Get_Translated_Preset_Name_Ini(obj);
		str.Format("%s (%s) destroyed a %s (Driver: %s - Owner: %s)",str3,str2,str6,str4,str5);
		delete[] str3;
		delete[] str2;
		delete[] str4;
		delete[] str5;
		delete[] str6;
		SSGMGameLog::Log_Message(str,"_VEHKILL");
	}
	if (Get_Vehicle_Mode(obj) == VEHICLE_TYPE_TURRET)
	{
		Vector3	victimpos =	Commands->Get_Position(obj);
		Vector3	damagerpos = Commands->Get_Position(killer);
		const char *str = Get_Translated_Preset_Name_Ex(obj);
		const char *str2 = Get_Translated_Preset_Name_Ex(killer);
		SSGMGameLog::Log_Gamelog("KILLED;BUILDING;%d;%s;%d;%d;%d;%d;%d;%s;%d;%d;%d;%d;%s;%s;%s",Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), int(victimpos.Y), int(victimpos.X), int(victimpos.Z), int(Commands->Get_Facing(obj)), Commands->Get_ID(killer),	Commands->Get_Preset_Name(killer),	int(damagerpos.Y), int(damagerpos.X), int(damagerpos.Z), int(Commands->Get_Facing(killer)),Get_Current_Weapon(killer),str,str2);
		delete[] str;
		delete[] str2;
	}
	else
	{
		Vector3	victimpos =	Commands->Get_Position(obj);
		Vector3	damagerpos = Commands->Get_Position(killer);
		const char *str3 = Get_Translated_Preset_Name_Ex(obj);
		const char *str2 = Get_Translated_Preset_Name_Ex(killer);
		SSGMGameLog::Log_Gamelog("KILLED;VEHICLE;%d;%s;%d;%d;%d;%d;%d;%s;%d;%d;%d;%d;%s;%s;%s",Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), int(victimpos.Y), int(victimpos.X), int(victimpos.Z), int(Commands->Get_Facing(obj)), Commands->Get_ID(killer),	Commands->Get_Preset_Name(killer),	int(damagerpos.Y), int(damagerpos.X), int(damagerpos.Z), int(Commands->Get_Facing(killer)),Get_Current_Weapon(killer),str3,str2);
		delete[] str3;
		delete[] str2;
	}
}

void SSGM_Vehicle::Destroyed(GameObject *obj)
{
	if (IconModelID)
	{
		if (Commands->Find_Object(IconModelID))
		{
			Commands->Destroy_Object(Commands->Find_Object(IconModelID));
			IconModelID = 0;
		}
	}
	if (Get_Vehicle_Mode(obj) == VEHICLE_TYPE_TURRET)
	{
		Vector3	pos	= Commands->Get_Position(obj);
		SSGMGameLog::Log_Gamelog("DESTROYED;BUILDING;%d;%s;%d;%d;%d", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), int(pos.Y), int(pos.X), int(pos.Z));
	}
	else
	{
		Vector3	pos	= Commands->Get_Position(obj);
		SSGMGameLog::Log_Gamelog("DESTROYED;VEHICLE;%d;%s;%d;%d;%d", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), int(pos.Y), int(pos.X), int(pos.Z));
	}
}

void SSGM_Vehicle::Detach(GameObject *obj)
{
	ScriptImpClass::Detach(obj);
	if (IconModelID)
	{
		if (Commands->Find_Object(IconModelID))
		{
			Commands->Destroy_Object(Commands->Find_Object(IconModelID));
			IconModelID = 0;
		}
	}
}

void SSGM_Vehicle::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (amount != 0)
	{
		if (Get_Vehicle_Mode(obj) == VEHICLE_TYPE_TURRET)
		{
			Vector3	victimpos =	Commands->Get_Position(obj);
			Vector3	damagerpos = Commands->Get_Position(damager);
			SSGMGameLog::Log_Gamelog("DAMAGED;BUILDING;%d;%s;%d;%d;%d;%d;%d;%s;%d;%d;%d;%d;%f;%d;%d;%d", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), int(victimpos.Y), int(victimpos.X), int(victimpos.Z), int(Commands->Get_Facing(obj)), Commands->Get_ID(damager), Commands->Get_Preset_Name(damager), int(damagerpos.Y), int(damagerpos.X),	int(damagerpos.Z), int(Commands->Get_Facing(damager)), amount, int(Commands->Get_Health(obj)), int(Commands->Get_Shield_Strength(obj)),	int(Commands->Get_Points(obj)));
		}
		else
		{
			Vector3	victimpos =	Commands->Get_Position(obj);
			Vector3	damagerpos = Commands->Get_Position(damager);
			SSGMGameLog::Log_Gamelog("DAMAGED;VEHICLE;%d;%s;%d;%d;%d;%d;%d;%s;%d;%d;%d;%d;%f;%d;%d;%d", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj),	int(victimpos.Y), int(victimpos.X),	int(victimpos.Z), int(Commands->Get_Facing(obj)), Commands->Get_ID(damager), Commands->Get_Preset_Name(damager), int(damagerpos.Y),	int(damagerpos.X), int(damagerpos.Z), int(Commands->Get_Facing(damager)), amount, int(Commands->Get_Health(obj)), int(Commands->Get_Shield_Strength(obj)), int(Commands->Get_Points(damager)));
		}
	}
	if (ShowExtraMessages)
	{
		if (triggerrepairing)
		{
			if (strcmp(Commands->Get_Preset_Name(obj),"CnC_Nod_Harvester") == 0)
			{
				if (Commands->Is_A_Star(damager))
				{
					if (amount < 0.0f)
					{
						char repairmsg[250];
						const char *bld = Get_Translated_Preset_Name_Ex(obj);
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
			else if (strcmp(Commands->Get_Preset_Name(obj),"CnC_GDI_Harvester") == 0)
			{
				if (Commands->Is_A_Star(damager))
				{
					if (amount < 0.0f)
					{
						char repairmsg[250];
						const char *bld = Get_Translated_Preset_Name_Ex(obj);
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
		}
		if (triggerrepaired)
		{
			if ( (strcmp(Commands->Get_Preset_Name(obj),"CnC_Nod_Harvester") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"CnC_GDI_Harvester") == 0) )
			{
				if (amount < 0.0f)
				{
					if (Commands->Get_Shield_Strength(obj) == Commands->Get_Max_Shield_Strength(obj))
					{
						char repairmsg[250];
						const char *bld = Get_Translated_Preset_Name_Ex(obj);
						const char *str = Get_Player_Name(damager);
						sprintf(repairmsg,"%s repaired the %s",str,bld);
						delete[] str;
						Send_Message_Team(Commands->Get_Player_Type(damager),104,234,40,repairmsg);
						delete[] bld;
						triggerrepaired = false;
						Create_2D_WAV_Sound_Team("m00evag_dsgn0019i1evag_snd.wav",Commands->Get_Player_Type(damager));
						Commands->Start_Timer(obj,this,30.0f,22223);
					}
				}
			}
		}
	}
}

void SSGM_Vehicle::Timer_Expired(GameObject *obj, int number)
{
	if (number == 22222)
	{
		triggerrepairing = true;
	}
	else if (number == 22223)
	{
		triggerrepaired = true;
	}
}

void SSGM_Vehicle_Wreckage::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,WreckDestroySelfTime,1001);//destroy the wreck after timer expires
	Set_Skin(obj,"Blamo");//needed to avoid destruction of the wreck on vehicle Killed
	Commands->Start_Timer(obj,this,2.0f,1000);//set skin to CNCVehicleHeavy again
	Set_Max_Health(obj,Commands->Get_Health(obj) + Commands->Get_Shield_Strength(obj));
	Set_Max_Shield_Strength(obj,0);
	Commands->Set_Health(obj,100);
	Commands->Disable_Physical_Collisions(obj);
	Commands->Set_Player_Type(obj,-2);
	Set_Death_Points(obj,0);
	Set_Damage_Points(obj,0);
}

void SSGM_Vehicle_Wreckage::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1000)
	{
		Set_Skin(obj,"SkinVehicleLight");
	}
	else if (number == 1001)
	{
		Commands->Destroy_Object(obj);
	}
}

void SSGM_Vehicle_Wreckage::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	if (amount < 0)
	{
		if (Commands->Get_Max_Health(obj) == Commands->Get_Health(obj) && Commands->Get_Max_Shield_Strength(obj) == Commands->Get_Shield_Strength(obj))
		{
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 3;
			float facing = Commands->Get_Facing(obj);
			const char *preset = Get_Parameter("preset");
			Commands->Destroy_Object(obj);
			Damage_All_Objects_Area(999999,"Laser_NoBuilding",pos,5,0,0);
			GameObject *veh = Commands->Create_Object(preset,pos);
			Commands->Set_Facing(veh,facing);
			Commands->Set_Health(veh,1);
			Commands->Set_Shield_Strength(veh,0);
			Commands->Set_Player_Type(veh,-2);
		}
	}
}

ScriptRegistrant<SSGM_Vehicle> SSGM_Vehicle_Registrant("SSGM_Vehicle","");
ScriptRegistrant<SSGM_Vehicle_Wreckage> SSGM_Vehicle_Wreckage_Registrant("SSGM_Vehicle_Wreckage","preset:string");
