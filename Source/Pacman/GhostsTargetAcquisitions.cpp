#include "GhostsTargetAcquisitions.h"
#include "AbstractMap.h"
#include "BoardPawn.h"
#include "PacmanPawn.h"
#include "PacmanSettings.h"
#include "Tile.h"


FTileIndex GhostsTargetAcquisitions::Blinky(const UAbstractMap& map, const APacmanPawn& pacman, const ABoardPawn& itself, const TMap<enum ECharacterTag, ABoardPawn*>& otherPawns) {
	return map.GetCharacterTileIndex(pacman.GetTag());
}


FTileIndex GhostsTargetAcquisitions::Pinky(const UAbstractMap& map, const APacmanPawn& pacman, const ABoardPawn& itself, const TMap<enum ECharacterTag, ABoardPawn*>& otherPawns) {
	return map.GetCharacterTileIndex(pacman.GetTag()) + FTileIndex{ 0, 4 }.RotateTowards(pacman.GetMovingDirection());
}


FTileIndex GhostsTargetAcquisitions::Inky(const UAbstractMap& map, const APacmanPawn& pacman, const ABoardPawn& itself, const TMap<enum ECharacterTag, ABoardPawn*>& otherPawns) {
	auto blinkyIndex = map.GetCharacterTileIndex(otherPawns[ECharacterTag::BLINKY]->GetTag());
	auto pacmanIndex = map.GetCharacterTileIndex(pacman.GetTag());

	// To determine the target tile, get Pacman position and reflect it towards Blinky's position
	auto pacmanToBlinky = blinkyIndex - pacmanIndex;
	return blinkyIndex + pacmanToBlinky;
}


FTileIndex GhostsTargetAcquisitions::Clyde(const UAbstractMap& map, const APacmanPawn& pacman, const ABoardPawn& itself, const TMap<enum ECharacterTag, ABoardPawn*>& otherPawns) {
	auto pacmanIndex = map.GetCharacterTileIndex(pacman.GetTag());
	auto itselfIndex = map.GetCharacterTileIndex(itself.GetTag());
	float distanceFromPacman = (pacmanIndex - itselfIndex).Length();

	// If Clyde is further away from Pacman than more than 8 units, it behaves like Blinky
	if (distanceFromPacman > 8.f) {
		return Blinky(map, pacman, itself, otherPawns);
	}
	// Else the target tile is the same as in scatter mode
	else {
		auto scatterTile = Cast<APacmanSettings>(itself.GetWorld()->GetWorldSettings())->ScatterTiles[itself.GetTag()];
		return scatterTile->Index;
	}
}
