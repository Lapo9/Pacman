#include "StandardFood.h"
#include "PacmanPawn.h"
#include "PacmanLevelState.h"


AStandardFood::AStandardFood() {
}


void AStandardFood::BeginPlay() {
	Super::BeginPlay();
	LevelState->AddStandardFood(); // Tell the level state there is one more food
	UE_LOG(LogTemp, Display, TEXT("New standard food %s created"), *GetName());
}


// Destroys this actor (with an animation/particle effect)
void AStandardFood::AutoDestroy() {
	auto name = GetName();
	verifyf(Destroy(), TEXT("AStandardFood could not be destroyed for unknown reasons."));
	UE_LOG(LogTemp, Display, TEXT("Standard food %s destroyed"), *name);
	// TODO particle effect
}


// Called when another component starts overlapping with this one.
void AStandardFood::OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult) {
	if (otherActor->IsA(APacmanPawn::StaticClass())) {
		LevelState->NotifyStandardFoodEaten(Value);
		AutoDestroy();
	}
}
