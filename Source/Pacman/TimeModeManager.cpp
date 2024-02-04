#include "TimeModeManager.h"
#include "PacmanGameMode.h"
#include "PacmanLevelState.h"
#include "GhostPawn.h"
#include "PacmanPawn.h"
#include "UObject/Class.h"


// Sets default values for this component's properties
UTimeModeManager::UTimeModeManager() {
	PrimaryComponentTick.bCanEverTick = true;
}


// Returns the current mode.
EGhostMode UTimeModeManager::GetCurrentMode() {
	return CurrentLevelSettings->ModesSchedule[CurrentModeIndex - 1].Mode;
}


// Resets the component and loads the specified settings to the ghosts, Pacman and initializes the schedules. Doesn't start the timers.
void UTimeModeManager::Init(ULevelSettings* settings) {
	verifyf(!bStarted, TEXT("Cannot change the settings after level started")); // If the level already started it's not allowed to change settings
	Reset();
	GameMode = Cast<APacmanGameMode>(GetWorld()->GetAuthGameMode()); // Get the game mode
	GhostRespawnTimers.Reserve(Cast<APacmanLevelState>(GameMode->GameState)->GetBoardPawns().Num()); // At most we need this number of timers (pretty rare to have all allocated)

	CurrentLevelSettings = settings;
	CurrentLevelSettings->FruitsSchedule.Sort(); // Sort by how many food items to activate

	// Find the ghosts by their name and set their speeds, also set Pacman speed
	for (TObjectIterator<ABoardPawn> pawn; pawn; ++pawn) {
		if (pawn->GetWorld() != GetWorld()) continue; // Skip objects that are not in the actual scene (maybe they were in the editor or somewhere else)
		// If the pawn is a ghost...
		if (auto ghost = Cast<AGhostPawn>(*pawn); ghost) {
			auto ghostItem = CurrentLevelSettings->GhostsSchedule.FindByPredicate([&ghost](const auto& ghostItem) { return ghostItem.GhostId == ghost->GetId(); });
			verifyf(ghostItem, TEXT("Cannot find a ghost with UniqueId = %s"), *ghost->GetId());
			ghostItem->Ghost = ghost;
			ghost->SetBaseSpeed(ghostItem->BaseSpeed);
		}
		else if (auto pacman = Cast<APacmanPawn>(*pawn); pacman) pacman->SetBaseSpeed(CurrentLevelSettings->PacmanBaseSpeed);
	}

	UE_LOG(LogTemp, Display, TEXT("New settings loaded"));
}


// Starts the level.
void UTimeModeManager::Start() {
	UE_LOG(LogTemp, Display, TEXT("Starting schedules..."));
	bStarted = true;

	// Start the first timer for mode and ghosts
	StartNextModeTimer();
	StartNextGhostTimer();
}


// Should be called when a ghost dies.
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


// Starts the next timer for the ghosts.
void UTimeModeManager::StartNextGhostTimer() {
	if ((int)CurrentGhostIndex >= CurrentLevelSettings->GhostsSchedule.Num()) return;
	auto& ghostItem = CurrentLevelSettings->GhostsSchedule[CurrentGhostIndex];
	ActivateGhost(*ghostItem.Ghost);

	// Set the timer for the next ghost activation
	GetWorld()->GetTimerManager().SetTimer(GhostTimer, this, &UTimeModeManager::StartNextGhostTimer, (float)ghostItem.AfterLastGhost, false);
	CurrentGhostIndex++;
}


// Activates the specified ghost in the current mode.
void UTimeModeManager::ActivateGhost(AGhostPawn& ghost) {
	auto mode = CurrentLevelSettings->ModesSchedule[CurrentModeIndex - 1].Mode;
	UE_LOG(LogTemp, Display, TEXT("Activating ghost: %s - %s"), *ghost.GetName(), *UEnum::GetValueAsString<EGhostMode>(mode));
	GameMode->SetGhostModeUnless(ghost, mode);
}


// +1 on the food eaten in this level. If a fruit threshold is reached, the fruit is spawned.
void UTimeModeManager::NotifyStandardFoodDecreasedBy1(unsigned int remainingFood) {
	for (int i = CurrentLevelSettings->FruitsSchedule.Num() - 1; i >= 0 && remainingFood <= CurrentLevelSettings->FruitsSchedule[i].RemainingFoodThreshold; --i) {
		// We want to spawn the food only if the threshold has been reached in with this specific function call (not every time a food is eaten)
		if (auto& fruitScheduleItem = CurrentLevelSettings->FruitsSchedule[i]; fruitScheduleItem.RemainingFoodThreshold == remainingFood) {
			FTransform transform{ fruitScheduleItem.SpawnPosition };
			GetWorld()->SpawnActor<AFruitFood>(fruitScheduleItem.Fruit->GetAuthoritativeClass(), transform);
		}
	}
}


// Should be called when Pacman eats a power pellet, turns all ghosts into FRIGHTENED mode (unless they are in DEAD or HOME mode).
void UTimeModeManager::NotifyPowerPelletEaten() {
	GameMode->SetGhostsModeUnless(EGhostMode::FRIGHTENED, { EGhostMode::DEAD, EGhostMode::HOME });
	GetWorld()->GetTimerManager().SetTimer(PowerPelletTimer, this, &UTimeModeManager::NotifyPowerPelletEnded, CurrentLevelSettings->PowerPelletDuration, false);
}


// Should be called when a power pellet ends (due to time or becasue all ghosts got eaten).
void UTimeModeManager::NotifyPowerPelletEnded() {
	UE_LOG(LogTemp, Display, TEXT("Power pellet ended"));
	GameMode->SetGhostsModeUnless(GetCurrentMode(), { EGhostMode::DEAD, EGhostMode::HOME }); // Resume the current mode for all the ghosts (unless they are in DEAD or HOME mode)
}


// Should be called when the level ends, for whatever reason.
void UTimeModeManager::NotifyLevelEnded() {
	Reset();
}


// Resets all the timers and counters in the level.
void UTimeModeManager::Reset() {
	// Clear all the timers
	auto& timerManager = GetWorld()->GetTimerManager();
	timerManager.ClearTimer(GhostTimer);
	timerManager.ClearTimer(ModeTimer);
	timerManager.ClearTimer(PowerPelletTimer);
	for (auto& timer : GhostRespawnTimers) timerManager.ClearTimer(timer.Value);
	GhostRespawnTimers.Empty();

	// Reset all counters
	CurrentGhostIndex = 0;
	CurrentModeIndex = 0;
	CurrentLevelSettings = nullptr;
	bStarted = false;
}


// Starts the next timer for the modes.
void UTimeModeManager::StartNextModeTimer() {
	if ((int)CurrentModeIndex >= CurrentLevelSettings->ModesSchedule.Num()) return; // If we have no new modes, do nothing

	auto& modeItem = CurrentLevelSettings->ModesSchedule[CurrentModeIndex];
	UE_LOG(LogTemp, Display, TEXT("Starting next mode: %s - %i seconds"), *UEnum::GetValueAsString<EGhostMode>(modeItem.Mode), modeItem.Duration);
	
	// Set the mode for all the ghosts in SCATTERor CHASE modes
	GameMode->SetGhostsModeUnless(modeItem.Mode, { EGhostMode::DEAD, EGhostMode::HOME, EGhostMode::FRIGHTENED });
	
	// Set the timer for the next mode activation
	GetWorld()->GetTimerManager().SetTimer(ModeTimer, this, &UTimeModeManager::StartNextModeTimer, (float)modeItem.Duration, false);
	CurrentModeIndex++;
}




