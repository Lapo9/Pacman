#include "GhostsOnCollideReactions.h"
#include "PacmanPawn.h"
#include "GhostPawn.h"
#include "PacmanLevelState.h"


void UGhostOnCollideReactionKillPacman::React(GHOST_ON_COLLIDE_REACTION_PARAMS) const {
	if (collidedWith->IsA<APacmanPawn>()) {
		itself->PlayPacmanEatenSound();
		Cast<APacmanLevelState>(itself->GetWorld()->GetGameState())->NotifyPacmanDead();
	}
}


void UGhostOnCollideReactionDie::React(GHOST_ON_COLLIDE_REACTION_PARAMS) const {
	if (collidedWith->IsA<APacmanPawn>()) {
		itself->PlayDeathSound();
		Cast<APacmanLevelState>(itself->GetWorld()->GetGameState())->NotifyGhostEaten(*itself);
	}
}


void UGhostOnCollideReactionNothing::React(GHOST_ON_COLLIDE_REACTION_PARAMS) const {
	// Do nothing
}
