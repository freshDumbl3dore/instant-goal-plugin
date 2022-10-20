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
int InstantGoalPlugin::clicked = 0;

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



	//if (cvarManager->getCvar("enabled")) {

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

			//Calculate nearest goal

			/*
			Vector goal1Diff = tw.GetGoals().Get(0).GetLocation() - ballLoc;
			Vector goal2Diff = tw.GetGoals().Get(1).GetLocation() - ballLoc;
			float goal1DiffF = abs(goal1Diff.X) + abs(goal1Diff.Y) + abs(goal1Diff.Z);
			float goal2DiffF = abs(goal2Diff.X) + abs(goal2Diff.Y) + abs(goal2Diff.Z);
			*/
			int target = 0;
			/*
			if (goal1DiffF > goal2DiffF)
			{
				target = 1;
			}
			*/

			Vector goalTarget = tw.GenerateGoalAimLocation(target, ballLoc);
			goalTarget.Z = tw.GetGoalExtent(target).Z;
			float Speed = 2000.0;
			Vector shot = tw.GenerateShot(ballLoc, goalTarget, Speed);

			ball.SetLocation(tw.GetGoals().Get(target).GetLocation());
			tw.GetBall().SetVelocity(shot);
		}
		}, "Puts the ball in the goal", PERMISSION_FREEPLAY | PERMISSION_PAUSEMENU_CLOSED);

}


void InstantGoalPlugin::onUnload()
{
	CVarWrapper enableCvar = cvarManager->getCvar("plugin_bind");
	if (!enableCvar) {
		return;
	}

	std::string bind = enableCvar.getStringValue();
	cvarManager->removeBind(bind);
}
