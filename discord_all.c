/*
	discord_all.c: BlastEm's Discord interface for all platforms (main source).
	Copyright 2020 Kelsey Boey.
	Licenced under the GNU GPL v3.
*/

// NOTE: currently works on win64 and linux64 (macos untested)

#include <stdio.h>
#include <stdlib.h>
#include "discord_all.h"
#include "util.h"
#include "romdb.h"
#include "system.h"

int init_discord()
{
	extern struct application app;
	memset(&app, 0, sizeof(app));
	extern enum EDiscordResult result;
	struct DiscordCreateParams params;
	params.client_id = 660727500179111956;
	params.flags = DiscordCreateFlags_Default;
	if (DiscordCreate(DISCORD_VERSION, &params, &app.core) != DiscordResult_Ok)
	{
		warning("DiscordCreate failed\n");
		app.core = NULL; // in case api isn't available
		app.activities = NULL;
		return 1;
	}
	printf("Discord API initialised\n");
	app.activities = app.core->get_activity_manager(app.core);
	return 0;
}

void update_discord_activities(int in_menu)
{
	if (app.core == NULL) return;
	extern rom_info *info;
	extern system_header *current_system;
	struct DiscordActivity activity;
	memset(&activity, 0, sizeof(activity));
	activity.timestamps.start = time(NULL);
	const char *name = (in_menu != 1) ? current_system->info.name : "Menu";
	strcpy(activity.details, name);
	app.activities->update_activity(app.activities, &activity, NULL, NULL);
	return;
}

void run_discord_callbacks()
{
	if (app.core == NULL) return;
	app.core->run_callbacks(app.core);
	return;
}

void close_discord()
{
	if (app.core == NULL) return; // doesn't need to do anything anyway
	app.core = NULL;
	app.activities = NULL;
	return;
}
