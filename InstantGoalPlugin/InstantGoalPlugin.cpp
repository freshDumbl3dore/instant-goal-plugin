#include "pch.h"
#include "InstantGoalPlugin.h"

#include <stdlib.h>
#include <string.h>

#include "bakkesmod\wrappers\ArrayWrapper.h"
#include "bakkesmod\wrappers\GameEvent\TutorialWrapper.h"
#include "bakkesmod\wrappers\GameObject\BallWrapper.h"
#include "bakkesmod\wrappers\GameObject\CarWrapper.h"


BAKKESMOD_PLUGIN(InstantGoalPlugin, "Instant Goal Plugin", plugin_version, PLUGINTYPE_FREEPLAY | PLUGINTYPE_CUSTOM_TRAINING);

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

bool enabled = false;
std::string bind_goal = "Zero";
std::string bind_camera = "Nine";

void InstantGoalPlugin::onLoad()
{
	_globalCvarManager = cvarManager;
	cvarManager->log("Plugin loaded!");



	cvarManager->setBind("Zero", "instant_goal");
	cvarManager->setBind("Nine", "toggle_camera");
	cvarManager->registerCvar("plugin_enabled", "0", "Enable the Plugin", false, true, 0.0, true, 1.0)
		.addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) {
			enabled = cvar.getBoolValue();
		});
	cvarManager->registerCvar("plugin_bind", "Zero", "Bind the Instant Goal", false)
		.addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) {
			bind_goal = cvar.getStringValue();
		});
	cvarManager->registerCvar("camera_bind", "Nine", "Bind the Cmaera Toggle", false)
		.addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) {
			bind_camera = cvar.getStringValue();
		});



	
	cvarManager->registerNotifier("instant_goal", [&cm = this->cvarManager, &gw = this->gameWrapper](std::vector<std::string>) {

		if (enabled) {
			if (!gw->IsInFreeplay())
				return;
			ServerWrapper tw = gw->GetGameEventAsServer();
			CarWrapper player = tw.GetCars().Get(0);
			BallWrapper ball = tw.GetGameBalls().Get(0);
			if (player.IsNull() || tw.GetBall().IsNull()) {
				return;
			}

			Vector ballLoc = tw.GetBall().GetLocation();
			Vector goalTarget = tw.GetGoalLocation(1);
			float Speed = 1999.0;
			Vector shot = tw.GenerateShot(ballLoc, goalTarget, Speed);

			//teleport the ball to the goalline and score 
			ball.SetLocation(tw.GetGoals().Get(1).GetLocation());
			tw.GetBall().SetVelocity(shot);
		}

	}, "Puts the ball in the goal", PERMISSION_ALL);


	cvarManager->registerNotifier("toggle_camera", [&cm = this->cvarManager, &gw = this->gameWrapper](std::vector<std::string>) {
		
		if (enabled) {
			cm->executeCommand("toggle cl_goalreplay_pov 0 1");
		}

	}, "toggle between DirectorCam and POVCam", PERMISSION_ALL);



}


void InstantGoalPlugin::onUnload()
{
	//delete the current keybind for the plugin
	CVarWrapper bind_goal = cvarManager->getCvar("plugin_bind");
	CVarWrapper bind_camera = cvarManager->getCvar("camera_bind");
	if (!bind_goal || !bind_camera) {
		return;
	}
	std::string str_bind_goal = bind_goal.getStringValue();
	std::string str_bind_camera = bind_camera.getStringValue();
	cvarManager->removeBind(str_bind_goal);
	cvarManager->removeBind(str_bind_camera);

}
