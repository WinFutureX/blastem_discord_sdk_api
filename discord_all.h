/*
	discord_all.h: BlastEm's Discord interface for all platforms (header).
	Copyright (C) 2020 Kelsey Boey
	
	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.
	
	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef DISCORD_ALL_H
#define DISCORD_ALL_H

#include "discord_game_sdk.h"

typedef struct
{
	struct IDiscordCore *core;
	struct IDiscordActivityManager *activities;
} application;

// game status
typedef enum
{
	DISCORD_GAME_LOADED_RUNNING,
	DISCORD_GAME_LOADED_PAUSED,
	DISCORD_GAME_NOT_LOADED
} discord_game_status;

// prototypes
extern int discord_startup();
extern void discord_update_activity(int status);
extern void discord_run_callbacks();
extern void discord_shutdown();

#endif
