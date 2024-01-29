#include "PowerPelletFood.h"
#include "PacmanLevelState.h"
#include "PacmanPawn.h"

APowerPelletFood::APowerPelletFood() {
}

void APowerPelletFood::BeginPlay() {
	Super::BeginPlay();
	UE_LOG(LogTemp, Display, TEXT("New power pellet food %s created"), *GetName());
}

void APowerPelletFood::AutoDestroy() {
	auto name = GetName();
	verifyf(Destroy(), TEXT("APowerPelletFood could not be destroyed for unknown reasons."));
	UE_LOG(LogTemp, Display, TEXT("Power pellet food %s destroyed"), *name);
	// TODO particle effect
}

void APowerPelletFood::OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult) {
	if (otherActor->IsA(APacmanPawn::StaticClass())) {
		LevelState->NotifyPowerPelletEaten(Value);
		AutoDestroy();
	}
}
