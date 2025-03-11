/*
Plugin Name
Copyright (C) <Year> <Developer> <Email Address>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>
*/

#include <obs-module.h>
#include <obs-frontend-api.h>

#include <plugin-support.h>
#include "QRecDirPanel.h"

extern "C"
{
	OBS_DECLARE_MODULE()
	OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US")

	bool obs_module_load(void) {
		obs_log(LOG_INFO, "plugin loading...");

		// Create our custom panel and set it as the widget for the dock.
		RecDirPanel *panel = new RecDirPanel();

		// Add the dock widget to OBS’s main window.
		obs_frontend_add_dock_by_id("recdirpnl", "Recording Directory", panel);

		obs_log(LOG_INFO, "plugin loading successfully (version %s)", PLUGIN_VERSION);

		return true;
	}

	void obs_module_unload(void) {
		obs_log(LOG_INFO, "plugin unloaded");
	}

	// Provide a short description of the plugin.
	const char *obs_module_description(void) {
		return "Plugin to quickly set the recording directory via a dockable panel.";
	}
} // extern "C"
