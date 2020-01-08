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
#else
#include <dlfcn.h>
#endif

int init_discord()
{
	// attempt to load SDK API library
	#ifdef _WIN32 // for windows
	typedef int (__stdcall *f_crdiscord)();
	void *handle;
	handle = LoadLibrary("discord_game_sdk.dll");
	if (handle == NULL)
	{
		warning("Couldn't load SDK API library\n");
		return 1;
	}
	f_crdiscord create_discord = (f_crdiscord)GetProcAddress(handle, "DiscordCreate");
	if (create_discord == NULL)
	{
		warning("Couldn't find the DiscordCreate function\n");
		return 1;
	}
	#else // for linux and possibly macos
	void *handle;
	double (*create_discord)(DiscordVersion version, struct DiscordCreateParams* params, struct IDiscordCore** result);
	char *error;
	#ifdef __linux__
	handle = dlopen("discord_game_sdk.so", RTLD_LAZY);
	#elif __APPLE__
	handle = dlopen("discord_game_sdk.dylib", RTLD_LAZY);
	#endif
	if (!handle)
	{
		warning("Couldn't load SDK API library\n");
		return 1;
	}
	create_discord = dlsym(handle, "DiscordCreate");
	if ((error = dlerror()) != NULL)
	{
		warning("Couldn't find the DiscordCreate function\n");
		return 1;
	}
	#endif
	extern struct application app;
	memset(&app, 0, sizeof(app));
	static enum EDiscordResult result;
	struct DiscordCreateParams params;
	params.client_id = 660727500179111956;
	params.flags = DiscordCreateFlags_Default;
	result = create_discord(DISCORD_VERSION, &params, &app.core);
	if (result != DiscordResult_Ok)
	{
		warning("DiscordCreate failed - returned %d\n", result);
		app.core = NULL; // in case api isn't available
		app.activities = NULL;
		return 1;
	}
	printf("Discord API initialised\n");
	app.activities = app.core->get_activity_manager(app.core);
	return 0;
}

void update_discord_activities(int status)
{
	/*
		values for status:
		0: game loaded and running
		1: game loaded, but paused
		2: game not loaded, ignore
	*/
	if (app.core == NULL) return;
	extern rom_info *info;
	extern system_header *current_system;
	struct DiscordActivity activity;
	memset(&activity, 0, sizeof(activity));
	activity.timestamps.start = time(NULL);
	const char *running;
	if (status < 2)
	{
		strcpy(activity.details, current_system->info.name);
		running = (status == 0) ? "Running" : "Paused";
	} else
	{
		running = "Idle";
	}
	strcpy(activity.state, running);
	strcpy(activity.assets.large_image, "icon");
	strcpy(activity.assets.large_text, "A fast and accurate Sega Mega Drive emulator");
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
