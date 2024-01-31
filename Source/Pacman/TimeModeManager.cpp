#include "TimeModeManager.h"
#include "PacmanGameMode.h"
#include "GhostPawn.h"
#include "UObject/Class.h"


// Sets default values for this component's properties
UTimeModeManager::UTimeModeManager() {
	PrimaryComponentTick.bCanEverTick = true;
}


EGhostMode UTimeModeManager::GetCurrentMode() {
	return CurrentLevelSettings->ModesSchedule[FMath::Min((int)CurrentModeIndex, CurrentLevelSettings->ModesSchedule.Num() - 1)].Mode;
}


void UTimeModeManager::SetSettings(ULevelSettings* settings) {
	verifyf(!Started, TEXT("Cannot change the settings after level started")); // If the level already started it's not allowed to change settings
	
	// Reset
	FoodEaten = 0;
	GetWorld()->GetTimerManager().ClearTimer(GhostTimer);
	CurrentGhostIndex = 0;
	GetWorld()->GetTimerManager().ClearTimer(ModeTimer);
	CurrentModeIndex = 0;

	CurrentLevelSettings = settings;
	CurrentLevelSettings->FruitsSchedule.Sort(); // Sort by how many food items to activate

	// Find the ghosts by their name
	for (auto& ghostItem : CurrentLevelSettings->GhostsSchedule) {
		for (TObjectIterator<AGhostPawn> ghost; ghost; ++ghost) {
			if (ghost->GetWorld() != GetWorld()) continue; // Skip objects that are not in the actual scene (maybe they were in the editor or somewhere else)
			if (ghost->GetId() == ghostItem.GhostId) {
				ghostItem.Ghost = *ghost;
				break;
			}
		}
		verifyf(ghostItem.Ghost != nullptr, TEXT("Cannot find a ghost with UniqueId = %s"), *ghostItem.GhostId);
	}

	UE_LOG(LogTemp, Display, TEXT("New settings bound"));
}


void UTimeModeManager::Start() {
	UE_LOG(LogTemp, Display, TEXT("Starting level..."));
	Started = true;

	// Start the first timer for mode and ghosts
	StartNextModeTimer();
	StartNextGhostTimer();
}


void UTimeModeManager::NotifyGhostDied(AGhostPawn& ghost) {
	// Find the duration for the dead ghost
	auto cooldown = CurrentLevelSettings->GhostsSchedule.FindByPredicate([&ghost](const auto& item) { return item.Ghost == &ghost; })->RespawnCooldown;
	auto& timer = GhostRespawnTimers.Add(&ghost, FTimerHandle{}); // Add a timer to the pool
	
	// Set the timer for the respawn
	GetWorld()->GetTimerManager().SetTimer(timer, [this, &ghost]() {
		ActivateGhost(ghost); // Activate the ghost
		GhostRespawnTimers.FindAndRemoveChecked(&ghost); // Remove the timer from the pool
										   }, cooldown, false);
}


// Called when the game starts
void UTimeModeManager::BeginPlay() {
	
}


void UTimeModeManager::StartNextGhostTimer() {
	if ((int)CurrentGhostIndex >= CurrentLevelSettings->GhostsSchedule.Num()) return;
	auto& ghostItem = CurrentLevelSettings->GhostsSchedule[CurrentGhostIndex];
	ActivateGhost(*ghostItem.Ghost);

	// Set the timer for the next ghost activation
	GetWorld()->GetTimerManager().SetTimer(GhostTimer, this, &UTimeModeManager::StartNextGhostTimer, (float)ghostItem.AfterLastGhost, false);
	CurrentGhostIndex++;
}


void UTimeModeManager::ActivateGhost(AGhostPawn& ghost) {
	auto mode = CurrentLevelSettings->ModesSchedule[FMath::Min((int)CurrentModeIndex, CurrentLevelSettings->ModesSchedule.Num() - 1)].Mode;
	UE_LOG(LogTemp, Display, TEXT("Activating ghost: %s - %s"), *ghost.GetName(), *UEnum::GetValueAsString<EGhostMode>(mode));
	Cast<APacmanGameMode>(GetWorld()->GetAuthGameMode())->SetGhostModeUnless(ghost, mode);
}


void UTimeModeManager::NotifyFoodEaten() {
	FoodEaten++;

	for (int i = 0; FoodEaten <= CurrentLevelSettings->FruitsSchedule[i].AvailableFoodThreshold; ++i) {
		// TODO spawn food
	}
}


void UTimeModeManager::StartNextModeTimer() {
	if ((int)CurrentModeIndex >= CurrentLevelSettings->ModesSchedule.Num()) return; // If we have no new modes, do nothing

	auto& modeItem = CurrentLevelSettings->ModesSchedule[CurrentModeIndex];
	UE_LOG(LogTemp, Display, TEXT("Starting next mode: %s - %i seconds"), *UEnum::GetValueAsString<EGhostMode>(modeItem.Mode), modeItem.Duration);
	
	// Set the mode for all the ghosts in SCATTERor STANDARD modes
	Cast<APacmanGameMode>(GetWorld()->GetAuthGameMode())->SetGhostsModeUnless(modeItem.Mode, { EGhostMode::DEAD, EGhostMode::HOME, EGhostMode::FRIGHTENED });
	
	// Set the timer for the next mode activation
	GetWorld()->GetTimerManager().SetTimer(ModeTimer, this, &UTimeModeManager::StartNextModeTimer, (float)modeItem.Duration, false);
	CurrentModeIndex++;
}




