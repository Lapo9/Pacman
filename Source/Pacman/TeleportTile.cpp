#include "TeleportTile.h"
#include "BoardPawn.h"


ATeleportTile::ATeleportTile() : Super{ ETileType::TELEPORT } {
}

void ATeleportTile::PawnOnTileCenter(ABoardPawn& pawn) const {
	// Move the pawn to the new tile (only if it didn't get teleported in the last movement, to avoid infinite recursion)
	if (!pawn.TeleportedFromTile) {
		pawn.TeleportedFromTile = this; // Store the tile from which it teleported from
		pawn.SetLocation2d(TeleportToTile->GetCenter());
	}

	// If the pawn just teleported, don't teleport it again (potentially causing an infinite loop) and act just like a normal walkable tile.
	else Super::PawnOnTileCenter(pawn);
}

void ATeleportTile::PawnLeftTileCenter(ABoardPawn& pawn) const {
	// If the pawn has just been teleported from this tile (and therefore the PawnLeftTileCenter was caused by the teleportation itself), do nothing.
	// Else, act like a standard walkable tile (and therefore erase the TeleportedFromTile, so that the pawn can be teleported again in the future)
	if (pawn.TeleportedFromTile != this) Super::PawnLeftTileCenter(pawn);
}
