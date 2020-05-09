# The BlastEm Discord SDK API
A simple Discord interface for the BlastEm emulator that enables Rich Presence.
# Implementing the interface
~~AAAAAAAAAA~~

It's rather messy.
  1. Drop `discord_all.c`, `discord_all.h` and `discord_game_sdk.h` into BlastEm's source root.
  2. Put `discord_update_activity(<int>)` in blastem.c's `for` loop (whereever necessary) and in `ui_idle_loop` in nuklear_ui/blastem_nuklear.c
  3. Put `discord_run_callbacks()` in `render_framebuffer_updated` in render_sdl.c
  4. Put `#include <discord_all.h>` in all the aforementioned source files.
  5. Amend the Makefile, such that:
      ``` make
      # redacted for brevity
      DISCORD_OBJS = discord_all.o
      MAIN_OBJS=blastem.o system.o ... $(DISCORD_OBJS)
      ```
Or forgo the above steps, and do things your own way instead.
