#include "GhostsOnCollideReactions.h"
#include "PacmanPawn.h"
#include "GhostPawn.h"
#include "PacmanLevelState.h"


void UGhostOnCollideReactionKillPacman::React(GHOST_ON_COLLIDE_REACTION_PARAMS) const {
	if (collidedWith->IsA<APacmanPawn>()) {

	}
}


void UGhostOnCollideReactionDie::React(GHOST_ON_COLLIDE_REACTION_PARAMS) const {
	if (collidedWith->IsA<APacmanPawn>()) {
		Cast<APacmanLevelState>(itself->GetWorld()->GetGameState())->NotifyGhostEaten(*itself);
	}
}


void UGhostOnCollideReactionNothing::React(GHOST_ON_COLLIDE_REACTION_PARAMS) const {
	// Do nothing
}
