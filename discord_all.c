/*
	discord_all.c: BlastEm's Discord interface for all platforms (main source).
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

// NOTE: currently works on win64 and linux64 (macos untested)

#include <stdio.h>
#include <stdlib.h>
#include "discord_all.h"
#include "util.h"
#include "romdb.h"
#include "system.h"

#ifdef _WIN32
#include <windows.h>
#define DISCORD_LIB "discord_game_sdk.dll"
#else
#include <dlfcn.h>
#ifdef __APPLE__
#define DISCORD_LIB "discord_game_sdk.dylib"
#else
#define DISCORD_LIB "discord_game_sdk.so"
#endif
#endif

int discord_startup()
{
	// attempt to load SDK API library
	#ifdef _WIN32
	// for windows
	void *discord_handle = LoadLibrary(DISCORD_LIB);
	typedef int (__stdcall *f_discord_create)(DiscordVersion version, struct DiscordCreateParams* params, struct IDiscordCore** result);
	f_discord_create discord_create = (f_discord_create)GetProcAddress(discord_handle, "DiscordCreate");
	#else
	// for linux and possibly macos
	void *discord_handle = dlopen(DISCORD_LIB, RTLD_LAZY);
	uint32_t (*discord_create)(DiscordVersion version, struct DiscordCreateParams* params, struct IDiscordCore** result);
	discord_create = dlsym(discord_handle, "DiscordCreate");
	#endif
	if (discord_create == NULL)
	{
		if (discord_handle == NULL) warning("Couldn't load SDK API library\n");
		else warning("Couldn't find the DiscordCreate function\n");
		return 1;
	}
	extern struct application app;
	memset(&app, 0, sizeof(app));
	static enum EDiscordResult result;
	struct DiscordCreateParams params;
	params.client_id = 660727500179111956;
	params.flags = DiscordCreateFlags_NoRequireDiscord;
	result = discord_create(DISCORD_VERSION, &params, &app.core);
	if (result != DiscordResult_Ok)
	{
		warning("DiscordCreate failed - returned %d\n", result);
		return 1;
	}
	printf("Discord API initialised\n");
	app.activities = app.core->get_activity_manager(app.core);
	return 0;
}

void discord_update_activities(int status)
{
	/*
		values for status:
		0: game loaded and running
		1: game loaded, but paused
		2: game not loaded, ignore
	*/
	if (app.core == NULL) return;
	if (app.activities == NULL) return;
	extern rom_info *info;
	extern system_header *current_system;
	struct DiscordActivity activity;
	memset(&activity, 0, sizeof(activity));
	activity.timestamps.start = time(NULL);
	const char *running;
	if (status < 2)
	{
		strcpy(activity.state, current_system->info.name);
		running = (status == 0) ? "Running" : "Paused";
	} else
	{
		running = "Idle";
	}
	strcpy(activity.details, running);
	strcpy(activity.assets.large_image, "icon");
	strcpy(activity.assets.large_text, "A fast and accurate Sega Mega Drive emulator");
	app.activities->update_activity(app.activities, &activity, NULL, NULL);
	return;
}

void discord_run_callbacks()
{
	if (app.core == NULL) return;
	app.core->run_callbacks(app.core);
	return;
}

void discord_shutdown()
{
	if (app.core == NULL) return; // doesn't need to do anything anyway
	if (app.activities != NULL)
	{
		app.activities->clear_activity(app.activities, NULL, NULL);
		app.core->run_callbacks(app.core); // one last time
	}
	app.core->destroy(app.core);
	app.core = NULL;
	app.activities = NULL;
	return;
}
