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
	ImGui::TextUnformatted("Plugin settings");


	const char* items[] = { "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Zero", "F7", "F8", "F9", "F11", "F12",
		"NumPadOne", "NumPadTwo", "NumPadThree", "NumPadFour", "NumPadFive", "NumPadSix", "NumPadSeven", "NumPadEight", "NumPadNine", "NumPadZero",
		"XboxTypeS_LeftThumbStick", "XboxTypeS_RightThumbStick", "XboxTypeS_DPad_Up", "XboxTypeS_DPad_Left", "XboxTypeS_DPad_Right",  "XboxTypeS_DPad_Down",
		"XboxTypeS_LeftX", "XboxTypeS_LeftY", "XboxTypeS_RightX", "XboxTypeS_RightY", "XboxTypeS_X", "XboxTypeS_Y", "XboxTypeS_A", "XboxTypeS_A", };
	static int item_current = 9;


	CVarWrapper enableCvar = cvarManager->getCvar("plugin_enabled");
	if (!enableCvar) {
		return;
	}
	bool enabled = enableCvar.getBoolValue();

	if (ImGui::Checkbox("Enable plugin", &enabled)) {
		enableCvar.setValue(enabled);
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Toggle Plugin");
	}




	cvarManager->removeBind(items[item_current]);
	ImGui::Combo("Select Key-Binding", &item_current, items, IM_ARRAYSIZE(items));

	ImGui::SameLine();
	ImGui::NewLine();

	cvarManager->setBind(items[item_current], "instant_goal");
	enableCvar = cvarManager->getCvar("plugin_bind");
	if (!enableCvar) {
		return;
	}
	enableCvar.setValue(items[item_current]);


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

/*
// Don't call this yourself, BM will call this function with a pointer to the current ImGui context
void Instant_Goal_Plugin::SetImGuiContext(uintptr_t ctx)
{
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}
*/

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

