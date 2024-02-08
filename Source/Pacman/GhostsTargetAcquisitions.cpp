#include "GhostsTargetAcquisitions.h"
#include "AbstractMap.h"
#include "BoardPawn.h"
#include "GhostPawn.h"
#include "PacmanPawn.h"
#include "PacmanSettings.h"
#include "PacmanLevelState.h"
#include "Tile.h"


FTileIndex UGhostTargetAcquisitionBlinky::AcquireTarget(GHOST_TARGET_ACQUISITION_PARAMS) const {
	return TargetAcquisitionFunctions::Blinky(levelState, itself);
}


FTileIndex UGhostTargetAcquisitionPinky::AcquireTarget(GHOST_TARGET_ACQUISITION_PARAMS) const {
	return TargetAcquisitionFunctions::Pinky(levelState, itself);
}


FTileIndex UGhostTargetAcquisitionInky::AcquireTarget(GHOST_TARGET_ACQUISITION_PARAMS) const {
	return TargetAcquisitionFunctions::Inky(levelState, itself);
}


FTileIndex UGhostTargetAcquisitionClyde::AcquireTarget(GHOST_TARGET_ACQUISITION_PARAMS) const {
	return TargetAcquisitionFunctions::Clyde(levelState, itself);
}


FTileIndex UGhostTargetAcquisitionScatter::AcquireTarget(GHOST_TARGET_ACQUISITION_PARAMS) const {
	return TargetAcquisitionFunctions::Scatter(levelState, itself);
}


FTileIndex UGhostTargetAcquisitionHome::AcquireTarget(GHOST_TARGET_ACQUISITION_PARAMS) const {
	return TargetAcquisitionFunctions::Home(levelState, itself);
}


FTileIndex UGhostTargetAcquisitionRandom::AcquireTarget(GHOST_TARGET_ACQUISITION_PARAMS) const {
	return TargetAcquisitionFunctions::Random(levelState, itself);
}


FTileIndex UGhostTargetAcquisitionDead::AcquireTarget(GHOST_TARGET_ACQUISITION_PARAMS) const {
	return TargetAcquisitionFunctions::Dead(levelState, itself);
}




FTileIndex TargetAcquisitionFunctions::Blinky(GHOST_TARGET_ACQUISITION_PARAMS) {
	auto& map = levelState.GetAbstractMap();
	auto target = map.GetCharacterTileIndex(*levelState.GetPacman());
	return target;
}


FTileIndex TargetAcquisitionFunctions::Pinky(GHOST_TARGET_ACQUISITION_PARAMS) {
	auto& map = levelState.GetAbstractMap();
	auto& pacman = *levelState.GetPacman();
	return map.GetCharacterTileIndex(pacman) + FTileIndex{ 0, 4 }.RotateTowards(pacman.GetMovingDirection());
}


FTileIndex TargetAcquisitionFunctions::Inky(GHOST_TARGET_ACQUISITION_PARAMS) {
	auto& map = levelState.GetAbstractMap();
	auto& pacman = *levelState.GetPacman();
	auto blinky = levelState.GetBoardPawn(ECharacterTag::BLINKY);

	if (!blinky) return UGhostTargetAcquisitionBlinky{}.AcquireTarget(levelState, itself); // if there is no Blinky, act like Blinky as a fallback option

	auto blinkyIndex = map.GetCharacterTileIndex(*blinky);
	auto pacmanIndex = map.GetCharacterTileIndex(pacman);

	// To determine the target tile, get Pacman position and reflect it towards Blinky's position
	auto pacmanToBlinky = blinkyIndex - pacmanIndex;
	return blinkyIndex + pacmanToBlinky;
}


FTileIndex TargetAcquisitionFunctions::Clyde(GHOST_TARGET_ACQUISITION_PARAMS) {
	auto& map = levelState.GetAbstractMap();
	auto& pacman = *levelState.GetPacman();

	auto pacmanIndex = map.GetCharacterTileIndex(pacman);
	auto itselfIndex = map.GetCharacterTileIndex(itself);
	float distanceFromPacman = (pacmanIndex - itselfIndex).Length();

	// If Clyde is further away from Pacman than more than 8 units, it behaves like Blinky
	if (distanceFromPacman > 8.f) {
		return TargetAcquisitionFunctions::Blinky(levelState, itself);
	}
	// Else the target tile is the same as in scatter mode
	else {
		auto scatterTile = itself.GetScatterTile();
		return scatterTile->Index;
	}
}


FTileIndex TargetAcquisitionFunctions::Home(GHOST_TARGET_ACQUISITION_PARAMS){
	return itself.GetHomeTile()->Index;
}


FTileIndex TargetAcquisitionFunctions::Scatter(GHOST_TARGET_ACQUISITION_PARAMS) {
	return itself.GetScatterTile()->Index;
}


FTileIndex TargetAcquisitionFunctions::Dead(GHOST_TARGET_ACQUISITION_PARAMS) {
	return itself.GetRespawnTile()->Index;
}


FTileIndex TargetAcquisitionFunctions::Random(GHOST_TARGET_ACQUISITION_PARAMS) {
	auto size = levelState.GetAbstractMap().GetSize();
	int randCol = FMath::RandRange(0, size.Col - 1);
	int randRow = FMath::RandRange(0, size.Row - 1);
	return { randCol, randRow };
}
