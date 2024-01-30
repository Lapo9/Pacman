#include "TimeModeManager.h"
#include "PacmanGameMode.h"
#include "GhostPawn.h"
#include "UObject/Class.h"


// Sets default values for this component's properties
UTimeModeManager::UTimeModeManager() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UTimeModeManager::ResumeCurrentMode() {
	Cast<APacmanGameMode>(GetWorld()->GetAuthGameMode())->SetGhostsMode(ModesSchedule[CurrentModeIndex].Mode);
}


// Called when the game starts
void UTimeModeManager::BeginPlay() {
	// Start the first timer for mode and ghosts
	StartNextModeTimer();
	StartNextGhostTimer();

	FruitsSchedule.Sort(); // Sort by how many food items to activate
}


void UTimeModeManager::StartNextGhostTimer() {
	if ((int)CurrentGhostIndex >= GhostsSchedule.Num()) return;

	auto& ghostItem = GhostsSchedule[CurrentGhostIndex];
	UE_LOG(LogTemp, Display, TEXT("Activating ghost: %s - %s - %i seconds"), *ghostItem.Ghost->GetName(), *UEnum::GetValueAsString<EGhostMode>(ghostItem.Mode), ghostItem.AfterLastGhost);
	Cast<APacmanGameMode>(GetWorld()->GetAuthGameMode())->SetGhostMode(*ghostItem.Ghost, ghostItem.Mode);
	GetWorld()->GetTimerManager().SetTimer(GhostTimer, this, &UTimeModeManager::StartNextGhostTimer, (float)ghostItem.AfterLastGhost, false);
	CurrentGhostIndex++;
}

void UTimeModeManager::AddFoodEaten() {
	FoodEaten++;

	for()
}


void UTimeModeManager::StartNextModeTimer() {
	if ((int)CurrentModeIndex >= ModesSchedule.Num()) return;

	auto& modeItem = ModesSchedule[CurrentModeIndex];
	UE_LOG(LogTemp, Display, TEXT("Starting next mode: %s - %i seconds"), *UEnum::GetValueAsString<EGhostMode>(modeItem.Mode), modeItem.Duration);
	ResumeCurrentMode();
	GetWorld()->GetTimerManager().SetTimer(ModeTimer, this, &UTimeModeManager::StartNextModeTimer, (float)modeItem.Duration, false);
	CurrentModeIndex++;
}

