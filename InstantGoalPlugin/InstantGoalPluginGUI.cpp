#include "pch.h"
#include "InstantGoalPlugin.h"

/* Plugin Settings Window code here*/
std::string InstantGoalPlugin::GetPluginName() {
	return "InstantGoalPlugin";
}


void InstantGoalPlugin::SetImGuiContext(uintptr_t ctx) {
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}


// Render the plugin settings here
// This will show up in bakkesmod when the plugin is loaded at
//  f2 -> plugins -> KakkesPlugin
void InstantGoalPlugin::RenderSettings() {

	//list of selectable keybinds
	const char* items[] = { "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Zero", "F7", "F8", "F9", "F11", "F12",
		"NumPadOne", "NumPadTwo", "NumPadThree", "NumPadFour", "NumPadFive", "NumPadSix", "NumPadSeven", "NumPadEight", "NumPadNine", "NumPadZero",
		"XboxTypeS_LeftThumbStick", "XboxTypeS_RightThumbStick", "XboxTypeS_DPad_Up", "XboxTypeS_DPad_Left", "XboxTypeS_DPad_Right",  "XboxTypeS_DPad_Down",
		"XboxTypeS_LeftX", "XboxTypeS_LeftY", "XboxTypeS_RightX", "XboxTypeS_RightY", "XboxTypeS_X", "XboxTypeS_Y", "XboxTypeS_A", "XboxTypeS_A", };
	//set default keybnd to key "Zero"
	static int goal_bind = 9;
	static int camera_bind = 8;


	CVarWrapper enableCvar = cvarManager->getCvar("plugin_enabled");
	if (!enableCvar) {
		return;
	}
	bool isEnabled = enableCvar.getBoolValue();



	ImGui::TextUnformatted("Plugin settings");

	//Checkbox to enable/disable the plugin
	if (ImGui::Checkbox("Enable plugin", &isEnabled)) {
		enableCvar.setValue(isEnabled);
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Enable/Disable Plugin");
	}


	//key-bind for instant goal
	//remove old bind and set a new bind
	cvarManager->removeBind(items[goal_bind]);
	ImGui::Combo("Select key binding for Instant Goal", &goal_bind, items, IM_ARRAYSIZE(items));
	cvarManager->setBind(items[goal_bind], "instant_goal");

	//key-bind for camera toggle
	//remove old bind and set a new bind
	cvarManager->removeBind(items[camera_bind]);
	ImGui::Combo("Select key binding for Camera Toggle", &camera_bind, items, IM_ARRAYSIZE(items));
	cvarManager->setBind(items[camera_bind], "toggle_camera");

	ImGui::TextUnformatted("(Check out the Bindings-Tab to not overwrite any active bindings)");



	CVarWrapper cvar_plugin_bind = cvarManager->getCvar("plugin_bind");
	CVarWrapper cvar_camera_bind = cvarManager->getCvar("camera_bind");
	if (!cvar_plugin_bind || !cvar_camera_bind) {
		return;
	}
	cvar_plugin_bind.setValue(items[goal_bind]);
	cvar_camera_bind.setValue(items[camera_bind]);
}





// Do ImGui rendering here
void InstantGoalPlugin::Render()
{
	if (!ImGui::Begin(menuTitle_.c_str(), &isWindowOpen_, ImGuiWindowFlags_None))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	ImGui::End();

	if (!isWindowOpen_)
	{
		cvarManager->executeCommand("togglemenu " + GetMenuName());
	}
}

// Name of the menu that is used to toggle the window.
std::string InstantGoalPlugin::GetMenuName()
{
	return "Instant_Goal_Plugin";
}

// Title to give the menu
std::string InstantGoalPlugin::GetMenuTitle()
{
	return menuTitle_;
}


// Should events such as mouse clicks/key inputs be blocked so they won't reach the game
bool InstantGoalPlugin::ShouldBlockInput()
{
	return ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
}

// Return true if window should be interactive
bool InstantGoalPlugin::IsActiveOverlay()
{
	return true;
}

// Called when window is opened
void InstantGoalPlugin::OnOpen()
{
	isWindowOpen_ = true;
}

// Called when window is closed
void InstantGoalPlugin::OnClose()
{
	isWindowOpen_ = false;
}

