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
#ifndef TT_INCLUDE__SMARTGAMEOBJDEF_H
#define TT_INCLUDE__SMARTGAMEOBJDEF_H

#include "scripts.h"
#include "ArmedGameObjDef.h"
#include "engine_common.h"
#include "engine_threading.h"
#include "engine_string.h"
#include "engine_vector.h"

class SmartGameObjDef : public ArmedGameObjDef
{
public:
	SmartGameObjDef( void );
	virtual bool								Save( ChunkSaveClass &csave );
	virtual bool								Load( ChunkLoadClass &cload );
	bool Is_Stealthed() const {return IsStealthUnit;}
#ifdef DDBEDIT
	virtual void                        Dump (FileClass &file);
#endif
	DECLARE_EDITABLE( SmartGameObjDef, ArmedGameObjDef );
protected:
	float											SightRange;
	float											SightArc;
	float											ListenerScale;
	bool											IsStealthUnit;
	friend	class								SmartGameObj;
	friend class PresetDump;
}; // size: 224


#endif
