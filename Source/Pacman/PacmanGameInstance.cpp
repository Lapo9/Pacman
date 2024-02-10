#include "PacmanGameInstance.h"


unsigned int UPacmanGameInstance::GetLevel() const {
	return CurrentLevel;
}


void UPacmanGameInstance::LevelEnded(bool win) {
	if (win) {
		CurrentLevel++;
	}
	else {
		CurrentLevel = 0;
		Lives = 3;
	}
}


int UPacmanGameInstance::AddPoints(unsigned int value) {
	Points += value;
	UE_LOG(LogTemp, Display, TEXT("Points: %i"), Points);
	return Points;
}


void UPacmanGameInstance::AddExtraLife() {
	Lives++;
}

