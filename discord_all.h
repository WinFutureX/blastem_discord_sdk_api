/*
	discord_all.h: BlastEm's Discord interface for all platforms (header).
	Copyright 2020 Kelsey Boey.
	Licenced under the GNU GPL v3.
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
extern void update_discord_activities(int in_menu);
extern void run_discord_callbacks();
extern void close_discord();

#endif
