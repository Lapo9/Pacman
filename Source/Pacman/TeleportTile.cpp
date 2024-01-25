#include "TeleportTile.h"
#include "BoardPawn.h"


ETileType ATeleportTile::GetType() const {
	return ETileType::TELEPORT;
}

void ATeleportTile::PawnOnTileCenter(ABoardPawn& pawn) const {
	// Move the pawn to the new tile (only if it didn't get teleported in the last movement, to avoid infinite recursion)
	if (!pawn.JustTeleported) pawn.SetLocation2D(TeleportToTile->GetCenter());

	// If the pawn just teleported, don't teleport it again (potentially causing an infinite loop) and act just like a normal walkable tile.
	else Super::PawnOnTileCenter(pawn);
}

void ATeleportTile::PawnLeftTileCenter(ABoardPawn& pawn) const {
	// If the pawn has the just teleported flag set and leaves this tile, act like a normal walkable tile (i.e. unset the flag).
	if (pawn.JustTeleported) Super::PawnLeftTileCenter(pawn);

	// If the pawn has actually just been teleported (therefore it left this tile center), set the flag
	else pawn.JustTeleported = true;
}
