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

struct application
{
	struct IDiscordCore *core;
	struct IDiscordActivityManager *activities;
} app;

extern enum EDiscordResult DiscordCreate(DiscordVersion version, struct DiscordCreateParams* params, struct IDiscordCore** result);

// prototypes
extern int init_discord();
extern void update_discord_activities(int status);
extern void run_discord_callbacks();
extern void close_discord();

#endif
