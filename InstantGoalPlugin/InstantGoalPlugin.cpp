#include "pch.h"
#include "InstantGoalPlugin.h"

#include <stdlib.h>

#include "bakkesmod\wrappers\ArrayWrapper.h"
#include "bakkesmod\wrappers\GameEvent\TutorialWrapper.h"
#include "bakkesmod\wrappers\GameObject\BallWrapper.h"
#include "bakkesmod\wrappers\GameObject\CarWrapper.h"


BAKKESMOD_PLUGIN(InstantGoalPlugin, "Instant Goal Plugin", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

bool enabled = false;
std::string bind = "Zero";

void InstantGoalPlugin::onLoad()
{
	_globalCvarManager = cvarManager;
	cvarManager->log("Plugin loaded!");



	cvarManager->setBind("Zero", "instant_goal");
	cvarManager->registerCvar("plugin_enabled", "0", "Enable the Plugin", false, true, 0.0, true, 1.0)
		.addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) {
			enabled = cvar.getBoolValue();
		});
	cvarManager->registerCvar("plugin_bind", "Zero", "Bind the Plugin", false)
		.addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) {
			bind = cvar.getStringValue();
		});




	cvarManager->registerNotifier("instant_goal", [&cm = this->cvarManager, &gw = this->gameWrapper](std::vector<std::string>) {

		if (enabled) {
			if (!gw->IsInFreeplay())
				return;
			ServerWrapper tw = gw->GetGameEventAsServer();

			//If there are less than 2 goals, its probably a workshop map, dont rebound here
			if (tw.GetGoals().Count() < 2 || tw.GetCars().Count() == 0)
				return;

			CarWrapper player = tw.GetCars().Get(0);
			BallWrapper ball = tw.GetGameBalls().Get(0);
			if (player.IsNull() || tw.GetBall().IsNull())
				return;

			Vector ballLoc = tw.GetBall().GetLocation();

			Vector goalTarget = tw.GenerateGoalAimLocation(0, ballLoc);
			goalTarget.Z = tw.GetGoalExtent(0).Z;
			float Speed = 2000.0;
			Vector shot = tw.GenerateShot(ballLoc, goalTarget, Speed);

			ball.SetLocation(tw.GetGoals().Get(0).GetLocation());
			tw.GetBall().SetVelocity(shot);
		}
		}, "Puts the ball in the goal", PERMISSION_FREEPLAY | PERMISSION_PAUSEMENU_CLOSED);

}


void InstantGoalPlugin::onUnload()
{
	//delete the current keybind for the plugin
	CVarWrapper enableCvar = cvarManager->getCvar("plugin_bind");
	if (!enableCvar) {
		return;
	}

	std::string bind = enableCvar.getStringValue();
	cvarManager->removeBind(bind);
}
