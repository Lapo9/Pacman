#include "PacmanGameInstance.h"


unsigned int UPacmanGameInstance::GetLevel() const {
	return CurrentLevel;
}


void UPacmanGameInstance::StartNewGame() {
	CurrentLevel = 0;
	Lives = 3;
	Points = 0;
}


void UPacmanGameInstance::StartNextLevel() {
	CurrentLevel++;
}


int UPacmanGameInstance::AddPoints(unsigned int value) {
	Points += value;
	UE_LOG(LogTemp, Display, TEXT("Points: %i"), Points);
	return Points;
}


void UPacmanGameInstance::AddExtraLife() {
	Lives++;
}

